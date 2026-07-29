#include "dashed/detail/sweep.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <variant>
#include <vector>

namespace dashed::detail {
namespace {

class RepeatBlocks {
 public:
  [[nodiscard]] bool initialize(
      const Domain& domain) {
    blocks_.clear();
    blocks_.reserve(domain.segment_count());

    for (const Segment& segment :
         domain.segments()) {
      const auto* repeat =
          std::get_if<RepeatSegment>(&segment);

      if (repeat == nullptr) {
        blocks_.clear();
        return false;
      }

      blocks_.push_back(repeat);
    }

    return true;
  }

  [[nodiscard]] std::size_t size() const noexcept {
    return blocks_.size();
  }

  [[nodiscard]] const RepeatSegment& at(
      std::ptrdiff_t index) const {
    assert(index >= 0);
    assert(
        static_cast<std::size_t>(index) <
        blocks_.size());

    return *blocks_[
        static_cast<std::size_t>(index)];
  }

  [[nodiscard]] const RepeatSegment& front()
      const {
    return *blocks_.front();
  }

  [[nodiscard]] const RepeatSegment& back()
      const {
    return *blocks_.back();
  }

 private:
  std::vector<const RepeatSegment*> blocks_;
};


struct Forward {
  [[nodiscard]] static bool less(
      const SweepPosition& lhs,
      const SweepPosition& rhs) noexcept {
    if (lhs.segment == rhs.segment) {
      return lhs.offset < rhs.offset;
    }

    return lhs.segment < rhs.segment;
  }

  [[nodiscard]] static std::ptrdiff_t next(
      std::ptrdiff_t index) noexcept {
    return index + 1;
  }

  [[nodiscard]] static constexpr bool forward()
      noexcept {
    return true;
  }
};


struct Backward {
  [[nodiscard]] static bool less(
      const SweepPosition& lhs,
      const SweepPosition& rhs) noexcept {
    if (lhs.segment == rhs.segment) {
      return lhs.offset < rhs.offset;
    }

    return lhs.segment > rhs.segment;
  }

  [[nodiscard]] static std::ptrdiff_t next(
      std::ptrdiff_t index) noexcept {
    return index - 1;
  }

  [[nodiscard]] static constexpr bool forward()
      noexcept {
    return false;
  }
};


[[nodiscard]] bool compatible(
    const RepeatSegment& lhs,
    const RepeatSegment& rhs) noexcept {
  return !lhs.values.disjoint(rhs.values);
}


template<class Direction>
[[nodiscard]] bool advance(
    SweepPosition& position,
    const SweepPosition& limit) {
  position.segment =
      Direction::next(position.segment);
  position.offset = 0;

  if (!Direction::less(position, limit)) {
    position = limit;
    return false;
  }

  return true;
}


[[nodiscard]] SweepPosition dual(
    const RepeatBlocks& blocks,
    const SweepPosition& position) {
  assert(position.segment >= 0);
  const RepeatSegment& block =
      blocks.at(position.segment);

  assert(position.offset <= block.upper);

  return SweepPosition{
      position.segment,
      static_cast<Length>(
          block.upper - position.offset)};
}


template<class Direction>
[[nodiscard]] SweepPosition push_minimum(
    const RepeatBlocks& target,
    const RepeatSegment& subject,
    SweepPosition& cursor,
    const SweepPosition& limit) {
  SweepPosition candidate = cursor;
  std::uint64_t remaining = subject.lower;

  while (remaining > 0) {
    if (!Direction::less(cursor, limit)) {
      return limit;
    }

    const RepeatSegment& target_block =
        target.at(cursor.segment);

    if (!compatible(subject, target_block)) {
      const Length mandatory =
          target_block.lower;

      if (!advance<Direction>(
              cursor,
              limit)) {
        return limit;
      }

      // A mandatory incompatible block cannot occur inside the match.
      // Restart the candidate immediately after that block.
      if (cursor.offset < mandatory) {
        remaining = subject.lower;
        candidate = cursor;
      }

      continue;
    }

    const std::uint64_t capacity =
        static_cast<std::uint64_t>(
            target_block.upper -
            cursor.offset);

    if (remaining > capacity) {
      remaining -= capacity;

      if (!advance<Direction>(
              cursor,
              limit)) {
        return limit;
      }
    } else {
      cursor.offset =
          static_cast<Length>(
              cursor.offset + remaining);
      remaining = 0;
    }
  }

  if (Direction::less(candidate, limit)) {
    const RepeatSegment& candidate_block =
        target.at(candidate.segment);

    if (candidate.offset ==
        candidate_block.upper) {
      (void) advance<Direction>(
          candidate,
          limit);
    }
  }

  return candidate;
}


template<class Direction>
[[nodiscard]] SweepPosition stretch_maximum(
    const RepeatBlocks& target,
    const RepeatSegment& subject,
    SweepPosition cursor,
    const SweepPosition& limit) {
  std::uint64_t remaining = subject.upper;

  while (Direction::less(cursor, limit)) {
    const RepeatSegment& target_block =
        target.at(cursor.segment);

    std::int64_t mandatory = 0;

    if constexpr (Direction::forward()) {
      mandatory =
          static_cast<std::int64_t>(
              target_block.lower) -
          static_cast<std::int64_t>(
              cursor.offset);
    } else {
      const Length optional =
          static_cast<Length>(
              target_block.upper -
              target_block.lower);

      if (cursor.offset < optional) {
        cursor.offset = optional;
        mandatory =
            static_cast<std::int64_t>(
                target_block.lower);
      } else {
        mandatory =
            static_cast<std::int64_t>(
                target_block.upper) -
            static_cast<std::int64_t>(
                cursor.offset);
      }
    }

    if (mandatory <= 0) {
      if (!advance<Direction>(
              cursor,
              limit)) {
        return limit;
      }

      continue;
    }

    if (!compatible(subject, target_block)) {
      return cursor;
    }

    const auto required =
        static_cast<std::uint64_t>(mandatory);

    if (remaining < required) {
      cursor.offset =
          static_cast<Length>(
              cursor.offset + remaining);
      return cursor;
    }

    remaining -= required;

    if (!advance<Direction>(
            cursor,
            limit)) {
      return limit;
    }
  }

  return cursor;
}


[[nodiscard]] SweepStatus initialize_bounds(
    const RepeatBlocks& subject,
    const RepeatBlocks& target,
    std::vector<SweepPosition>& earliest_start,
    std::vector<SweepPosition>& latest_end) {
  const std::size_t subject_size =
      subject.size();
  const std::size_t target_size =
      target.size();

  if (subject_size == 0 ||
      target_size == 0) {
    return SweepStatus::unsupported;
  }

  const SweepPosition first_forward{0, 0};

  const SweepPosition first_backward{
      static_cast<std::ptrdiff_t>(
          target_size - 1),
      0};

  const SweepPosition last_forward{
      static_cast<std::ptrdiff_t>(
          target_size - 1),
      target.back().upper};

  const SweepPosition last_backward{
      0,
      target.front().upper};

  earliest_start.assign(
      subject_size,
      first_forward);

  latest_end.assign(
      subject_size,
      first_backward);

  SweepPosition cursor = first_forward;

  for (std::size_t i = 0;
       i < subject_size;
       ++i) {
    cursor = stretch_maximum<Forward>(
        target,
        subject.at(
            static_cast<std::ptrdiff_t>(i)),
        cursor,
        last_forward);

    latest_end[i] =
        dual(target, cursor);
  }

  if (Backward::less(
          first_backward,
          latest_end.back())) {
    return SweepStatus::infeasible;
  }

  cursor = first_backward;

  for (std::size_t reverse = subject_size;
       reverse > 0;
       --reverse) {
    const std::size_t i = reverse - 1;

    cursor = stretch_maximum<Backward>(
        target,
        subject.at(
            static_cast<std::ptrdiff_t>(i)),
        cursor,
        last_backward);

    earliest_start[i] =
        dual(target, cursor);
  }

  if (Forward::less(
          first_forward,
          earliest_start.front())) {
    return SweepStatus::infeasible;
  }

  return SweepStatus::feasible;
}


[[nodiscard]] SweepStatus tighten_bounds(
    const RepeatBlocks& subject,
    const RepeatBlocks& target,
    std::vector<SweepPosition>& earliest_start,
    std::vector<SweepPosition>& latest_end) {
  const std::size_t n = subject.size();

  const SweepPosition last_forward{
      static_cast<std::ptrdiff_t>(
          target.size() - 1),
      target.back().upper};

  const SweepPosition last_backward{
      0,
      target.front().upper};

  // Earliest starts and ends.
  for (std::size_t i = 0; i < n; ++i) {
    const RepeatSegment& block =
        subject.at(
            static_cast<std::ptrdiff_t>(i));

    if (block.lower == 0) {
      if (i + 1 < n &&
          Forward::less(
              earliest_start[i + 1],
              earliest_start[i])) {
        earliest_start[i + 1] =
            earliest_start[i];
      }

      continue;
    }

    SweepPosition earliest_end =
        earliest_start[i];

    const SweepPosition start =
        push_minimum<Forward>(
            target,
            block,
            earliest_end,
            last_forward);

    if (!Forward::less(
            start,
            last_forward)) {
      return SweepStatus::infeasible;
    }

    if (i + 1 < n &&
        Forward::less(
            earliest_start[i + 1],
            earliest_end)) {
      earliest_start[i + 1] =
          earliest_end;
    }

    if (Forward::less(
            earliest_start[i],
            start)) {
      earliest_start[i] = start;
    }
  }

  // Latest starts and ends.
  for (std::size_t reverse = n;
       reverse > 0;
       --reverse) {
    const std::size_t i = reverse - 1;
    const RepeatSegment& block =
        subject.at(
            static_cast<std::ptrdiff_t>(i));

    if (block.lower == 0) {
      if (i > 0 &&
          Backward::less(
              latest_end[i - 1],
              latest_end[i])) {
        latest_end[i - 1] =
            latest_end[i];
      }

      continue;
    }

    SweepPosition latest_start =
        latest_end[i];

    const SweepPosition end =
        push_minimum<Backward>(
            target,
            block,
            latest_start,
            last_backward);

    if (!Backward::less(
            end,
            last_backward)) {
      return SweepStatus::infeasible;
    }

    if (i > 0 &&
        Backward::less(
            latest_end[i - 1],
            latest_start)) {
      latest_end[i - 1] =
          latest_start;
    }

    if (Backward::less(
            latest_end[i],
            end)) {
      latest_end[i] = end;
    }
  }

  return SweepStatus::feasible;
}

}  // namespace


SweepStatus analyze_repeat_sweep(
    const Domain& subject_domain,
    const Domain& target_domain,
    SweepAnalysis& analysis) {
  analysis.blocks.clear();

  if (subject_domain.failed() ||
      target_domain.failed()) {
    return SweepStatus::infeasible;
  }

  RepeatBlocks subject;
  RepeatBlocks target;

  if (!subject.initialize(subject_domain) ||
      !target.initialize(target_domain)) {
    return SweepStatus::unsupported;
  }

  if (subject.size() == 0 ||
      target.size() == 0) {
    return SweepStatus::unsupported;
  }

  std::vector<SweepPosition> earliest_start;
  std::vector<SweepPosition> latest_end;

  SweepStatus status =
      initialize_bounds(
          subject,
          target,
          earliest_start,
          latest_end);

  if (status != SweepStatus::feasible) {
    return status;
  }

  status = tighten_bounds(
      subject,
      target,
      earliest_start,
      latest_end);

  if (status != SweepStatus::feasible) {
    return status;
  }

  analysis.blocks.reserve(subject.size());

  for (std::size_t i = 0;
       i < subject.size();
       ++i) {
    const SweepPosition latest_start =
        i == 0
            ? earliest_start.front()
            : dual(target, latest_end[i - 1]);

    const SweepPosition earliest_end =
        i + 1 < subject.size()
            ? earliest_start[i + 1]
            : dual(target, latest_end[i]);

    analysis.blocks.push_back(
        SweepBlockMatch{
            earliest_start[i],
            earliest_end,
            latest_start,
            dual(target, latest_end[i])});
  }

  return SweepStatus::feasible;
}


SweepStatus project_repeat_values(
    const Domain& subject,
    const Domain& target,
    Domain& refined) {
  SweepAnalysis analysis;

  const SweepStatus analysis_status =
      analyze_repeat_sweep(
          subject,
          target,
          analysis);

  if (analysis_status != SweepStatus::feasible) {
    return analysis_status;
  }

  std::vector<const RepeatSegment*> subject_blocks;
  std::vector<const RepeatSegment*> target_blocks;

  subject_blocks.reserve(subject.segment_count());
  target_blocks.reserve(target.segment_count());

  for (const Segment& segment : subject.segments()) {
    const auto* repeat =
        std::get_if<RepeatSegment>(&segment);

    if (repeat == nullptr) {
      return SweepStatus::unsupported;
    }

    subject_blocks.push_back(repeat);
  }

  for (const Segment& segment : target.segments()) {
    const auto* repeat =
        std::get_if<RepeatSegment>(&segment);

    if (repeat == nullptr) {
      return SweepStatus::unsupported;
    }

    target_blocks.push_back(repeat);
  }

  if (analysis.blocks.size() !=
          subject_blocks.size() ||
      target_blocks.empty()) {
    return SweepStatus::unsupported;
  }

  auto feasible_overlap =
      [&](const SweepBlockMatch& match,
          std::size_t target_index) {
        if (match.earliest_start.segment < 0 ||
            match.latest_end.segment < 0) {
          return false;
        }

        const auto first =
            static_cast<std::size_t>(
                match.earliest_start.segment);

        const auto last =
            static_cast<std::size_t>(
                match.latest_end.segment);

        if (first >= target_blocks.size() ||
            last >= target_blocks.size() ||
            first > last ||
            target_index < first ||
            target_index > last) {
          return false;
        }

        if (first == last) {
          return
              target_index == first &&
              match.earliest_start.offset <
                  match.latest_end.offset;
        }

        if (target_index == first) {
          return
              match.earliest_start.offset <
              target_blocks[target_index]->upper;
        }

        if (target_index == last) {
          return match.latest_end.offset > 0;
        }

        return true;
      };

  std::vector<Segment> projected;
  projected.reserve(subject_blocks.size());

  for (std::size_t subject_index = 0;
       subject_index < subject_blocks.size();
       ++subject_index) {
    const RepeatSegment& subject_block =
        *subject_blocks[subject_index];

    const SweepBlockMatch& match =
        analysis.blocks[subject_index];

    std::vector<IntRange> feasible_ranges;

    for (std::size_t target_index = 0;
         target_index < target_blocks.size();
         ++target_index) {
      if (!feasible_overlap(
              match,
              target_index)) {
        continue;
      }

      const std::vector<IntRange> ranges =
          target_blocks[target_index]->
              values.ranges();

      feasible_ranges.insert(
          feasible_ranges.end(),
          ranges.begin(),
          ranges.end());
    }

    const ValueSet feasible_values{
        Span<const IntRange>(feasible_ranges)};

    ValueSet refined_values =
        subject_block.values.intersected(
            feasible_values);

    Length lower = subject_block.lower;
    Length upper = subject_block.upper;

    if (refined_values.empty()) {
      if (lower > 0) {
        return SweepStatus::infeasible;
      }

      // An optional block with no feasible value can only occur zero times.
      upper = 0;
    }

    projected.push_back(
        RepeatSegment{
            std::move(refined_values),
            lower,
            upper});
  }

  Domain candidate(
      std::move(projected),
      subject.min_length(),
      subject.max_length());

  if (candidate.failed()) {
    return SweepStatus::infeasible;
  }

  refined = std::move(candidate);
  return SweepStatus::feasible;
}


SweepStatus project_against_exact_target(
    const Domain& subject,
    const Domain& target,
    Domain& refined) {
  SweepAnalysis analysis;

  const SweepStatus analysis_status =
      analyze_repeat_sweep(
          subject,
          target,
          analysis);

  if (analysis_status != SweepStatus::feasible) {
    return analysis_status;
  }

  std::vector<const RepeatSegment*> subject_blocks;
  std::vector<const RepeatSegment*> target_blocks;

  subject_blocks.reserve(subject.segment_count());
  target_blocks.reserve(target.segment_count());

  for (const Segment& segment : subject.segments()) {
    const auto* repeat =
        std::get_if<RepeatSegment>(&segment);

    if (repeat == nullptr) {
      return SweepStatus::unsupported;
    }

    subject_blocks.push_back(repeat);
  }

  for (const Segment& segment : target.segments()) {
    const auto* repeat =
        std::get_if<RepeatSegment>(&segment);

    if (repeat == nullptr ||
        repeat->lower != repeat->upper) {
      return SweepStatus::unsupported;
    }

    target_blocks.push_back(repeat);
  }

  if (analysis.blocks.size() !=
      subject_blocks.size()) {
    return SweepStatus::unsupported;
  }

  std::vector<std::uint64_t> target_begin(
      target_blocks.size());
  std::vector<std::uint64_t> target_end(
      target_blocks.size());

  std::uint64_t cursor = 0;

  for (std::size_t i = 0;
       i < target_blocks.size();
       ++i) {
    target_begin[i] = cursor;
    cursor += target_blocks[i]->upper;
    target_end[i] = cursor;
  }

  auto absolute_position =
      [&](const SweepPosition& position,
          std::uint64_t& absolute) {
        if (position.segment < 0) {
          return false;
        }

        const auto index =
            static_cast<std::size_t>(
                position.segment);

        if (index >= target_blocks.size() ||
            position.offset >
                target_blocks[index]->upper) {
          return false;
        }

        absolute =
            target_begin[index] +
            position.offset;

        return true;
      };

  auto overlap =
      [](std::uint64_t lhs_begin,
         std::uint64_t lhs_end,
         std::uint64_t rhs_begin,
         std::uint64_t rhs_end) {
        const std::uint64_t begin =
            std::max(lhs_begin, rhs_begin);
        const std::uint64_t end =
            std::min(lhs_end, rhs_end);

        return end > begin
            ? end - begin
            : std::uint64_t{0};
      };

  struct Piece {
    ValueSet values;
    Length lower;
    Length upper;
  };

  std::vector<Segment> projected;

  for (std::size_t block_index = 0;
       block_index < subject_blocks.size();
       ++block_index) {
    const RepeatSegment& subject_block =
        *subject_blocks[block_index];

    const SweepBlockMatch& match =
        analysis.blocks[block_index];

    std::uint64_t feasible_begin = 0;
    std::uint64_t feasible_end = 0;
    std::uint64_t mandatory_begin = 0;
    std::uint64_t mandatory_end = 0;

    if (!absolute_position(
            match.earliest_start,
            feasible_begin) ||
        !absolute_position(
            match.latest_end,
            feasible_end) ||
        !absolute_position(
            match.latest_start,
            mandatory_begin) ||
        !absolute_position(
            match.earliest_end,
            mandatory_end)) {
      return SweepStatus::unsupported;
    }

    if (feasible_end < feasible_begin) {
      return SweepStatus::infeasible;
    }

    if (mandatory_end < mandatory_begin) {
      mandatory_end = mandatory_begin;
    }

    std::vector<Piece> pieces;

    for (std::size_t target_index = 0;
         target_index < target_blocks.size();
         ++target_index) {
      const std::uint64_t feasible_count =
          overlap(
              feasible_begin,
              feasible_end,
              target_begin[target_index],
              target_end[target_index]);

      if (feasible_count == 0) {
        continue;
      }

      const std::uint64_t mandatory_count =
          overlap(
              mandatory_begin,
              mandatory_end,
              target_begin[target_index],
              target_end[target_index]);

      ValueSet values =
          subject_block.values.intersected(
              target_blocks[target_index]->values);

      if (values.empty()) {
        if (mandatory_count > 0) {
          return SweepStatus::infeasible;
        }

        continue;
      }

      const std::uint64_t upper =
          std::min<std::uint64_t>(
              feasible_count,
              subject_block.upper);

      if (mandatory_count > upper) {
        return SweepStatus::infeasible;
      }

      pieces.push_back(
          Piece{
              std::move(values),
              static_cast<Length>(
                  mandatory_count),
              static_cast<Length>(
                  upper)});
    }

    std::uint64_t total_lower = 0;
    std::uint64_t total_upper = 0;

    for (const Piece& piece : pieces) {
      total_lower += piece.lower;
      total_upper += piece.upper;
    }

    if (total_upper < subject_block.lower ||
        total_lower > subject_block.upper) {
      return SweepStatus::infeasible;
    }

    // Enforce the subject block's total repetition bounds over the pieces.
    bool changed = true;

    while (changed) {
      changed = false;

      total_lower = 0;
      total_upper = 0;

      for (const Piece& piece : pieces) {
        total_lower += piece.lower;
        total_upper += piece.upper;
      }

      for (Piece& piece : pieces) {
        const std::uint64_t other_upper =
            total_upper - piece.upper;

        const std::uint64_t required_lower =
            subject_block.lower > other_upper
                ? subject_block.lower - other_upper
                : 0;

        const std::uint64_t other_lower =
            total_lower - piece.lower;

        if (other_lower > subject_block.upper) {
          return SweepStatus::infeasible;
        }

        const std::uint64_t allowed_upper =
            subject_block.upper - other_lower;

        const Length new_lower =
            static_cast<Length>(
                std::max<std::uint64_t>(
                    piece.lower,
                    required_lower));

        const Length new_upper =
            static_cast<Length>(
                std::min<std::uint64_t>(
                    piece.upper,
                    allowed_upper));

        if (new_lower > new_upper) {
          return SweepStatus::infeasible;
        }

        if (new_lower != piece.lower ||
            new_upper != piece.upper) {
          piece.lower = new_lower;
          piece.upper = new_upper;
          changed = true;
        }
      }
    }

    for (Piece& piece : pieces) {
      if (piece.upper == 0) {
        continue;
      }

      projected.push_back(
          RepeatSegment{
              std::move(piece.values),
              piece.lower,
              piece.upper});
    }
  }

  Domain candidate(
      std::move(projected),
      subject.min_length(),
      subject.max_length());

  if (candidate.failed()) {
    return SweepStatus::infeasible;
  }

  refined = std::move(candidate);
  return SweepStatus::feasible;
}

}  // namespace dashed::detail
