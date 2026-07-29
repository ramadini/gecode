#pragma once

#include <cstddef>
#include <type_traits>
#include <vector>

namespace dashed {

/**
 * Minimal C++17 span used by the standalone domain kernel.
 *
 * Dashed intentionally follows Gecode 6.4's C++17 baseline instead of
 * requiring std::span and therefore C++20.
 */
template <class T>
class Span {
 public:
  using element_type = T;
  using value_type = std::remove_cv_t<T>;
  using pointer = T*;
  using reference = T&;
  using iterator = pointer;

  constexpr Span() noexcept = default;
  constexpr Span(pointer data, std::size_t size) noexcept
      : data_(data), size_(size) {}

  template <class U, class Allocator,
            std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>, int> = 0>
  Span(std::vector<U, Allocator>& values) noexcept
      : data_(values.data()), size_(values.size()) {}

  template <class U, class Allocator,
            std::enable_if_t<std::is_convertible_v<const U (*)[], T (*)[]>, int> = 0>
  Span(const std::vector<U, Allocator>& values) noexcept
      : data_(values.data()), size_(values.size()) {}

  template <class U, std::size_t N,
            std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>, int> = 0>
  constexpr Span(U (&values)[N]) noexcept : data_(values), size_(N) {}

  [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }
  [[nodiscard]] constexpr std::size_t size() const noexcept { return size_; }
  [[nodiscard]] constexpr pointer data() const noexcept { return data_; }
  [[nodiscard]] constexpr iterator begin() const noexcept { return data_; }
  [[nodiscard]] constexpr iterator end() const noexcept { return data_ + size_; }
  [[nodiscard]] constexpr reference operator[](std::size_t index) const noexcept {
    return data_[index];
  }

 private:
  pointer data_ = nullptr;
  std::size_t size_ = 0;
};

}  // namespace dashed
