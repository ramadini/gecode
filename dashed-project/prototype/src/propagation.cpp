#include "dashed/propagation.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>

namespace dashed {
namespace {

Change assign_exact(Domain& target, const Domain& exact) {
  if (!exact.assigned()) {
    throw std::logic_error("assign_exact requires an assigned source");
  }
  if (!target.accepts(exact)) {
    target.fail();
    return Change::failed;
  }
  if (target == exact) {
    return Change::none;
  }
  target = exact;
  return Change::assigned;
}

bool same_value(const Domain& lhs, const Domain& rhs) {
  return lhs.assigned_equal(rhs);
}

Length clamp_length(std::int64_t value) {
  if (value <= 0) {
    return 0;
  }
  if (value >= static_cast<std::int64_t>(kUnboundedLength)) {
    return kUnboundedLength;
  }
  return static_cast<Length>(value);
}

}  // namespace

bool BoolDomain::value() const {
  if (!assigned()) {
    throw std::logic_error("value requested from unassigned BoolDomain");
  }
  return may_be_true;
}

Change BoolDomain::force(bool forced_value) noexcept {
  if (failed()) {
    return Change::failed;
  }
  if (forced_value) {
    if (!may_be_true) {
      may_be_false = false;
      return Change::failed;
    }
    if (!may_be_false) {
      return Change::none;
    }
    may_be_false = false;
  } else {
    if (!may_be_false) {
      may_be_true = false;
      return Change::failed;
    }
    if (!may_be_true) {
      return Change::none;
    }
    may_be_true = false;
  }
  return Change::assigned;
}

PropagationResult propagate_equal(Domain& x, Domain& y) {
  PropagationResult result;
  if (x.failed() || y.failed()) {
    x.fail();
    y.fail();
    result.left = result.right = Change::failed;
    return result;
  }

  const Length lower = std::max(x.min_length(), y.min_length());
  const Length upper = std::min(x.max_length(), y.max_length());
  result.left = combine(result.left, x.tighten_length(lower, upper));
  result.right = combine(result.right, y.tighten_length(lower, upper));
  if (result.failed()) {
    return result;
  }

  if (x.assigned()) {
    result.right = combine(result.right, assign_exact(y, x));
  }
  if (!result.failed() && y.assigned()) {
    result.left = combine(result.left, assign_exact(x, y));
  }
  if (result.failed()) {
    return result;
  }

  if (!x.assigned() && !y.assigned()) {
    result.left = combine(result.left, x.intersect_same_shape(y));
    if (!result.failed()) {
      result.right = combine(result.right, y.intersect_same_shape(x));
    }
  }

  result.subsumed = !result.failed() && same_value(x, y);
  return result;
}

PropagationResult propagate_not_equal(Domain& x, Domain& y) {
  PropagationResult result;
  if (x.failed() || y.failed()) {
    result.left = result.right = Change::failed;
    return result;
  }
  if (x.max_length() < y.min_length() || y.max_length() < x.min_length()) {
    result.subsumed = true;
    return result;
  }
  if (x.assigned() && y.assigned()) {
    if (x.assigned_equal(y)) {
      x.fail();
      result.left = Change::failed;
    } else {
      result.subsumed = true;
    }
  }
  return result;
}

PropagationResult propagate_reified_equal(Domain& x, Domain& y,
                                            BoolDomain& b) {
  PropagationResult result;
  if (b.failed()) {
    result.result = Change::failed;
    return result;
  }

  if (!x.may_equal(y)) {
    result.result = b.force(false);
    result.subsumed = !result.failed() && b.assigned();
    return result;
  }
  if (x.assigned() && y.assigned()) {
    result.result = b.force(x.assigned_equal(y));
    result.subsumed = !result.failed();
    return result;
  }
  if (b.assigned()) {
    PropagationResult inner = b.value() ? propagate_equal(x, y)
                                        : propagate_not_equal(x, y);
    inner.result = Change::none;
    return inner;
  }
  return result;
}

PropagationResult propagate_reified_not_equal(Domain& x, Domain& y,
                                                BoolDomain& b) {
  BoolDomain equality{b.may_be_true, b.may_be_false};
  PropagationResult result = propagate_reified_equal(x, y, equality);
  b.may_be_false = equality.may_be_true;
  b.may_be_true = equality.may_be_false;
  return result;
}

PropagationResult propagate_concat(Domain& z, Domain& x, Domain& y) {
  PropagationResult result;
  if (z.failed() || x.failed() || y.failed()) {
    result.left = result.right = result.result = Change::failed;
    return result;
  }

  bool again = true;
  while (again && !result.failed()) {
    again = false;
    const Length old_z_min = z.min_length();
    const Length old_z_max = z.max_length();
    const Length old_x_min = x.min_length();
    const Length old_x_max = x.max_length();
    const Length old_y_min = y.min_length();
    const Length old_y_max = y.max_length();

    const std::uint64_t sum_min =
        static_cast<std::uint64_t>(x.min_length()) + y.min_length();
    const std::uint64_t sum_max =
        static_cast<std::uint64_t>(x.max_length()) + y.max_length();
    result.result = combine(
        result.result,
        z.tighten_length(
            sum_min >= kUnboundedLength ? kUnboundedLength
                                        : static_cast<Length>(sum_min),
            sum_max >= kUnboundedLength ? kUnboundedLength
                                        : static_cast<Length>(sum_max)));
    if (result.failed()) {
      break;
    }

    const Length x_lower =
        z.min_length() > y.max_length() ? z.min_length() - y.max_length() : 0;
    const Length x_upper =
        z.max_length() >= y.min_length() ? z.max_length() - y.min_length() : 0;
    result.left = combine(result.left, x.tighten_length(x_lower, x_upper));
    if (result.failed()) {
      break;
    }

    const Length y_lower =
        z.min_length() > x.max_length() ? z.min_length() - x.max_length() : 0;
    const Length y_upper =
        z.max_length() >= x.min_length() ? z.max_length() - x.min_length() : 0;
    result.right = combine(result.right, y.tighten_length(y_lower, y_upper));

    again = old_z_min != z.min_length() || old_z_max != z.max_length() ||
            old_x_min != x.min_length() || old_x_max != x.max_length() ||
            old_y_min != y.min_length() || old_y_max != y.max_length();
  }
  if (result.failed()) {
    return result;
  }

  if (x.assigned() && y.assigned()) {
    Domain exact = x.concatenated(y);
    result.result = combine(result.result, assign_exact(z, exact));
  }
  if (result.failed()) {
    return result;
  }

  if (z.assigned() && x.assigned()) {
    if (x.min_length() > z.min_length() ||
        z.assigned_prefix(x.min_length()).value() != x.value()) {
      z.fail();
      result.result = Change::failed;
      return result;
    }
    Domain suffix = z.assigned_suffix(z.min_length() - x.min_length());
    result.right = combine(result.right, assign_exact(y, suffix));
  }
  if (result.failed()) {
    return result;
  }

  if (z.assigned() && y.assigned()) {
    if (y.min_length() > z.min_length() ||
        z.assigned_suffix(y.min_length()).value() != y.value()) {
      z.fail();
      result.result = Change::failed;
      return result;
    }
    Domain prefix = z.assigned_prefix(z.min_length() - y.min_length());
    result.left = combine(result.left, assign_exact(x, prefix));
  }

  result.subsumed = !result.failed() && z.assigned() && x.assigned() &&
                    y.assigned() &&
                    z.assigned_equal(x.concatenated(y));
  return result;
}

PropagationResult propagate_length(Domain& x, IntBounds& length) {
  PropagationResult result;
  if (x.failed() || length.failed || length.lower > length.upper) {
    x.fail();
    length.failed = true;
    result.left = result.result = Change::failed;
    return result;
  }

  const std::int64_t old_lower = length.lower;
  const std::int64_t old_upper = length.upper;
  length.lower = std::max<std::int64_t>(length.lower, x.min_length());
  length.upper = std::min<std::int64_t>(length.upper, x.max_length());
  if (length.lower > length.upper) {
    x.fail();
    length.failed = true;
    result.left = result.result = Change::failed;
    return result;
  }
  if (length.lower != old_lower || length.upper != old_upper) {
    result.result = length.assigned() ? Change::assigned : Change::length;
  }

  result.left = x.tighten_length(clamp_length(length.lower),
                                 clamp_length(length.upper));
  if (result.left == Change::failed) {
    length.failed = true;
    result.result = Change::failed;
    return result;
  }
  result.subsumed = !x.failed() && length.assigned() &&
                    x.min_length() == x.max_length() &&
                    static_cast<std::int64_t>(x.min_length()) == length.lower;
  return result;
}

}  // namespace dashed
