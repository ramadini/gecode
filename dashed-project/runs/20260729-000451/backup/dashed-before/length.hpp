#ifndef __GECODE_DASHED_LENGTH_HPP__
#define __GECODE_DASHED_LENGTH_HPP__

#include <gecode/dashed/list-view.hpp>
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
    const unsigned int gecode_max =
        static_cast<unsigned int>(Int::Limits::max);

    // No Gecode IntVar can represent such a length.
    if (x.min_length() > gecode_max)
      return ES_FAILED;

    const int xmin =
        static_cast<int>(x.min_length());

    const int xmax =
        (x.max_length() > gecode_max)
            ? Int::Limits::max
            : static_cast<int>(x.max_length());

    // List -> integer propagation.
    GECODE_ME_CHECK(n.gq(home, xmin));
    GECODE_ME_CHECK(n.lq(home, xmax));

    // Integer -> list propagation.
    // n is nonnegative after n.gq(home, xmin).
    GECODE_ME_CHECK(
        x.tighten_length(
            home,
            static_cast<unsigned int>(n.min()),
            static_cast<unsigned int>(n.max())));

    // The equation is permanently true once both length bounds
    // coincide and n is assigned to that value.
    if (n.assigned() &&
        x.min_length() == x.max_length()) {
      return home.ES_SUBSUMED(*this);
    }

    return ES_FIX;
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
