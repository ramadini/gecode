#ifndef __GECODE_DASHED_REL_HPP__
#define __GECODE_DASHED_REL_HPP__

#include <gecode/dashed/propagator-adapter.hpp>
#include <gecode/int.hh>

namespace Gecode { namespace Dashed {

class Equal final : public BinaryPropagator<ListView, PC_LIST_ANY> {
  using Base = BinaryPropagator<ListView, PC_LIST_ANY>;

protected:
  Equal(Home home, ListView x0, ListView x1)
      : Base(home, x0, x1) {}

  Equal(Space& home, Equal& other)
      : Base(home, other) {}

public:
  static ExecStatus post(Home home, ListView x0, ListView x1) {
    if (x0.varimp() == x1.varimp())
      return ES_OK;
    (void) new (home) Equal(home, x0, x1);
    return ES_OK;
  }

  Actor* copy(Space& home) override {
    return new (home) Equal(home, *this);
  }

  ExecStatus propagate(Space& home,
                       const ModEventDelta&) override {
    return Adapter::equal(home, *this, x0, x1);
  }
};

class NotEqual final : public BinaryPropagator<ListView, PC_LIST_ANY> {
  using Base = BinaryPropagator<ListView, PC_LIST_ANY>;

protected:
  NotEqual(Home home, ListView x0, ListView x1)
      : Base(home, x0, x1) {}

  NotEqual(Space& home, NotEqual& other)
      : Base(home, other) {}

public:
  static ExecStatus post(Home home, ListView x0, ListView x1) {
    if (x0.varimp() == x1.varimp())
      return ES_FAILED;
    (void) new (home) NotEqual(home, x0, x1);
    return ES_OK;
  }

  Actor* copy(Space& home) override {
    return new (home) NotEqual(home, *this);
  }

  ExecStatus propagate(Space& home,
                       const ModEventDelta&) override {
    return Adapter::not_equal(home, *this, x0, x1);
  }
};

class ReEqual final
    : public Int::ReBinaryPropagator<ListView, PC_LIST_ANY,
                                     Int::BoolView> {
  using Base = Int::ReBinaryPropagator<ListView, PC_LIST_ANY,
                                        Int::BoolView>;

protected:
  ReEqual(Home home, ListView x0, ListView x1, Int::BoolView b)
      : Base(home, x0, x1, b) {}

  ReEqual(Space& home, ReEqual& other)
      : Base(home, other) {}

public:
  static ExecStatus post(Home home, ListView x0, ListView x1,
                         Int::BoolView b) {
    if (x0.varimp() == x1.varimp())
      return me_failed(b.one(home)) ? ES_FAILED : ES_OK;
    (void) new (home) ReEqual(home, x0, x1, b);
    return ES_OK;
  }

  Actor* copy(Space& home) override {
    return new (home) ReEqual(home, *this);
  }

  ExecStatus propagate(Space& home,
                       const ModEventDelta&) override {
    return Adapter::reified_equal(home, *this, x0, x1, b);
  }
};

class ReNotEqual final
    : public Int::ReBinaryPropagator<ListView, PC_LIST_ANY,
                                     Int::BoolView> {
  using Base = Int::ReBinaryPropagator<ListView, PC_LIST_ANY,
                                        Int::BoolView>;

protected:
  ReNotEqual(Home home, ListView x0, ListView x1, Int::BoolView b)
      : Base(home, x0, x1, b) {}

  ReNotEqual(Space& home, ReNotEqual& other)
      : Base(home, other) {}

public:
  static ExecStatus post(Home home, ListView x0, ListView x1,
                         Int::BoolView b) {
    if (x0.varimp() == x1.varimp())
      return me_failed(b.zero(home)) ? ES_FAILED : ES_OK;
    (void) new (home) ReNotEqual(home, x0, x1, b);
    return ES_OK;
  }

  Actor* copy(Space& home) override {
    return new (home) ReNotEqual(home, *this);
  }

  ExecStatus propagate(Space& home,
                       const ModEventDelta&) override {
    return Adapter::reified_not_equal(home, *this, x0, x1, b);
  }
};

}} // namespace Gecode::Dashed

namespace Gecode {

inline void rel(Home home, ListVar x, IntRelType relation, ListVar y,
                IntPropLevel = IPL_DEF) {
  GECODE_POST;

  switch (relation) {
  case IRT_EQ:
    GECODE_ES_FAIL(Dashed::Equal::post(
        home, Dashed::ListView(x), Dashed::ListView(y)));
    break;
  case IRT_NQ:
    GECODE_ES_FAIL(Dashed::NotEqual::post(
        home, Dashed::ListView(x), Dashed::ListView(y)));
    break;
  default:
    throw Int::UnknownRelation("Dashed::rel");
  }
}

inline void rel(Home home, ListVar x, IntRelType relation, ListVar y,
                Reify reification, IntPropLevel = IPL_DEF) {
  GECODE_POST;

  // The standalone kernel currently implements equivalence reification.
  // Half-reification is intentionally rejected rather than approximated.
  if (reification.mode() != RM_EQV)
    throw Int::UnknownReifyMode("Dashed::rel");

  Int::BoolView b(reification.var());
  switch (relation) {
  case IRT_EQ:
    GECODE_ES_FAIL(Dashed::ReEqual::post(
        home, Dashed::ListView(x), Dashed::ListView(y), b));
    break;
  case IRT_NQ:
    GECODE_ES_FAIL(Dashed::ReNotEqual::post(
        home, Dashed::ListView(x), Dashed::ListView(y), b));
    break;
  default:
    throw Int::UnknownRelation("Dashed::rel");
  }
}

} // namespace Gecode

#endif
