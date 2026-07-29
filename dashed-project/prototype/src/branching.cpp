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

void check_alternative(unsigned int alternative) {
  if (alternative > 1) {
    throw std::invalid_argument(
        "branch alternative must be zero or one");
  }
}

int balanced_value_pivot(const ValueSet& values) {
  const std::uint64_t cardinality = values.cardinality();
  if (cardinality < 2) {
    throw std::logic_error(
        "balanced_value_pivot requires at least two values");
  }

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

ValueSet value_side(
    int pivot,
    unsigned int alternative) {
  check_alternative(alternative);

  if (alternative == 0) {
    return ValueSet(
        std::numeric_limits<int>::min(),
        pivot);
  }

  if (pivot == std::numeric_limits<int>::max()) {
    return ValueSet();
  }

  return ValueSet(
      pivot + 1,
      std::numeric_limits<int>::max());
}

Domain failed_copy(const Domain& domain) {
  Domain failed = domain;
  failed.fail();
  return failed;
}

ValueSet position_values(
    const Domain& domain,
    Length position) {
  if (domain.failed() ||
      domain.min_length() != domain.max_length() ||
      position >= domain.max_length()) {
    throw std::invalid_argument(
        "value branch requires a present fixed position");
  }

  Length offset = 0;

  for (const Segment& segment : domain.segments()) {
    if (const auto* literal =
            std::get_if<LiteralSegment>(&segment)) {
      const Length width =
          static_cast<Length>(literal->literal.size());

      if (position < offset + width) {
        return ValueSet(
            literal->literal[
                static_cast<std::size_t>(position - offset)]);
      }

      offset = static_cast<Length>(offset + width);
      continue;
    }

    const auto& repeat =
        std::get<RepeatSegment>(segment);

    if (!repeat.exact_count()) {
      throw std::invalid_argument(
          "value branch crossed a variable-width block");
    }

    if (position < offset + repeat.lower) {
      return repeat.values;
    }

    offset = static_cast<Length>(offset + repeat.lower);
  }

  throw std::out_of_range(
      "value branch position outside domain");
}

Domain restrict_position(
    const Domain& domain,
    Length position,
    const ValueSet& allowed) {
  if (domain.failed()) {
    return domain;
  }

  if (domain.min_length() != domain.max_length() ||
      position >= domain.max_length()) {
    throw std::invalid_argument(
        "value branch requires a present fixed position");
  }

  std::vector<Segment> result;
  result.reserve(domain.segment_count() + 2);
  Length offset = 0;
  bool restricted = false;

  for (const Segment& segment : domain.segments()) {
    if (restricted) {
      result.push_back(segment);
      continue;
    }

    if (const auto* literal =
            std::get_if<LiteralSegment>(&segment)) {
      const Length width =
          static_cast<Length>(literal->literal.size());

      if (position >= offset + width) {
        result.push_back(segment);
        offset = static_cast<Length>(offset + width);
        continue;
      }

      const int value =
          literal->literal[
              static_cast<std::size_t>(position - offset)];

      if (!allowed.contains(value)) {
        return failed_copy(domain);
      }

      result.push_back(segment);
      restricted = true;
      continue;
    }

    const auto& repeat =
        std::get<RepeatSegment>(segment);

    if (!repeat.exact_count()) {
      throw std::invalid_argument(
          "value branch crossed a variable-width block");
    }

    if (position >= offset + repeat.lower) {
      result.push_back(segment);
      offset = static_cast<Length>(offset + repeat.lower);
      continue;
    }

    const ValueSet selected =
        repeat.values.intersected(allowed);

    if (selected.empty()) {
      return failed_copy(domain);
    }

    const Length local =
        static_cast<Length>(position - offset);

    if (local > 0) {
      result.push_back(
          RepeatSegment{
              repeat.values,
              local,
              local});
    }

    result.push_back(
        RepeatSegment{
            selected,
            1,
            1});

    const Length suffix =
        static_cast<Length>(repeat.lower - local - 1);

    if (suffix > 0) {
      result.push_back(
          RepeatSegment{
              repeat.values,
              suffix,
              suffix});
    }

    restricted = true;
  }

  if (!restricted) {
    throw std::out_of_range(
        "value branch position outside domain");
  }

  return Domain(
      std::move(result),
      domain.min_length(),
      domain.max_length());
}

Domain apply_semantic_branch(
    const Domain& domain,
    const BranchDecision& decision,
    unsigned int alternative) {
  check_alternative(alternative);

  if (domain.failed()) {
    return domain;
  }

  switch (decision.kind) {
    case BranchKind::repeat_count: {
      Domain restricted = domain;

      if (alternative == 0) {
        restricted.tighten_length(
            0,
            decision.length_pivot);
      } else {
        if (decision.length_pivot ==
            std::numeric_limits<Length>::max()) {
          restricted.fail();
        } else {
          restricted.tighten_length(
              static_cast<Length>(
                  decision.length_pivot + 1),
              std::numeric_limits<Length>::max());
        }
      }

      return restricted;
    }

    case BranchKind::value_set:
      return restrict_position(
          domain,
          decision.position,
          value_side(
              decision.value_pivot,
              alternative));
  }

  throw std::logic_error("unknown branch kind");
}

}  // namespace

std::optional<BranchDecision> choose_branch(
    const Domain& domain) {
  if (domain.failed() || domain.assigned()) {
    return std::nullopt;
  }

  std::size_t variable_count_segments = 0;

  for (const Segment& segment : domain.segments()) {
    const auto* repeat =
        std::get_if<RepeatSegment>(&segment);

    if (repeat != nullptr &&
        repeat->lower != repeat->upper) {
      ++variable_count_segments;
    }
  }

  if (variable_count_segments > 1) {
    return std::nullopt;
  }

  if (variable_count_segments == 1) {
    const std::uint64_t lower = domain.min_length();
    const std::uint64_t upper = domain.max_length();
    const Length pivot =
        static_cast<Length>(
            lower + (upper - lower) / 2);

    return BranchDecision{
        BranchKind::repeat_count,
        pivot,
        0,
        0};
  }

  Length position = 0;

  for (const Segment& segment : domain.segments()) {
    if (const auto* literal =
            std::get_if<LiteralSegment>(&segment)) {
      position = static_cast<Length>(
          position + literal->literal.size());
      continue;
    }

    const auto& repeat =
        std::get<RepeatSegment>(segment);

    if (!repeat.exact_count()) {
      return std::nullopt;
    }

    if (repeat.lower > 0 &&
        repeat.values.cardinality() >= 2) {
      return BranchDecision{
          BranchKind::value_set,
          0,
          position,
          balanced_value_pivot(repeat.values)};
    }

    position = static_cast<Length>(
        position + repeat.lower);
  }

  return std::nullopt;
}

Domain apply_branch(
    const Domain& domain,
    const BranchDecision& decision,
    unsigned int alternative) {
  return apply_semantic_branch(
      domain,
      decision,
      alternative);
}

BranchLiteralStatus branch_literal_status(
    const Domain& domain,
    const BranchDecision& decision,
    unsigned int alternative) {
  check_alternative(alternative);

  if (domain.failed()) {
    return BranchLiteralStatus::failed;
  }

  switch (decision.kind) {
    case BranchKind::repeat_count:
      if (alternative == 0) {
        if (domain.min_length() > decision.length_pivot) {
          return BranchLiteralStatus::failed;
        }
        if (domain.max_length() <= decision.length_pivot) {
          return BranchLiteralStatus::subsumed;
        }
      } else {
        if (domain.max_length() <= decision.length_pivot) {
          return BranchLiteralStatus::failed;
        }
        if (domain.min_length() > decision.length_pivot) {
          return BranchLiteralStatus::subsumed;
        }
      }
      return BranchLiteralStatus::undecided;

    case BranchKind::value_set: {
      const ValueSet current =
          position_values(domain, decision.position);
      const ValueSet selected =
          value_side(decision.value_pivot, alternative);

      if (current.disjoint(selected)) {
        return BranchLiteralStatus::failed;
      }

      return selected.contains(current)
          ? BranchLiteralStatus::subsumed
          : BranchLiteralStatus::undecided;
    }
  }

  throw std::logic_error("unknown branch kind");
}

Domain prune_branch_literal(
    const Domain& domain,
    const BranchDecision& decision,
    unsigned int alternative) {
  check_alternative(alternative);
  return apply_semantic_branch(
      domain,
      decision,
      1U - alternative);
}

}  // namespace dashed
