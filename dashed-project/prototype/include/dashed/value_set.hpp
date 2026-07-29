#pragma once

#include "dashed/span.hpp"

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace dashed {

struct IntRange {
  int lower = 0;
  int upper = -1;

  [[nodiscard]] bool empty() const noexcept { return lower > upper; }
  [[nodiscard]] std::uint64_t cardinality() const noexcept;
  friend constexpr bool operator==(const IntRange& lhs,
                                   const IntRange& rhs) noexcept {
    return lhs.lower == rhs.lower && lhs.upper == rhs.upper;
  }
  friend constexpr bool operator!=(const IntRange& lhs,
                                   const IntRange& rhs) noexcept {
    return !(lhs == rhs);
  }
};

/**
 * Immutable finite integer set with a small-object representation.
 *
 * Empty, singleton, and interval domains do not allocate. A union of two or
 * more ranges owns one immutable shared vector. Copying a ValueSet is therefore
 * cheap and safe across persistent Domain values.
 */
class ValueSet {
 public:
  ValueSet() noexcept = default;
  explicit ValueSet(int value) noexcept;
  ValueSet(int lower, int upper);
  explicit ValueSet(Span<const IntRange> ranges);
  ValueSet(std::initializer_list<IntRange> ranges);

  [[nodiscard]] static ValueSet from_values(Span<const int> values);

  [[nodiscard]] bool empty() const noexcept;
  [[nodiscard]] bool singleton() const noexcept;
  [[nodiscard]] int min() const;
  [[nodiscard]] int max() const;
  [[nodiscard]] std::uint64_t cardinality() const noexcept;
  [[nodiscard]] bool contains(int value) const noexcept;
  [[nodiscard]] bool contains(const ValueSet& other) const noexcept;
  [[nodiscard]] bool disjoint(const ValueSet& other) const noexcept;
  [[nodiscard]] std::optional<int> singleton_value() const noexcept;
  [[nodiscard]] std::vector<IntRange> ranges() const;
  [[nodiscard]] ValueSet intersected(const ValueSet& other) const;
  [[nodiscard]] ValueSet without(int value) const;
  [[nodiscard]] std::string to_string() const;
  [[nodiscard]] const void* storage_id() const noexcept;
  [[nodiscard]] std::size_t referenced_dynamic_bytes() const noexcept;
  [[nodiscard]] std::size_t dynamic_bytes_if_unique() const noexcept;

  friend bool operator==(const ValueSet& lhs, const ValueSet& rhs) noexcept;
  friend bool operator!=(const ValueSet& lhs, const ValueSet& rhs) noexcept {
    return !(lhs == rhs);
  }

 private:
  struct EmptyTag {};
  struct Singleton {
    int value;
    friend constexpr bool operator==(const Singleton& lhs,
                                     const Singleton& rhs) noexcept {
      return lhs.value == rhs.value;
    }
  };
  using SharedRanges = std::shared_ptr<const std::vector<IntRange>>;
  using Storage = std::variant<EmptyTag, Singleton, IntRange, SharedRanges>;

  Storage storage_ = EmptyTag{};

  static std::vector<IntRange> normalize(Span<const IntRange> ranges);
  explicit ValueSet(std::vector<IntRange>&& normalized);
};

}  // namespace dashed
