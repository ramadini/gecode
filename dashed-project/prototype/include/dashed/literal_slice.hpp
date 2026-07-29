#pragma once

#include "dashed/span.hpp"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

namespace dashed {

/**
 * Zero-copy view of an immutable exact integer sequence.
 *
 * A long fixed sequence is represented by one shared vector and one small
 * slice descriptor, rather than one dashed block per element.
 */
class LiteralSlice {
 public:
  using Storage = std::shared_ptr<const std::vector<int>>;

  LiteralSlice() = default;
  explicit LiteralSlice(std::vector<int> values);
  LiteralSlice(Storage storage, std::size_t offset, std::size_t size);

  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
  [[nodiscard]] std::size_t size() const noexcept { return size_; }
  [[nodiscard]] int operator[](std::size_t index) const;
  [[nodiscard]] Span<const int> span() const noexcept;
  [[nodiscard]] LiteralSlice slice(std::size_t offset,
                                   std::size_t size) const;
  [[nodiscard]] const Storage& storage() const noexcept { return storage_; }
  [[nodiscard]] std::size_t offset() const noexcept { return offset_; }
  [[nodiscard]] bool shares_storage_with(const LiteralSlice& other) const noexcept;
  [[nodiscard]] bool contiguous_with(const LiteralSlice& other) const noexcept;
  [[nodiscard]] LiteralSlice merged_with(const LiteralSlice& other) const;
  [[nodiscard]] const void* storage_id() const noexcept { return storage_.get(); }
  [[nodiscard]] std::size_t referenced_dynamic_bytes() const noexcept;
  [[nodiscard]] std::size_t dynamic_bytes_if_unique() const noexcept;

  friend bool operator==(const LiteralSlice& lhs,
                         const LiteralSlice& rhs) noexcept;

 private:
  Storage storage_;
  std::size_t offset_ = 0;
  std::size_t size_ = 0;
};

}  // namespace dashed
