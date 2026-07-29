#include "dashed/dashed.hpp"

#include <chrono>
#include <cstddef>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  std::size_t n = 100000;
  if (argc > 1) {
    n = static_cast<std::size_t>(std::stoull(argv[1]));
  }
  if (n > dashed::kUnboundedLength) {
    std::cerr << "length exceeds Dashed length representation\n";
    return 2;
  }

  std::vector<dashed::Segment> old_style;
  old_style.reserve(n + 2);
  old_style.push_back(
      dashed::RepeatSegment{dashed::ValueSet(-100, 100), 0, 10});
  for (std::size_t i = 0; i < n; ++i) {
    old_style.push_back(dashed::RepeatSegment{
        dashed::ValueSet(static_cast<int>(i % 251)), 1, 1});
  }
  old_style.push_back(
      dashed::RepeatSegment{dashed::ValueSet(-100, 100), 0, 10});

  const auto start = std::chrono::steady_clock::now();
  dashed::Domain compact(
      std::move(old_style), static_cast<dashed::Length>(n),
      static_cast<dashed::Length>(n + 20));
  const auto stop = std::chrono::steady_clock::now();

  std::cout << "fixed_unit_blocks_input=" << n << '\n'
            << "segments_after_normalization=" << compact.segment_count()
            << '\n'
            << "structural_bytes=" << compact.structural_bytes() << '\n'
            << "referenced_dynamic_bytes="
            << compact.referenced_dynamic_bytes() << '\n'
            << "elapsed_us="
            << std::chrono::duration_cast<std::chrono::microseconds>(stop - start)
                   .count()
            << '\n';

  return compact.failed() || compact.segment_count() != 3 ? 1 : 0;
}
