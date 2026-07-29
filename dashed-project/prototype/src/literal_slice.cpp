#include "dashed/literal_slice.hpp"

#include <algorithm>

namespace dashed {

LiteralSlice::LiteralSlice(std::vector<int> values)
    : storage_(std::make_shared<const std::vector<int>>(std::move(values))),
      offset_(0),
      size_(storage_->size()) {}

LiteralSlice::LiteralSlice(Storage storage, std::size_t offset,
                           std::size_t size)
    : storage_(std::move(storage)), offset_(offset), size_(size) {
  if (!storage_) {
    if (offset_ != 0 || size_ != 0) {
      throw std::invalid_argument("non-empty LiteralSlice without storage");
    }
    return;
  }
  if (offset_ > storage_->size() || size_ > storage_->size() - offset_) {
    throw std::out_of_range("LiteralSlice outside backing storage");
  }
}

int LiteralSlice::operator[](std::size_t index) const {
  if (index >= size_) {
    throw std::out_of_range("LiteralSlice index out of range");
  }
  return (*storage_)[offset_ + index];
}

Span<const int> LiteralSlice::span() const noexcept {
  if (size_ == 0) {
    return {};
  }
  return Span<const int>(storage_->data() + offset_, size_);
}

LiteralSlice LiteralSlice::slice(std::size_t offset, std::size_t size) const {
  if (offset > size_ || size > size_ - offset) {
    throw std::out_of_range("LiteralSlice sub-slice outside range");
  }
  return LiteralSlice(storage_, offset_ + offset, size);
}

bool LiteralSlice::shares_storage_with(const LiteralSlice& other) const noexcept {
  return storage_ && storage_ == other.storage_;
}

bool LiteralSlice::contiguous_with(const LiteralSlice& other) const noexcept {
  return shares_storage_with(other) && offset_ + size_ == other.offset_;
}

LiteralSlice LiteralSlice::merged_with(const LiteralSlice& other) const {
  if (!contiguous_with(other)) {
    throw std::invalid_argument("literal slices are not contiguous");
  }
  return LiteralSlice(storage_, offset_, size_ + other.size_);
}

std::size_t LiteralSlice::referenced_dynamic_bytes() const noexcept {
  if (!storage_) {
    return 0;
  }
  return sizeof(std::vector<int>) + storage_->capacity() * sizeof(int);
}

std::size_t LiteralSlice::dynamic_bytes_if_unique() const noexcept {
  if (storage_ && storage_.use_count() == 1) {
    return sizeof(std::vector<int>) + storage_->capacity() * sizeof(int);
  }
  return 0;
}

bool operator==(const LiteralSlice& lhs, const LiteralSlice& rhs) noexcept {
  if (lhs.size_ != rhs.size_) {
    return false;
  }
  if (lhs.storage_ == rhs.storage_ && lhs.offset_ == rhs.offset_) {
    return true;
  }
  return std::equal(lhs.span().begin(), lhs.span().end(), rhs.span().begin());
}

}  // namespace dashed
