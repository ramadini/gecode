#include "dashed/propagation.hpp"
#include "dashed/detail/sweep.hpp"

#include <algorithm>
#include <limits>
#include <optional>
#include <stdexcept>
#include <vector>

namespace dashed {
namespace {




Change assign_exact(Domain& target, const Domain& exact) {
  if (!exact.assigned()) {
    throw std::logic_error("assign_exact requires an assigned source");
  }
  if (!target.accepts(exact)) {
    target.fail();
    return Change::failed;
  }
  // Preserve the target's existing representation when both domains
  // already denote the same concrete list. This is stronger than structural
  // equality and retains shared LiteralSlice storage.
  if (target == exact ||
      (target.assigned() &&
       target.assigned_equal(exact))) {
    return Change::none;
  }

  target = exact;
  return Change::assigned;
}

bool same_value(const Domain& lhs, const Domain& rhs) {
  return lhs.assigned_equal(rhs);
}


bool single_repeat_domain(
    const Domain& domain) {
  return
      domain.segment_count() == 1 &&
      std::holds_alternative<RepeatSegment>(
          domain.segments().front());
}

Change replace_domain(
    Domain& target,
    const Domain& replacement) {
  if (replacement.failed()) {
    target.fail();
    return Change::failed;
  }

  if (target == replacement) {
    return Change::none;
  }

  const bool was_assigned =
      target.assigned();

  const bool length_changed =
      target.min_length() !=
          replacement.min_length() ||
      target.max_length() !=
          replacement.max_length();

  target = replacement;

  if (!was_assigned &&
      target.assigned()) {
    return Change::assigned;
  }

  return length_changed
      ? Change::both
      : Change::domain;
}

Length clamp_length(std::int64_t value) {
  if (value <= 0) {
    return 0;
  }
  if (value >= static_cast<std::int64_t>(kUnboundedLength)) {
    return kUnboundedLength;
  }
  return static_cast<Length>(value);
}


int assigned_value_at(
    const Domain& assigned,
    Length position) {
  if (!assigned.assigned() ||
      position >= assigned.min_length()) {
    throw std::logic_error(
        "assigned_value_at requires a valid assigned position");
  }

  std::uint64_t remaining = position;

  for (const Segment& segment :
       assigned.segments()) {
    if (const auto* literal =
            std::get_if<LiteralSegment>(
                &segment)) {
      const std::uint64_t count =
          literal->literal.size();

      if (remaining < count) {
        return literal->literal[
            static_cast<std::size_t>(
                remaining)];
      }

      remaining -= count;
      continue;
    }

    const auto& repeat =
        std::get<RepeatSegment>(
            segment);

    const auto value =
        repeat.values.singleton_value();

    if (!repeat.exact_count() ||
        !value.has_value()) {
      throw std::logic_error(
          "assigned domain contains a non-exact segment");
    }

    if (remaining < repeat.upper) {
      return *value;
    }

    remaining -= repeat.upper;
  }

  throw std::logic_error(
      "assigned domain length is inconsistent");
}


class AssignedCursor {
 public:
  explicit AssignedCursor(
      const Domain& assigned)
      : assigned_(assigned) {
    if (!assigned_.assigned()) {
      throw std::logic_error(
          "AssignedCursor requires an assigned domain");
    }
  }

  [[nodiscard]] bool done() const noexcept {
    return segment_index_ >=
        assigned_.segments().size();
  }

  [[nodiscard]] std::optional<int> take_one() {
    if (done()) {
      return std::nullopt;
    }

    const Segment& segment =
        assigned_.segments()[segment_index_];

    int value = 0;
    std::uint64_t width = 0;

    if (const auto* literal =
            std::get_if<LiteralSegment>(
                &segment)) {
      width = literal->literal.size();
      value = literal->literal[
          static_cast<std::size_t>(
              offset_)];
    } else {
      const auto& repeat =
          std::get<RepeatSegment>(
              segment);

      const auto singleton =
          repeat.values.singleton_value();

      if (!repeat.exact_count() ||
          !singleton.has_value()) {
        throw std::logic_error(
            "assigned domain contains a non-exact segment");
      }

      width = repeat.upper;
      value = *singleton;
    }

    ++offset_;

    if (offset_ == width) {
      ++segment_index_;
      offset_ = 0;
    }

    return value;
  }

  [[nodiscard]] bool match_value(
      int expected,
      std::uint64_t count) {
    while (count != 0) {
      if (done()) {
        return false;
      }

      const Segment& segment =
          assigned_.segments()[segment_index_];

      if (const auto* literal =
              std::get_if<LiteralSegment>(
                  &segment)) {
        const std::uint64_t width =
            literal->literal.size();

        const std::uint64_t available =
            width - offset_;

        const std::uint64_t take =
            std::min(
                available,
                count);

        for (std::uint64_t index = 0;
             index < take;
             ++index) {
          if (literal->literal[
                  static_cast<std::size_t>(
                      offset_ + index)] !=
              expected) {
            return false;
          }
        }

        offset_ += take;
        count -= take;

        if (offset_ == width) {
          ++segment_index_;
          offset_ = 0;
        }

        continue;
      }

      const auto& repeat =
          std::get<RepeatSegment>(
              segment);

      const auto singleton =
          repeat.values.singleton_value();

      if (!repeat.exact_count() ||
          !singleton.has_value()) {
        throw std::logic_error(
            "assigned domain contains a non-exact segment");
      }

      if (*singleton != expected) {
        return false;
      }

      const std::uint64_t width =
          repeat.upper;

      const std::uint64_t available =
          width - offset_;

      const std::uint64_t take =
          std::min(
              available,
              count);

      offset_ += take;
      count -= take;

      if (offset_ == width) {
        ++segment_index_;
        offset_ = 0;
      }
    }

    return true;
  }

 private:
  const Domain& assigned_;
  std::size_t segment_index_ = 0;
  std::uint64_t offset_ = 0;
};


std::optional<Change> exclude_assigned_single_witness(
    Domain& target,
    const Domain& assigned) {
  if (target.failed() ||
      target.assigned() ||
      !assigned.assigned() ||
      target.min_length() != target.max_length() ||
      target.min_length() != assigned.min_length()) {
    return std::nullopt;
  }

  // This exact pruning rule requires fixed segment boundaries. Exactly one
  // position may vary, and it must be represented by a one-element repeat
  // segment so removing the assigned value cannot affect another position.
  AssignedCursor cursor(assigned);

  std::optional<std::size_t>
      witness_segment;
  int forbidden = 0;

  for (std::size_t index = 0;
       index < target.segments().size();
       ++index) {
    const Segment& segment =
        target.segments()[index];

    if (const auto* literal =
            std::get_if<LiteralSegment>(
                &segment)) {
      for (int value :
           literal->literal.span()) {
        const auto assigned_value =
            cursor.take_one();

        if (!assigned_value.has_value() ||
            *assigned_value != value) {
          return std::nullopt;
        }
      }

      continue;
    }

    const auto& repeat =
        std::get<RepeatSegment>(
            segment);

    if (!repeat.exact_count()) {
      return std::nullopt;
    }

    if (const auto singleton =
            repeat.values.singleton_value();
        singleton.has_value()) {
      if (!cursor.match_value(
              *singleton,
              repeat.upper)) {
        return std::nullopt;
      }

      continue;
    }

    if (repeat.upper != 1 ||
        witness_segment.has_value()) {
      return std::nullopt;
    }

    const auto assigned_value =
        cursor.take_one();

    if (!assigned_value.has_value() ||
        !repeat.values.contains(
            *assigned_value)) {
      return std::nullopt;
    }

    witness_segment = index;
    forbidden = *assigned_value;
  }

  if (!witness_segment.has_value() ||
      !cursor.done()) {
    return std::nullopt;
  }

  std::vector<Segment> refined =
      target.segments();

  auto& witness =
      std::get<RepeatSegment>(
          refined[*witness_segment]);

  witness.values =
      witness.values.without(
          forbidden);

  Domain replacement(
      std::move(refined),
      target.min_length(),
      target.max_length());

  if (replacement.failed()) {
    target.fail();
    return Change::failed;
  }

  return replace_domain(
      target,
      replacement);
}


std::optional<Change> exclude_assigned_endpoint_count(
    Domain& target,
    const Domain& assigned) {
  if (target.failed() ||
      target.assigned() ||
      !assigned.assigned()) {
    return std::nullopt;
  }

  std::optional<std::size_t> variable_segment;
  std::uint64_t fixed_width = 0;

  for (std::size_t index = 0;
       index < target.segments().size();
       ++index) {
    const Segment& segment =
        target.segments()[index];

    if (const auto* literal =
            std::get_if<LiteralSegment>(
                &segment)) {
      fixed_width += literal->literal.size();
      continue;
    }

    const auto& repeat =
        std::get<RepeatSegment>(
            segment);

    if (!repeat.values.singleton()) {
      return std::nullopt;
    }

    if (repeat.exact_count()) {
      fixed_width += repeat.upper;
      continue;
    }

    if (variable_segment.has_value()) {
      return std::nullopt;
    }

    variable_segment = index;
  }

  if (!variable_segment.has_value() ||
      assigned.min_length() < fixed_width) {
    return std::nullopt;
  }

  const std::uint64_t selected_count =
      static_cast<std::uint64_t>(
          assigned.min_length()) -
      fixed_width;

  if (selected_count >
      static_cast<std::uint64_t>(
          kUnboundedLength)) {
    return std::nullopt;
  }

  const auto& variable =
      std::get<RepeatSegment>(
          target.segments()[
              *variable_segment]);

  const Length count =
      static_cast<Length>(
          selected_count);

  if (count < variable.lower ||
      count > variable.upper) {
    return std::nullopt;
  }

  AssignedCursor cursor(assigned);

  for (std::size_t index = 0;
       index < target.segments().size();
       ++index) {
    const Segment& segment =
        target.segments()[index];

    if (const auto* literal =
            std::get_if<LiteralSegment>(
                &segment)) {
      for (int value :
           literal->literal.span()) {
        const auto assigned_value =
            cursor.take_one();

        if (!assigned_value.has_value() ||
            *assigned_value != value) {
          return std::nullopt;
        }
      }

      continue;
    }

    const auto& repeat =
        std::get<RepeatSegment>(
            segment);

    const auto singleton =
        repeat.values.singleton_value();

    if (!singleton.has_value()) {
      return std::nullopt;
    }

    const Length width =
        index == *variable_segment
            ? count
            : repeat.upper;

    if (!cursor.match_value(
            *singleton,
            width)) {
      return std::nullopt;
    }
  }

  if (!cursor.done()) {
    return std::nullopt;
  }

  if (count != variable.lower &&
      count != variable.upper) {
    // Removing an interior count would create a hole in the interval and is
    // not representable by one canonical repeat segment.
    return std::nullopt;
  }

  std::vector<Segment> refined =
      target.segments();

  auto& replacement_variable =
      std::get<RepeatSegment>(
          refined[*variable_segment]);

  if (count == replacement_variable.lower) {
    ++replacement_variable.lower;
  } else {
    --replacement_variable.upper;
  }

  Domain replacement(
      std::move(refined),
      target.min_length(),
      target.max_length());

  if (replacement.failed()) {
    target.fail();
    return Change::failed;
  }

  return replace_domain(
      target,
      replacement);
}


void append_segments(
    std::vector<Segment>& destination,
    const Domain& source) {
  destination.insert(
      destination.end(),
      source.segments().begin(),
      source.segments().end());
}


Domain assigned_prefix_interval_projection(
    const Domain& assigned,
    Length lower,
    Length upper) {
  if (!assigned.assigned() ||
      lower > upper ||
      upper > assigned.min_length()) {
    throw std::logic_error(
        "invalid assigned prefix interval");
  }

  std::vector<Segment> segments;

  const Domain mandatory =
      assigned.assigned_prefix(lower);

  segments.reserve(
      mandatory.segment_count() +
      static_cast<std::size_t>(
          upper - lower));

  append_segments(
      segments,
      mandatory);

  for (Length position = lower;
       position < upper;
       ++position) {
    segments.push_back(
        RepeatSegment{
            ValueSet(
                assigned_value_at(
                    assigned,
                    position)),
            0,
            1});
  }

  return Domain(
      std::move(segments),
      lower,
      upper);
}


Domain assigned_suffix_interval_projection(
    const Domain& assigned,
    Length split_lower,
    Length split_upper) {
  if (!assigned.assigned() ||
      split_lower > split_upper ||
      split_upper > assigned.min_length()) {
    throw std::logic_error(
        "invalid assigned suffix interval");
  }

  const Length total =
      assigned.min_length();

  std::vector<Segment> segments;

  const Domain mandatory =
      assigned.assigned_suffix(
          static_cast<Length>(
              total - split_upper));

  segments.reserve(
      mandatory.segment_count() +
      static_cast<std::size_t>(
          split_upper - split_lower));

  // A suffix can begin anywhere in the uncertain split window.
  // Optional singleton blocks preserve value order. For windows wider
  // than one position this is a sound dashed over-approximation.
  for (Length position = split_lower;
       position < split_upper;
       ++position) {
    segments.push_back(
        RepeatSegment{
            ValueSet(
                assigned_value_at(
                    assigned,
                    position)),
            0,
            1});
  }

  append_segments(
      segments,
      mandatory);

  return Domain(
      std::move(segments),
      static_cast<Length>(
          total - split_upper),
      static_cast<Length>(
          total - split_lower));
}


Length subtract_length_bound(
    Length bound,
    Length amount) {
  if (bound == kUnboundedLength) {
    return kUnboundedLength;
  }

  return static_cast<Length>(
      bound - amount);
}


bool assigned_concat_split_bounds(
    const Domain& result,
    const Domain& left,
    const Domain& right,
    Length& feasible_lower,
    Length& feasible_upper) {
  if (!result.assigned()) {
    throw std::logic_error(
        "assigned concat split filtering requires an assigned result");
  }

  const Length total =
      result.min_length();

  const Length candidate_lower =
      std::max(
          left.min_length(),
          total > right.max_length()
              ? static_cast<Length>(
                    total - right.max_length())
              : Length{0});

  const Length candidate_upper =
      std::min(
          left.max_length(),
          total >= right.min_length()
              ? static_cast<Length>(
                    total - right.min_length())
              : Length{0});

  if (candidate_lower >
      candidate_upper) {
    return false;
  }

  bool found = false;

  for (Length split = candidate_lower;
       split <= candidate_upper;
       ++split) {
    const Domain prefix =
        result.assigned_prefix(split);

    const Domain suffix =
        result.assigned_suffix(
            static_cast<Length>(
                total - split));

    if (left.accepts(prefix) &&
        right.accepts(suffix)) {
      if (!found) {
        feasible_lower = split;
      }

      feasible_upper = split;
      found = true;
    }

    if (split == candidate_upper) {
      break;
    }
  }

  return found;
}


enum class ExactBoundaryStatus {
  unsupported,
  mismatch,
  matched,
};


ExactBoundaryStatus strip_exact_prefix(
    const Domain& whole,
    const Domain& prefix,
    Domain& remainder) {
  if (!prefix.assigned()) {
    return ExactBoundaryStatus::unsupported;
  }

  const std::vector<int> expected =
      prefix.value();

  const Length prefix_length =
      static_cast<Length>(
          expected.size());

  if (whole.min_length() < prefix_length ||
      whole.max_length() < prefix_length) {
    return ExactBoundaryStatus::mismatch;
  }

  const auto finish_remainder =
      [&](std::vector<Segment> segments) {
        remainder = Domain(
            std::move(segments),
            static_cast<Length>(
                whole.min_length() -
                prefix_length),
            subtract_length_bound(
                whole.max_length(),
                prefix_length));

        return ExactBoundaryStatus::matched;
      };

  std::size_t consumed = 0;
  std::size_t segment_index = 0;

  while (consumed < expected.size()) {
    if (segment_index >=
        whole.segments().size()) {
      return ExactBoundaryStatus::mismatch;
    }

    const Segment& segment =
        whole.segments()[segment_index];

    if (const auto* literal =
            std::get_if<LiteralSegment>(
                &segment)) {
      const std::size_t count =
          literal->literal.size();

      const std::size_t take =
          std::min(
              count,
              expected.size() - consumed);

      for (std::size_t offset = 0;
           offset < take;
           ++offset) {
        if (literal->literal[offset] !=
            expected[consumed + offset]) {
          return ExactBoundaryStatus::mismatch;
        }
      }

      consumed += take;

      if (take < count) {
        std::vector<Segment> segments;

        segments.reserve(
            whole.segments().size() -
            segment_index);

        segments.push_back(
            LiteralSegment{
                literal->literal.slice(
                    take,
                    count - take)});

        segments.insert(
            segments.end(),
            whole.segments().begin() +
                static_cast<std::ptrdiff_t>(
                    segment_index + 1),
            whole.segments().end());

        return finish_remainder(
            std::move(segments));
      }

      ++segment_index;
      continue;
    }

    const auto& repeat =
        std::get<RepeatSegment>(
            segment);

    const std::size_t remaining =
        expected.size() - consumed;

    if (repeat.exact_count()) {
      const std::size_t count =
          static_cast<std::size_t>(
              repeat.upper);

      const std::size_t take =
          std::min(
              count,
              remaining);

      for (std::size_t offset = 0;
           offset < take;
           ++offset) {
        if (!repeat.values.contains(
                expected[
                    consumed + offset])) {
          return ExactBoundaryStatus::mismatch;
        }
      }

      consumed += take;

      if (take < count) {
        const Length remaining_count =
            static_cast<Length>(
                count - take);

        std::vector<Segment> segments;

        segments.reserve(
            whole.segments().size() -
            segment_index);

        segments.push_back(
            RepeatSegment{
                repeat.values,
                remaining_count,
                remaining_count});

        segments.insert(
            segments.end(),
            whole.segments().begin() +
                static_cast<std::ptrdiff_t>(
                    segment_index + 1),
            whole.segments().end());

        return finish_remainder(
            std::move(segments));
      }

      ++segment_index;
      continue;
    }

    // Only the lower-bound portion is guaranteed to belong to this
    // repeat block. Crossing into its optional portion would make the
    // structural boundary ambiguous.
    const std::size_t mandatory =
        static_cast<std::size_t>(
            repeat.lower);

    const std::size_t take =
        std::min(
            mandatory,
            remaining);

    for (std::size_t offset = 0;
         offset < take;
         ++offset) {
      if (!repeat.values.contains(
              expected[
                  consumed + offset])) {
        return ExactBoundaryStatus::mismatch;
      }
    }

    consumed += take;

    if (remaining <= mandatory) {
      const Length consumed_count =
          static_cast<Length>(take);

      const Length remaining_lower =
          static_cast<Length>(
              repeat.lower -
              consumed_count);

      const Length remaining_upper =
          subtract_length_bound(
              repeat.upper,
              consumed_count);

      std::vector<Segment> segments;

      segments.reserve(
          whole.segments().size() -
          segment_index);

      if (remaining_upper != 0) {
        segments.push_back(
            RepeatSegment{
                repeat.values,
                remaining_lower,
                remaining_upper});
      }

      segments.insert(
          segments.end(),
          whole.segments().begin() +
              static_cast<std::ptrdiff_t>(
                  segment_index + 1),
          whole.segments().end());

      return finish_remainder(
          std::move(segments));
    }

    return ExactBoundaryStatus::unsupported;
  }

  std::vector<Segment> segments(
      whole.segments().begin() +
          static_cast<std::ptrdiff_t>(
              segment_index),
      whole.segments().end());

  return finish_remainder(
      std::move(segments));
}


ExactBoundaryStatus strip_exact_suffix(
    const Domain& whole,
    const Domain& suffix,
    Domain& remainder) {
  if (!suffix.assigned()) {
    return ExactBoundaryStatus::unsupported;
  }

  const std::vector<int> expected =
      suffix.value();

  const Length suffix_length =
      static_cast<Length>(
          expected.size());

  if (whole.min_length() < suffix_length ||
      whole.max_length() < suffix_length) {
    return ExactBoundaryStatus::mismatch;
  }

  const auto finish_remainder =
      [&](std::vector<Segment> segments) {
        remainder = Domain(
            std::move(segments),
            static_cast<Length>(
                whole.min_length() -
                suffix_length),
            subtract_length_bound(
                whole.max_length(),
                suffix_length));

        return ExactBoundaryStatus::matched;
      };

  std::size_t consumed = 0;
  std::size_t segment_index =
      whole.segments().size();

  while (consumed < expected.size()) {
    if (segment_index == 0) {
      return ExactBoundaryStatus::mismatch;
    }

    const Segment& segment =
        whole.segments()[
            segment_index - 1];

    if (const auto* literal =
            std::get_if<LiteralSegment>(
                &segment)) {
      const std::size_t count =
          literal->literal.size();

      const std::size_t take =
          std::min(
              count,
              expected.size() - consumed);

      const std::size_t literal_start =
          count - take;

      const std::size_t expected_start =
          expected.size() -
          consumed -
          take;

      for (std::size_t offset = 0;
           offset < take;
           ++offset) {
        if (literal->literal[
                literal_start + offset] !=
            expected[
                expected_start + offset]) {
          return ExactBoundaryStatus::mismatch;
        }
      }

      consumed += take;

      if (take < count) {
        std::vector<Segment> segments;

        segments.reserve(
            segment_index);

        segments.insert(
            segments.end(),
            whole.segments().begin(),
            whole.segments().begin() +
                static_cast<std::ptrdiff_t>(
                    segment_index - 1));

        segments.push_back(
            LiteralSegment{
                literal->literal.slice(
                    0,
                    count - take)});

        return finish_remainder(
            std::move(segments));
      }

      --segment_index;
      continue;
    }

    const auto& repeat =
        std::get<RepeatSegment>(
            segment);

    const std::size_t remaining =
        expected.size() - consumed;

    if (repeat.exact_count()) {
      const std::size_t count =
          static_cast<std::size_t>(
              repeat.upper);

      const std::size_t take =
          std::min(
              count,
              remaining);

      const std::size_t expected_start =
          expected.size() -
          consumed -
          take;

      for (std::size_t offset = 0;
           offset < take;
           ++offset) {
        if (!repeat.values.contains(
                expected[
                    expected_start +
                    offset])) {
          return ExactBoundaryStatus::mismatch;
        }
      }

      consumed += take;

      if (take < count) {
        const Length remaining_count =
            static_cast<Length>(
                count - take);

        std::vector<Segment> segments;

        segments.reserve(
            segment_index);

        segments.insert(
            segments.end(),
            whole.segments().begin(),
            whole.segments().begin() +
                static_cast<std::ptrdiff_t>(
                    segment_index - 1));

        segments.push_back(
            RepeatSegment{
                repeat.values,
                remaining_count,
                remaining_count});

        return finish_remainder(
            std::move(segments));
      }

      --segment_index;
      continue;
    }

    // Only the lower-bound portion is guaranteed to belong to this
    // repeat block. Crossing into its optional portion would make the
    // structural boundary ambiguous.
    const std::size_t mandatory =
        static_cast<std::size_t>(
            repeat.lower);

    const std::size_t take =
        std::min(
            mandatory,
            remaining);

    const std::size_t expected_start =
        expected.size() -
        consumed -
        take;

    for (std::size_t offset = 0;
         offset < take;
         ++offset) {
      if (!repeat.values.contains(
              expected[
                  expected_start +
                  offset])) {
        return ExactBoundaryStatus::mismatch;
      }
    }

    consumed += take;

    if (remaining <= mandatory) {
      const Length consumed_count =
          static_cast<Length>(take);

      const Length remaining_lower =
          static_cast<Length>(
              repeat.lower -
              consumed_count);

      const Length remaining_upper =
          subtract_length_bound(
              repeat.upper,
              consumed_count);

      std::vector<Segment> segments;

      segments.reserve(
          segment_index);

      segments.insert(
          segments.end(),
          whole.segments().begin(),
          whole.segments().begin() +
              static_cast<std::ptrdiff_t>(
                  segment_index - 1));

      if (remaining_upper != 0) {
        segments.push_back(
            RepeatSegment{
                repeat.values,
                remaining_lower,
                remaining_upper});
      }

      return finish_remainder(
          std::move(segments));
    }

    return ExactBoundaryStatus::unsupported;
  }

  std::vector<Segment> segments(
      whole.segments().begin(),
      whole.segments().begin() +
          static_cast<std::ptrdiff_t>(
              segment_index));

  return finish_remainder(
      std::move(segments));
}


enum class AnchoredRepeatStatus {
  unsupported,
  infeasible,
  feasible,
};


AnchoredRepeatStatus intersect_anchored_repeat(
    const Domain& lhs,
    const Domain& rhs,
    Domain& refined) {
  if (lhs.segment_count() != 2 ||
      rhs.segment_count() != 2) {
    return AnchoredRepeatStatus::unsupported;
  }

  std::size_t repeat_index = 2;
  std::size_t literal_index = 2;

  for (std::size_t index = 0;
       index < 2;
       ++index) {
    const Segment& left =
        lhs.segments()[index];

    const Segment& right =
        rhs.segments()[index];

    if (std::holds_alternative<RepeatSegment>(left) &&
        std::holds_alternative<RepeatSegment>(right)) {
      if (repeat_index != 2) {
        return AnchoredRepeatStatus::unsupported;
      }

      repeat_index = index;
      continue;
    }

    if (std::holds_alternative<LiteralSegment>(left) &&
        std::holds_alternative<LiteralSegment>(right)) {
      if (literal_index != 2) {
        return AnchoredRepeatStatus::unsupported;
      }

      const auto& left_literal =
          std::get<LiteralSegment>(
              left).literal;

      const auto& right_literal =
          std::get<LiteralSegment>(
              right).literal;

      if (!(left_literal == right_literal)) {
        return AnchoredRepeatStatus::unsupported;
      }

      literal_index = index;
      continue;
    }

    return AnchoredRepeatStatus::unsupported;
  }

  if (repeat_index == 2 ||
      literal_index == 2) {
    return AnchoredRepeatStatus::unsupported;
  }

  const auto& left_repeat =
      std::get<RepeatSegment>(
          lhs.segments()[repeat_index]);

  const auto& right_repeat =
      std::get<RepeatSegment>(
          rhs.segments()[repeat_index]);

  const Length lower =
      std::max(
          left_repeat.lower,
          right_repeat.lower);

  Length upper =
      std::min(
          left_repeat.upper,
          right_repeat.upper);

  if (lower > upper) {
    return AnchoredRepeatStatus::infeasible;
  }

  ValueSet values =
      left_repeat.values.intersected(
          right_repeat.values);

  if (values.empty()) {
    if (lower != 0) {
      return AnchoredRepeatStatus::infeasible;
    }

    upper = 0;
  }

  std::vector<Segment> segments =
      lhs.segments();

  if (upper == 0) {
    segments.erase(
        segments.begin() +
        static_cast<std::ptrdiff_t>(
            repeat_index));
  } else {
    segments[repeat_index] =
        RepeatSegment{
            std::move(values),
            lower,
            upper};
  }

  const Length domain_lower =
      std::max(
          lhs.min_length(),
          rhs.min_length());

  const Length domain_upper =
      std::min(
          lhs.max_length(),
          rhs.max_length());

  if (domain_lower > domain_upper) {
    return AnchoredRepeatStatus::infeasible;
  }

  Domain candidate(
      std::move(segments),
      domain_lower,
      domain_upper);

  if (candidate.failed()) {
    return AnchoredRepeatStatus::infeasible;
  }

  refined = std::move(candidate);
  return AnchoredRepeatStatus::feasible;
}


Change project_boundary_remainder(
    Domain& target,
    const Domain& remainder) {
  if (remainder.assigned()) {
    return assign_exact(
        target,
        remainder);
  }

  Domain refined_target = target;
  Domain remainder_probe = remainder;

  const PropagationResult projection =
      propagate_equal(
          refined_target,
          remainder_probe);

  if (projection.failed()) {
    target.fail();
    return Change::failed;
  }

  Domain anchored_refinement;

  switch (intersect_anchored_repeat(
      refined_target,
      remainder_probe,
      anchored_refinement)) {
    case AnchoredRepeatStatus::infeasible:
      target.fail();
      return Change::failed;

    case AnchoredRepeatStatus::feasible:
      return replace_domain(
          target,
          anchored_refinement);

    case AnchoredRepeatStatus::unsupported:
      return replace_domain(
          target,
          refined_target);
  }

  return Change::none;
}

}  // namespace

bool BoolDomain::value() const {
  if (!assigned()) {
    throw std::logic_error("value requested from unassigned BoolDomain");
  }
  return may_be_true;
}

Change BoolDomain::force(bool forced_value) noexcept {
  if (failed()) {
    return Change::failed;
  }
  if (forced_value) {
    if (!may_be_true) {
      may_be_false = false;
      return Change::failed;
    }
    if (!may_be_false) {
      return Change::none;
    }
    may_be_false = false;
  } else {
    if (!may_be_false) {
      may_be_true = false;
      return Change::failed;
    }
    if (!may_be_true) {
      return Change::none;
    }
    may_be_true = false;
  }
  return Change::assigned;
}

Change project_sweep(
    Domain& subject,
    const Domain& target) {
  Domain refined = subject;

  detail::SweepStatus status =
      detail::project_against_exact_target(
          subject,
          target,
          refined);

  if (status ==
      detail::SweepStatus::unsupported) {
    refined = subject;

    status =
        detail::project_repeat_regions(
            subject,
            target,
            refined);
  }

  if (status ==
      detail::SweepStatus::unsupported) {
    refined = subject;

    status =
        detail::project_repeat_values(
            subject,
            target,
            refined);
  }

  switch (status) {
    case detail::SweepStatus::feasible:
      return replace_domain(
          subject,
          refined);

    case detail::SweepStatus::infeasible:
      subject.fail();
      return Change::failed;

    case detail::SweepStatus::unsupported:
      return Change::none;
  }

  return Change::none;
}


PropagationResult propagate_equal(Domain& x, Domain& y) {
  PropagationResult result;
  if (x.failed() || y.failed()) {
    x.fail();
    y.fail();
    result.left = result.right = Change::failed;
    return result;
  }

  const Length lower = std::max(x.min_length(), y.min_length());
  const Length upper = std::min(x.max_length(), y.max_length());
  result.left = combine(result.left, x.tighten_length(lower, upper));
  result.right = combine(result.right, y.tighten_length(lower, upper));
  if (result.failed()) {
    return result;
  }

  if (x.assigned()) {
    result.right = combine(result.right, assign_exact(y, x));
  }
  if (!result.failed() && y.assigned()) {
    result.left = combine(result.left, assign_exact(x, y));
  }
  if (result.failed()) {
    return result;
  }

  if (!x.assigned() && !y.assigned()) {
    if (single_repeat_domain(x)) {
      result.left = combine(
          result.left,
          x.intersect_single_repeat(y));

      if (!result.failed()) {
        // x now represents the exact common language.
        result.right = combine(
            result.right,
            replace_domain(y, x));
      }
    } else if (single_repeat_domain(y)) {
      result.right = combine(
          result.right,
          y.intersect_single_repeat(x));

      if (!result.failed()) {
        // y now represents the exact common language.
        result.left = combine(
            result.left,
            replace_domain(x, y));
      }
    } else {
      // First try the segmented sweep when either operand provides an
      // exact target layout. Unsupported cases deliberately fall through
      // to the existing same-shape intersection.
      result.left = combine(
          result.left,
          project_sweep(x, y));

      if (!result.failed()) {
        result.right = combine(
            result.right,
            project_sweep(y, x));
      }

      if (!result.failed()) {
        result.left = combine(
            result.left,
            x.intersect_same_shape(y));
      }

      if (!result.failed()) {
        result.right = combine(
            result.right,
            y.intersect_same_shape(x));
      }
    }
  }

  result.subsumed = !result.failed() && same_value(x, y);
  return result;
}

static bool equality_may_be_possible(
    const Domain& x,
    const Domain& y) {
  // Preserve the inexpensive checks for failed domains, incompatible
  // lengths, and assigned-value membership.
  if (!x.may_equal(y)) {
    return false;
  }

  // Run the equality propagator transactionally on copies. A detected
  // failure proves that the two represented languages are disjoint.
  //
  // A non-failure is conservative: propagation may not prove every
  // impossible intersection, so this function never claims more than the
  // current equality algorithm establishes.
  Domain probe_x = x;
  Domain probe_y = y;

  const PropagationResult probe =
      propagate_equal(probe_x, probe_y);

  return !probe.failed();
}


PropagationResult propagate_not_equal(Domain& x, Domain& y) {
  PropagationResult result;
  if (x.failed() || y.failed()) {
    result.left = result.right = Change::failed;
    return result;
  }
  if (!equality_may_be_possible(x, y)) {
    result.subsumed = true;
    return result;
  }
  if (x.assigned() && y.assigned()) {
    if (x.assigned_equal(y)) {
      x.fail();
      result.left = Change::failed;
    } else {
      result.subsumed = true;
    }
    return result;
  }

  if (x.assigned()) {
    auto pruning =
        exclude_assigned_single_witness(
            y,
            x);

    if (!pruning.has_value()) {
      pruning =
          exclude_assigned_endpoint_count(
              y,
              x);
    }

    if (pruning.has_value()) {
      result.right = *pruning;
      result.subsumed =
          !result.failed();
      return result;
    }
  }

  if (y.assigned()) {
    auto pruning =
        exclude_assigned_single_witness(
            x,
            y);

    if (!pruning.has_value()) {
      pruning =
          exclude_assigned_endpoint_count(
              x,
              y);
    }

    if (pruning.has_value()) {
      result.left = *pruning;
      result.subsumed =
          !result.failed();
      return result;
    }
  }

  return result;
}

PropagationResult propagate_reified_equal(Domain& x, Domain& y,
                                            BoolDomain& b) {
  PropagationResult result;
  if (b.failed()) {
    result.result = Change::failed;
    return result;
  }

  if (!equality_may_be_possible(x, y)) {
    result.result = b.force(false);
    result.subsumed = !result.failed() && b.assigned();
    return result;
  }
  if (x.assigned() && y.assigned()) {
    result.result = b.force(x.assigned_equal(y));
    result.subsumed = !result.failed();
    return result;
  }
  if (b.assigned()) {
    PropagationResult inner = b.value() ? propagate_equal(x, y)
                                        : propagate_not_equal(x, y);
    inner.result = Change::none;
    return inner;
  }
  return result;
}

PropagationResult propagate_reified_not_equal(Domain& x, Domain& y,
                                                BoolDomain& b) {
  BoolDomain equality{b.may_be_true, b.may_be_false};
  PropagationResult result = propagate_reified_equal(x, y, equality);
  b.may_be_false = equality.may_be_true;
  b.may_be_true = equality.may_be_false;
  return result;
}

PropagationResult propagate_implied_equal(
    Domain& x,
    Domain& y,
    BoolDomain& b) {
  PropagationResult result;

  if (x.failed() || y.failed()) {
    result.left = result.right =
        Change::failed;
    return result;
  }

  if (b.failed()) {
    result.result = Change::failed;
    return result;
  }

  if (b.assigned()) {
    if (!b.value()) {
      result.subsumed = true;
      return result;
    }

    result =
        propagate_equal(
            x,
            y);

    result.result = Change::none;
    return result;
  }

  if (!equality_may_be_possible(
          x,
          y)) {
    result.result =
        b.force(false);

    result.subsumed =
        !result.failed() &&
        b.assigned();

    return result;
  }

  if (x.assigned() &&
      y.assigned() &&
      x.assigned_equal(y)) {
    // The consequent is already true, so either Boolean value
    // satisfies the implication.
    result.subsumed = true;
  }

  return result;
}


PropagationResult propagate_equal_implies(
    Domain& x,
    Domain& y,
    BoolDomain& b) {
  PropagationResult result;

  if (x.failed() || y.failed()) {
    result.left = result.right =
        Change::failed;
    return result;
  }

  if (b.failed()) {
    result.result = Change::failed;
    return result;
  }

  if (b.assigned()) {
    if (b.value()) {
      result.subsumed = true;
      return result;
    }

    result =
        propagate_not_equal(
            x,
            y);

    result.result = Change::none;
    return result;
  }

  if (!equality_may_be_possible(
          x,
          y)) {
    // The antecedent is false.
    result.subsumed = true;
    return result;
  }

  if (x.assigned() &&
      y.assigned() &&
      x.assigned_equal(y)) {
    result.result =
        b.force(true);

    result.subsumed =
        !result.failed() &&
        b.assigned();
  }

  return result;
}


PropagationResult propagate_implied_not_equal(
    Domain& x,
    Domain& y,
    BoolDomain& b) {
  PropagationResult result;

  if (x.failed() || y.failed()) {
    result.left = result.right =
        Change::failed;
    return result;
  }

  if (b.failed()) {
    result.result = Change::failed;
    return result;
  }

  if (b.assigned()) {
    if (!b.value()) {
      result.subsumed = true;
      return result;
    }

    result =
        propagate_not_equal(
            x,
            y);

    result.result = Change::none;
    return result;
  }

  if (!equality_may_be_possible(
          x,
          y)) {
    // Disequality is already entailed.
    result.subsumed = true;
    return result;
  }

  if (x.assigned() &&
      y.assigned() &&
      x.assigned_equal(y)) {
    result.result =
        b.force(false);

    result.subsumed =
        !result.failed() &&
        b.assigned();
  }

  return result;
}


PropagationResult propagate_not_equal_implies(
    Domain& x,
    Domain& y,
    BoolDomain& b) {
  PropagationResult result;

  if (x.failed() || y.failed()) {
    result.left = result.right =
        Change::failed;
    return result;
  }

  if (b.failed()) {
    result.result = Change::failed;
    return result;
  }

  if (b.assigned()) {
    if (b.value()) {
      result.subsumed = true;
      return result;
    }

    result =
        propagate_equal(
            x,
            y);

    result.result = Change::none;
    return result;
  }

  if (!equality_may_be_possible(
          x,
          y)) {
    result.result =
        b.force(true);

    result.subsumed =
        !result.failed() &&
        b.assigned();

    return result;
  }

  if (x.assigned() &&
      y.assigned() &&
      x.assigned_equal(y)) {
    // Disequality is false, so the implication is already true.
    result.subsumed = true;
  }

  return result;
}


PropagationResult propagate_concat(Domain& z, Domain& x, Domain& y) {
  PropagationResult result;
  if (z.failed() || x.failed() || y.failed()) {
    result.left = result.right = result.result = Change::failed;
    return result;
  }

  bool again = true;
  while (again && !result.failed()) {
    again = false;
    const Length old_z_min = z.min_length();
    const Length old_z_max = z.max_length();
    const Length old_x_min = x.min_length();
    const Length old_x_max = x.max_length();
    const Length old_y_min = y.min_length();
    const Length old_y_max = y.max_length();

    const std::uint64_t sum_min =
        static_cast<std::uint64_t>(x.min_length()) + y.min_length();
    const std::uint64_t sum_max =
        static_cast<std::uint64_t>(x.max_length()) + y.max_length();
    result.result = combine(
        result.result,
        z.tighten_length(
            sum_min >= kUnboundedLength ? kUnboundedLength
                                        : static_cast<Length>(sum_min),
            sum_max >= kUnboundedLength ? kUnboundedLength
                                        : static_cast<Length>(sum_max)));
    if (result.failed()) {
      break;
    }

    const Length x_lower =
        z.min_length() > y.max_length() ? z.min_length() - y.max_length() : 0;
    const Length x_upper =
        z.max_length() >= y.min_length() ? z.max_length() - y.min_length() : 0;
    result.left = combine(result.left, x.tighten_length(x_lower, x_upper));
    if (result.failed()) {
      break;
    }

    const Length y_lower =
        z.min_length() > x.max_length() ? z.min_length() - x.max_length() : 0;
    const Length y_upper =
        z.max_length() >= x.min_length() ? z.max_length() - x.min_length() : 0;
    result.right = combine(result.right, y.tighten_length(y_lower, y_upper));

    again = old_z_min != z.min_length() || old_z_max != z.max_length() ||
            old_x_min != x.min_length() || old_x_max != x.max_length() ||
            old_y_min != y.min_length() || old_y_max != y.max_length();
  }
  if (result.failed()) {
    return result;
  }

  if (z.assigned()) {
    // Exact feasible split points for an assigned result.
    //
    // Prefix and suffix membership must hold at the same split. Checking
    // the operands independently can retain incompatible split choices.
    Length feasible_lower = 0;
    Length feasible_upper = 0;

    if (!assigned_concat_split_bounds(
            z,
            x,
            y,
            feasible_lower,
            feasible_upper)) {
      // When both operand lengths are exact, the exact-split rule below
      // assigns the concrete prefix and suffix and reports failure on
      // the incompatible operand.
      if (x.min_length() != x.max_length() ||
          y.min_length() != y.max_length()) {
        z.fail();
        result.result = Change::failed;
        return result;
      }
    } else {
      result.left = combine(
          result.left,
          x.tighten_length(
              feasible_lower,
              feasible_upper));

      const Length total =
          z.min_length();

      result.right = combine(
          result.right,
          y.tighten_length(
              static_cast<Length>(
                  total - feasible_upper),
              static_cast<Length>(
                  total - feasible_lower)));

      if (result.failed()) {
        return result;
      }
    }
  }

  if (z.assigned() &&
      x.min_length() == x.max_length() &&
      y.min_length() == y.max_length()) {
    // Exact operand lengths determine a unique split point.
    const std::uint64_t total_length =
        static_cast<std::uint64_t>(
            x.min_length()) +
        static_cast<std::uint64_t>(
            y.min_length());

    if (total_length != z.min_length()) {
      // The preceding length fixpoint should normally detect this, but
      // retain a defensive consistency check at the structural step.
      z.fail();
      result.result = Change::failed;
      return result;
    }

    const Domain prefix =
        z.assigned_prefix(
            x.min_length());

    const Domain suffix =
        z.assigned_suffix(
            y.min_length());

    // Validate both operands before replacing either one. This keeps the
    // successful split atomic from the caller's perspective.
    const bool prefix_accepted =
        x.accepts(prefix);

    const bool suffix_accepted =
        y.accepts(suffix);

    if (!prefix_accepted ||
        !suffix_accepted) {
      if (!prefix_accepted) {
        x.fail();
        result.left = Change::failed;
      }

      if (!suffix_accepted) {
        y.fail();
        result.right = Change::failed;
      }

      return result;
    }

    result.left = combine(
        result.left,
        assign_exact(x, prefix));

    result.right = combine(
        result.right,
        assign_exact(y, suffix));
  }

  if (result.failed()) {
    return result;
  }


  if (z.assigned()) {
    // A range of operand lengths determines an interval of split points.
    //
    // For every feasible split s:
    //
    //   x = z[0..s)
    //   y = z[s..|z|)
    //
    // The certain portions become exact prefix/suffix segments and values
    // around the movable boundary become optional singleton segments.
    const Length total =
        z.min_length();

    const Length split_lower =
        std::max(
            x.min_length(),
            total > y.max_length()
                ? static_cast<Length>(
                      total - y.max_length())
                : Length{0});

    const Length split_upper =
        std::min(
            x.max_length(),
            total >= y.min_length()
                ? static_cast<Length>(
                      total - y.min_length())
                : Length{0});

    if (split_lower > split_upper) {
      z.fail();
      result.result = Change::failed;
      return result;
    }

    if (split_lower < split_upper) {
      const Domain prefix_projection =
          assigned_prefix_interval_projection(
              z,
              split_lower,
              split_upper);

      const Domain suffix_projection =
          assigned_suffix_interval_projection(
              z,
              split_lower,
              split_upper);

      // Refine copies first. Neither real operand changes unless both
      // directional intersections are consistent.
      Domain refined_x = x;
      Domain refined_y = y;

      Domain prefix_probe =
          prefix_projection;

      Domain suffix_probe =
          suffix_projection;

      const PropagationResult x_probe =
          propagate_equal(
              refined_x,
              prefix_probe);

      const PropagationResult y_probe =
          propagate_equal(
              refined_y,
              suffix_probe);

      if (x_probe.failed() ||
          y_probe.failed()) {
        if (x_probe.failed()) {
          x.fail();
          result.left = Change::failed;
        }

        if (y_probe.failed()) {
          y.fail();
          result.right = Change::failed;
        }

        return result;
      }

      result.left = combine(
          result.left,
          replace_domain(
              x,
              refined_x));

      result.right = combine(
          result.right,
          replace_domain(
              y,
              refined_y));
    }
  }

  if (result.failed()) {
    return result;
  }

  {
    // Concatenating the operand domains gives a forward approximation
    // of the result language.
    const Domain concatenated =
        x.concatenated(y);

    // This conservative test is safe to use as proof of failure.
    if (!z.may_equal(concatenated)) {
      z.fail();
      result.result = Change::failed;
      return result;
    }

    // General segmented equality propagation may fail conservatively.
    // Restrict structural forward projection to the exact single-repeat
    // intersection path.
    if (z.segment_count() == 1 &&
        std::holds_alternative<RepeatSegment>(
            z.segments().front())) {
      Domain refined_z = z;
      Domain concatenated_probe =
          concatenated;

      const PropagationResult forward =
          propagate_equal(
              refined_z,
              concatenated_probe);

      // This restricted single-repeat intersection is exact, so failure
      // proves that the concat result cannot match z.
      if (forward.failed()) {
        z.fail();
        result.result = Change::failed;
        return result;
      }

      result.result = combine(
          result.result,
          replace_domain(
              z,
              refined_z));
    }
  }

  if (result.failed()) {
    return result;
  }

  if (!z.assigned() &&
      x.assigned() &&
      !y.assigned()) {
    // An assigned left operand can consume an exact structural prefix.
    Domain remainder;

    const ExactBoundaryStatus status =
        strip_exact_prefix(
            z,
            x,
            remainder);

    if (status ==
        ExactBoundaryStatus::mismatch) {
      z.fail();
      result.result = Change::failed;
      return result;
    }

    if (status ==
        ExactBoundaryStatus::matched) {
      result.right = combine(
          result.right,
          project_boundary_remainder(
              y,
              remainder));

      if (result.failed()) {
        return result;
      }
    }
  }

  if (!z.assigned() &&
      y.assigned() &&
      !x.assigned()) {
    // An assigned right operand can consume an exact structural suffix.
    Domain remainder;

    const ExactBoundaryStatus status =
        strip_exact_suffix(
            z,
            y,
            remainder);

    if (status ==
        ExactBoundaryStatus::mismatch) {
      z.fail();
      result.result = Change::failed;
      return result;
    }

    if (status ==
        ExactBoundaryStatus::matched) {
      result.left = combine(
          result.left,
          project_boundary_remainder(
              x,
              remainder));

      if (result.failed()) {
        return result;
      }
    }
  }

  if (x.assigned() && y.assigned()) {
    Domain exact = x.concatenated(y);
    result.result = combine(result.result, assign_exact(z, exact));
  }
  if (result.failed()) {
    return result;
  }

  if (z.assigned() && x.assigned()) {
    if (x.min_length() > z.min_length() ||
        z.assigned_prefix(x.min_length()).value() != x.value()) {
      z.fail();
      result.result = Change::failed;
      return result;
    }
    Domain suffix = z.assigned_suffix(z.min_length() - x.min_length());
    result.right = combine(result.right, assign_exact(y, suffix));
  }
  if (result.failed()) {
    return result;
  }

  if (z.assigned() && y.assigned()) {
    if (y.min_length() > z.min_length() ||
        z.assigned_suffix(y.min_length()).value() != y.value()) {
      z.fail();
      result.result = Change::failed;
      return result;
    }
    Domain prefix = z.assigned_prefix(z.min_length() - y.min_length());
    result.left = combine(result.left, assign_exact(x, prefix));
  }

  result.subsumed = !result.failed() && z.assigned() && x.assigned() &&
                    y.assigned() &&
                    z.assigned_equal(x.concatenated(y));
  return result;
}

PropagationResult propagate_length(Domain& x, IntBounds& length) {
  PropagationResult result;
  if (x.failed() || length.failed || length.lower > length.upper) {
    x.fail();
    length.failed = true;
    result.left = result.result = Change::failed;
    return result;
  }

  const std::int64_t old_lower = length.lower;
  const std::int64_t old_upper = length.upper;
  length.lower = std::max<std::int64_t>(length.lower, x.min_length());
  length.upper = std::min<std::int64_t>(length.upper, x.max_length());
  if (length.lower > length.upper) {
    x.fail();
    length.failed = true;
    result.left = result.result = Change::failed;
    return result;
  }
  if (length.lower != old_lower || length.upper != old_upper) {
    result.result = length.assigned() ? Change::assigned : Change::length;
  }

  result.left = x.tighten_length(clamp_length(length.lower),
                                 clamp_length(length.upper));
  if (result.left == Change::failed) {
    length.failed = true;
    result.result = Change::failed;
    return result;
  }
  result.subsumed = !x.failed() && length.assigned() &&
                    x.min_length() == x.max_length() &&
                    static_cast<std::int64_t>(x.min_length()) == length.lower;
  return result;
}

}  // namespace dashed
