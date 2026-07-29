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

Language intersection(Language left, const Language& right) {
  Language result;
  std::set_intersection(left.begin(), left.end(), right.begin(), right.end(),
                        std::inserter(result, result.end()));
  return result;
}

bool contains_all(const Language& superset, const Language& subset) {
  return std::includes(superset.begin(), superset.end(),
                       subset.begin(), subset.end());
}

void add_match(std::vector<Row>& rows,
               std::string id,
               Domain left,
               Domain right,
               const Domain& expected_left,
               const Domain& expected_right) {
  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());
  assert(left == expected_left);
  assert(right == expected_right);
  rows.push_back(Row{std::move(id), "sat", "match",
                     left.to_string(), right.to_string()});
}

void add_test03(std::vector<Row>& rows) {
  Domain left({repeat('a', 'c', 0, 300), repeat('a', 'b', 1, 1)}, 1, 301);
  Domain right({repeat('a', 'c', 0, 300), repeat('b', 'c', 0, 200)}, 0, 500);
  const Domain expected_left = left;
  const Domain expected_right(
      {repeat('a', 'c', 0, 300), repeat('b', 'c', 0, 200)}, 1, 301);
  add_match(rows, "str_test2.test03", std::move(left), std::move(right),
            expected_left, expected_right);
}

void add_test04(std::vector<Row>& rows) {
  const Domain original_left(
      {repeat('a', 'b', 1, 1), repeat('a', 'b', 1, 1),
       repeat('c', 'd', 1, 1)},
      3, 3);
  const Domain original_right(
      {repeat('a', 'd', 0, 2), singleton('d', 1, 1)}, 1, 3);

  Domain left = original_left;
  Domain right = original_right;
  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());

  const Domain exact(
      {repeat('a', 'b', 2, 2), singleton('d', 1, 1)}, 3, 3);
  assert(left == exact);
  assert(right == exact);

  const Domain legacy_expected(
      {repeat('a', 'b', 1, 1), singleton('d', 1, 1),
       repeat('c', 'd', 1, 1)},
      3, 3);

  const std::vector<int> alphabet{
      symbol('a'), symbol('b'), symbol('c'), symbol('d')};
  const Language common = intersection(
      language(original_left, alphabet, 3),
      language(original_right, alphabet, 3));
  assert(language(exact, alphabet, 3) == common);
  assert(language(legacy_expected, alphabet, 3) != common);

  // The historical expected domain both drops valid strings and admits
  // strings that were not in the original equality intersection.
  assert(common.count({symbol('a'), symbol('a'), symbol('d')}) == 1);
  assert(!legacy_expected.accepts(
      Span<const int>(std::vector<int>{
          symbol('a'), symbol('a'), symbol('d')})));
  assert(legacy_expected.accepts(
      Span<const int>(std::vector<int>{
          symbol('a'), symbol('d'), symbol('c')})));
  assert(common.count({symbol('a'), symbol('d'), symbol('c')}) == 0);

  rows.push_back(Row{"str_test2.test04", "sat",
                     "corrected-legacy-unsound",
                     left.to_string(), right.to_string()});
}

void add_test05(std::vector<Row>& rows) {
  Domain left(
      {repeat('a', 'b', 1, 1), repeat('e', 'f', 1, 1),
       repeat('c', 'd', 1, 1)},
      3, 3);
  Domain right(
      {repeat('a', 'd', 0, 2), singleton('d', 1, 1)}, 1, 3);
  const auto result = dashed::propagate_equal(left, right);
  assert(result.failed());
  rows.push_back(Row{"str_test2.test05", "unsat", "match-unsat",
                     "FAIL", "FAIL"});
}

void add_test06(std::vector<Row>& rows) {
  const Domain original_left(
      {repeat('x', 'z', 1, 1), repeat('a', 'b', 0, 2),
       singleton('y', 1, 1)},
      2, 4);
  const Domain original_right(
      {repeat('x', 'z', 1, 1), repeat('a', 'c', 0, 1),
       repeat('b', 'c', 1, 1), repeat('x', 'z', 1, 1)},
      3, 4);

  Domain left = original_left;
  Domain right = original_right;
  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());

  const Domain exact_common(
      {repeat('x', 'z', 1, 1), repeat('a', 'b', 0, 1),
       singleton('b', 1, 1), singleton('y', 1, 1)},
      3, 4);
  assert(right == exact_common);

  const std::vector<int> alphabet{
      symbol('a'), symbol('b'), symbol('c'),
      symbol('x'), symbol('y'), symbol('z')};
  const Language common = intersection(
      language(original_left, alphabet, 4),
      language(original_right, alphabet, 4));
  const Language left_after = language(left, alphabet, 4);
  const Language right_after = language(right, alphabet, 4);

  assert(right_after == common);
  assert(contains_all(left_after, common));
  assert(left_after.size() > common.size());

  // This witness is retained only by the weaker left projection.
  const std::vector<int> extra{
      symbol('x'), symbol('a'), symbol('a'), symbol('y')};
  assert(left.accepts(Span<const int>(extra)));
  assert(common.count(extra) == 0);

  rows.push_back(Row{"str_test2.test06", "sat", "sound-weaker-left",
                     left.to_string(), right.to_string()});
}

void add_test07(std::vector<Row>& rows) {
  Domain left(
      {repeat('a', 'b', 0, 2), repeat('a', 'd', 1, 1),
       repeat('f', 'h', 0, 2)},
      1, 5);
  Domain right(
      {repeat('a', 'b', 1, 1), repeat('c', 'd', 1, 1)}, 2, 2);
  const Domain expected = right;
  add_match(rows, "str_test2.test07", std::move(left), std::move(right),
            expected, expected);
}

void add_test08(std::vector<Row>& rows) {
  Domain left(
      {repeat('a', 'd', 0, 2), repeat('c', 'd', 1, 1)}, 1, 3);
  Domain right(
      {repeat('b', 'c', 1, 1), repeat('a', 'd', 0, 2)}, 1, 3);
  const Domain expected_left = left;
  const Domain expected_right = right;
  add_match(rows, "str_test2.test08", std::move(left), std::move(right),
            expected_left, expected_right);
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
  add_test03(rows);
  add_test04(rows);
  add_test05(rows);
  add_test06(rows);
  add_test07(rows);
  add_test08(rows);

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

  std::cerr << "Extended G-Strings equality differential passed: "
            << rows.size() << " cases\n";
  return 0;
}
