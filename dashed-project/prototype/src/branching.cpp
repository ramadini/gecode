#include "dashed/branching.hpp"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dashed {
namespace {

int balanced_value_pivot(const ValueSet& values) {
  const std::uint64_t cardinality = values.cardinality();
  if (cardinality < 2) {
    throw std::logic_error(
        "balanced_value_pivot requires at least two values");
  }

  // The left branch receives floor(cardinality / 2) values. This guarantees
  // two non-empty alternatives and remains balanced for sparse range sets.
  std::uint64_t remaining = cardinality / 2;

  for (const IntRange& range : values.ranges()) {
    const std::uint64_t width = range.cardinality();
    if (remaining > width) {
      remaining -= width;
      continue;
    }

    const std::int64_t pivot =
        static_cast<std::int64_t>(range.lower) +
        static_cast<std::int64_t>(remaining) - 1;

    return static_cast<int>(pivot);
  }

  throw std::logic_error(
      "could not select a ValueSet branch pivot");
}

Domain replace_repeat(
    const Domain& domain,
    std::size_t segment_index,
    std::vector<Segment> replacement) {
  std::vector<Segment> segments;
  segments.reserve(
      domain.segments().size() - 1 + replacement.size());

  segments.insert(
      segments.end(),
      domain.segments().begin(),
      domain.segments().begin() +
          static_cast<std::ptrdiff_t>(segment_index));

  segments.insert(
      segments.end(),
      std::make_move_iterator(replacement.begin()),
      std::make_move_iterator(replacement.end()));

  segments.insert(
      segments.end(),
      domain.segments().begin() +
          static_cast<std::ptrdiff_t>(segment_index + 1),
      domain.segments().end());

  return Domain(
      std::move(segments),
      domain.min_length(),
      domain.max_length());
}

}  // namespace

std::optional<BranchDecision> choose_branch(
    const Domain& domain) {
  if (domain.failed() || domain.assigned()) {
    return std::nullopt;
  }

  // A count interval can be partitioned exactly only when every other
  // segment count is fixed. With two variable-width neighboring blocks, the
  // same concrete list can admit different internal decompositions and would
  // occur in both count alternatives.
  std::size_t variable_count_index =
      domain.segments().size();
  std::size_t variable_count_segments = 0;

  for (std::size_t index = 0;
       index < domain.segments().size();
       ++index) {
    const auto* repeat =
        std::get_if<RepeatSegment>(
            &domain.segments()[index]);

    if (repeat == nullptr ||
        repeat->lower == repeat->upper) {
      continue;
    }

    variable_count_index = index;
    ++variable_count_segments;
  }

  if (variable_count_segments > 1) {
    return std::nullopt;
  }

  if (variable_count_segments == 1) {
    const auto& repeat =
        std::get<RepeatSegment>(
            domain.segments()[variable_count_index]);

    const std::uint64_t lower = repeat.lower;
    const std::uint64_t upper = repeat.upper;
    const Length pivot =
        static_cast<Length>(
            lower + (upper - lower) / 2);

    return BranchDecision{
        BranchKind::repeat_count,
        variable_count_index,
        pivot,
        0};
  }

  // Once counts are exact, split the first non-singleton block alphabet by
  // value order. Literal runs are already exact and require no decisions.
  for (std::size_t index = 0;
       index < domain.segments().size();
       ++index) {
    const auto* repeat =
        std::get_if<RepeatSegment>(
            &domain.segments()[index]);

    if (repeat == nullptr ||
        repeat->values.cardinality() < 2) {
      continue;
    }

    return BranchDecision{
        BranchKind::value_set,
        index,
        0,
        balanced_value_pivot(
            repeat->values)};
  }

  return std::nullopt;
}

Domain apply_branch(
    const Domain& domain,
    const BranchDecision& decision,
    unsigned int alternative) {
  if (alternative > 1) {
    throw std::invalid_argument(
        "branch alternative must be zero or one");
  }

  if (domain.failed()) {
    return domain;
  }

  if (decision.segment >=
      domain.segments().size()) {
    throw std::out_of_range(
        "branch segment index outside domain");
  }

  const auto* source =
      std::get_if<RepeatSegment>(
          &domain.segments()[decision.segment]);

  if (source == nullptr) {
    throw std::invalid_argument(
        "branch decision does not select a repeat block");
  }

  RepeatSegment restricted = *source;
  std::vector<Segment> replacement;

  switch (decision.kind) {
    case BranchKind::repeat_count:
      if (decision.count_pivot < source->lower ||
          decision.count_pivot >= source->upper) {
        throw std::invalid_argument(
            "invalid repeat-count branch pivot");
      }

      if (alternative == 0) {
        restricted.upper =
            decision.count_pivot;
      } else {
        restricted.lower =
            static_cast<Length>(
                decision.count_pivot + 1);
      }

      replacement.push_back(
          std::move(restricted));
      break;

    case BranchKind::value_set: {
      if (!source->exact_count() ||
          source->lower == 0 ||
          source->values.cardinality() < 2 ||
          decision.value_pivot <
              source->values.min() ||
          decision.value_pivot >=
              source->values.max()) {
        throw std::invalid_argument(
            "invalid value-set branch pivot");
      }

      const int lower = alternative == 0
          ? std::numeric_limits<int>::min()
          : decision.value_pivot + 1;

      const int upper = alternative == 0
          ? decision.value_pivot
          : std::numeric_limits<int>::max();

      ValueSet selected =
          source->values.intersected(
              ValueSet(lower, upper));

      if (selected.empty()) {
        throw std::invalid_argument(
            "value-set branch produced an empty side");
      }

      // Restricting the alphabet of the complete repeated block would lose
      // mixed lists such as [0,1]. Isolate one logical occurrence and retain
      // the original alphabet for the exact remainder instead.
      replacement.push_back(
          RepeatSegment{
              std::move(selected),
              1,
              1});

      if (source->lower > 1) {
        const Length remainder =
            static_cast<Length>(
                source->lower - 1);

        replacement.push_back(
            RepeatSegment{
                source->values,
                remainder,
                remainder});
      }
      break;
    }
  }

  return replace_repeat(
      domain,
      decision.segment,
      std::move(replacement));
}

}  // namespace dashed
