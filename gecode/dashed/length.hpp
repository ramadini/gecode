#ifndef __GECODE_DASHED_LENGTH_HPP__
#define __GECODE_DASHED_LENGTH_HPP__

#include <gecode/dashed/propagator-adapter.hpp>
#include <gecode/int.hh>

namespace Gecode { namespace Dashed {

/// Propagator for n = length(x).
class Length final : public Propagator {
private:
  ListView x;
  Int::IntView n;

  Length(Home home, ListView x0, Int::IntView n0)
      : Propagator(home),
        x(x0),
        n(n0) {
    x.subscribe(home, *this, PC_LIST_LEN);
    n.subscribe(home, *this, Int::PC_INT_BND);
  }

  Length(Space& home, Length& other)
      : Propagator(home, other) {
    x.update(home, other.x);
    n.update(home, other.n);
  }

public:
  static ExecStatus post(
      Home home,
      ListView x,
      Int::IntView n) {
    (void) new (home) Length(home, x, n);
    return ES_OK;
  }

  Actor* copy(Space& home) override {
    return new (home) Length(home, *this);
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

}} // namespace Gecode::Dashed


namespace Gecode {

/// Post n = length(x).
inline void
length(Home home, ListVar x, IntVar n) {
  GECODE_POST;

  GECODE_ES_FAIL(
      Dashed::Length::post(
          home,
          Dashed::ListView(x),
          Int::IntView(n)));
}

} // namespace Gecode

#endif
