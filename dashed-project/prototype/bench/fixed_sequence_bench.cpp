#include "dashed/dashed.hpp"

#include <chrono>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <vector>

int main(int argc, char** argv) {
  std::size_t n = 1000000;
  if (argc > 1) {
    n = static_cast<std::size_t>(std::stoull(argv[1]));
  }

  std::vector<int> values(n);
  std::iota(values.begin(), values.end(), 0);
  const auto start = std::chrono::steady_clock::now();
  dashed::Domain domain = dashed::Domain::fixed(std::move(values));
  dashed::Domain left = domain.assigned_prefix(
      static_cast<dashed::Length>(n / 2));
  dashed::Domain right = domain.assigned_suffix(
      static_cast<dashed::Length>(n - n / 2));
  dashed::Domain joined = left.concatenated(right);
  const auto stop = std::chrono::steady_clock::now();

  std::cout << "elements=" << n << '\n'
            << "segments(original)=" << domain.segment_count() << '\n'
            << "segments(joined)=" << joined.segment_count() << '\n'
            << "structural_bytes=" << domain.structural_bytes() << '\n'
            << "referenced_dynamic_bytes="
            << domain.referenced_dynamic_bytes() << '\n'
            << "owned_dynamic_bytes=" << domain.owned_dynamic_bytes() << '\n'
            << "elapsed_us="
            << std::chrono::duration_cast<std::chrono::microseconds>(stop - start)
                   .count()
            << '\n';
  return joined == domain ? 0 : 1;
}
