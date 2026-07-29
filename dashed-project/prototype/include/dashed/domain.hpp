#pragma once

#include "dashed/change.hpp"
#include "dashed/literal_slice.hpp"
#include "dashed/span.hpp"
#include "dashed/value_set.hpp"

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <limits>
#include <string>
#include <variant>
#include <vector>

namespace dashed {

using Length = std::uint32_t;
inline constexpr Length kUnboundedLength =
    std::numeric_limits<Length>::max();

struct RepeatSegment {
  ValueSet values;
  Length lower = 0;
  Length upper = 0;

  [[nodiscard]] bool exact_count() const noexcept { return lower == upper; }
  [[nodiscard]] bool exact_value() const noexcept { return values.singleton(); }
  friend bool operator==(const RepeatSegment& lhs,
                         const RepeatSegment& rhs) noexcept {
    return lhs.values == rhs.values && lhs.lower == rhs.lower &&
           lhs.upper == rhs.upper;
  }
  friend bool operator!=(const RepeatSegment& lhs,
                         const RepeatSegment& rhs) noexcept {
    return !(lhs == rhs);
  }
};

struct LiteralSegment {
  LiteralSlice literal;
  friend bool operator==(const LiteralSegment& lhs,
                         const LiteralSegment& rhs) noexcept {
    return lhs.literal == rhs.literal;
  }
  friend bool operator!=(const LiteralSegment& lhs,
                         const LiteralSegment& rhs) noexcept {
    return !(lhs == rhs);
  }
};

using Segment = std::variant<RepeatSegment, LiteralSegment>;

/**
 * Persistent, canonical dashed-list domain.
 *
 * RepeatSegment preserves the original dashed block (S, lower, upper).
 * LiteralSegment is an exact-run optimization: arbitrary fixed sequences are
 * stored densely and shared between clones, slices, and concatenations.
 */
class Domain {
 public:
  Domain();
  Domain(std::vector<Segment> segments, Length min_length, Length max_length);

  [[nodiscard]] static Domain empty_list();
  [[nodiscard]] static Domain fixed(std::vector<int> values);
  [[nodiscard]] static Domain fixed(LiteralSlice values);
  [[nodiscard]] static Domain repeat(ValueSet values, Length lower,
                                     Length upper);
  [[nodiscard]] static Domain top(ValueSet alphabet, Length min_length,
                                  Length max_length);

  [[nodiscard]] bool failed() const noexcept { return failed_; }
  [[nodiscard]] bool assigned() const noexcept;
  [[nodiscard]] Length min_length() const noexcept { return min_length_; }
  [[nodiscard]] Length max_length() const noexcept { return max_length_; }
  [[nodiscard]] const std::vector<Segment>& segments() const noexcept {
    return segments_;
  }
  [[nodiscard]] std::size_t segment_count() const noexcept {
    return segments_.size();
  }

  [[nodiscard]] std::vector<int> value() const;
  [[nodiscard]] bool accepts(Span<const int> candidate) const;
  [[nodiscard]] bool accepts(const Domain& assigned_candidate) const;
  [[nodiscard]] bool assigned_equal(const Domain& other) const;
  [[nodiscard]] bool may_equal(const Domain& other) const;
  [[nodiscard]] std::string to_string() const;

  Change tighten_length(Length lower, Length upper);
  Change restrict_to_fixed(std::vector<int> value);
  Change restrict_to_fixed(LiteralSlice value);
  Change intersect_same_shape(const Domain& other);
  void normalize();
  void fail() noexcept;

  [[nodiscard]] Domain concatenated(const Domain& other) const;
  [[nodiscard]] Domain assigned_prefix(Length length) const;
  [[nodiscard]] Domain assigned_suffix(Length length) const;

  [[nodiscard]] std::size_t structural_bytes() const noexcept;
  [[nodiscard]] std::size_t referenced_dynamic_bytes() const noexcept;
  [[nodiscard]] std::size_t owned_dynamic_bytes() const noexcept;

  friend bool operator==(const Domain& lhs, const Domain& rhs) noexcept;
  friend std::ostream& operator<<(std::ostream& out, const Domain& domain);

 private:
  std::vector<Segment> segments_;
  Length min_length_ = 0;
  Length max_length_ = 0;
  bool failed_ = false;

  [[nodiscard]] static Length saturating_add(Length lhs, Length rhs) noexcept;
  [[nodiscard]] static Length saturating_sub(Length lhs, Length rhs) noexcept;
  [[nodiscard]] Length segment_min_sum() const noexcept;
  [[nodiscard]] Length segment_max_sum() const noexcept;
  [[nodiscard]] bool append_assigned(std::vector<int>& out) const;
  [[nodiscard]] std::vector<LiteralSlice> assigned_literal_slices() const;
  void tighten_segment_counts_from_global_length();
};

}  // namespace dashed
