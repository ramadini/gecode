#include "dashed/detail/sweep.hpp"

#include <cassert>
#include <iostream>
#include <vector>

namespace {

using dashed::Domain;
using dashed::RepeatSegment;
using dashed::ValueSet;
using dashed::detail::SweepAnalysis;
using dashed::detail::SweepPosition;
using dashed::detail::SweepStatus;


void expect_position(
    const SweepPosition& actual,
    std::ptrdiff_t segment,
    dashed::Length offset) {
  assert(actual.segment == segment);
  assert(actual.offset == offset);
}


void test_legacy_test02_matching_bounds() {
  Domain x(
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

  Domain y(
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

  // Equality propagation normally performs this common-length
  // intersection before invoking the sweep analysis.
  const auto length_change =
      x.tighten_length(
          y.min_length(),
          y.max_length());

  assert(!dashed::failed(length_change));
  assert(x.min_length() == 270);
  assert(x.max_length() == 270);

  // Global length tightening also refines x's first block to 200..220.
  const auto& first =
      std::get<RepeatSegment>(
          x.segments().front());

  assert(first.lower == 200);
  assert(first.upper == 220);

  SweepAnalysis analysis;

  const SweepStatus status =
      dashed::detail::analyze_repeat_sweep(
          x,
          y,
          analysis);

  assert(status == SweepStatus::feasible);
  assert(analysis.blocks.size() == 3);

  // First block:
  // earliest  [0, 200]
  // latest    [0, 210]
  expect_position(
      analysis.blocks[0].earliest_start,
      0,
      0);
  expect_position(
      analysis.blocks[0].earliest_end,
      0,
      200);
  expect_position(
      analysis.blocks[0].latest_start,
      0,
      0);
  expect_position(
      analysis.blocks[0].latest_end,
      0,
      210);

  // Fixed {4}^50 block:
  // earliest [200, 250]
  // latest   [210, 260]
  expect_position(
      analysis.blocks[1].earliest_start,
      0,
      200);
  expect_position(
      analysis.blocks[1].earliest_end,
      0,
      250);
  expect_position(
      analysis.blocks[1].latest_start,
      0,
      210);
  expect_position(
      analysis.blocks[1].latest_end,
      0,
      260);

  // Last block:
  // earliest [250, 270]
  // latest   [260, 270]
  expect_position(
      analysis.blocks[2].earliest_start,
      0,
      250);
  expect_position(
      analysis.blocks[2].earliest_end,
      1,
      10);
  expect_position(
      analysis.blocks[2].latest_start,
      0,
      260);
  expect_position(
      analysis.blocks[2].latest_end,
      1,
      10);
}


void test_legacy_test02_projection() {
  Domain x(
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

  const Domain y(
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

  const auto length_change =
      x.tighten_length(270, 270);

  assert(!dashed::failed(length_change));

  Domain refined = x;

  const SweepStatus status =
      dashed::detail::project_against_exact_target(
          x,
          y,
          refined);

  assert(status == SweepStatus::feasible);

  const Domain expected(
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

  std::cerr << "projected: " << refined << '\n';
  std::cerr << "expected:  " << expected << '\n';

  assert(refined == expected);
}

void test_projection_detects_mandatory_incompatibility() {
  // The common length forces the first subject block to occur twice.
  // Its final {2} block must then align with the target's final {3},
  // making equality impossible.
  Domain subject(
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

  const Domain target(
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

  const auto length_change =
      subject.tighten_length(3, 3);

  assert(!dashed::failed(length_change));

  Domain refined = subject;

  const SweepStatus status =
      dashed::detail::project_against_exact_target(
          subject,
          target,
          refined);

  assert(status == SweepStatus::infeasible);

  // Projection is transactional: its output changes only on success.
  assert(refined == subject);
}

void test_variable_width_value_projection() {
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

  const auto y_length =
      y.tighten_length(0, 800);

  assert(!dashed::failed(y_length));

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

  Domain refined_x = x;

  const SweepStatus x_status =
      dashed::detail::project_repeat_values(
          x,
          y,
          refined_x);

  assert(x_status == SweepStatus::feasible);
  assert(refined_x == expected_x);

  // The same operation is directional. Applying it in the reverse
  // direction leaves y's values unchanged for this example.
  Domain refined_y = y;

  const SweepStatus y_status =
      dashed::detail::project_repeat_values(
          y,
          x,
          refined_y);

  assert(y_status == SweepStatus::feasible);
  assert(refined_y == y);
}

void test_boundary_cardinality_projection() {
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

  assert(
      !dashed::failed(
          x.tighten_length(1, 4)));

  Domain refined = x;

  const SweepStatus status =
      dashed::detail::project_repeat_regions(
          x,
          y,
          refined);

  assert(status == SweepStatus::feasible);

  const Domain expected(
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

  std::cerr
      << "region projected: "
      << refined << '\n';
  std::cerr
      << "region expected:  "
      << expected << '\n';

  assert(refined == expected);
}

void test_optional_suffix_literal_target_projection() {
  // State after the first directional equality sweep in legacy Test 14.
  const Domain target(
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

  // Normalization should store the two exact singleton blocks as a literal.
  assert(target.segment_count() == 2);
  assert(std::holds_alternative<dashed::LiteralSegment>(
      target.segments().back()));

  const Domain subject(
      {
          RepeatSegment{
              ValueSet(1, 3),
              1,
              5},
          RepeatSegment{
              ValueSet(4),
              0,
              1},
      },
      2,
      6);

  const Domain expected = target;

  SweepAnalysis analysis;

  const SweepStatus analysis_status =
      dashed::detail::analyze_repeat_sweep(
          subject,
          target,
          analysis);

  std::cerr
      << "literal-target analysis status: "
      << static_cast<int>(analysis_status)
      << '\n';

  for (std::size_t i = 0;
       i < analysis.blocks.size();
       ++i) {
    const auto& block = analysis.blocks[i];

    std::cerr
        << "block " << i
        << " earliest-start=("
        << block.earliest_start.segment << ','
        << block.earliest_start.offset << ')'
        << " earliest-end=("
        << block.earliest_end.segment << ','
        << block.earliest_end.offset << ')'
        << " latest-start=("
        << block.latest_start.segment << ','
        << block.latest_start.offset << ')'
        << " latest-end=("
        << block.latest_end.segment << ','
        << block.latest_end.offset << ')'
        << '\n';
  }

  Domain refined = subject;

  const SweepStatus status =
      dashed::detail::project_repeat_regions(
          subject,
          target,
          refined);

  std::cerr
      << "literal-target projection status: "
      << static_cast<int>(status)
      << '\n';
  std::cerr
      << "literal-target projected: "
      << refined << '\n';
  std::cerr
      << "literal-target expected:  "
      << expected << '\n';

  assert(status == SweepStatus::feasible);
  assert(refined == expected);
}

void test_fixed_literal_known_prefix_and_suffix() {
  {
    // Generic version of old Test 17.
    //
    // The leading 9 is accepted only by the first subject block, so the
    // projected representation must preserve it as an exact known prefix.
    const Domain target =
        Domain::fixed({9, 1, 2, 3});

    assert(target.segment_count() == 1);
    assert(std::holds_alternative<dashed::LiteralSegment>(
        target.segments().front()));

    const std::size_t bytes_before =
        target.referenced_dynamic_bytes();

    Domain subject(
        {
            RepeatSegment{
                ValueSet(0, 9),
                1,
                4},
            RepeatSegment{
                ValueSet(0, 8),
                0,
                4},
        },
        1,
        8);

    const auto length_change =
        subject.tighten_length(4, 4);

    assert(!dashed::failed(length_change));

    Domain refined = subject;

    const SweepStatus status =
        dashed::detail::project_repeat_regions(
            subject,
            target,
            refined);

    const Domain expected(
        {
            RepeatSegment{
                ValueSet(9),
                1,
                1},
            RepeatSegment{
                ValueSet(1, 3),
                3,
                3},
        },
        4,
        4);

    std::cerr
        << "known-prefix projected: "
        << refined << '\n';
    std::cerr
        << "known-prefix expected:  "
        << expected << '\n';

    assert(status == SweepStatus::feasible);
    assert(refined == expected);

    // The target remains one shared literal rather than being rewritten
    // into one domain segment per value.
    assert(target.segment_count() == 1);
    assert(target.referenced_dynamic_bytes() ==
           bytes_before);
  }

  {
    // Generic stress version of old Test 18.
    //
    // The final 9 is accepted only by the second subject block, so it must
    // be preserved as an exact known suffix. The long literal also checks
    // that virtual traversal does not expand the target domain.
    constexpr dashed::Length n = 4096;

    std::vector<int> values(
        static_cast<std::size_t>(n));

    for (std::size_t i = 0;
         i < values.size();
         ++i) {
      values[i] =
          1 + static_cast<int>(i % 3);
    }

    values.back() = 9;

    const Domain target =
        Domain::fixed(std::move(values));

    assert(target.segment_count() == 1);
    assert(std::holds_alternative<dashed::LiteralSegment>(
        target.segments().front()));

    const std::size_t bytes_before =
        target.referenced_dynamic_bytes();

    Domain subject(
        {
            RepeatSegment{
                ValueSet(0, 8),
                0,
                n},
            RepeatSegment{
                ValueSet(0, 9),
                1,
                n},
        },
        1,
        static_cast<dashed::Length>(2 * n));

    const auto length_change =
        subject.tighten_length(n, n);

    assert(!dashed::failed(length_change));

    Domain refined = subject;

    const SweepStatus status =
        dashed::detail::project_repeat_regions(
            subject,
            target,
            refined);

    const Domain expected(
        {
            RepeatSegment{
                ValueSet(1, 3),
                static_cast<dashed::Length>(n - 1),
                static_cast<dashed::Length>(n - 1)},
            RepeatSegment{
                ValueSet(9),
                1,
                1},
        },
        n,
        n);

    std::cerr
        << "known-suffix segments: "
        << refined.segment_count()
        << '\n';

    assert(status == SweepStatus::feasible);
    assert(refined == expected);

    assert(target.segment_count() == 1);
    assert(target.referenced_dynamic_bytes() ==
           bytes_before);
  }
}

void test_literals_are_not_expanded() {
  const Domain subject =
      Domain::repeat(
          ValueSet(1, 3),
          3,
          3);

  const Domain target =
      Domain::fixed({1, 2, 3});

  assert(target.segment_count() == 1);
  assert(std::holds_alternative<dashed::LiteralSegment>(
      target.segments().front()));

  const std::size_t bytes_before =
      target.referenced_dynamic_bytes();

  SweepAnalysis analysis;

  const SweepStatus status =
      dashed::detail::analyze_repeat_sweep(
          subject,
          target,
          analysis);

  assert(status == SweepStatus::feasible);

  // The subject still has one logical block. The target literal is traversed
  // through virtual singleton views and is not rewritten into three segments.
  assert(analysis.blocks.size() == 1);
  assert(target.segment_count() == 1);
  assert(target.referenced_dynamic_bytes() ==
         bytes_before);
}

}  // namespace


int main() {
  test_legacy_test02_matching_bounds();
  test_legacy_test02_projection();
  test_projection_detects_mandatory_incompatibility();
  test_variable_width_value_projection();
  test_boundary_cardinality_projection();
  test_optional_suffix_literal_target_projection();
  test_fixed_literal_known_prefix_and_suffix();
  test_literals_are_not_expanded();

  std::cout
      << "Dashed sweep matching tests passed\n";
}
