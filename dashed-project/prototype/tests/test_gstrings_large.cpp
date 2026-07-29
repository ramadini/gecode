#include "dashed/dashed.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace {

using dashed::Domain;
using dashed::IntRange;
using dashed::RepeatSegment;
using dashed::Span;
using dashed::ValueSet;
using Language = std::set<std::vector<int>>;

struct Row {
  std::string id;
  std::string status;
  std::string classification;
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

ValueSet sparse(std::initializer_list<IntRange> ranges) {
  return ValueSet(ranges);
}

RepeatSegment repeat(ValueSet values,
                     dashed::Length minimum,
                     dashed::Length maximum) {
  return RepeatSegment{std::move(values), minimum, maximum};
}

RepeatSegment repeat(char lower, char upper,
                     dashed::Length minimum,
                     dashed::Length maximum) {
  return repeat(symbols(lower, upper), minimum, maximum);
}

void enumerate_recursive(const Domain& domain,
                         const std::vector<int>& alphabet,
                         std::size_t target_length,
                         std::vector<int>& candidate,
                         Language& result) {
  if (candidate.size() == target_length) {
    if (domain.accepts(Span<const int>(candidate))) {
      result.insert(candidate);
    }
    return;
  }

  for (int value : alphabet) {
    candidate.push_back(value);
    enumerate_recursive(domain, alphabet, target_length, candidate, result);
    candidate.pop_back();
  }
}

Language language(const Domain& domain,
                  const std::vector<int>& alphabet,
                  std::size_t maximum_length) {
  Language result;
  std::vector<int> candidate;
  for (std::size_t length = 0; length <= maximum_length; ++length) {
    enumerate_recursive(domain, alphabet, length, candidate, result);
  }
  return result;
}

Language intersection(const Language& left, const Language& right) {
  Language result;
  std::set_intersection(left.begin(), left.end(), right.begin(), right.end(),
                        std::inserter(result, result.end()));
  return result;
}

bool contains_all(const Language& superset, const Language& subset) {
  return std::includes(superset.begin(), superset.end(),
                       subset.begin(), subset.end());
}

void check_scaled_test10() {
  const ValueSet left_tail = sparse({
      IntRange{symbol('a'), symbol('a')},
      IntRange{symbol('d'), symbol('e')},
  });
  const ValueSet right_head = sparse({
      IntRange{symbol('a'), symbol('b')},
      IntRange{symbol('d'), symbol('d')},
  });

  const Domain original_left(
      {repeat('a', 'c', 0, 2), repeat('b', 'c', 1, 1),
       repeat('b', 'c', 0, 1), repeat(left_tail, 0, 1)},
      1, 5);
  const Domain original_right(
      {repeat(right_head, 0, 1), repeat('c', 'd', 0, 6)},
      0, 7);

  Domain left = original_left;
  Domain right = original_right;
  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());

  const std::vector<int> alphabet{
      symbol('a'), symbol('b'), symbol('c'), symbol('d'), symbol('e')};
  const Language common = intersection(
      language(original_left, alphabet, 5),
      language(original_right, alphabet, 5));
  const Language left_after = language(left, alphabet, 5);
  const Language right_after = language(right, alphabet, 5);

  assert(contains_all(left_after, common));
  assert(contains_all(right_after, common));
  assert(left_after.size() > common.size());
  assert(right_after.size() > common.size());
}

void add_test10(std::vector<Row>& rows) {
  const ValueSet left_tail = sparse({
      IntRange{symbol('a'), symbol('a')},
      IntRange{symbol('d'), symbol('e')},
  });
  const ValueSet right_head = sparse({
      IntRange{symbol('a'), symbol('b')},
      IntRange{symbol('d'), symbol('d')},
  });

  Domain left(
      {repeat('a', 'c', 0, 400), repeat('b', 'c', 3, 3),
       repeat('b', 'c', 0, 97), repeat(left_tail, 0, 300)},
      3, 800);
  Domain right(
      {repeat(right_head, 0, 200), repeat('c', 'd', 0, 900)},
      0, 1100);

  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());

  const ValueSet filtered_tail = sparse({
      IntRange{symbol('a'), symbol('a')},
      IntRange{symbol('d'), symbol('d')},
  });
  const Domain expected_left(
      {repeat('a', 'c', 0, 400), repeat('b', 'c', 3, 100),
       repeat(filtered_tail, 0, 300)},
      3, 800);
  const Domain expected_right(
      {repeat(right_head, 0, 200), repeat('c', 'd', 0, 800)},
      3, 800);

  assert(left == expected_left);
  assert(right == expected_right);
  check_scaled_test10();

  rows.push_back(Row{"str_test2.test10", "sat", "match-sound-weaker-both",
                     left.to_string(), right.to_string()});
}

void check_scaled_test12() {
  const Domain original_left(
      {repeat('b', 'b', 0, 3), repeat('c', 'd', 0, 3)}, 0, 6);
  const Domain original_right(
      {repeat('b', 'c', 0, 2)}, 0, 2);

  Domain left = original_left;
  Domain right = original_right;
  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());

  const std::vector<int> alphabet{
      symbol('b'), symbol('c'), symbol('d')};
  const Language common = intersection(
      language(original_left, alphabet, 2),
      language(original_right, alphabet, 2));

  assert(language(left, alphabet, 2) == common);
  assert(language(right, alphabet, 2) == common);
}

void add_test12(std::vector<Row>& rows) {
  Domain left(
      {repeat('b', 'b', 0, 100000), repeat('c', 'd', 0, 100000)},
      0, 200000);
  Domain right({repeat('b', 'c', 0, 2000)}, 0, 2000);

  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());

  const Domain expected(
      {repeat('b', 'b', 0, 2000), repeat('c', 'c', 0, 2000)},
      0, 2000);
  assert(left == expected);
  assert(right == expected);
  check_scaled_test12();

  rows.push_back(Row{"str_test2.test12", "sat", "match-exact",
                     left.to_string(), right.to_string()});
}

void check_scaled_test13() {
  const Domain original_left({repeat('a', 'b', 0, 4)}, 0, 4);
  const Domain original_right(
      {repeat('a', 'c', 0, 4), repeat('b', 'b', 1, 1)}, 1, 4);

  Domain left = original_left;
  Domain right = original_right;
  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());

  const std::vector<int> alphabet{
      symbol('a'), symbol('b'), symbol('c')};
  const Language common = intersection(
      language(original_left, alphabet, 4),
      language(original_right, alphabet, 4));

  assert(language(left, alphabet, 4) == common);
  assert(language(right, alphabet, 4) == common);
  assert(common.count({symbol('b')}) == 1);
}

void add_test13(std::vector<Row>& rows) {
  Domain left(
      {repeat('a', 'b', 0, dashed::kUnboundedLength)},
      0, dashed::kUnboundedLength);
  Domain right(
      {repeat('a', 'c', 0, dashed::kUnboundedLength),
       repeat('b', 'b', 1, 1)},
      1, dashed::kUnboundedLength);

  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());

  const Domain expected(
      {repeat('a', 'b', 0, dashed::kUnboundedLength - 1),
       repeat('b', 'b', 1, 1)},
      1, dashed::kUnboundedLength);
  assert(left == expected);
  assert(right == expected);
  assert(left.accepts(std::vector<int>{symbol('b')}));
  check_scaled_test13();

  rows.push_back(Row{"str_test2.test13", "sat", "match-exact",
                     left.to_string(), right.to_string()});
}

void add_test15(std::vector<Row>& rows) {
  Domain left({repeat('a', 'a', 0, 6)}, 0, 6);
  Domain right(
      {repeat('a', 'a', 0, 5), repeat('a', 'a', 1, 1),
       repeat('a', 'a', 0, 5)},
      1, 11);

  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());

  const Domain expected({repeat('a', 'a', 1, 6)}, 1, 6);
  assert(left == expected);
  assert(right == expected);

  rows.push_back(Row{"str_test2.test15", "sat", "match-exact",
                     left.to_string(), right.to_string()});
}

void write_report(const std::vector<Row>& rows, std::ostream& output) {
  output << "case\tstatus\tclassification\tleft\tright\n";
  for (const Row& row : rows) {
    output << row.id << '\t' << row.status << '\t' << row.classification
           << '\t' << row.left << '\t' << row.right << '\n';
  }
}

}  // namespace

int main(int argc, char** argv) {
  std::vector<Row> rows;
  add_test10(rows);
  add_test12(rows);
  add_test13(rows);
  add_test15(rows);

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

  std::cerr << "Large/unbounded G-Strings equality differential passed: "
            << rows.size() << " cases\n";
  return 0;
}
