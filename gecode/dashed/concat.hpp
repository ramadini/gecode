#ifndef __GECODE_DASHED_CONCAT_HPP__
#define __GECODE_DASHED_CONCAT_HPP__

#include <gecode/dashed/propagator-adapter.hpp>
#include <gecode/int.hh>

namespace Gecode { namespace Dashed {

/// Propagator for z = x ++ y.
class Concat final : public TernaryPropagator<ListView, PC_LIST_ANY> {
  using Base = TernaryPropagator<ListView, PC_LIST_ANY>;

protected:
  // Stored order is z, x, y to match the standalone kernel adapter.
  Concat(Home home, ListView z, ListView x, ListView y)
      : Base(home, z, x, y) {}

  Concat(Space& home, Concat& other)
      : Base(home, other) {}

public:
  static ExecStatus post(Home home, ListView z,
                         ListView x, ListView y) {
    (void) new (home) Concat(home, z, x, y);
    return ES_OK;
  }

  Actor* copy(Space& home) override {
    return new (home) Concat(home, *this);
  }

  ExecStatus propagate(Space& home,
                       const ModEventDelta&) override {
    return Adapter::concat(home, *this, x0, x1, x2);
  }
};

}} // namespace Gecode::Dashed

namespace Gecode {

/// Post z = x ++ y.
inline void concat(Home home, ListVar x, ListVar y, ListVar z) {
  GECODE_POST;

  // Alias-safe transactional commits require a dedicated implementation.
  // Reject aliases explicitly for this first native milestone.
  if (x.varimp() == y.varimp() ||
      x.varimp() == z.varimp() ||
      y.varimp() == z.varimp())
    throw Int::ArgumentSame("Dashed::concat");

  GECODE_ES_FAIL(Dashed::Concat::post(
      home, Dashed::ListView(z),
      Dashed::ListView(x), Dashed::ListView(y)));
}

} // namespace Gecode

#endif
