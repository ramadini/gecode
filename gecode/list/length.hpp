#ifndef __GECODE_LIST_LENGTH_HPP__
#define __GECODE_LIST_LENGTH_HPP__

#include <gecode/list/propagator-adapter.hpp>
#include <gecode/int.hh>

namespace Gecode { namespace List {

/// Propagator for n = length(x).
class LengthPropagator final : public Propagator {
private:
  ListView x;
  Int::IntView n;

  LengthPropagator(Home home, ListView x0, Int::IntView n0)
      : Propagator(home),
        x(x0),
        n(n0) {
    x.subscribe(home, *this, PC_LIST_LEN);
    n.subscribe(home, *this, Int::PC_INT_BND);
  }

  LengthPropagator(Space& home, LengthPropagator& other)
      : Propagator(home, other) {
    x.update(home, other.x);
    n.update(home, other.n);
  }

public:
  static ExecStatus post(
      Home home,
      ListView x,
      Int::IntView n) {
    (void) new (home) LengthPropagator(home, x, n);
    return ES_OK;
  }

  Actor* copy(Space& home) override {
    return new (home) LengthPropagator(home, *this);
  }

  PropCost cost(
      const Space&,
      const ModEventDelta&) const override {
    return PropCost::binary(PropCost::LO);
  }

  void reschedule(Space& home) override {
    x.reschedule(home, *this, PC_LIST_LEN);
    n.reschedule(home, *this, Int::PC_INT_BND);
  }

  ExecStatus propagate(
      Space& home,
      const ModEventDelta&) override {
    return Adapter::length(
        home,
        *this,
        x,
        n);
  }

  size_t dispose(Space& home) override {
    x.cancel(home, *this, PC_LIST_LEN);
    n.cancel(home, *this, Int::PC_INT_BND);

    (void) Propagator::dispose(home);
    return sizeof(*this);
  }
};

}} // namespace Gecode::List


namespace Gecode {

/// Post n = length(x).
inline void
length(Home home, ListVar x, IntVar n) {
  GECODE_POST;

  GECODE_ES_FAIL(
      List::LengthPropagator::post(
          home,
          List::ListView(x),
          Int::IntView(n)));
}

} // namespace Gecode

#endif
