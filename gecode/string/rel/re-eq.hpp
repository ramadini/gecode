#include <gecode/int/bool.hh>

namespace Gecode { namespace String {

  template<class CtrlView, ReifyMode rm>
  forceinline
  ReEq<CtrlView, rm>::ReEq(Home home, StringView y0, StringView y1, CtrlView y2) 
  : Propagator(home), x0(y0), x1(y1), b(y2) {
    b .subscribe (home, *this, Gecode::Int::PC_INT_VAL);
    x0.subscribe (home, *this, PC_STRING_DOM);
    x1.subscribe (home, *this, PC_STRING_DOM);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline
  ReEq<CtrlView, rm>::ReEq(Space& home, ReEq& p)
  : Propagator(home, p) {
    x0.update (home, p.x0);
    x1.update (home, p.x1);
    b .update (home, p.b);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline PropCost
  ReEq<CtrlView, rm>::cost(const Space&, const ModEventDelta&) const {
    return PropCost::ternary(PropCost::LO);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline void
  ReEq<CtrlView, rm>::reschedule(Space& home) {
    b .reschedule(home, *this, Gecode::Int::PC_INT_VAL);
    x0.reschedule(home, *this, PC_STRING_DOM);
    x1.reschedule(home, *this, PC_STRING_DOM);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline size_t
  ReEq<CtrlView, rm>::dispose(Space& home) {
    b .cancel(home, *this, Gecode::Int::PC_INT_VAL);
    x0.cancel(home, *this, PC_STRING_DOM);
    x1.cancel(home, *this, PC_STRING_DOM);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline ExecStatus
  ReEq<CtrlView,rm>::post(Home home, StringView x0, StringView x1, CtrlView b) {
    (void) new (home) ReEq<CtrlView, rm>(home, x0, x1, b);
    return ES_OK;
  }

  template<class CtrlView, ReifyMode rm>
  forceinline Actor*
  ReEq<CtrlView, rm>::copy(Space& home) {
    return new (home) ReEq<CtrlView, rm>(home, *this);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline ExecStatus
  ReEq<CtrlView, rm>::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"ReEq::propagate "<<b<<" <> "<<x0<<" = "<<x1<<std::endl;
    if (b.zero()) {
      if (rm == RM_IMP)
        return home.ES_SUBSUMED(*this);
      GECODE_REWRITE(*this, (Nq::post(home(*this), x0, x1)));
    }
    if (b.one()) {
      if (rm == RM_PMI)
        return home.ES_SUBSUMED(*this);
      GECODE_REWRITE(*this, (Eq::post(home(*this), x0, x1)));
    }
    if (!x0.check_equate(x1)) {
      if (rm != RM_PMI)
        GECODE_ME_CHECK(b.zero_none(home));
      return home.ES_SUBSUMED(*this);
    }
    else
      if (x0.assigned() && x1.assigned()) {
        if (rm != RM_PMI)
          GECODE_ME_CHECK(b.one_none(home));
        return home.ES_SUBSUMED(*this);
      }
    // std::cerr<<"propagated: "<<b<<" <> "<<x0<<" = "<<x1<<std::endl;
    return ES_FIX;
  }

}}
