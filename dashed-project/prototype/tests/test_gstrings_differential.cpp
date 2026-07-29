#include "dashed/dashed.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

using dashed::Domain;
using dashed::RepeatSegment;
using dashed::ValueSet;

struct DifferentialRow {
  std::string id;
  std::string status;
  std::string left;
  std::string right;
};

int symbol(char value) {
  assert(value >= 'a' && value <= 'z');
  return static_cast<int>(value - 'a') + 1;
}

ValueSet symbols(char lower, char upper) {
  return ValueSet(symbol(lower), symbol(upper));
}

RepeatSegment repeat(char lower, char upper,
                     dashed::Length minimum,
                     dashed::Length maximum) {
  return RepeatSegment{symbols(lower, upper), minimum, maximum};
}

RepeatSegment singleton(char value,
                        dashed::Length minimum,
                        dashed::Length maximum) {
  return repeat(value, value, minimum, maximum);
}

void propagate_twice(Domain& left, Domain& right) {
  const auto first = dashed::propagate_equal(left, right);
  assert(!first.failed());
  const auto second = dashed::propagate_equal(left, right);
  assert(!second.failed());
}

void add_exact_case(std::vector<DifferentialRow>& rows,
                    std::string id,
                    Domain left,
                    Domain right,
                    const Domain& expected_left,
                    const Domain& expected_right) {
  propagate_twice(left, right);
  assert(left == expected_left);
  assert(right == expected_right);
  rows.push_back(DifferentialRow{
      std::move(id),
      "sat",
      left.to_string(),
      right.to_string(),
  });
}

void add_test01(std::vector<DifferentialRow>& rows) {
  Domain left = Domain::repeat(symbols('a', 'b'), 0, 4);
  Domain right(
      {
          repeat('a', 'b', 0, 2),
          singleton('a', 3, 3),
      },
      3,
      5);
  const Domain expected(
      {
          repeat('a', 'b', 0, 1),
          singleton('a', 3, 3),
      },
      3,
      4);
  add_exact_case(rows, "str_test2.test01", std::move(left),
                 std::move(right), expected, expected);
}

void add_test02(std::vector<DifferentialRow>& rows) {
  constexpr dashed::Length n = 10;
  Domain left(
      {
          repeat('a', 'c', 0, 30 * n),
          singleton('d', 5 * n, 5 * n),
          repeat('c', 'f', 0, 2 * n),
      },
      5 * n,
      37 * n);
  Domain right(
      {
          repeat('b', 'd', 26 * n, 26 * n),
          singleton('f', n, n),
      },
      27 * n,
      27 * n);
  const Domain expected_left(
      {
          repeat('b', 'c', 20 * n, 21 * n),
          singleton('d', 5 * n, 5 * n),
          repeat('c', 'd', 0, n),
          singleton('f', n, n),
      },
      27 * n,
      27 * n);
  const Domain expected_right(
      {
          repeat('b', 'd', 26 * n, 26 * n),
          singleton('f', n, n),
      },
      27 * n,
      27 * n);
  add_exact_case(rows, "str_test2.test02", std::move(left),
                 std::move(right), expected_left, expected_right);
}

void add_test09(std::vector<DifferentialRow>& rows) {
  Domain left(
      {
          repeat('a', 'c', 0, 500),
          repeat('d', 'e', 0, 300),
      },
      0,
      800);
  Domain right(
      {
          repeat('a', 'b', 0, 200),
          repeat('c', 'd', 0, 900),
      },
      0,
      1100);
  const Domain expected_left(
      {
          repeat('a', 'c', 0, 500),
          singleton('d', 0, 300),
      },
      0,
      800);
  const Domain expected_right(
      {
          repeat('a', 'b', 0, 200),
          repeat('c', 'd', 0, 800),
      },
      0,
      800);
  add_exact_case(rows, "str_test2.test09", std::move(left),
                 std::move(right), expected_left, expected_right);
}

void add_test11(std::vector<DifferentialRow>& rows) {
  Domain left = Domain::repeat(symbols('a', 'c'), 0, 400);
  Domain right = Domain::repeat(symbols('d', 'd'), 0, 900);
  const Domain expected = Domain::empty_list();
  add_exact_case(rows, "str_test2.test11", std::move(left),
                 std::move(right), expected, expected);
}

void add_test14(std::vector<DifferentialRow>& rows) {
  Domain left(
      {
          repeat('a', 'b', 0, 6),
          singleton('c', 1, 1),
          singleton('d', 1, 1),
      },
      2,
      8);
  Domain right(
      {
          repeat('a', 'c', 0, 5),
          singleton('d', 0, 1),
      },
      0,
      6);
  const Domain expected(
      {
          repeat('a', 'b', 0, 4),
          singleton('c', 1, 1),
          singleton('d', 1, 1),
      },
      2,
      6);
  add_exact_case(rows, "str_test2.test14", std::move(left),
                 std::move(right), expected, expected);
}

void add_test16(std::vector<DifferentialRow>& rows) {
  Domain left(
      {
          repeat('a', 'c', 0, 3),
          singleton('d', 1, 1),
          repeat('c', 'f', 0, 2),
      },
      1,
      6);
  Domain right(
      {
          repeat('b', 'd', 0, 3),
          singleton('f', 1, 1),
      },
      1,
      4);
  const Domain expected_left(
      {
          repeat('b', 'c', 0, 2),
          singleton('d', 1, 1),
          repeat('c', 'd', 0, 1),
          singleton('f', 1, 1),
      },
      2,
      4);
  const Domain expected_right(
      {
          repeat('b', 'd', 1, 3),
          singleton('f', 1, 1),
      },
      2,
      4);
  add_exact_case(rows, "str_test2.test16", std::move(left),
                 std::move(right), expected_left, expected_right);
}

void write_report(const std::vector<DifferentialRow>& rows,
                  std::ostream& output) {
  output << "case\tstatus\tleft\tright\n";
  for (const DifferentialRow& row : rows) {
    output << row.id << '\t'
           << row.status << '\t'
           << row.left << '\t'
           << row.right << '\n';
  }
}

}  // namespace

int main(int argc, char** argv) {
  std::vector<DifferentialRow> rows;
  add_test01(rows);
  add_test02(rows);
  add_test09(rows);
  add_test11(rows);
  add_test14(rows);
  add_test16(rows);

  if (argc == 1) {
    write_report(rows, std::cout);
  } else if (argc == 3 && std::string(argv[1]) == "--report") {
    std::ofstream report(argv[2]);
    assert(report.good());
    write_report(rows, report);
    assert(report.good());
  } else {
    std::cerr << "usage: " << argv[0] << " [--report PATH]\n";
    return 2;
  }

  std::cerr << "G-Strings equality differential fixtures passed: "
            << rows.size() << " cases\n";
  return 0;
}
