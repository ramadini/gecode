#include "dashed/detail/sweep.hpp"

#include <cassert>
#include <iostream>

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

void test_literals_are_not_expanded() {
  const Domain literal =
      Domain::fixed({1, 2, 3});

  const Domain repeated =
      Domain::repeat(
          ValueSet(1, 3),
          3,
          3);

  SweepAnalysis analysis;

  const SweepStatus status =
      dashed::detail::analyze_repeat_sweep(
          literal,
          repeated,
          analysis);

  // Literal support will use a segmented literal cursor in a later
  // checkpoint. It must not silently expand the literal here.
  assert(status == SweepStatus::unsupported);
  assert(analysis.blocks.empty());
}

}  // namespace


int main() {
  test_legacy_test02_matching_bounds();
  test_legacy_test02_projection();
  test_projection_detects_mandatory_incompatibility();
  test_literals_are_not_expanded();

  std::cout
      << "Dashed sweep matching tests passed\n";
}
