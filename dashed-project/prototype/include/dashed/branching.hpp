#pragma once

#include "dashed/domain.hpp"

#include <cstddef>
#include <optional>

namespace dashed {

enum class BranchKind {
  repeat_count,
  value_set
};

/**
 * Stable description of a binary domain partition.
 *
 * repeat_count: alternative 0 restricts the selected block to counts <= pivot;
 *               alternative 1 restricts it to counts > pivot.
 * value_set:    alternative 0 retains values <= value_pivot at one isolated
 *               logical position; alternative 1 retains values above it.
 */
struct BranchDecision {
  BranchKind kind = BranchKind::repeat_count;
  Length length_pivot = 0;
  Length position = 0;
  int value_pivot = 0;
};

enum class BranchLiteralStatus {
  failed,
  subsumed,
  undecided
};

/** Choose the first deterministic exact split, or no exact decision yet. */
[[nodiscard]] std::optional<BranchDecision> choose_branch(
    const Domain& domain);

/** Apply one alternative of a previously selected exact branch decision. */
[[nodiscard]] Domain apply_branch(
    const Domain& domain,
    const BranchDecision& decision,
    unsigned int alternative);

/** Classify an archived branch alternative over any descendant domain. */
[[nodiscard]] BranchLiteralStatus branch_literal_status(
    const Domain& domain,
    const BranchDecision& decision,
    unsigned int alternative);

/** Remove an archived branch alternative from a descendant domain. */
[[nodiscard]] Domain prune_branch_literal(
    const Domain& domain,
    const BranchDecision& decision,
    unsigned int alternative);

}  // namespace dashed
