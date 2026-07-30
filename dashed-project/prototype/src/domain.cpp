#include "dashed/domain.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <limits>
#include <numeric>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

namespace dashed {
namespace {

Length segment_min(const Segment& segment) noexcept {
  if (const auto* repeat = std::get_if<RepeatSegment>(&segment)) {
    return repeat->lower;
  }
  return static_cast<Length>(
      std::get<LiteralSegment>(segment).literal.size());
}

Length segment_max(const Segment& segment) noexcept {
  if (const auto* repeat = std::get_if<RepeatSegment>(&segment)) {
    return repeat->upper;
  }
  return static_cast<Length>(
      std::get<LiteralSegment>(segment).literal.size());
}

bool segment_assigned(const Segment& segment) noexcept {
  if (const auto* repeat = std::get_if<RepeatSegment>(&segment)) {
    return repeat->exact_count() && repeat->exact_value();
  }
  return true;
}

bool needs_structural_normalization(
    const std::vector<Segment>& segments) noexcept {
  const RepeatSegment* previous_repeat = nullptr;
  bool previous_unit_singleton = false;
  bool previous_literal = false;

  for (const Segment& segment : segments) {
    if (const auto* repeat = std::get_if<RepeatSegment>(&segment)) {
      if (repeat->lower > repeat->upper || repeat->upper == 0 ||
          (repeat->values.empty() && repeat->lower > 0)) {
        return true;
      }

      const bool unit_singleton =
          repeat->lower == 1 && repeat->upper == 1 &&
          repeat->values.singleton();
      if ((previous_repeat != nullptr &&
           previous_repeat->values == repeat->values) ||
          (previous_unit_singleton && unit_singleton)) {
        return true;
      }

      previous_repeat = repeat;
      previous_unit_singleton = unit_singleton;
      previous_literal = false;
      continue;
    }

    const LiteralSlice& literal =
        std::get<LiteralSegment>(segment).literal;
    if (literal.empty()) {
      return true;
    }

    const int first = literal[0];
    if (std::all_of(
            literal.span().begin() + 1, literal.span().end(),
            [first](int value) { return value == first; })) {
      return true;
    }
    if (previous_literal) {
      return true;
    }

    previous_repeat = nullptr;
    previous_unit_singleton = false;
    previous_literal = true;
  }

  return false;
}

bool has_adjacent_literal_segments(
    const std::vector<Segment>& segments) noexcept {
  for (std::size_t index = 1; index < segments.size(); ++index) {
    if (std::holds_alternative<LiteralSegment>(segments[index - 1]) &&
        std::holds_alternative<LiteralSegment>(segments[index])) {
      return true;
    }
  }
  return false;
}

void merge_adjacent_literal_segments(std::vector<Segment>& segments) {
  if (!has_adjacent_literal_segments(segments)) {
    return;
  }
  std::vector<Segment> merged;
  merged.reserve(segments.size());

  std::size_t begin = 0;
  while (begin < segments.size()) {
    auto* first = std::get_if<LiteralSegment>(&segments[begin]);
    if (first == nullptr) {
      merged.push_back(std::move(segments[begin]));
      ++begin;
      continue;
    }

    std::size_t end = begin + 1;
    std::size_t total = first->literal.size();
    LiteralSlice contiguous = first->literal;
    bool zero_copy = true;

    while (end < segments.size()) {
      auto* next = std::get_if<LiteralSegment>(&segments[end]);
      if (next == nullptr) {
        break;
      }

      if (next->literal.size() >
          std::numeric_limits<std::size_t>::max() - total) {
        throw std::length_error("adjacent literal run is too large");
      }
      total += next->literal.size();

      if (zero_copy && contiguous.contiguous_with(next->literal)) {
        contiguous = contiguous.merged_with(next->literal);
      } else {
        zero_copy = false;
      }
      ++end;
    }

    if (end == begin + 1) {
      merged.push_back(std::move(segments[begin]));
    } else if (zero_copy) {
      merged.push_back(LiteralSegment{std::move(contiguous)});
    } else {
      std::vector<int> values;
      values.reserve(total);
      for (std::size_t index = begin; index < end; ++index) {
        const auto& literal =
            std::get<LiteralSegment>(segments[index]).literal;
        values.insert(values.end(), literal.span().begin(), literal.span().end());
      }
      merged.push_back(
          LiteralSegment{LiteralSlice(std::move(values))});
    }

    begin = end;
  }

  segments = std::move(merged);
}

std::string literal_to_string(const LiteralSlice& literal) {
  std::ostringstream out;
  out << '[';
  for (std::size_t i = 0; i < literal.size(); ++i) {
    if (i != 0) {
      out << ',';
    }
    out << literal[i];
  }
  out << ']';
  return out.str();
}

}  // namespace

Domain::Domain() = default;

Domain::Domain(std::vector<Segment> segments, Length min_length,
               Length max_length)
    : segments_(std::move(segments)),
      min_length_(min_length),
      max_length_(max_length) {
  normalize();
}

Domain Domain::empty_list() { return Domain({}, 0, 0); }

Domain Domain::fixed(std::vector<int> values) {
  return fixed(LiteralSlice(std::move(values)));
}

Domain Domain::fixed(LiteralSlice values) {
  const Length length = static_cast<Length>(values.size());
  if (values.empty()) {
    return empty_list();
  }
  return Domain({LiteralSegment{std::move(values)}}, length, length);
}

Domain Domain::repeat(ValueSet values, Length lower, Length upper) {
  return Domain({RepeatSegment{std::move(values), lower, upper}}, lower, upper);
}

Domain Domain::top(ValueSet alphabet, Length min_length, Length max_length) {
  return repeat(std::move(alphabet), min_length, max_length);
}

bool Domain::assigned() const noexcept {
  if (failed_ || min_length_ != max_length_) {
    return false;
  }
  return std::all_of(segments_.begin(), segments_.end(), segment_assigned);
}

std::vector<int> Domain::value() const {
  if (!assigned()) {
    throw std::logic_error("value requested from non-assigned Domain");
  }
  std::vector<int> result;
  result.reserve(min_length_);
  if (!append_assigned(result)) {
    throw std::logic_error("assigned Domain has inconsistent segments");
  }
  return result;
}

bool Domain::accepts(Span<const int> candidate) const {
  if (failed_ || candidate.size() < min_length_ ||
      candidate.size() > max_length_) {
    return false;
  }

  const std::size_t n = candidate.size();
  std::vector<unsigned char> reachable(n + 1, 0);
  std::vector<unsigned char> next(n + 1, 0);
  reachable[0] = 1;

  for (const Segment& segment : segments_) {
    std::fill(next.begin(), next.end(), 0);
    if (const auto* literal = std::get_if<LiteralSegment>(&segment)) {
      const auto expected = literal->literal.span();
      for (std::size_t start = 0; start <= n; ++start) {
        if (!reachable[start] || expected.size() > n - start) {
          continue;
        }
        if (std::equal(expected.begin(), expected.end(),
                       candidate.begin() + static_cast<std::ptrdiff_t>(start))) {
          next[start + expected.size()] = 1;
        }
      }
    } else {
      const auto& repeat = std::get<RepeatSegment>(segment);
      for (std::size_t start = 0; start <= n; ++start) {
        if (!reachable[start]) {
          continue;
        }
        const std::size_t max_count = std::min<std::size_t>(
            repeat.upper, n - start);
        if (repeat.lower == 0) {
          next[start] = 1;
        }
        for (std::size_t count = 1; count <= max_count; ++count) {
          if (!repeat.values.contains(candidate[start + count - 1])) {
            break;
          }
          if (count >= repeat.lower) {
            next[start + count] = 1;
          }
        }
      }
    }
    reachable.swap(next);
  }
  return reachable[n] != 0;
}


bool Domain::accepts(const Domain& assigned_candidate) const {
  if (!assigned_candidate.assigned() || failed_) {
    return false;
  }
  const Length n = assigned_candidate.min_length();
  if (n < min_length_ || n > max_length_) {
    return false;
  }

  // Fast path for the overwhelmingly common top/repeated-alphabet domain.
  if (segments_.size() == 1) {
    if (const auto* repeat = std::get_if<RepeatSegment>(&segments_.front())) {
      if (n < repeat->lower || n > repeat->upper) {
        return false;
      }
      for (const Segment& segment : assigned_candidate.segments_) {
        if (const auto* literal = std::get_if<LiteralSegment>(&segment)) {
          for (int value : literal->literal.span()) {
            if (!repeat->values.contains(value)) {
              return false;
            }
          }
        } else {
          const auto& exact = std::get<RepeatSegment>(segment);
          const auto value = exact.values.singleton_value();
          if (!exact.exact_count() || !value.has_value() ||
              !repeat->values.contains(*value)) {
            return false;
          }
        }
      }
      return true;
    }
  }

  if (assigned()) {
    return assigned_equal(assigned_candidate);
  }

  // General dashed membership currently uses a dense candidate. This fallback
  // is correct; the sweep port will replace it with a segmented cursor.
  return accepts(assigned_candidate.value());
}

bool Domain::assigned_equal(const Domain& other) const {
  if (!assigned() || !other.assigned() || min_length_ != other.min_length_) {
    return false;
  }
  if (*this == other) {
    return true;
  }

  auto left = value();
  auto right = other.value();
  return left == right;
}

bool Domain::may_equal(const Domain& other) const {
  if (failed_ || other.failed_ || max_length_ < other.min_length_ ||
      other.max_length_ < min_length_) {
    return false;
  }
  if (assigned()) {
    return other.accepts(*this);
  }
  if (other.assigned()) {
    return accepts(other);
  }
  return true;
}

std::string Domain::to_string() const {
  if (failed_) {
    return "FAIL";
  }
  std::ostringstream out;
  out << "len=" << min_length_ << ".." << max_length_ << " <";
  for (std::size_t i = 0; i < segments_.size(); ++i) {
    if (i != 0) {
      out << " | ";
    }
    if (const auto* repeat = std::get_if<RepeatSegment>(&segments_[i])) {
      out << repeat->values.to_string() << '^' << repeat->lower << ".."
          << repeat->upper;
    } else {
      out << '=' << literal_to_string(
          std::get<LiteralSegment>(segments_[i]).literal);
    }
  }
  out << '>';
  return out.str();
}

Change Domain::tighten_length(Length lower, Length upper) {
  if (failed_) {
    return Change::failed;
  }
  const Length new_lower = std::max(min_length_, lower);
  const Length new_upper = std::min(max_length_, upper);
  if (new_lower > new_upper) {
    fail();
    return Change::failed;
  }
  if (new_lower == min_length_ && new_upper == max_length_) {
    return Change::none;
  }
  const bool was_assigned = assigned();
  min_length_ = new_lower;
  max_length_ = new_upper;
  normalize();
  if (failed_) {
    return Change::failed;
  }
  if (!was_assigned && assigned()) {
    return Change::assigned;
  }
  return Change::length;
}

Change Domain::restrict_to_fixed(std::vector<int> fixed_value) {
  return restrict_to_fixed(LiteralSlice(std::move(fixed_value)));
}

Change Domain::restrict_to_fixed(LiteralSlice fixed_value) {
  if (failed_) {
    return Change::failed;
  }
  if (!accepts(fixed_value.span())) {
    fail();
    return Change::failed;
  }
  Domain exact = Domain::fixed(std::move(fixed_value));
  if (*this == exact) {
    return Change::none;
  }
  *this = std::move(exact);
  return Change::assigned;
}

Change Domain::intersect_same_shape(const Domain& other) {
  if (failed_ || other.failed_) {
    fail();
    return Change::failed;
  }
  Change change = tighten_length(other.min_length_, other.max_length_);
  if (dashed::failed(change)) {
    return change;
  }
  if (segments_.size() != other.segments_.size()) {
    return change;
  }

  // This fast path is intentionally limited to fixed segment boundaries.
  // Intersecting variable-width corresponding blocks can be unsound because
  // equal concrete lists may admit different internal block alignments.
  for (std::size_t i = 0; i < segments_.size(); ++i) {
    if (segments_[i].index() != other.segments_[i].index()) {
      return change;
    }
    if (const auto* lhs = std::get_if<RepeatSegment>(&segments_[i])) {
      const auto& rhs = std::get<RepeatSegment>(other.segments_[i]);
      if (!lhs->exact_count() || !rhs.exact_count() ||
          lhs->lower != rhs.lower) {
        return change;
      }
    }
  }

  bool domain_changed = false;
  for (std::size_t i = 0; i < segments_.size(); ++i) {
    if (auto* lhs = std::get_if<RepeatSegment>(&segments_[i])) {
      const auto& rhs = std::get<RepeatSegment>(other.segments_[i]);
      ValueSet intersection = lhs->values.intersected(rhs.values);
      if (intersection.empty() && lhs->lower > 0) {
        fail();
        return Change::failed;
      }
      if (intersection != lhs->values) {
        lhs->values = std::move(intersection);
        domain_changed = true;
      }
    } else {
      const auto& lhs_literal = std::get<LiteralSegment>(segments_[i]).literal;
      const auto& rhs_literal = std::get<LiteralSegment>(other.segments_[i]).literal;
      if (!(lhs_literal == rhs_literal)) {
        fail();
        return Change::failed;
      }
    }
  }
  if (domain_changed) {
    normalize();
    return assigned() ? Change::assigned : Change::domain;
  }
  return change;
}

Change Domain::intersect_single_repeat(
    const Domain& other) {
  if (failed_ || other.failed_) {
    fail();
    return Change::failed;
  }

  Change change =
      tighten_length(
          other.min_length_,
          other.max_length_);

  if (dashed::failed(change)) {
    return change;
  }

  if (segments_.size() != 1) {
    return change;
  }

  const auto* source =
      std::get_if<RepeatSegment>(
          &segments_.front());

  if (source == nullptr) {
    return change;
  }

  // A single repeat segment imposes only:
  //
  //   1. one common alphabet for every position; and
  //   2. a global minimum/maximum length.
  //
  // Intersecting every segment of the other domain with that alphabet,
  // while retaining the already-intersected global length bounds, is
  // therefore an exact representation of the language intersection.
  const ValueSet alphabet = source->values;

  std::vector<Segment> refined;
  refined.reserve(other.segments_.size());

  for (const Segment& segment :
       other.segments_) {
    if (const auto* repeat =
            std::get_if<RepeatSegment>(
                &segment)) {
      ValueSet values =
          alphabet.intersected(
              repeat->values);

      Length lower = repeat->lower;
      Length upper = repeat->upper;

      if (values.empty()) {
        if (lower > 0) {
          fail();
          return Change::failed;
        }

        // An optional block with no remaining values can only have
        // count zero. normalize() will subsequently remove it.
        upper = 0;
      }

      refined.push_back(
          RepeatSegment{
              std::move(values),
              lower,
              upper});
    } else {
      const auto& literal =
          std::get<LiteralSegment>(
              segment);

      for (int value :
           literal.literal.span()) {
        if (!alphabet.contains(value)) {
          fail();
          return Change::failed;
        }
      }

      refined.push_back(literal);
    }
  }

  Domain candidate(
      std::move(refined),
      min_length_,
      max_length_);

  if (candidate.failed()) {
    fail();
    return Change::failed;
  }

  if (candidate == *this) {
    return change;
  }

  const bool was_assigned = assigned();

  *this = std::move(candidate);

  const Change refinement =
      !was_assigned && assigned()
          ? Change::assigned
          : Change::domain;

  return combine(change, refinement);
}

void Domain::normalize() {
  if (failed_) {
    return;
  }
  if (min_length_ > max_length_) {
    fail();
    return;
  }

  bool repeat_canonicalization = false;
  do {
    const bool did_canonicalize =
        needs_structural_normalization(segments_);
    if (did_canonicalize) {
      std::vector<Segment> normalized;
      normalized.reserve(segments_.size());

      // Append a repeat segment while maintaining the invariant that adjacent
      // repeat segments with equal value sets are merged.
      auto append_repeat = [&](RepeatSegment repeat) {
        if (repeat.upper == 0) {
          return;
        }

        if (!normalized.empty()) {
          if (auto* previous =
                  std::get_if<RepeatSegment>(&normalized.back());
              previous != nullptr &&
              previous->values == repeat.values) {
            previous->lower =
                saturating_add(previous->lower, repeat.lower);
            previous->upper =
                saturating_add(previous->upper, repeat.upper);
            return;
          }
        }

        normalized.push_back(std::move(repeat));
      };

      // Append an exact literal. Uniform literals are represented as exact
      // singleton repeat segments, avoiding storage proportional to their length.
      auto append_literal = [&](LiteralSegment literal) {
        if (literal.literal.empty()) {
          return;
        }

        const int first = literal.literal[0];
        bool uniform = true;

        for (std::size_t i = 1;
             i < literal.literal.size();
             ++i) {
          if (literal.literal[i] != first) {
            uniform = false;
            break;
          }
        }

        if (uniform) {
          const Length count =
              static_cast<Length>(literal.literal.size());

          append_repeat(
              RepeatSegment{ValueSet(first), count, count});
          return;
        }

        if (!normalized.empty()) {
          if (auto* previous =
                  std::get_if<LiteralSegment>(&normalized.back());
              previous != nullptr &&
              previous->literal.contiguous_with(
                  literal.literal)) {
            previous->literal =
                previous->literal.merged_with(
                    literal.literal);
            return;
          }
        }

        normalized.push_back(std::move(literal));
      };

      // G-Strings often represented a fixed sequence as one singleton block per
      // element. Accumulate consecutive unit singleton blocks. Uniform runs become
      // exact singleton repeat segments; mixed runs become one immutable literal.
      std::vector<int> pending_unit_literals;

      auto flush_pending_unit_literals = [&]() {
        if (pending_unit_literals.empty()) {
          return;
        }

        const int first = pending_unit_literals.front();
        const bool uniform = std::all_of(
            pending_unit_literals.begin(),
            pending_unit_literals.end(),
            [first](int value) {
              return value == first;
            });

        if (uniform) {
          const Length count =
              static_cast<Length>(
                  pending_unit_literals.size());

          append_repeat(
              RepeatSegment{ValueSet(first), count, count});
        } else {
          append_literal(
              LiteralSegment{
                  LiteralSlice(
                      std::move(pending_unit_literals))});
        }

        pending_unit_literals.clear();
      };

      for (Segment& segment : segments_) {
        if (auto* repeat =
                std::get_if<RepeatSegment>(&segment)) {
          if (repeat->lower > repeat->upper ||
              (repeat->values.empty() &&
               repeat->lower > 0)) {
            fail();
            return;
          }

          if (repeat->upper == 0) {
            continue;
          }

          if (repeat->lower == 1 &&
              repeat->upper == 1 &&
              repeat->values.singleton()) {
            pending_unit_literals.push_back(
                *repeat->values.singleton_value());
            continue;
          }

          flush_pending_unit_literals();
          append_repeat(std::move(*repeat));
        } else {
          flush_pending_unit_literals();

          append_literal(
              std::get<LiteralSegment>(
                  std::move(segment)));
        }
      }

      flush_pending_unit_literals();
      segments_ = std::move(normalized);
      merge_adjacent_literal_segments(segments_);
    }

    const Length segment_minimum = segment_min_sum();
    const Length segment_maximum = segment_max_sum();
    min_length_ = std::max(min_length_, segment_minimum);
    max_length_ = std::min(max_length_, segment_maximum);
    if (min_length_ > max_length_) {
      fail();
      return;
    }

    // An already-canonical exact domain cannot be refined further.
    //
    // Equal segment minimum and maximum sums imply that every segment count
    // is exact. Matching global bounds therefore leave nothing for count
    // tightening to change. Because needs_structural_normalization() was
    // false, there are also no adjacent literal segments left for the final
    // merge pass.
    if (!did_canonicalize &&
        segment_minimum == segment_maximum &&
        min_length_ == segment_minimum &&
        max_length_ == segment_maximum) {
      return;
    }

    const CountTighteningResult tightening =
        tighten_segment_counts_from_global_length();
    if (failed_) {
      return;
    }
    repeat_canonicalization =
        tightening.structural_change ||
        (did_canonicalize && tightening.changed &&
         needs_structural_normalization(segments_));

    // A tightened count can expose a new zero-width separator, make adjacent
    // repeat blocks mergeable, or turn several singleton blocks into one
    // literal. Repeat the canonicalization pass until count bounds stabilize.
  } while (repeat_canonicalization);

  // Count tightening can remove a separator block and expose exact literal
  // slices that came from different immutable payloads. Canonicalize the
  // whole adjacent run in one pass: contiguous slices keep sharing storage,
  // while unrelated slices are copied exactly once into one immutable value.
  merge_adjacent_literal_segments(segments_);
}

void Domain::fail() noexcept {
  segments_.clear();
  min_length_ = 1;
  max_length_ = 0;
  failed_ = true;
}

Domain Domain::concatenated(const Domain& other) const {
  if (failed_ || other.failed_) {
    Domain result;
    result.fail();
    return result;
  }
  std::vector<Segment> segments;
  segments.reserve(segments_.size() + other.segments_.size());
  segments.insert(segments.end(), segments_.begin(), segments_.end());
  segments.insert(segments.end(), other.segments_.begin(), other.segments_.end());
  return Domain(std::move(segments), saturating_add(min_length_, other.min_length_),
                saturating_add(max_length_, other.max_length_));
}

Domain Domain::assigned_prefix(Length length) const {
  if (!assigned() || length > min_length_) {
    Domain result;
    result.fail();
    return result;
  }
  if (length == 0) {
    return empty_list();
  }

  Length remaining = length;
  std::vector<Segment> prefix;
  for (const Segment& segment : segments_) {
    if (remaining == 0) {
      break;
    }
    if (const auto* literal = std::get_if<LiteralSegment>(&segment)) {
      const Length take = std::min<Length>(
          remaining, static_cast<Length>(literal->literal.size()));
      prefix.push_back(LiteralSegment{literal->literal.slice(0, take)});
      remaining -= take;
    } else {
      const auto& repeat = std::get<RepeatSegment>(segment);
      const Length take = std::min(remaining, repeat.lower);
      prefix.push_back(RepeatSegment{repeat.values, take, take});
      remaining -= take;
    }
  }
  if (remaining != 0) {
    Domain result;
    result.fail();
    return result;
  }
  return Domain(std::move(prefix), length, length);
}

Domain Domain::assigned_suffix(Length length) const {
  if (!assigned() || length > min_length_) {
    Domain result;
    result.fail();
    return result;
  }
  if (length == 0) {
    return empty_list();
  }

  Length remaining = length;
  std::deque<Segment> suffix;
  for (auto it = segments_.rbegin(); it != segments_.rend() && remaining > 0;
       ++it) {
    if (const auto* literal = std::get_if<LiteralSegment>(&*it)) {
      const Length available = static_cast<Length>(literal->literal.size());
      const Length take = std::min(remaining, available);
      suffix.push_front(LiteralSegment{
          literal->literal.slice(available - take, take)});
      remaining -= take;
    } else {
      const auto& repeat = std::get<RepeatSegment>(*it);
      const Length take = std::min(remaining, repeat.lower);
      suffix.push_front(RepeatSegment{repeat.values, take, take});
      remaining -= take;
    }
  }
  if (remaining != 0) {
    Domain result;
    result.fail();
    return result;
  }
  return Domain(std::vector<Segment>(suffix.begin(), suffix.end()), length,
                length);
}

std::size_t Domain::structural_bytes() const noexcept {
  return sizeof(*this) + segments_.capacity() * sizeof(Segment);
}

std::size_t Domain::referenced_dynamic_bytes() const noexcept {
  std::size_t bytes = 0;
  std::unordered_set<const void*> seen;
  for (const Segment& segment : segments_) {
    if (const auto* repeat = std::get_if<RepeatSegment>(&segment)) {
      const void* key = repeat->values.storage_id();
      if (key != nullptr && seen.insert(key).second) {
        bytes += repeat->values.referenced_dynamic_bytes();
      }
    } else {
      const auto& literal = std::get<LiteralSegment>(segment).literal;
      const void* key = literal.storage_id();
      if (key != nullptr && seen.insert(key).second) {
        bytes += literal.referenced_dynamic_bytes();
      }
    }
  }
  return bytes;
}

std::size_t Domain::owned_dynamic_bytes() const noexcept {
  std::size_t bytes = 0;
  std::unordered_set<const void*> seen;
  for (const Segment& segment : segments_) {
    if (const auto* repeat = std::get_if<RepeatSegment>(&segment)) {
      bytes += repeat->values.dynamic_bytes_if_unique();
    } else {
      const auto& literal = std::get<LiteralSegment>(segment).literal;
      const void* key = literal.storage().get();
      if (key != nullptr && seen.insert(key).second) {
        bytes += literal.dynamic_bytes_if_unique();
      }
    }
  }
  return bytes;
}

bool operator==(const Domain& lhs, const Domain& rhs) noexcept {
  return lhs.failed_ == rhs.failed_ && lhs.min_length_ == rhs.min_length_ &&
         lhs.max_length_ == rhs.max_length_ && lhs.segments_ == rhs.segments_;
}

std::ostream& operator<<(std::ostream& out, const Domain& domain) {
  return out << domain.to_string();
}

Length Domain::saturating_add(Length lhs, Length rhs) noexcept {
  if (kUnboundedLength - lhs < rhs) {
    return kUnboundedLength;
  }
  return lhs + rhs;
}

Length Domain::saturating_sub(Length lhs, Length rhs) noexcept {
  return lhs > rhs ? lhs - rhs : 0;
}

Length Domain::segment_min_sum() const noexcept {
  Length total = 0;
  for (const Segment& segment : segments_) {
    total = saturating_add(total, segment_min(segment));
  }
  return total;
}

Length Domain::segment_max_sum() const noexcept {
  Length total = 0;
  for (const Segment& segment : segments_) {
    total = saturating_add(total, segment_max(segment));
  }
  return total;
}

bool Domain::append_assigned(std::vector<int>& out) const {
  for (const Segment& segment : segments_) {
    if (const auto* literal = std::get_if<LiteralSegment>(&segment)) {
      const auto values = literal->literal.span();
      out.insert(out.end(), values.begin(), values.end());
    } else {
      const auto& repeat = std::get<RepeatSegment>(segment);
      const auto value = repeat.values.singleton_value();
      if (!repeat.exact_count() || !value.has_value()) {
        return false;
      }
      out.insert(out.end(), repeat.lower, *value);
    }
  }
  return out.size() == min_length_;
}

std::vector<LiteralSlice> Domain::assigned_literal_slices() const {
  std::vector<LiteralSlice> result;
  if (!assigned()) {
    return result;
  }
  for (const Segment& segment : segments_) {
    if (const auto* literal = std::get_if<LiteralSegment>(&segment)) {
      result.push_back(literal->literal);
    }
  }
  return result;
}

Domain::CountTighteningResult
Domain::tighten_segment_counts_from_global_length() {
  CountTighteningResult result;
  if (failed_) {
    return result;
  }

  Length total_min = 0;
  Length total_max = 0;
  for (const Segment& segment : segments_) {
    total_min = saturating_add(total_min, segment_min(segment));
    total_max = saturating_add(total_max, segment_max(segment));
  }
  if (total_min > max_length_ || total_max < min_length_) {
    fail();
    return result;
  }

  auto tighten_repeat = [&](RepeatSegment& repeat,
                            Length others_min,
                            Length others_max) {
    const Length forced_lower =
        min_length_ > others_max ? min_length_ - others_max : 0;
    const Length permitted_upper =
        max_length_ >= others_min ? max_length_ - others_min : 0;

    const Length new_lower = std::max(repeat.lower, forced_lower);
    const Length new_upper = std::min(repeat.upper, permitted_upper);
    if (new_lower > new_upper ||
        (repeat.values.empty() && new_lower > 0)) {
      fail();
      return;
    }
    if (new_lower == repeat.lower && new_upper == repeat.upper) {
      return;
    }

    const bool was_unit_singleton =
        repeat.lower == 1 && repeat.upper == 1 &&
        repeat.values.singleton();
    const bool is_unit_singleton =
        new_lower == 1 && new_upper == 1 &&
        repeat.values.singleton();
    result.changed = true;
    result.structural_change =
        result.structural_change || new_upper == 0 ||
        (!was_unit_singleton && is_unit_singleton);
    repeat.lower = new_lower;
    repeat.upper = new_upper;
  };

  // Bounds consistency for one positive unit-coefficient sum reaches its
  // closure after this simultaneous projection. For finite totals, each
  // segment's complement is available by exact subtraction, so the common
  // path needs no scratch allocation.
  if (total_min != kUnboundedLength && total_max != kUnboundedLength) {
    for (Segment& segment : segments_) {
      auto* repeat = std::get_if<RepeatSegment>(&segment);
      if (repeat == nullptr) {
        continue;
      }
      tighten_repeat(
          *repeat,
          total_min - repeat->lower,
          total_max - repeat->upper);
      if (failed_) {
        return result;
      }
    }
    return result;
  }

  // A saturated total cannot be safely "unsummed" by subtraction. Preserve
  // each omitted segment's exact capped contribution with prefix/suffix sums.
  std::vector<Length> suffix_min(segments_.size() + 1, 0);
  std::vector<Length> suffix_max(segments_.size() + 1, 0);
  for (std::size_t reverse = segments_.size(); reverse > 0; --reverse) {
    const std::size_t index = reverse - 1;
    suffix_min[index] = saturating_add(
        segment_min(segments_[index]), suffix_min[index + 1]);
    suffix_max[index] = saturating_add(
        segment_max(segments_[index]), suffix_max[index + 1]);
  }

  Length prefix_min = 0;
  Length prefix_max = 0;
  for (std::size_t index = 0; index < segments_.size(); ++index) {
    const Length current_min = segment_min(segments_[index]);
    const Length current_max = segment_max(segments_[index]);
    auto* repeat = std::get_if<RepeatSegment>(&segments_[index]);
    if (repeat != nullptr) {
      tighten_repeat(
          *repeat,
          saturating_add(prefix_min, suffix_min[index + 1]),
          saturating_add(prefix_max, suffix_max[index + 1]));
      if (failed_) {
        return result;
      }
    }
    prefix_min = saturating_add(prefix_min, current_min);
    prefix_max = saturating_add(prefix_max, current_max);
  }

  return result;
}


}  // namespace dashed
