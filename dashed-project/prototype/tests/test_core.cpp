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

void test_segmented_equality_sweep_symmetry_and_failure() {
  auto make_structured = []() -> Domain {
    return Domain(
        {
            RepeatSegment{
                ValueSet(1, 3),
                0,
                300},
            RepeatSegment{
                ValueSet(4),
                50,
                50},
            RepeatSegment{
                ValueSet(3, 6),
                0,
                20},
        },
        50,
        370);
  };

  auto make_exact_target = []() -> Domain {
    return Domain(
        {
            RepeatSegment{
                ValueSet(2, 4),
                260,
                260},
            RepeatSegment{
                ValueSet(6),
                10,
                10},
        },
        270,
        270);
  };

  const Domain expected_structured(
      {
          RepeatSegment{
              ValueSet(2, 3),
              200,
              210},
          RepeatSegment{
              ValueSet(4),
              50,
              50},
          RepeatSegment{
              ValueSet(3, 4),
              0,
              10},
          RepeatSegment{
              ValueSet(6),
              10,
              10},
      },
      270,
      270);

  const Domain expected_exact =
      make_exact_target();

  {
    // Put the exact-layout operand first. The directional projection must
    // still refine the second operand.
    Domain left = make_exact_target();
    Domain right = make_structured();

    const auto first =
        dashed::propagate_equal(left, right);

    assert(!first.failed());
    assert(left == expected_exact);
    assert(right == expected_structured);

    const auto second =
        dashed::propagate_equal(left, right);

    assert(!second.failed());
    assert(left == expected_exact);
    assert(right == expected_structured);
    assert(!second.subsumed);
  }

  auto make_incompatible_subject = []() -> Domain {
    return Domain(
        {
            RepeatSegment{
                ValueSet(1),
                1,
                2},
            RepeatSegment{
                ValueSet(2),
                1,
                1},
        },
        2,
        3);
  };

  auto make_incompatible_target = []() -> Domain {
    return Domain(
        {
            RepeatSegment{
                ValueSet(1),
                2,
                2},
            RepeatSegment{
                ValueSet(3),
                1,
                1},
        },
        3,
        3);
  };

  {
    Domain left =
        make_incompatible_subject();
    Domain right =
        make_incompatible_target();

    const auto result =
        dashed::propagate_equal(left, right);

    assert(result.failed());

    // Failure ownership depends on which propagation path detects the
    // contradiction. The PropagationResult is the public guarantee.
    assert(left.failed() || right.failed());
  }

  {
    // Failure must also be detected with the exact target first.
    Domain left =
        make_incompatible_target();
    Domain right =
        make_incompatible_subject();

    const auto result =
        dashed::propagate_equal(left, right);

    assert(result.failed());

    // Failure ownership depends on which propagation path detects the
    // contradiction. The PropagationResult is the public guarantee.
    assert(left.failed() || right.failed());
  }
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

void test_legacy_equality_empty_only_intersection_test11() {
  // Port of old str_test2::test11 using generic integers.
  //
  // The value sets are disjoint, but both blocks are optional. Therefore
  // the empty list is the unique common value and equality must not fail.
  Domain x =
      Domain::repeat(
          ValueSet(1, 3),
          0,
          400);

  Domain y =
      Domain::repeat(
          ValueSet(4),
          0,
          900);

  const Domain expected =
      Domain::fixed(std::vector<int>{});

  const auto result =
      dashed::propagate_equal(x, y);

  std::cerr
      << "empty-only x: "
      << x << '\n';
  std::cerr
      << "empty-only y: "
      << y << '\n';
  std::cerr
      << "empty expected: "
      << expected << '\n';

  assert(!result.failed());
  assert(x == expected);
  assert(y == expected);

  assert(x.assigned());
  assert(y.assigned());
  assert(x.value().empty());
  assert(y.value().empty());

  assert(result.subsumed);
}

void test_legacy_equality_optional_suffix_test14() {
  // Port of old str_test2::test14 using generic integers:
  //
  // x = {1,2}^0..6 {3}^1 {4}^1
  // y = {1,2,3}^0..5 {4}^0..1
  //
  // Equality forces y's final {4} block to occur once. The preceding
  // {3} also becomes explicit, and x's first block is limited to four
  // occurrences by the common maximum length.
  Domain x(
      {
          RepeatSegment{
              ValueSet(1, 2),
              0,
              6},
          RepeatSegment{
              ValueSet(3),
              1,
              1},
          RepeatSegment{
              ValueSet(4),
              1,
              1},
      },
      2,
      8);

  Domain y(
      {
          RepeatSegment{
              ValueSet(1, 3),
              0,
              5},
          RepeatSegment{
              ValueSet(4),
              0,
              1},
      },
      0,
      6);

  const Domain expected(
      {
          RepeatSegment{
              ValueSet(1, 2),
              0,
              4},
          RepeatSegment{
              ValueSet(3),
              1,
              1},
          RepeatSegment{
              ValueSet(4),
              1,
              1},
      },
      2,
      6);

  const auto first =
      dashed::propagate_equal(x, y);

  assert(!first.failed());

  const auto second =
      dashed::propagate_equal(x, y);

  assert(!second.failed());

  std::cerr
      << "optional-suffix x: "
      << x << '\n';
  std::cerr
      << "optional-suffix y: "
      << y << '\n';
  std::cerr
      << "expected:          "
      << expected << '\n';

  assert(x == expected);
  assert(y == expected);

  // Equal abstract domains do not imply that independently selected
  // concrete lists must already be equal.
  assert(!second.subsumed);
}

void test_legacy_equality_boundary_cardinality_test16() {
  // Port of old str_test2::test16 using generic integers:
  //
  // x = {1..3}^0..3 {4}^1 {3..6}^0..2
  // y = {2..4}^0..3 {6}^1
  //
  // Equality makes the final 6 mandatory, tightens the blocks before it,
  // and requires at least one occurrence of y's first block.
  Domain x(
      {
          RepeatSegment{
              ValueSet(1, 3),
              0,
              3},
          RepeatSegment{
              ValueSet(4),
              1,
              1},
          RepeatSegment{
              ValueSet(3, 6),
              0,
              2},
      },
      1,
      6);

  Domain y(
      {
          RepeatSegment{
              ValueSet(2, 4),
              0,
              3},
          RepeatSegment{
              ValueSet(6),
              1,
              1},
      },
      1,
      4);

  const Domain expected_x(
      {
          RepeatSegment{
              ValueSet(2, 3),
              0,
              2},
          RepeatSegment{
              ValueSet(4),
              1,
              1},
          RepeatSegment{
              ValueSet(3, 4),
              0,
              1},
          RepeatSegment{
              ValueSet(6),
              1,
              1},
      },
      2,
      4);

  const Domain expected_y(
      {
          RepeatSegment{
              ValueSet(2, 4),
              1,
              3},
          RepeatSegment{
              ValueSet(6),
              1,
              1},
      },
      2,
      4);

  const auto first =
      dashed::propagate_equal(x, y);

  assert(!first.failed());

  const auto second =
      dashed::propagate_equal(x, y);

  assert(!second.failed());

  std::cerr
      << "boundary-cardinality x: "
      << x << '\n';
  std::cerr
      << "boundary-cardinality y: "
      << y << '\n';
  std::cerr
      << "expected x:             "
      << expected_x << '\n';
  std::cerr
      << "expected y:             "
      << expected_y << '\n';

  assert(x == expected_x);
  assert(y == expected_y);
  assert(!second.subsumed);
}

void test_legacy_equality_variable_width_test09() {
  // Port of old str_test2::test09 using generic integers.
  //
  // x = {1..3}^0..500 {4..5}^0..300
  // y = {1..2}^0..200 {3..4}^0..900
  //
  // The common maximum length is 800.
  //
  // Value 5 is impossible anywhere in y, so it must be removed from
  // x's second block. Value 3 must remain in x's first block because
  // variable block boundaries allow it to align with y's second block.
  Domain x(
      {
          RepeatSegment{
              ValueSet(1, 3),
              0,
              500},
          RepeatSegment{
              ValueSet(4, 5),
              0,
              300},
      },
      0,
      800);

  Domain y(
      {
          RepeatSegment{
              ValueSet(1, 2),
              0,
              200},
          RepeatSegment{
              ValueSet(3, 4),
              0,
              900},
      },
      0,
      1100);

  const Domain expected_x(
      {
          RepeatSegment{
              ValueSet(1, 3),
              0,
              500},
          RepeatSegment{
              ValueSet(4),
              0,
              300},
      },
      0,
      800);

  const Domain expected_y(
      {
          RepeatSegment{
              ValueSet(1, 2),
              0,
              200},
          RepeatSegment{
              ValueSet(3, 4),
              0,
              800},
      },
      0,
      800);

  const auto first =
      dashed::propagate_equal(x, y);

  assert(!first.failed());

  const auto second =
      dashed::propagate_equal(x, y);

  assert(!second.failed());

  std::cerr
      << "variable-width x: "
      << x << '\n';
  std::cerr
      << "variable-width y: "
      << y << '\n';
  std::cerr
      << "expected x:       "
      << expected_x << '\n';
  std::cerr
      << "expected y:       "
      << expected_y << '\n';

  assert(x == expected_x);
  assert(y == expected_y);
  assert(!second.subsumed);
}

void test_legacy_equality_multi_segment_sweep() {
  // Port of old str_test2::test02, replacing characters with integers.
  //
  // x = {1..3}^0..300 {4}^50 {3..6}^0..20
  // y = {2..4}^260 {6}^10
  //
  // The common length is exactly 270. Aligning the boundary before the
  // final ten 6s should refine x to the representation below.
  Domain x(
      {
          RepeatSegment{ValueSet(1, 3), 0, 300},
          RepeatSegment{ValueSet(4), 50, 50},
          RepeatSegment{ValueSet(3, 6), 0, 20},
      },
      50,
      370);

  Domain y(
      {
          RepeatSegment{ValueSet(2, 4), 260, 260},
          RepeatSegment{ValueSet(6), 10, 10},
      },
      270,
      270);

  const Domain expected_x(
      {
          RepeatSegment{ValueSet(2, 3), 200, 210},
          RepeatSegment{ValueSet(4), 50, 50},
          RepeatSegment{ValueSet(3, 4), 0, 10},
          RepeatSegment{ValueSet(6), 10, 10},
      },
      270,
      270);

  const Domain expected_y(
      {
          RepeatSegment{ValueSet(2, 4), 260, 260},
          RepeatSegment{ValueSet(6), 10, 10},
      },
      270,
      270);

  const auto first =
      dashed::propagate_equal(x, y);

  assert(!first.failed());

  const auto second =
      dashed::propagate_equal(x, y);

  assert(!second.failed());

  std::cerr
      << "legacy multi-segment x: "
      << x << '\n';
  std::cerr
      << "legacy multi-segment y: "
      << y << '\n';
  std::cerr
      << "expected x:             "
      << expected_x << '\n';
  std::cerr
      << "expected y:             "
      << expected_y << '\n';

  assert(x == expected_x);
  assert(y == expected_y);
  assert(!second.subsumed);
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

void test_concat_forward_projection_guards() {
  {
    Domain x =
        Domain::fixed({1});

    Domain y =
        Domain::repeat(
            ValueSet(2),
            1,
            2);

    Domain z =
        Domain::repeat(
            ValueSet(9),
            2,
            3);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(result.failed());
    assert(z.failed());
  }

  {
    Domain whole =
        Domain::fixed(
            {10, 11, 12, 13, 14});

    Domain x =
        whole.assigned_prefix(2);

    Domain y =
        whole.assigned_suffix(3);

    Domain z =
        Domain::top(
            ValueSet(-100, 100),
            5,
            5);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!result.failed());
    assert(result.subsumed);
    assert(z.assigned());
    assert(z.assigned_equal(whole));
    assert(z.segment_count() == 1);

    const auto& whole_literal =
        std::get<LiteralSegment>(
            whole.segments().front()).literal;

    const auto& result_literal =
        std::get<LiteralSegment>(
            z.segments().front()).literal;

    assert(
        result_literal.shares_storage_with(
            whole_literal));
  }
}


void test_concat_projects_segmented_suffix_remainder() {
  {
    Domain z(
        {
            RepeatSegment{
                ValueSet(30, 40),
                1,
                3},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        50, 51, 90})},
        },
        4,
        6);

    Domain x(
        {
            RepeatSegment{
                ValueSet(35, 45),
                2,
                4},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    Domain y =
        Domain::fixed({90});

    const Domain expected_x(
        {
            RepeatSegment{
                ValueSet(35, 40),
                2,
                3},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        5);

    const auto first =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!first.failed());
    assert(x == expected_x);

    const auto second =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!second.failed());
    assert(x == expected_x);
    assert(!second.subsumed);
  }

  {
    Domain z(
        {
            RepeatSegment{
                ValueSet(30, 40),
                1,
                3},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        50, 51, 90})},
        },
        4,
        6);

    Domain x(
        {
            RepeatSegment{
                ValueSet(60, 70),
                2,
                4},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    Domain y =
        Domain::fixed({90});

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(result.failed());
    assert(x.failed());
  }

  {
    Domain z(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        70000, 80000})},
            RepeatSegment{
                ValueSet(100000, 200000),
                1,
                5},
            RepeatSegment{
                ValueSet(-50000),
                1,
                1},
        },
        4,
        8);

    Domain x(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        70000, 80000})},
            RepeatSegment{
                ValueSet(150000, 250000),
                3,
                8},
        },
        5,
        10);

    Domain y =
        Domain::fixed({-50000});

    const Domain expected_x(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        70000, 80000})},
            RepeatSegment{
                ValueSet(150000, 200000),
                3,
                5},
        },
        5,
        7);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!result.failed());
    assert(x == expected_x);
  }
}


void test_concat_strips_mandatory_repeat_suffix() {
  {
    Domain z(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        20, 30})},
            RepeatSegment{
                ValueSet(7, 9),
                3,
                5},
        },
        5,
        7);

    Domain x =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain y =
        Domain::fixed(
            {8, 9});

    const Domain expected_x(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        20, 30})},
            RepeatSegment{
                ValueSet(7, 9),
                1,
                3},
        },
        3,
        5);

    const auto first =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!first.failed());
    assert(x == expected_x);

    const auto second =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!second.failed());
    assert(x == expected_x);
  }

  {
    Domain z(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        20, 30})},
            RepeatSegment{
                ValueSet(7, 9),
                3,
                5},
        },
        5,
        7);

    Domain x =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain y =
        Domain::fixed(
            {7, 8, 9});

    const Domain expected_x(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        20, 30})},
            RepeatSegment{
                ValueSet(7, 9),
                0,
                2},
        },
        2,
        4);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!result.failed());
    assert(x == expected_x);
  }

  {
    // Exact non-singleton repeats can also be stripped.
    Domain z(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        50})},
            RepeatSegment{
                ValueSet(1, 3),
                4,
                4},
        },
        5,
        5);

    Domain x =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain y =
        Domain::fixed(
            {1, 3});

    const Domain expected_x(
        {
            RepeatSegment{
                ValueSet(50),
                1,
                1},
            RepeatSegment{
                ValueSet(1, 3),
                2,
                2},
        },
        3,
        3);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!result.failed());
    assert(x == expected_x);
  }

  {
    Domain z(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        20, 30})},
            RepeatSegment{
                ValueSet(7, 9),
                3,
                5},
        },
        5,
        7);

    Domain x =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain y =
        Domain::fixed(
            {99, 9});

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(result.failed());
  }

  {
    // The suffix crosses beyond the mandatory count. The boundary is
    // ambiguous, so propagation remains conservative.
    Domain z(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        20, 7})},
            RepeatSegment{
                ValueSet(7),
                1,
                3},
        },
        3,
        5);

    Domain x =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain y =
        Domain::fixed(
            {7, 7});

    const Domain expected_x =
        Domain::top(
            ValueSet(-100, 100),
            1,
            3);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!result.failed());
    assert(x == expected_x);
  }
}


void test_concat_strips_partial_suffix_segments() {
  {
    const LiteralSlice storage(
        std::vector<int>{
            30, 40, 50, 60});

    Domain z(
        {
            RepeatSegment{
                ValueSet(10, 20),
                1,
                2},
            LiteralSegment{storage},
        },
        5,
        6);

    Domain x =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain y =
        Domain::fixed(
            {50, 60});

    const Domain expected_x(
        {
            RepeatSegment{
                ValueSet(10, 20),
                1,
                2},
            LiteralSegment{
                storage.slice(0, 2)},
        },
        3,
        4);

    const auto first =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!first.failed());
    assert(x == expected_x);

    const auto& projected_literal =
        std::get<LiteralSegment>(
            x.segments().back()).literal;

    assert(
        projected_literal.shares_storage_with(
            storage));

    const auto second =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!second.failed());
    assert(x == expected_x);
  }

  {
    Domain z(
        {
            RepeatSegment{
                ValueSet(10, 20),
                1,
                2},
            RepeatSegment{
                ValueSet(7),
                5,
                5},
        },
        6,
        7);

    Domain x =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain y =
        Domain::fixed(
            {7, 7, 7});

    const Domain expected_x(
        {
            RepeatSegment{
                ValueSet(10, 20),
                1,
                2},
            RepeatSegment{
                ValueSet(7),
                2,
                2},
        },
        3,
        4);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!result.failed());
    assert(x == expected_x);
  }

  {
    Domain z(
        {
            RepeatSegment{
                ValueSet(10, 20),
                1,
                2},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        30, 40, 50, 60})},
        },
        5,
        6);

    Domain x =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain y =
        Domain::fixed(
            {99, 60});

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(result.failed());
  }

  {
    Domain z(
        {
            RepeatSegment{
                ValueSet(10, 20),
                1,
                2},
            RepeatSegment{
                ValueSet(7),
                5,
                5},
        },
        6,
        7);

    Domain x =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain y =
        Domain::fixed(
            {8, 7, 7});

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(result.failed());
  }
}


void test_concat_strips_exact_suffix() {
  {
    Domain x =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain y =
        Domain::fixed(
            {30, 40});

    Domain z(
        {
            RepeatSegment{
                ValueSet(10, 20),
                1,
                3},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        30, 40})},
        },
        3,
        5);

    const Domain expected_x =
        Domain::repeat(
            ValueSet(10, 20),
            1,
            3);

    const auto first =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!first.failed());
    assert(x == expected_x);

    const auto second =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!second.failed());
    assert(x == expected_x);
    assert(!second.subsumed);
  }

  {
    Domain x =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain y =
        Domain::fixed(
            {99, 40});

    Domain z(
        {
            RepeatSegment{
                ValueSet(10, 20),
                1,
                3},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        30, 40})},
        },
        3,
        5);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(result.failed());
  }
}


void test_concat_projects_segmented_prefix_remainder() {
  {
    Domain z(
        {
            RepeatSegment{
                ValueSet(10),
                1,
                1},
            RepeatSegment{
                ValueSet(30, 40),
                1,
                3},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    Domain x =
        Domain::fixed({10});

    Domain y(
        {
            RepeatSegment{
                ValueSet(35, 45),
                2,
                4},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    const Domain expected_y(
        {
            RepeatSegment{
                ValueSet(35, 40),
                2,
                3},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        5);

    const auto first =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!first.failed());
    assert(y == expected_y);

    const auto second =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!second.failed());
    assert(y == expected_y);
    assert(!second.subsumed);
  }

  {
    // The structural suffix agrees, but the repeat alphabets are
    // disjoint. Boundary projection must prove failure.
    Domain z(
        {
            RepeatSegment{
                ValueSet(10),
                1,
                1},
            RepeatSegment{
                ValueSet(30, 40),
                1,
                3},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    Domain x =
        Domain::fixed({10});

    Domain y(
        {
            RepeatSegment{
                ValueSet(60, 70),
                2,
                4},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(result.failed());
    assert(y.failed());
  }

  {
    // Global length narrowing participates in the segmented
    // intersection before the structural projection is committed.
    Domain z(
        {
            RepeatSegment{
                ValueSet(-50000),
                1,
                1},
            RepeatSegment{
                ValueSet(100000, 200000),
                1,
                5},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        70000, 80000})},
        },
        4,
        8);

    Domain x =
        Domain::fixed({-50000});

    Domain y(
        {
            RepeatSegment{
                ValueSet(150000, 250000),
                3,
                8},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        70000, 80000})},
        },
        5,
        10);

    const Domain expected_y(
        {
            RepeatSegment{
                ValueSet(150000, 200000),
                3,
                5},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        70000, 80000})},
        },
        5,
        7);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!result.failed());
    assert(y == expected_y);
  }
}


void test_concat_strips_mandatory_repeat_prefix() {
  {
    Domain z(
        {
            RepeatSegment{
                ValueSet(7, 9),
                3,
                5},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        20, 30})},
        },
        5,
        7);

    Domain x =
        Domain::fixed(
            {7, 8});

    Domain y =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    const Domain expected_y(
        {
            RepeatSegment{
                ValueSet(7, 9),
                1,
                3},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        20, 30})},
        },
        3,
        5);

    const auto first =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!first.failed());
    assert(y == expected_y);

    const auto second =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!second.failed());
    assert(y == expected_y);
  }

  {
    Domain z(
        {
            RepeatSegment{
                ValueSet(7, 9),
                3,
                5},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        20, 30})},
        },
        5,
        7);

    Domain x =
        Domain::fixed(
            {7, 8, 9});

    Domain y =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    const Domain expected_y(
        {
            RepeatSegment{
                ValueSet(7, 9),
                0,
                2},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        20, 30})},
        },
        2,
        4);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!result.failed());
    assert(y == expected_y);
  }

  {
    // Exact non-singleton repeats can also be stripped.
    Domain z(
        {
            RepeatSegment{
                ValueSet(1, 3),
                4,
                4},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        50})},
        },
        5,
        5);

    Domain x =
        Domain::fixed(
            {1, 3});

    Domain y =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    const Domain expected_y(
        {
            RepeatSegment{
                ValueSet(1, 3),
                2,
                2},
            RepeatSegment{
                ValueSet(50),
                1,
                1},
        },
        3,
        3);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!result.failed());
    assert(y == expected_y);
  }

  {
    Domain z(
        {
            RepeatSegment{
                ValueSet(7, 9),
                3,
                5},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        20, 30})},
        },
        5,
        7);

    Domain x =
        Domain::fixed(
            {7, 99});

    Domain y =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(result.failed());
  }

  {
    // The prefix crosses beyond the mandatory count. The boundary is
    // ambiguous, so propagation remains conservative.
    Domain z(
        {
            RepeatSegment{
                ValueSet(7),
                1,
                3},
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        7, 20})},
        },
        3,
        5);

    Domain x =
        Domain::fixed(
            {7, 7});

    Domain y =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    const Domain expected_y =
        Domain::top(
            ValueSet(-100, 100),
            1,
            3);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!result.failed());
    assert(y == expected_y);
  }
}


void test_concat_strips_partial_prefix_segments() {
  {
    const LiteralSlice storage(
        std::vector<int>{
            10, 20, 30, 40});

    Domain z(
        {
            LiteralSegment{storage},
            RepeatSegment{
                ValueSet(50, 60),
                1,
                2},
        },
        5,
        6);

    Domain x =
        Domain::fixed(
            {10, 20});

    Domain y =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    const Domain expected_y(
        {
            LiteralSegment{
                storage.slice(2, 2)},
            RepeatSegment{
                ValueSet(50, 60),
                1,
                2},
        },
        3,
        4);

    const auto first =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!first.failed());
    assert(y == expected_y);

    const auto& projected_literal =
        std::get<LiteralSegment>(
            y.segments().front()).literal;

    assert(
        projected_literal.shares_storage_with(
            storage));

    const auto second =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!second.failed());
    assert(y == expected_y);
  }

  {
    Domain z(
        {
            RepeatSegment{
                ValueSet(7),
                5,
                5},
            RepeatSegment{
                ValueSet(10, 20),
                1,
                2},
        },
        6,
        7);

    Domain x =
        Domain::fixed(
            {7, 7});

    Domain y =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    const Domain expected_y(
        {
            RepeatSegment{
                ValueSet(7),
                3,
                3},
            RepeatSegment{
                ValueSet(10, 20),
                1,
                2},
        },
        4,
        5);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!result.failed());
    assert(y == expected_y);
  }

  {
    Domain z(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        10, 20, 30, 40})},
            RepeatSegment{
                ValueSet(50, 60),
                1,
                2},
        },
        5,
        6);

    Domain x =
        Domain::fixed(
            {10, 99});

    Domain y =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(result.failed());
  }

  {
    Domain z(
        {
            RepeatSegment{
                ValueSet(7),
                5,
                5},
            RepeatSegment{
                ValueSet(10, 20),
                1,
                2},
        },
        6,
        7);

    Domain x =
        Domain::fixed(
            {7, 8});

    Domain y =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(result.failed());
  }

  {
    std::vector<int> values;
    values.reserve(4096);

    for (int value = 0;
         value < 4096;
         ++value) {
      values.push_back(value);
    }

    const LiteralSlice storage(
        std::move(values));

    Domain z(
        {
            LiteralSegment{storage},
            RepeatSegment{
                ValueSet(-1),
                0,
                1},
        },
        4096,
        4097);

    std::vector<int> prefix_values;
    prefix_values.reserve(2048);

    for (int value = 0;
         value < 2048;
         ++value) {
      prefix_values.push_back(value);
    }

    Domain x =
        Domain::fixed(
            std::move(prefix_values));

    Domain y =
        Domain::top(
            ValueSet(-10000, 10000),
            0,
            5000);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!result.failed());
    assert(y.min_length() == 2048);
    assert(y.max_length() == 2049);
    assert(y.segment_count() == 2);

    const auto& suffix_literal =
        std::get<LiteralSegment>(
            y.segments().front()).literal;

    assert(suffix_literal.size() == 2048);
    assert(suffix_literal[0] == 2048);
    assert(suffix_literal[2047] == 4095);

    assert(
        suffix_literal.shares_storage_with(
            storage));
  }
}


void test_concat_strips_exact_prefix() {
  {
    Domain x =
        Domain::fixed(
            {10, 20});

    Domain y =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain z(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        10, 20})},
            RepeatSegment{
                ValueSet(30, 40),
                1,
                3},
        },
        3,
        5);

    const Domain expected_y =
        Domain::repeat(
            ValueSet(30, 40),
            1,
            3);

    const auto first =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!first.failed());
    assert(y == expected_y);

    const auto second =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(!second.failed());
    assert(y == expected_y);
    assert(!second.subsumed);
  }

  {
    Domain x =
        Domain::fixed(
            {10, 99});

    Domain y =
        Domain::top(
            ValueSet(-100, 100),
            0,
            10);

    Domain z(
        {
            LiteralSegment{
                LiteralSlice(
                    std::vector<int>{
                        10, 20})},
            RepeatSegment{
                ValueSet(30, 40),
                1,
                3},
        },
        3,
        5);

    const auto result =
        dashed::propagate_concat(
            z,
            x,
            y);

    assert(result.failed());
  }
}


void test_concat_projects_result_structure() {
  Domain x =
      Domain::fixed({10, 20});

  Domain y =
      Domain::repeat(
          ValueSet(30, 40),
          1,
          3);

  Domain z =
      Domain::top(
          ValueSet(-100, 100),
          0,
          10);

  const Domain expected_z(
      {
          LiteralSegment{
              LiteralSlice(
                  std::vector<int>{
                      10, 20})},
          RepeatSegment{
              ValueSet(30, 40),
              1,
              3},
      },
      3,
      5);

  const auto first =
      dashed::propagate_concat(
          z,
          x,
          y);

  assert(!first.failed());
  assert(z == expected_z);

  const auto second =
      dashed::propagate_concat(
          z,
          x,
          y);

  assert(!second.failed());
  assert(z == expected_z);

  assert(x.assigned());
  assert(!y.assigned());
  assert(!z.assigned());
  assert(!second.subsumed);
}


void test_concat_fixed_result_wide_split_interval() {
  // Feasible split points are 1, 2, and 3:
  //
  //   z = [10,20,30,40,50]
  //   |x| = 1..3
  //   |y| = 2..4
  //
  // The optional singleton sequence is a sound dashed
  // over-approximation of those three concrete splits.
  Domain z =
      Domain::fixed(
          {10, 20, 30, 40, 50});

  Domain x =
      Domain::top(
          ValueSet(-100, 100),
          1,
          3);

  Domain y =
      Domain::top(
          ValueSet(-100, 100),
          2,
          4);

  const Domain expected_x(
      {
          RepeatSegment{
              ValueSet(10),
              1,
              1},
          RepeatSegment{
              ValueSet(20),
              0,
              1},
          RepeatSegment{
              ValueSet(30),
              0,
              1},
      },
      1,
      3);

  const Domain expected_y(
      {
          RepeatSegment{
              ValueSet(20),
              0,
              1},
          RepeatSegment{
              ValueSet(30),
              0,
              1},
          LiteralSegment{
              LiteralSlice(
                  std::vector<int>{
                      40, 50})},
      },
      2,
      4);

  const auto first =
      dashed::propagate_concat(
          z,
          x,
          y);

  assert(!first.failed());

  const auto second =
      dashed::propagate_concat(
          z,
          x,
          y);

  assert(!second.failed());

  std::cerr
      << "wide concat x: "
      << x << '\n';
  std::cerr
      << "wide concat y: "
      << y << '\n';
  std::cerr
      << "expected x:    "
      << expected_x << '\n';
  std::cerr
      << "expected y:    "
      << expected_y << '\n';

  assert(x == expected_x);
  assert(y == expected_y);

  // Every concrete prefix represented by a feasible split is retained.
  assert(
      x.accepts(
          Domain::fixed({10})));

  assert(
      x.accepts(
          Domain::fixed({10, 20})));

  assert(
      x.accepts(
          Domain::fixed(
              {10, 20, 30})));

  // Every corresponding suffix is also retained.
  assert(
      y.accepts(
          Domain::fixed(
              {20, 30, 40, 50})));

  assert(
      y.accepts(
          Domain::fixed(
              {30, 40, 50})));

  assert(
      y.accepts(
          Domain::fixed(
              {40, 50})));

  assert(!x.assigned());
  assert(!y.assigned());
  assert(!second.subsumed);
}

void test_concat_fixed_result_split_interval() {
  // z is fixed, but the split point can be after position 1 or 2:
  //
  //   z = [10,20,30,40]
  //   |x| = 1..2
  //   |y| = 2..3
  //
  // Therefore:
  //
  //   x = [10] {20}^0..1
  //   y = {20}^0..1 [30,40]
  Domain z =
      Domain::fixed(
          {10, 20, 30, 40});

  Domain x =
      Domain::top(
          ValueSet(-100, 100),
          1,
          2);

  Domain y =
      Domain::top(
          ValueSet(-100, 100),
          2,
          3);

  const Domain expected_x(
      {
          RepeatSegment{
              ValueSet(10),
              1,
              1},
          RepeatSegment{
              ValueSet(20),
              0,
              1},
      },
      1,
      2);

  const Domain expected_y(
      {
          RepeatSegment{
              ValueSet(20),
              0,
              1},
          LiteralSegment{
              LiteralSlice(
                  std::vector<int>{
                      30, 40})},
      },
      2,
      3);

  const auto first =
      dashed::propagate_concat(
          z,
          x,
          y);

  assert(!first.failed());

  const auto second =
      dashed::propagate_concat(
          z,
          x,
          y);

  assert(!second.failed());

  std::cerr
      << "concat interval x: "
      << x << '\n';
  std::cerr
      << "concat interval y: "
      << y << '\n';
  std::cerr
      << "expected x:       "
      << expected_x << '\n';
  std::cerr
      << "expected y:       "
      << expected_y << '\n';

  assert(x == expected_x);
  assert(y == expected_y);

  assert(!x.assigned());
  assert(!y.assigned());
  assert(!second.subsumed);
}

void test_concat_fixed_result_exact_split() {
  // z is fixed and both operand lengths are exact:
  //
  //   [10,20,30,40,50] = x ++ y
  //   |x| = 2
  //   |y| = 3
  //
  // Therefore both operands arex| = 2
  //   |y| = 3
  //
  // Therefore both operands are uniquely determined.
  Domain z =
      Domain::fixed(
          {10, 20, 30, 40, 50});

  Domain x =
      Domain::top(
          ValueSet(-100, 100),
          2,
          2);

  Domain y =
      Domain::top(
          ValueSet(-100, 100),
          3,
          3);

  const Domain expected_x =
      Domain::fixed({10, 20});

  const Domain expected_y =
      Domain::fixed({30, 40, 50});

  const auto result =
      dashed::propagate_concat(z, x, y);

  std::cerr
      << "concat split x: "
      << x << '\n';
  std::cerr
      << "concat split y: "
      << y << '\n';
  std::cerr
      << "expected x:     "
      << expected_x << '\n';
  std::cerr
      << "expected y:     "
      << expected_y << '\n';

  assert(!result.failed());

  assert(x == expected_x);
  assert(y == expected_y);

  assert(x.assigned());
  assert(y.assigned());
  assert(result.subsumed);


  {
    // The split prefix is outside x's alphabet.
    Domain failed_z =
        Domain::fixed({10, 20, 30});

    Domain failed_x =
        Domain::top(
            ValueSet(-5, 5),
            1,
            1);

    Domain failed_y =
        Domain::top(
            ValueSet(-100, 100),
            2,
            2);

    const Domain original_y = failed_y;

    const auto failed_result =
        dashed::propagate_concat(
            failed_z,
            failed_x,
            failed_y);

    assert(failed_result.failed());
    assert(failed_x.failed());

    // Both halves are validated before either successful replacement.
    assert(failed_y == original_y);
    assert(!failed_y.assigned());
  }

  {
    // The split suffix is outside y's alphabet.
    Domain failed_z =
        Domain::fixed({1, 2, 50});

    Domain failed_x =
        Domain::top(
            ValueSet(-100, 100),
            2,
            2);

    Domain failed_y =
        Domain::top(
            ValueSet(-5, 5),
            1,
            1);

    const Domain original_x = failed_x;

    const auto failed_result =
        dashed::propagate_concat(
            failed_z,
            failed_x,
            failed_y);

    assert(failed_result.failed());
    assert(failed_y.failed());

    assert(failed_x == original_x);
    assert(!failed_x.assigned());
  }
}

void test_concat_length_failure() {
  Domain z = Domain::top(ValueSet(0, 1), 2, 2);
  Domain x = Domain::top(ValueSet(0, 1), 2, 3);
  Domain y = Domain::top(ValueSet(0, 1), 1, 3);
  auto result = dashed::propagate_concat(z, x, y);
  assert(result.failed());
}


void test_disequality_language_disjointness() {
  {
    // The length ranges overlap, but all nonempty values are incompatible.
    // Since both lists must be nonempty, equality is impossible.
    Domain x =
        Domain::repeat(
            ValueSet(1, 3),
            1,
            4);

    Domain y =
        Domain::repeat(
            ValueSet(10, 12),
            1,
            4);

    // may_equal() is intentionally a cheap conservative check. Both
    // domains are unassigned and their length intervals overlap, so it
    // does not prove language disjointness by itself.
    assert(x.may_equal(y));

    const auto result =
        dashed::propagate_not_equal(x, y);

    assert(!result.failed());
    assert(result.subsumed);

    // Entailment must not modify either operand.
    assert(
        x == Domain::repeat(
            ValueSet(1, 3),
            1,
            4));

    assert(
        y == Domain::repeat(
            ValueSet(10, 12),
            1,
            4));
  }

  {
    // Disjoint alphabets are insufficient when both domains contain [].
    Domain x =
        Domain::repeat(
            ValueSet(1, 3),
            0,
            4);

    Domain y =
        Domain::repeat(
            ValueSet(10, 12),
            0,
            4);

    assert(x.may_equal(y));

    const auto result =
        dashed::propagate_not_equal(x, y);

    assert(!result.failed());
    assert(!result.subsumed);
  }

  {
    // Only x contains [], so no common concrete list exists.
    Domain x =
        Domain::repeat(
            ValueSet(1, 3),
            0,
            4);

    Domain y =
        Domain::repeat(
            ValueSet(10, 12),
            1,
            4);

    // may_equal() is intentionally a cheap conservative check. Both
    // domains are unassigned and their length intervals overlap, so it
    // does not prove language disjointness by itself.
    assert(x.may_equal(y));

    const auto result =
        dashed::propagate_not_equal(x, y);

    assert(!result.failed());
    assert(result.subsumed);
  }
}

}  // namespace

int main() {
  test_concat_projects_segmented_suffix_remainder();
  test_concat_strips_mandatory_repeat_suffix();
  test_concat_strips_partial_suffix_segments();
  test_concat_strips_exact_suffix();
  test_concat_projects_segmented_prefix_remainder();
  test_concat_strips_mandatory_repeat_prefix();
  test_concat_strips_partial_prefix_segments();
  test_concat_strips_exact_prefix();
  test_disequality_language_disjointness();
  test_value_set();
  test_literal_slices_share();
  test_fixed_block_compaction();
  test_uniform_fixed_run_normalization();
  test_normalization();
  test_membership();
  test_length_propagation();
  test_segmented_equality_sweep_symmetry_and_failure();
  test_legacy_equality_empty_only_intersection_test11();
  test_legacy_equality_optional_suffix_test14();
  test_legacy_equality_boundary_cardinality_test16();
  test_legacy_equality_variable_width_test09();
  test_legacy_equality_multi_segment_sweep();
  test_single_repeat_equality_refinement_guards();
  test_legacy_equality_shape_refinement();
  test_equality();
  test_reified_relations();
  test_concat();
  test_concat_forward_projection_guards();
  test_concat_projects_result_structure();
  test_concat_fixed_result_wide_split_interval();
  test_concat_fixed_result_split_interval();
  test_concat_fixed_result_exact_split();
  test_concat_length_failure();
  std::cout << "All Dashed core tests passed.\n";
  return 0;
}
