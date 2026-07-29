#pragma once

#include <cstdint>

namespace dashed {

enum class Change : std::uint8_t {
  none = 0,
  length = 1,
  domain = 2,
  both = 3,
  assigned = 4,
  failed = 5
};

[[nodiscard]] constexpr bool failed(Change change) noexcept {
  return change == Change::failed;
}

[[nodiscard]] constexpr bool changed(Change change) noexcept {
  return change != Change::none && change != Change::failed;
}

[[nodiscard]] constexpr Change combine(Change lhs, Change rhs) noexcept {
  if (lhs == Change::failed || rhs == Change::failed) {
    return Change::failed;
  }
  if (lhs == Change::assigned || rhs == Change::assigned) {
    return Change::assigned;
  }
  if (lhs == Change::both || rhs == Change::both) {
    return Change::both;
  }
  if ((lhs == Change::length && rhs == Change::domain) ||
      (lhs == Change::domain && rhs == Change::length)) {
    return Change::both;
  }
  return lhs != Change::none ? lhs : rhs;
}

}  // namespace dashed
