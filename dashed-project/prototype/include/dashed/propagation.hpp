#pragma once

#include "dashed/change.hpp"
#include "dashed/domain.hpp"

#include <cstdint>

namespace dashed {

struct PropagationResult {
  Change left = Change::none;
  Change right = Change::none;
  Change result = Change::none;
  bool subsumed = false;

  [[nodiscard]] bool failed() const noexcept {
    return left == Change::failed || right == Change::failed ||
           result == Change::failed;
  }
};

/** Conservative kernel for x = y. */
PropagationResult propagate_equal(Domain& x, Domain& y);

/** Conservative kernel for x != y. */
PropagationResult propagate_not_equal(Domain& x, Domain& y);

struct BoolDomain {
  bool may_be_false = true;
  bool may_be_true = true;

  [[nodiscard]] bool failed() const noexcept {
    return !may_be_false && !may_be_true;
  }
  [[nodiscard]] bool assigned() const noexcept {
    return may_be_false != may_be_true;
  }
  [[nodiscard]] bool value() const;
  Change force(bool value) noexcept;
};

/** b <-> (x = y). */
PropagationResult propagate_reified_equal(Domain& x, Domain& y, BoolDomain& b);

/** b <-> (x != y). */
PropagationResult propagate_reified_not_equal(Domain& x, Domain& y, BoolDomain& b);


/** b -> (x = y). */
PropagationResult propagate_implied_equal(
    Domain& x,
    Domain& y,
    BoolDomain& b);

/** (x = y) -> b. */
PropagationResult propagate_equal_implies(
    Domain& x,
    Domain& y,
    BoolDomain& b);

/** b -> (x != y). */
PropagationResult propagate_implied_not_equal(
    Domain& x,
    Domain& y,
    BoolDomain& b);

/** (x != y) -> b. */
PropagationResult propagate_not_equal_implies(
    Domain& x,
    Domain& y,
    BoolDomain& b);

/** Conservative kernel for z = x ++ y. */
PropagationResult propagate_concat(Domain& z, Domain& x, Domain& y);

/** Propagate |x| = length for an interval-domain integer length variable. */
struct IntBounds {
  std::int64_t lower = 0;
  std::int64_t upper = 0;
  bool failed = false;

  [[nodiscard]] bool assigned() const noexcept {
    return !failed && lower == upper;
  }
};

PropagationResult propagate_length(Domain& x, IntBounds& length);

}  // namespace dashed
