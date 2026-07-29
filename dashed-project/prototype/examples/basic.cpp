#include "dashed/dashed.hpp"

#include <iostream>

int main() {
  dashed::Domain x = dashed::Domain::fixed({10, 20, 30});
  dashed::Domain y = dashed::Domain::top(dashed::ValueSet(-100, 100), 0, 5);
  dashed::Domain z = dashed::Domain::top(dashed::ValueSet(-100, 100), 0, 10);

  auto result = dashed::propagate_concat(z, x, y);
  std::cout << "x: " << x << '\n'
            << "y: " << y << '\n'
            << "z: " << z << '\n'
            << "failed: " << std::boolalpha << result.failed() << '\n';
}
