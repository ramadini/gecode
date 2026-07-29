#include "dashed/value_set.hpp"

#include <algorithm>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace dashed {

std::uint64_t IntRange::cardinality() const noexcept {
  if (empty()) {
    return 0;
  }
  return static_cast<std::uint64_t>(
             static_cast<std::int64_t>(upper) - static_cast<std::int64_t>(lower)) +
         1U;
}

ValueSet::ValueSet(int value) noexcept : storage_(Singleton{value}) {}

ValueSet::ValueSet(int lower, int upper) {
  if (lower > upper) {
    storage_ = EmptyTag{};
  } else if (lower == upper) {
    storage_ = Singleton{lower};
  } else {
    storage_ = IntRange{lower, upper};
  }
}

ValueSet::ValueSet(Span<const IntRange> ranges)
    : ValueSet(normalize(ranges)) {}

ValueSet::ValueSet(std::initializer_list<IntRange> ranges)
    : ValueSet(Span<const IntRange>(ranges.begin(), ranges.size())) {}

ValueSet ValueSet::from_values(Span<const int> values) {
  if (values.empty()) {
    return {};
  }
  std::vector<int> sorted(values.begin(), values.end());
  std::sort(sorted.begin(), sorted.end());
  sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());

  std::vector<IntRange> ranges;
  ranges.reserve(sorted.size());
  int lower = sorted.front();
  int upper = lower;
  for (std::size_t i = 1; i < sorted.size(); ++i) {
    if (static_cast<std::int64_t>(sorted[i]) ==
        static_cast<std::int64_t>(upper) + 1) {
      upper = sorted[i];
    } else {
      ranges.push_back({lower, upper});
      lower = upper = sorted[i];
    }
  }
  ranges.push_back({lower, upper});
  return ValueSet(std::move(ranges));
}

bool ValueSet::empty() const noexcept {
  return std::holds_alternative<EmptyTag>(storage_);
}

bool ValueSet::singleton() const noexcept {
  return std::holds_alternative<Singleton>(storage_);
}

int ValueSet::min() const {
  if (const auto* singleton = std::get_if<Singleton>(&storage_)) {
    return singleton->value;
  }
  if (const auto* interval = std::get_if<IntRange>(&storage_)) {
    return interval->lower;
  }
  if (const auto* ranges = std::get_if<SharedRanges>(&storage_)) {
    return (*ranges)->front().lower;
  }
  throw std::logic_error("minimum of empty ValueSet");
}

int ValueSet::max() const {
  if (const auto* singleton = std::get_if<Singleton>(&storage_)) {
    return singleton->value;
  }
  if (const auto* interval = std::get_if<IntRange>(&storage_)) {
    return interval->upper;
  }
  if (const auto* ranges = std::get_if<SharedRanges>(&storage_)) {
    return (*ranges)->back().upper;
  }
  throw std::logic_error("maximum of empty ValueSet");
}

std::uint64_t ValueSet::cardinality() const noexcept {
  if (std::holds_alternative<EmptyTag>(storage_)) {
    return 0;
  }
  if (std::holds_alternative<Singleton>(storage_)) {
    return 1;
  }
  if (const auto* interval = std::get_if<IntRange>(&storage_)) {
    return interval->cardinality();
  }
  std::uint64_t total = 0;
  for (const IntRange& range : **std::get_if<SharedRanges>(&storage_)) {
    const std::uint64_t count = range.cardinality();
    if (std::numeric_limits<std::uint64_t>::max() - total < count) {
      return std::numeric_limits<std::uint64_t>::max();
    }
    total += count;
  }
  return total;
}

bool ValueSet::contains(int value) const noexcept {
  if (const auto* singleton = std::get_if<Singleton>(&storage_)) {
    return singleton->value == value;
  }
  if (const auto* interval = std::get_if<IntRange>(&storage_)) {
    return interval->lower <= value && value <= interval->upper;
  }
  if (const auto* ranges = std::get_if<SharedRanges>(&storage_)) {
    const auto it = std::lower_bound(
        (*ranges)->begin(), (*ranges)->end(), value,
        [](const IntRange& range, int needle) { return range.upper < needle; });
    return it != (*ranges)->end() && it->lower <= value;
  }
  return false;
}

bool ValueSet::contains(const ValueSet& other) const noexcept {
  if (other.empty()) {
    return true;
  }
  if (empty()) {
    return false;
  }
  const auto lhs = ranges();
  const auto rhs = other.ranges();
  std::size_t i = 0;
  for (const IntRange& target : rhs) {
    while (i < lhs.size() && lhs[i].upper < target.lower) {
      ++i;
    }
    if (i == lhs.size() || lhs[i].lower > target.lower ||
        lhs[i].upper < target.upper) {
      return false;
    }
  }
  return true;
}

bool ValueSet::disjoint(const ValueSet& other) const noexcept {
  if (empty() || other.empty()) {
    return true;
  }
  const auto lhs = ranges();
  const auto rhs = other.ranges();
  std::size_t i = 0;
  std::size_t j = 0;
  while (i < lhs.size() && j < rhs.size()) {
    if (lhs[i].upper < rhs[j].lower) {
      ++i;
    } else if (rhs[j].upper < lhs[i].lower) {
      ++j;
    } else {
      return false;
    }
  }
  return true;
}

std::optional<int> ValueSet::singleton_value() const noexcept {
  if (const auto* singleton = std::get_if<Singleton>(&storage_)) {
    return singleton->value;
  }
  return std::nullopt;
}

std::vector<IntRange> ValueSet::ranges() const {
  if (const auto* singleton = std::get_if<Singleton>(&storage_)) {
    return {{singleton->value, singleton->value}};
  }
  if (const auto* interval = std::get_if<IntRange>(&storage_)) {
    return {*interval};
  }
  if (const auto* ranges = std::get_if<SharedRanges>(&storage_)) {
    return **ranges;
  }
  return {};
}

ValueSet ValueSet::intersected(const ValueSet& other) const {
  const auto lhs = ranges();
  const auto rhs = other.ranges();
  std::vector<IntRange> result;
  std::size_t i = 0;
  std::size_t j = 0;
  while (i < lhs.size() && j < rhs.size()) {
    const int lower = std::max(lhs[i].lower, rhs[j].lower);
    const int upper = std::min(lhs[i].upper, rhs[j].upper);
    if (lower <= upper) {
      result.push_back({lower, upper});
    }
    if (lhs[i].upper < rhs[j].upper) {
      ++i;
    } else {
      ++j;
    }
  }
  return ValueSet(std::move(result));
}

ValueSet ValueSet::without(int value) const {
  if (!contains(value)) {
    return *this;
  }
  std::vector<IntRange> result;
  for (const IntRange range : ranges()) {
    if (value < range.lower || value > range.upper) {
      result.push_back(range);
      continue;
    }
    if (range.lower < value) {
      result.push_back({range.lower, value - 1});
    }
    if (value < range.upper) {
      result.push_back({value + 1, range.upper});
    }
  }
  return ValueSet(std::move(result));
}

std::string ValueSet::to_string() const {
  if (empty()) {
    return "{}";
  }
  std::ostringstream out;
  out << '{';
  const auto rs = ranges();
  for (std::size_t i = 0; i < rs.size(); ++i) {
    if (i != 0) {
      out << ',';
    }
    if (rs[i].lower == rs[i].upper) {
      out << rs[i].lower;
    } else {
      out << rs[i].lower << ".." << rs[i].upper;
    }
  }
  out << '}';
  return out.str();
}

const void* ValueSet::storage_id() const noexcept {
  if (const auto* ranges = std::get_if<SharedRanges>(&storage_)) {
    return ranges->get();
  }
  return nullptr;
}

std::size_t ValueSet::referenced_dynamic_bytes() const noexcept {
  if (const auto* ranges = std::get_if<SharedRanges>(&storage_)) {
    return sizeof(std::vector<IntRange>) +
           (*ranges)->capacity() * sizeof(IntRange);
  }
  return 0;
}

std::size_t ValueSet::dynamic_bytes_if_unique() const noexcept {
  if (const auto* ranges = std::get_if<SharedRanges>(&storage_)) {
    if (ranges->use_count() == 1) {
      return sizeof(std::vector<IntRange>) +
             (*ranges)->capacity() * sizeof(IntRange);
    }
  }
  return 0;
}

bool operator==(const ValueSet& lhs, const ValueSet& rhs) noexcept {
  if (lhs.storage_.index() == rhs.storage_.index()) {
    if (std::holds_alternative<ValueSet::EmptyTag>(lhs.storage_)) {
      return true;
    }
    if (const auto* left = std::get_if<ValueSet::Singleton>(&lhs.storage_)) {
      return *left == std::get<ValueSet::Singleton>(rhs.storage_);
    }
    if (const auto* left = std::get_if<IntRange>(&lhs.storage_)) {
      return *left == std::get<IntRange>(rhs.storage_);
    }
    const auto& left = *std::get<ValueSet::SharedRanges>(lhs.storage_);
    const auto& right = *std::get<ValueSet::SharedRanges>(rhs.storage_);
    return &left == &right || left == right;
  }
  return lhs.ranges() == rhs.ranges();
}

std::vector<IntRange> ValueSet::normalize(Span<const IntRange> ranges) {
  std::vector<IntRange> sorted;
  sorted.reserve(ranges.size());
  for (const IntRange range : ranges) {
    if (!range.empty()) {
      sorted.push_back(range);
    }
  }
  std::sort(sorted.begin(), sorted.end(), [](const IntRange& lhs,
                                              const IntRange& rhs) {
    return lhs.lower < rhs.lower ||
           (lhs.lower == rhs.lower && lhs.upper < rhs.upper);
  });

  std::vector<IntRange> result;
  result.reserve(sorted.size());
  for (const IntRange range : sorted) {
    if (result.empty() ||
        static_cast<std::int64_t>(range.lower) >
            static_cast<std::int64_t>(result.back().upper) + 1) {
      result.push_back(range);
    } else {
      result.back().upper = std::max(result.back().upper, range.upper);
    }
  }
  return result;
}

ValueSet::ValueSet(std::vector<IntRange>&& normalized) {
  if (normalized.empty()) {
    storage_ = EmptyTag{};
  } else if (normalized.size() == 1) {
    const IntRange only = normalized.front();
    if (only.lower == only.upper) {
      storage_ = Singleton{only.lower};
    } else {
      storage_ = only;
    }
  } else {
    storage_ = std::make_shared<const std::vector<IntRange>>(
        std::move(normalized));
  }
}

}  // namespace dashed
