#ifndef __GECODE_LIST_BACKEND_HPP__
#define __GECODE_LIST_BACKEND_HPP__

#include <dashed/branching.hpp>
#include <dashed/propagation.hpp>
#include <gecode/list/domain.hpp>

namespace Gecode { namespace List { namespace Backend {

// This is the only native-module seam that names the current dashed engine.
using Change = dashed::Change;
using BoolDomain = dashed::BoolDomain;
using IntBounds = dashed::IntBounds;
using PropagationResult = dashed::PropagationResult;
using BranchKind = dashed::BranchKind;
using BranchDecision = dashed::BranchDecision;
using BranchLiteralStatus = dashed::BranchLiteralStatus;

inline bool changed(Change change) noexcept {
  return dashed::changed(change);
}

using dashed::apply_branch;
using dashed::branch_literal_status;
using dashed::choose_branch;
using dashed::prune_branch_literal;
using dashed::propagate_concat;
using dashed::propagate_equal;
using dashed::propagate_equal_implies;
using dashed::propagate_implied_equal;
using dashed::propagate_implied_not_equal;
using dashed::propagate_length;
using dashed::propagate_not_equal;
using dashed::propagate_not_equal_implies;
using dashed::propagate_reified_equal;
using dashed::propagate_reified_not_equal;

}}} // namespace Gecode::List::Backend

#endif
