#include "dashed/dashed.hpp"

#include <cassert>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {

using dashed::Domain;
using dashed::LiteralSegment;
using dashed::LiteralSlice;
using dashed::RepeatSegment;
using dashed::ValueSet;

constexpr int kLegacyMaximumSymbol = 1000;

struct Row {
  std::string id;
  std::string status;
  std::string classification;
  std::string left;
  std::string right;
};

std::vector<int> encode_legacy(std::string_view text) {
  std::vector<int> values;
  values.reserve(text.size());
  for (unsigned char value : text) {
    values.push_back(
        value == static_cast<unsigned char>('=')
            ? kLegacyMaximumSymbol
            : static_cast<int>(value));
  }
  return values;
}

std::uint64_t hash_values(const std::vector<int>& values) {
  std::uint64_t hash = UINT64_C(1469598103934665603);
  for (int value : values) {
    const std::uint32_t bits = static_cast<std::uint32_t>(value);
    for (unsigned int shift = 0; shift < 32; shift += 8) {
      hash ^= static_cast<std::uint8_t>(bits >> shift);
      hash *= UINT64_C(1099511628211);
    }
  }
  return hash;
}

std::string summary(const Domain& domain) {
  assert(domain.assigned());
  const std::vector<int> values = domain.value();
  std::ostringstream output;
  output << "len=" << values.size()
         << " segments=" << domain.segment_count()
         << " hash=" << std::hex << std::setw(16) << std::setfill('0')
         << hash_values(values);
  return output.str();
}

Domain fixed_text(std::string_view text) {
  return Domain::fixed(encode_legacy(text));
}

Domain prefix_boundary_domain() {
  return Domain(
      {
          RepeatSegment{
              ValueSet(0, kLegacyMaximumSymbol),
              1,
              dashed::kUnboundedLength},
          RepeatSegment{
              ValueSet(0, kLegacyMaximumSymbol - 1),
              0,
              dashed::kUnboundedLength},
      },
      1,
      dashed::kUnboundedLength);
}

Domain suffix_boundary_domain() {
  return Domain(
      {
          RepeatSegment{
              ValueSet(0, kLegacyMaximumSymbol),
              0,
              dashed::kUnboundedLength},
          RepeatSegment{
              ValueSet(0, kLegacyMaximumSymbol - 1),
              1,
              dashed::kUnboundedLength},
      },
      1,
      dashed::kUnboundedLength);
}

void check_fixed_boundary(
    const std::string& id,
    const std::vector<int>& values,
    bool prefix_boundary,
    const std::string& classification,
    std::vector<Row>* rows) {
  Domain left = Domain::fixed(values);
  Domain right = prefix_boundary
      ? prefix_boundary_domain()
      : suffix_boundary_domain();

  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());
  assert(result.subsumed);
  assert(left.assigned());
  assert(right.assigned());
  assert(left.assigned_equal(right));
  assert(left == right);
  assert(right.segment_count() == 1);

  if (rows != nullptr) {
    rows->push_back(Row{
        id,
        "sat",
        classification,
        summary(left),
        summary(right)});
  }
}

std::vector<int> long_test19_literal() {
  std::vector<int> values;
  values.reserve(8193);
  for (std::size_t index = 0; index < 8193; ++index) {
    values.push_back(static_cast<int>('0' + (index % 10)));
  }
  values[4096] = kLegacyMaximumSymbol;
  return values;
}

void add_test17(std::vector<Row>& rows) {
  check_fixed_boundary(
      "str_test2.test17",
      encode_legacy("0=1"),
      true,
      "match-exact",
      &rows);
}

void add_test18(std::vector<Row>& rows) {
  constexpr std::string_view fixed =
      "Zz1xJr2yi3kD0njK4mjOWu5HaEnNDg9Ha7o8AniRkfU66m8EpyDsD5yYEF4PAtfcK1fI2aO0xPMfA1gtXhV685G9bVD6MX7urD0Uxq5P2lGERM6iqQYjpgZhuRMNDUCccQHMcnGvUofrvJskrn2vbrKFwvPaNcKlLnqQql7Ut39SxWLnS0kcASqfnDKMLiQLTvXOkcZ0 =09GXSOVfpYV3pELJMYcqwPm5H1O0IQXnE8o3KvItPWQFQopN";
  check_fixed_boundary(
      "str_test2.test18",
      encode_legacy(fixed),
      false,
      "match-exact",
      &rows);
}

void add_test19(std::vector<Row>& rows) {
  check_fixed_boundary(
      "str_test2.test19",
      long_test19_literal(),
      true,
      "match-source-equivalent-stress",
      &rows);
}

void add_test20(std::vector<Row>& rows) {
  Domain left = fixed_text("ab =ab");
  Domain right(
      {
          LiteralSegment{LiteralSlice(encode_legacy("ab ="))},
          RepeatSegment{ValueSet(static_cast<int>('0')), 0, 2},
          RepeatSegment{ValueSet(static_cast<int>('a')), 1, 1},
          RepeatSegment{ValueSet(static_cast<int>('b')), 1, 1},
      },
      6,
      8);

  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());
  assert(result.subsumed);
  assert(left == right);
  assert(right.segment_count() == 1);

  rows.push_back(Row{
      "str_test2.test20",
      "sat",
      "match-exact-canonical",
      summary(left),
      summary(right)});
}

void add_test22(std::vector<Row>& rows) {
  Domain left(
      {
          RepeatSegment{ValueSet(static_cast<int>('a')), 1, 2},
          RepeatSegment{ValueSet(static_cast<int>('b')), 1, 1},
      },
      2,
      3);
  Domain right(
      {
          RepeatSegment{ValueSet(static_cast<int>('a')), 1, 1},
          RepeatSegment{ValueSet(
              static_cast<int>('a'), static_cast<int>('b')), 2, 3},
      },
      3,
      4);

  const auto result = dashed::propagate_equal(left, right);
  assert(!result.failed());
  assert(result.subsumed);
  const Domain expected(
      {
          RepeatSegment{ValueSet(static_cast<int>('a')), 2, 2},
          RepeatSegment{ValueSet(static_cast<int>('b')), 1, 1},
      },
      3,
      3);
  assert(left == expected);
  assert(right == expected);
  assert(left.value() == encode_legacy("aab"));

  rows.push_back(Row{
      "str_test2.test22",
      "sat",
      "match-exact-assigned",
      summary(left),
      summary(right)});
}

int hex_digit(char value) {
  if (value >= '0' && value <= '9') return value - '0';
  if (value >= 'a' && value <= 'f') return value - 'a' + 10;
  if (value >= 'A' && value <= 'F') return value - 'A' + 10;
  return -1;
}

std::string decode_hex(const std::string& encoded) {
  assert(encoded.size() % 2 == 0);
  std::string result;
  result.reserve(encoded.size() / 2);
  for (std::size_t index = 0; index < encoded.size(); index += 2) {
    const int high = hex_digit(encoded[index]);
    const int low = hex_digit(encoded[index + 1]);
    assert(high >= 0 && low >= 0);
    result.push_back(static_cast<char>((high << 4) | low));
  }
  return result;
}

void check_source_fixtures(const std::string& path) {
  std::ifstream input(path);
  assert(input.good());
  std::string line;
  std::size_t checked = 0;
  while (std::getline(input, line)) {
    const std::size_t tab = line.find('\t');
    assert(tab != std::string::npos);
    const std::string id = line.substr(0, tab);
    const std::string text = decode_hex(line.substr(tab + 1));
    if (id == "str_test2.test17") {
      check_fixed_boundary(id, encode_legacy(text), true, "", nullptr);
    } else if (id == "str_test2.test18") {
      check_fixed_boundary(id, encode_legacy(text), false, "", nullptr);
    } else if (id == "str_test2.test19") {
      check_fixed_boundary(id, encode_legacy(text), true, "", nullptr);
    } else if (id == "str_test2.test20") {
      assert(text == "ab =ab");
      Domain left = fixed_text(text);
      Domain right(
          {
              LiteralSegment{LiteralSlice(encode_legacy("ab ="))},
              RepeatSegment{ValueSet(static_cast<int>('0')), 0, 2},
              RepeatSegment{ValueSet(static_cast<int>('a')), 1, 1},
              RepeatSegment{ValueSet(static_cast<int>('b')), 1, 1},
          },
          6,
          8);
      const auto result = dashed::propagate_equal(left, right);
      assert(!result.failed());
      assert(left == right);
    } else {
      assert(false && "unexpected source fixture id");
    }
    ++checked;
  }
  assert(checked == 4);
  std::cerr << "Exact legacy fixed-literal fixtures passed: "
            << checked << " cases\n";
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
  if (argc == 3 && std::string(argv[1]) == "--source-fixtures") {
    check_source_fixtures(argv[2]);
    return 0;
  }

  std::vector<Row> rows;
  add_test17(rows);
  add_test18(rows);
  add_test19(rows);
  add_test20(rows);
  add_test22(rows);

  if (argc == 1) {
    write_report(rows, std::cout);
  } else if (argc == 3 && std::string(argv[1]) == "--report") {
    std::ofstream report(argv[2]);
    assert(report.good());
    write_report(rows, report);
    assert(report.good());
  } else {
    std::cerr << "usage: " << argv[0]
              << " [--report PATH | --source-fixtures PATH]\n";
    return 2;
  }

  std::cerr << "Fixed-literal G-Strings differential passed: "
            << rows.size() << " cases\n";
  return 0;
}
