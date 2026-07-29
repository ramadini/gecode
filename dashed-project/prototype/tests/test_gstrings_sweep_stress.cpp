#include "dashed/dashed.hpp"
#include "dashed/detail/sweep.hpp"

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
using dashed::Segment;
using dashed::ValueSet;
using dashed::detail::SweepAnalysis;
using dashed::detail::SweepStatus;

constexpr int kLegacyMaximumSymbol = 1000;

struct Row {
  std::string id;
  std::string status;
  std::string classification;
  std::string left;
  std::string right;
};

std::vector<int> encode(std::string_view text) {
  std::vector<int> values;
  values.reserve(text.size());
  for (unsigned char value : text) {
    values.push_back(static_cast<int>(value));
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

std::string assigned_summary(const Domain& domain) {
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
  return Domain::fixed(encode(text));
}

Domain test21_left() {
  constexpr int block_count = 50;
  std::vector<Segment> segments;
  segments.reserve(block_count);
  for (int index = 0; index < block_count; ++index) {
    segments.push_back(
        RepeatSegment{
            ValueSet(static_cast<int>('a') + index),
            1,
            block_count});
  }
  return Domain(std::move(segments), 0, block_count);
}

Domain test21_right() {
  constexpr int block_count = 100;
  std::vector<Segment> segments;
  segments.reserve(block_count);
  for (int index = 0; index < block_count; ++index) {
    segments.push_back(
        RepeatSegment{
            ValueSet(
                static_cast<int>('a') + index,
                static_cast<int>('a') + block_count - 1),
            0,
            block_count});
  }
  return Domain(std::move(segments), 0, block_count);
}

Domain test23_left() {
  std::vector<Segment> segments;
  segments.push_back(
      RepeatSegment{
          ValueSet(0, kLegacyMaximumSymbol),
          43,
          74});
  segments.push_back(
      LiteralSegment{
          LiteralSlice(encode("bT]?e]qCu|B,LSV!W("))});
  segments.push_back(
      RepeatSegment{
          ValueSet(0, kLegacyMaximumSymbol),
          0,
          31});
  return Domain(std::move(segments), 0, 100000);
}

Domain test23_right() {
  std::vector<Segment> segments;
  segments.push_back(
      LiteralSegment{
          LiteralSlice(encode(
              ":?@NbT;^AZR3IuW3ee:)DpBr%&C]=x=BqcG8[Pe.Uj`"))});
  segments.push_back(
      RepeatSegment{
          ValueSet(0, kLegacyMaximumSymbol),
          11,
          11});
  segments.push_back(
      LiteralSegment{
          LiteralSlice(encode("]c4]?e]qCu|B,LSV!W(e:"))});
  segments.push_back(
      RepeatSegment{
          ValueSet(0, kLegacyMaximumSymbol),
          0,
          17});
  return Domain(std::move(segments), 0, 100000);
}

bool compatible_fixed_value(
    std::vector<int>& fixed,
    std::size_t position,
    int value) {
  assert(position < fixed.size());
  if (fixed[position] == -1 || fixed[position] == value) {
    fixed[position] = value;
    return true;
  }
  return false;
}

bool place_fixed(
    std::vector<int>& fixed,
    std::size_t begin,
    std::string_view text) {
  for (std::size_t index = 0; index < text.size(); ++index) {
    if (!compatible_fixed_value(
            fixed,
            begin + index,
            static_cast<unsigned char>(text[index]))) {
      return false;
    }
  }
  return true;
}

bool test23_has_concrete_witness() {
  constexpr std::string_view x_fixed = "bT]?e]qCu|B,LSV!W(";
  constexpr std::string_view y_prefix =
      ":?@NbT;^AZR3IuW3ee:)DpBr%&C]=x=BqcG8[Pe.Uj`";
  constexpr std::string_view y_fixed = "]c4]?e]qCu|B,LSV!W(e:";

  for (std::size_t x_prefix = 43; x_prefix <= 74; ++x_prefix) {
    for (std::size_t x_suffix = 0; x_suffix <= 31; ++x_suffix) {
      const std::size_t length =
          x_prefix + x_fixed.size() + x_suffix;

      for (std::size_t y_suffix = 0; y_suffix <= 17; ++y_suffix) {
        const std::size_t y_length =
            y_prefix.size() + 11 + y_fixed.size() + y_suffix;
        if (length != y_length) {
          continue;
        }

        std::vector<int> fixed(length, -1);
        if (!place_fixed(fixed, x_prefix, x_fixed)) {
          continue;
        }
        if (!place_fixed(fixed, 0, y_prefix)) {
          continue;
        }
        if (!place_fixed(
                fixed,
                y_prefix.size() + 11,
                y_fixed)) {
          continue;
        }
        return true;
      }
    }
  }
  return false;
}

void add_test21(std::vector<Row>& rows) {
  const Domain left = test21_left();
  const Domain right = test21_right();

  std::vector<int> expected_values;
  expected_values.reserve(50);
  for (int index = 0; index < 50; ++index) {
    expected_values.push_back(static_cast<int>('a') + index);
  }
  const Domain expected = Domain::fixed(std::move(expected_values));

  // The global maximum forces all 50 lower-bounded blocks to occur once.
  // The post-tightening normalization pass must canonicalize that sequence.
  assert(left == expected);
  assert(left.segment_count() == 1);

  const Domain before_left = left;
  const Domain before_right = right;
  SweepAnalysis analysis;
  const SweepStatus status =
      dashed::detail::analyze_repeat_sweep(
          left,
          right,
          analysis);

  assert(status == SweepStatus::feasible);
  assert(analysis.blocks.size() == 50);
  assert(left == before_left);
  assert(right == before_right);

  rows.push_back(Row{
      "str_test2.test21",
      "sat",
      "match-sweep-feasible-canonicalized",
      assigned_summary(left),
      "analysis-blocks=50 target-segments=100"});
}

void add_test23(std::vector<Row>& rows) {
  const Domain left = test23_left();
  const Domain right = test23_right();
  const Domain before_left = left;
  const Domain before_right = right;

  SweepAnalysis analysis;
  const SweepStatus status =
      dashed::detail::analyze_repeat_sweep(
          left,
          right,
          analysis);

  // This reproduces the historical distinction: check_sweep succeeds even
  // though the two complete languages are disjoint. Sweep feasibility is a
  // structural cursor property, not a language-intersection oracle.
  assert(status == SweepStatus::feasible);
  assert(analysis.blocks.size() == 20);
  assert(!test23_has_concrete_witness());
  assert(left == before_left);
  assert(right == before_right);

  Domain refined = left;
  assert(
      dashed::detail::project_repeat_values(
          left,
          right,
          refined) == SweepStatus::unsupported);
  assert(refined == left);

  refined = left;
  assert(
      dashed::detail::project_repeat_regions(
          left,
          right,
          refined) == SweepStatus::unsupported);
  assert(refined == left);

  refined = left;
  assert(
      dashed::detail::project_against_exact_target(
          left,
          right,
          refined) == SweepStatus::unsupported);
  assert(refined == left);

  rows.push_back(Row{
      "str_test2.test23",
      "unsat",
      "match-low-level-sweep-feasible-non-oracle",
      "analysis=feasible blocks=20",
      "exact-language=disjoint"});
}

void write_report(
    const std::vector<Row>& rows,
    const std::string& path) {
  std::ofstream output(path);
  assert(output);
  output << "case\tstatus\tclassification\tleft\tright\n";
  for (const Row& row : rows) {
    output << row.id << '\t'
           << row.status << '\t'
           << row.classification << '\t'
           << row.left << '\t'
           << row.right << '\n';
  }
}

}  // namespace

int main(int argc, char** argv) {
  std::vector<Row> rows;
  add_test21(rows);
  add_test23(rows);

  if (argc == 3 && std::string(argv[1]) == "--report") {
    write_report(rows, argv[2]);
  } else {
    assert(argc == 1);
  }

  std::cout
      << "Lower-level G-Strings sweep stress differential passed: "
      << rows.size() << " cases\n";
  return 0;
}
