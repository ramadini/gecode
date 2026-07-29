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
    if (x0 == x1)
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
    if (x0 == x1)
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
    : public Int::ReBinaryPropagator<
          ListView,
          PC_LIST_ANY,
          Int::BoolView> {
  using Base =
      Int::ReBinaryPropagator<
          ListView,
          PC_LIST_ANY,
          Int::BoolView>;

protected:
  ReifyMode mode;

  ReEqual(
      Home home,
      ListView x0,
      ListView x1,
      Int::BoolView b,
      ReifyMode mode0)
      : Base(home, x0, x1, b),
        mode(mode0) {}

  ReEqual(
      Space& home,
      ReEqual& other)
      : Base(home, other),
        mode(other.mode) {}

public:
  static ExecStatus post(
      Home home,
      ListView x0,
      ListView x1,
      Int::BoolView b,
      ReifyMode mode) {
    if (x0 == x1) {
      switch (mode) {
        case RM_EQV:
        case RM_PMI:
          return me_failed(
              b.one(home))
              ? ES_FAILED
              : ES_OK;

        case RM_IMP:
          return ES_OK;
      }
    }

    (void) new (home)
        ReEqual(
            home,
            x0,
            x1,
            b,
            mode);

    return ES_OK;
  }

  Actor* copy(
      Space& home) override {
    return new (home)
        ReEqual(
            home,
            *this);
  }

  ExecStatus propagate(
      Space& home,
      const ModEventDelta&) override {
    return Adapter::reified_equal(
        home,
        *this,
        x0,
        x1,
        b,
        mode);
  }
};


class ReNotEqual final
    : public Int::ReBinaryPropagator<
          ListView,
          PC_LIST_ANY,
          Int::BoolView> {
  using Base =
      Int::ReBinaryPropagator<
          ListView,
          PC_LIST_ANY,
          Int::BoolView>;

protected:
  ReifyMode mode;

  ReNotEqual(
      Home home,
      ListView x0,
      ListView x1,
      Int::BoolView b,
      ReifyMode mode0)
      : Base(home, x0, x1, b),
        mode(mode0) {}

  ReNotEqual(
      Space& home,
      ReNotEqual& other)
      : Base(home, other),
        mode(other.mode) {}

public:
  static ExecStatus post(
      Home home,
      ListView x0,
      ListView x1,
      Int::BoolView b,
      ReifyMode mode) {
    if (x0 == x1) {
      switch (mode) {
        case RM_EQV:
        case RM_IMP:
          return me_failed(
              b.zero(home))
              ? ES_FAILED
              : ES_OK;

        case RM_PMI:
          return ES_OK;
      }
    }

    (void) new (home)
        ReNotEqual(
            home,
            x0,
            x1,
            b,
            mode);

    return ES_OK;
  }

  Actor* copy(
      Space& home) override {
    return new (home)
        ReNotEqual(
            home,
            *this);
  }

  ExecStatus propagate(
      Space& home,
      const ModEventDelta&) override {
    return Adapter::reified_not_equal(
        home,
        *this,
        x0,
        x1,
        b,
        mode);
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


  Int::BoolView b(reification.var());
  switch (relation) {
  case IRT_EQ:
    GECODE_ES_FAIL(Dashed::ReEqual::post(
        home,
        Dashed::ListView(x),
        Dashed::ListView(y),
        b,
        reification.mode()));
    break;
  case IRT_NQ:
    GECODE_ES_FAIL(Dashed::ReNotEqual::post(
        home,
        Dashed::ListView(x),
        Dashed::ListView(y),
        b,
        reification.mode()));
    break;
  default:
    throw Int::UnknownRelation("Dashed::rel");
  }
}

} // namespace Gecode

#endif
