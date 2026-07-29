#pragma once

// Transactional bridges from the tested standalone kernels to Gecode views.
// This header is a blueprint until ListVar is generated and compiled in a
// Gecode 6.4 source tree.

#include <gecode/list/backend.hpp>
#include <gecode/int.hh>

#include "list-view.hpp"

#include <utility>

namespace Gecode { namespace List {
namespace Adapter {

inline bool commit_list(Space& home, ListView& view,
                        Domain domain, Backend::Change change) {
  if (!Backend::changed(change))
    return change != Backend::Change::failed;
  return !me_failed(view.replace(home, std::move(domain)));
}

inline Backend::BoolDomain read_bool(const Int::BoolView& view) {
  return Backend::BoolDomain{!view.one(), !view.zero()};
}

inline bool commit_bool(Space& home, Int::BoolView& view,
                        const Backend::BoolDomain& domain) {
  if (domain.failed())
    return false;
  if (!domain.assigned())
    return true;
  const ModEvent me = domain.value() ? view.one(home) : view.zero(home);
  return !me_failed(me);
}

inline bool commit_bounds(Space& home, Int::IntView& view,
                          const Backend::IntBounds& bounds) {
  if (bounds.failed)
    return false;
  ModEvent me = view.gq(home, static_cast<long long>(bounds.lower));
  if (me_failed(me))
    return false;
  me = view.lq(home, static_cast<long long>(bounds.upper));
  return !me_failed(me);
}

inline ExecStatus equal(Space& home, Propagator& propagator,
                        ListView& x, ListView& y) {
  Domain dx = x.domain();
  Domain dy = y.domain();
  const auto result = Backend::propagate_equal(dx, dy);
  if (result.failed())
    return ES_FAILED;
  if (!commit_list(home, x, std::move(dx), result.left) ||
      !commit_list(home, y, std::move(dy), result.right))
    return ES_FAILED;
  return result.subsumed ? home.ES_SUBSUMED(propagator) : ES_FIX;
}

inline ExecStatus not_equal(Space& home, Propagator& propagator,
                            ListView& x, ListView& y) {
  Domain dx = x.domain();
  Domain dy = y.domain();
  const auto result = Backend::propagate_not_equal(dx, dy);
  if (result.failed())
    return ES_FAILED;
  if (!commit_list(home, x, std::move(dx), result.left) ||
      !commit_list(home, y, std::move(dy), result.right))
    return ES_FAILED;
  return result.subsumed ? home.ES_SUBSUMED(propagator) : ES_FIX;
}

inline ExecStatus reified_equal(
    Space& home,
    Propagator& propagator,
    ListView& x,
    ListView& y,
    Int::BoolView& truth,
    ReifyMode mode) {
  Domain dx = x.domain();
  Domain dy = y.domain();
  Backend::BoolDomain db =
      read_bool(truth);

  Backend::PropagationResult result;

  switch (mode) {
    case RM_EQV:
      result =
          Backend::propagate_reified_equal(
              dx,
              dy,
              db);
      break;

    case RM_IMP:
      result =
          Backend::propagate_implied_equal(
              dx,
              dy,
              db);
      break;

    case RM_PMI:
      result =
          Backend::propagate_equal_implies(
              dx,
              dy,
              db);
      break;
  }

  if (result.failed()) {
    return ES_FAILED;
  }

  if (!commit_list(
          home,
          x,
          std::move(dx),
          result.left) ||
      !commit_list(
          home,
          y,
          std::move(dy),
          result.right) ||
      !commit_bool(
          home,
          truth,
          db)) {
    return ES_FAILED;
  }

  return result.subsumed
      ? home.ES_SUBSUMED(propagator)
      : ES_FIX;
}

inline ExecStatus reified_not_equal(
    Space& home,
    Propagator& propagator,
    ListView& x,
    ListView& y,
    Int::BoolView& truth,
    ReifyMode mode) {
  Domain dx = x.domain();
  Domain dy = y.domain();
  Backend::BoolDomain db =
      read_bool(truth);

  Backend::PropagationResult result;

  switch (mode) {
    case RM_EQV:
      result =
          Backend::propagate_reified_not_equal(
              dx,
              dy,
              db);
      break;

    case RM_IMP:
      result =
          Backend::propagate_implied_not_equal(
              dx,
              dy,
              db);
      break;

    case RM_PMI:
      result =
          Backend::propagate_not_equal_implies(
              dx,
              dy,
              db);
      break;
  }

  if (result.failed()) {
    return ES_FAILED;
  }

  if (!commit_list(
          home,
          x,
          std::move(dx),
          result.left) ||
      !commit_list(
          home,
          y,
          std::move(dy),
          result.right) ||
      !commit_bool(
          home,
          truth,
          db)) {
    return ES_FAILED;
  }

  return result.subsumed
      ? home.ES_SUBSUMED(propagator)
      : ES_FIX;
}

inline ExecStatus concat(Space& home, Propagator& propagator,
                         ListView& z, ListView& x, ListView& y) {
  Domain dz = z.domain();
  Domain dx = x.domain();
  Domain dy = y.domain();
  const auto result = Backend::propagate_concat(dz, dx, dy);
  if (result.failed())
    return ES_FAILED;
  if (!commit_list(home, x, std::move(dx), result.left) ||
      !commit_list(home, y, std::move(dy), result.right) ||
      !commit_list(home, z, std::move(dz), result.result))
    return ES_FAILED;
  return result.subsumed ? home.ES_SUBSUMED(propagator) : ES_FIX;
}

inline ExecStatus length(Space& home, Propagator& propagator,
                         ListView& list, Int::IntView& length_view) {
  Domain domain = list.domain();
  Backend::IntBounds bounds{length_view.min(), length_view.max(), false};
  const auto result = Backend::propagate_length(domain, bounds);
  if (result.failed())
    return ES_FAILED;
  if (!commit_list(home, list, std::move(domain), result.left) ||
      !commit_bounds(home, length_view, bounds))
    return ES_FAILED;
  return result.subsumed ? home.ES_SUBSUMED(propagator) : ES_FIX;
}

}  // namespace Adapter
}}  // namespace Gecode::List
