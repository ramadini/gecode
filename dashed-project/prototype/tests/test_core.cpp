#include "dashed/dashed.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <vector>

using dashed::BoolDomain;
using dashed::Change;
using dashed::Domain;
using dashed::IntBounds;
using dashed::IntRange;
using dashed::LiteralSegment;
using dashed::LiteralSlice;
using dashed::RepeatSegment;
using dashed::ValueSet;

namespace {

void test_value_set() {
  ValueSet set{{IntRange{5, 7}, IntRange{1, 2}, IntRange{3, 4},
                IntRange{10, 10}}};
  assert(set.to_string() == "{1..7,10}");
  assert(set.cardinality() == 8);
  assert(set.contains(1));
  assert(set.contains(7));
  assert(!set.contains(8));
  assert(set.without(5).to_string() == "{1..4,6..7,10}");

  ValueSet other{{IntRange{2, 6}, IntRange{9, 11}}};
  assert(set.intersected(other).to_string() == "{2..6,10}");
}

void test_literal_slices_share() {
  std::vector<int> values(100000);
  std::iota(values.begin(), values.end(), -50000);
  Domain fixed = Domain::fixed(std::move(values));
  assert(fixed.assigned());
  assert(fixed.segment_count() == 1);

  Domain prefix = fixed.assigned_prefix(60000);
  Domain suffix = fixed.assigned_suffix(40000);
  assert(prefix.segment_count() == 1);
  assert(suffix.segment_count() == 1);

  const auto& p = std::get<LiteralSegment>(prefix.segments().front()).literal;
  const auto& s = std::get<LiteralSegment>(suffix.segments().front()).literal;
  const auto& whole =
      std::get<LiteralSegment>(fixed.segments().front()).literal;
  assert(p.shares_storage_with(whole));
  assert(s.shares_storage_with(whole));
  assert(p.contiguous_with(s));

  Domain recombined = prefix.concatenated(suffix);
  assert(recombined.segment_count() == 1);
  assert(recombined == fixed);
}


void test_fixed_block_compaction() {
  constexpr std::size_t n = 100000;
  std::vector<dashed::Segment> old_style;
  old_style.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    old_style.push_back(
        RepeatSegment{ValueSet(static_cast<int>(i % 17)), 1, 1});
  }
  Domain compact(std::move(old_style), static_cast<dashed::Length>(n),
                 static_cast<dashed::Length>(n));
  assert(compact.assigned());
  assert(compact.segment_count() == 1);
  assert(std::holds_alternative<LiteralSegment>(compact.segments().front()));

  Domain repeated = Domain::repeat(ValueSet(7), 100000, 100000);
  assert(repeated.segment_count() == 1);
  assert(std::holds_alternative<RepeatSegment>(repeated.segments().front()));
  assert(repeated.referenced_dynamic_bytes() == 0);

  std::vector<dashed::Segment> mixed;
  mixed.push_back(RepeatSegment{ValueSet(0, 9), 0, 10});
  for (int i = 0; i < 10000; ++i) {
    mixed.push_back(RepeatSegment{ValueSet(i % 23), 1, 1});
  }
  mixed.push_back(RepeatSegment{ValueSet(-5, 5), 0, 10});
  Domain few_blocks(std::move(mixed), 10000, 10020);
  assert(few_blocks.segment_count() == 3);
  assert(std::holds_alternative<LiteralSegment>(few_blocks.segments()[1]));
}

void test_uniform_fixed_run_normalization() {
  constexpr std::size_t n = 100000;
  const auto count = static_cast<dashed::Length>(n);

  // A dense fixed sequence containing one repeated value should not retain
  // an O(n) literal payload.
  Domain fixed = Domain::fixed(std::vector<int>(n, 7));

  assert(fixed.assigned());
  assert(fixed.segment_count() == 1);
  assert(std::holds_alternative<RepeatSegment>(
      fixed.segments().front()));

  const auto& fixed_run =
      std::get<RepeatSegment>(fixed.segments().front());

  const auto fixed_value =
      fixed_run.values.singleton_value();

  assert(fixed_value.has_value());
  assert(*fixed_value == 7);
  assert(fixed_run.lower == count);
  assert(fixed_run.upper == count);
  assert(fixed.referenced_dynamic_bytes() == 0);

  // The old G-Strings representation used one exact singleton block for
  // each position. An identical run should normalize to one repeat segment.
  std::vector<dashed::Segment> old_style;
  old_style.reserve(n);

  for (std::size_t i = 0; i < n; ++i) {
    old_style.push_back(
        RepeatSegment{ValueSet(7), 1, 1});
  }

  Domain compact(
      std::move(old_style),
      count,
      count);

  assert(compact.segment_count() == 1);
  assert(std::holds_alternative<RepeatSegment>(
      compact.segments().front()));

  const auto& compact_run =
      std::get<RepeatSegment>(compact.segments().front());

  assert(compact_run.values == ValueSet(7));
  assert(compact_run.lower == count);
  assert(compact_run.upper == count);
  assert(compact.referenced_dynamic_bytes() == 0);

  // Uniform literals must also merge with adjacent equal repeat segments.
  Domain merged(
      {
          RepeatSegment{ValueSet(7), 2, 2},
          LiteralSegment{
              LiteralSlice(std::vector<int>{7, 7, 7})},
          RepeatSegment{ValueSet(7), 1, 1},
      },
      6,
      6);

  assert(merged.segment_count() == 1);
  assert(std::holds_alternative<RepeatSegment>(
      merged.segments().front()));

  const auto& merged_run =
      std::get<RepeatSegment>(merged.segments().front());

  assert(merged_run.values == ValueSet(7));
  assert(merged_run.lower == 6);
  assert(merged_run.upper == 6);

  // A genuinely non-uniform fixed sequence must remain a literal.
  Domain nonuniform =
      Domain::fixed({7, 7, 8, 7});

  assert(nonuniform.segment_count() == 1);
  assert(std::holds_alternative<LiteralSegment>(
      nonuniform.segments().front()));
}

void test_normalization() {
  ValueSet digits(-1000, 1000);
  Domain domain(
      {RepeatSegment{digits, 1, 2}, RepeatSegment{digits, 3, 4}}, 0, 100);
  assert(domain.segment_count() == 1);
  const auto& segment = std::get<RepeatSegment>(domain.segments().front());
  assert(segment.lower == 4);
  assert(segment.upper == 6);
  assert(domain.min_length() == 4);
  assert(domain.max_length() == 6);
}

void test_membership() {
  Domain domain(
      {RepeatSegment{ValueSet(1, 2), 1, 2},
       LiteralSegment{LiteralSlice(std::vector<int>{9, 8})},
       RepeatSegment{ValueSet(-3), 0, 2}},
      3, 6);

  const std::vector<int> yes1{1, 9, 8};
  const std::vector<int> yes2{2, 1, 9, 8, -3, -3};
  const std::vector<int> no1{3, 9, 8};
  const std::vector<int> no2{1, 9, 7};
  assert(domain.accepts(yes1));
  assert(domain.accepts(yes2));
  assert(!domain.accepts(no1));
  assert(!domain.accepts(no2));
}

void test_length_propagation() {
  Domain x = Domain::top(ValueSet(-10, 10), 1, 20);
  IntBounds length{5, 8};
  auto result = dashed::propagate_length(x, length);
  assert(!result.failed());
  assert(x.min_length() == 5);
  assert(x.max_length() == 8);

  assert(x.tighten_length(7, 7) == Change::length);
  result = dashed::propagate_length(x, length);
  assert(!result.failed());
  assert(length.lower == 7 && length.upper == 7);
}

void test_legacy_equality_shape_refinement() {
  // Port of old str_test2::test01, using generic integers:
  //
  // x = {1,2}^0..4
  // y = {1,2}^0..2 {1}^3
  //
  // Equality restricts both domains to:
  // {1,2}^0..1 {1}^3
  Domain x =
      Domain::repeat(ValueSet(1, 2), 0, 4);

  Domain y(
      {
          RepeatSegment{ValueSet(1, 2), 0, 2},
          RepeatSegment{ValueSet(1), 3, 3},
      },
      3,
      5);

  const Domain expected(
      {
          RepeatSegment{ValueSet(1, 2), 0, 1},
          RepeatSegment{ValueSet(1), 3, 3},
      },
      3,
      4);

  const auto first =
      dashed::propagate_equal(x, y);
  assert(!first.failed());

  // A second call checks fixpoint/idempotence, matching the old test.
  const auto second =
      dashed::propagate_equal(x, y);
  assert(!second.failed());

  std::cerr << "legacy equality x: " << x << '\n';
  std::cerr << "legacy equality y: " << y << '\n';
  std::cerr << "expected:          " << expected << '\n';

  assert(x == expected);
  assert(y == expected);

  // Equal domains do not subsume x=y: the two variables could still
  // independently select different concrete lists.
  assert(!second.subsumed);
}

void test_single_repeat_equality_refinement_guards() {
  {
    // The common alphabet must be applied independently to each
    // segment of the structured side.
    Domain x =
        Domain::repeat(
            ValueSet(1, 3),
            2,
            3);

    Domain y(
        {
            RepeatSegment{
                ValueSet(2, 4),
                1,
                1},
            RepeatSegment{
                ValueSet(1, 2),
                1,
                2},
        },
        2,
        3);

    const Domain expected(
        {
            RepeatSegment{
                ValueSet(2, 3),
                1,
                1},
            RepeatSegment{
                ValueSet(1, 2),
                1,
                2},
        },
        2,
        3);

    const auto result =
        dashed::propagate_equal(x, y);

    assert(!result.failed());
    assert(x == expected);
    assert(y == expected);
    assert(!result.subsumed);
  }

  {
    // A mandatory segment disjoint from the single block's alphabet
    // makes equality impossible.
    Domain x =
        Domain::repeat(
            ValueSet(1),
            1,
            2);

    Domain y =
        Domain::repeat(
            ValueSet(2),
            1,
            2);

    const auto result =
        dashed::propagate_equal(x, y);

    assert(result.failed());
    assert(x.failed());
  }

  {
    // A disjoint optional segment is reduced to zero width and removed.
    Domain x =
        Domain::repeat(
            ValueSet(1),
            1,
            2);

    Domain y(
        {
            RepeatSegment{
                ValueSet(2),
                0,
                1},
            RepeatSegment{
                ValueSet(1),
                1,
                1},
        },
        1,
        2);

    const Domain expected =
        Domain::repeat(
            ValueSet(1),
            1,
            1);

    const auto result =
        dashed::propagate_equal(x, y);

    assert(!result.failed());
    assert(x == expected);
    assert(y == expected);
    assert(x.assigned());
    assert(y.assigned());
    assert(result.subsumed);
  }
}

void test_equality() {
  Domain fixed = Domain::fixed({4, 5, 6});
  Domain generic = Domain::top(ValueSet(0, 10), 0, 8);
  auto result = dashed::propagate_equal(fixed, generic);
  assert(!result.failed());
  assert(result.subsumed);
  assert(generic.assigned());
  assert(generic.value() == std::vector<int>({4, 5, 6}));

  Domain wrong = Domain::fixed({4, 5, 7});
  result = dashed::propagate_equal(fixed, wrong);
  assert(result.failed());
}

void test_reified_relations() {
  Domain x = Domain::fixed({1, 2});
  Domain y = Domain::fixed({1, 3});
  BoolDomain equal;
  auto result = dashed::propagate_reified_equal(x, y, equal);
  assert(!result.failed());
  assert(equal.assigned() && !equal.value());

  Domain a = Domain::top(ValueSet(0, 9), 2, 2);
  Domain b = Domain::fixed({8, 7});
  BoolDomain must_equal{false, true};
  result = dashed::propagate_reified_equal(a, b, must_equal);
  assert(!result.failed());
  assert(a.assigned());
  assert(a.value() == b.value());

  BoolDomain not_equal;
  result = dashed::propagate_reified_not_equal(x, y, not_equal);
  assert(!result.failed());
  assert(not_equal.assigned() && not_equal.value());
}

void test_concat() {
  Domain x = Domain::fixed({1, 2, 3});
  Domain y = Domain::fixed({4, 5});
  Domain z = Domain::top(ValueSet(0, 10), 0, 20);
  auto result = dashed::propagate_concat(z, x, y);
  assert(!result.failed());
  assert(result.subsumed);
  assert(z.value() == std::vector<int>({1, 2, 3, 4, 5}));
  assert(z.segment_count() == 2);  // Different immutable backing buffers.

  Domain whole = Domain::fixed({10, 11, 12, 13, 14});
  Domain prefix = Domain::fixed({10, 11});
  Domain suffix = Domain::top(ValueSet(0, 20), 0, 10);
  result = dashed::propagate_concat(whole, prefix, suffix);
  assert(!result.failed());
  assert(suffix.value() == std::vector<int>({12, 13, 14}));
  const auto& whole_lit =
      std::get<LiteralSegment>(whole.segments().front()).literal;
  const auto& suffix_lit =
      std::get<LiteralSegment>(suffix.segments().front()).literal;
  assert(suffix_lit.shares_storage_with(whole_lit));
}

void test_concat_length_failure() {
  Domain z = Domain::top(ValueSet(0, 1), 2, 2);
  Domain x = Domain::top(ValueSet(0, 1), 2, 3);
  Domain y = Domain::top(ValueSet(0, 1), 1, 3);
  auto result = dashed::propagate_concat(z, x, y);
  assert(result.failed());
}

}  // namespace

int main() {
  test_value_set();
  test_literal_slices_share();
  test_fixed_block_compaction();
  test_uniform_fixed_run_normalization();
  test_normalization();
  test_membership();
  test_length_propagation();
  test_single_repeat_equality_refinement_guards();
  test_legacy_equality_shape_refinement();
  test_equality();
  test_reified_relations();
  test_concat();
  test_concat_length_failure();
  std::cout << "All Dashed core tests passed.\n";
  return 0;
}
