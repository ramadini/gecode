#pragma once

#include "dashed/domain.hpp"

#include <cstddef>
#include <vector>

namespace dashed::detail {

struct SweepPosition {
  std::ptrdiff_t segment = -1;
  Length offset = 0;

  friend constexpr bool operator==(
      const SweepPosition& lhs,
      const SweepPosition& rhs) noexcept {
    return lhs.segment == rhs.segment &&
           lhs.offset == rhs.offset;
  }

  friend constexpr bool operator!=(
      const SweepPosition& lhs,
      const SweepPosition& rhs) noexcept {
    return !(lhs == rhs);
  }
};

struct SweepBlockMatch {
  // All positions use forward coordinates in the target domain.
  SweepPosition earliest_start;
  SweepPosition earliest_end;
  SweepPosition latest_start;
  SweepPosition latest_end;
};

struct SweepAnalysis {
  std::vector<SweepBlockMatch> blocks;
};

enum class SweepStatus {
  feasible,
  infeasible,
  unsupported
};

/**
 * Compute matching bounds for every block of subject in target.
 *
 * This initial implementation supports RepeatSegment-only domains. It does
 * not modify either domain.
 */
[[nodiscard]] SweepStatus analyze_repeat_sweep(
    const Domain& subject,
    const Domain& target,
    SweepAnalysis& analysis);


/**
 * Refine a repeat-only subject against a repeat-only target whose block
 * counts are exact. The target is not modified.
 *
 * Returns unsupported when either domain contains literals or the target
 * contains a variable-width block.
 */
[[nodiscard]] SweepStatus project_against_exact_target(
    const Domain& subject,
    const Domain& target,
    Domain& refined);


/**
 * Refine the value set of every repeat block in subject using the union of
 * target-block value sets in its feasible sweep region.
 *
 * Block cardinalities and structure are preserved. This supports variable
 * target widths but still requires RepeatSegment-only domains.
 */
[[nodiscard]] SweepStatus project_repeat_values(
    const Domain& subject,
    const Domain& target,
    Domain& refined);


/**
 * Refine a repeat-only subject using the optional and mandatory regions
 * identified by sweep analysis.
 *
 * A subject block can be replaced by multiple repeat segments when a
 * mandatory target prefix or suffix must be represented explicitly.
 */
[[nodiscard]] SweepStatus project_repeat_regions(
    const Domain& subject,
    const Domain& target,
    Domain& refined);

}  // namespace dashed::detail
