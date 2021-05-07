#include <gecode/int/bool.hh>

namespace Gecode { namespace String { namespace Rel {

  template<class View0, class View1, class CtrlView, ReifyMode rm> 
  forceinline ReEq<View0,View1,CtrlView,rm>
  ::ReEq(Home home, View0 xx0, View1 xx1, CtrlView bb) 
  : Propagator(home), x0(xx0), x1(xx1), b(bb) {
    b .subscribe (home, *this, Gecode::Int::PC_INT_VAL);
    x0.subscribe (home, *this, PC_STRING_ANY);
    x1.subscribe (home, *this, PC_STRING_ANY);
  }

  template<class View0, class View1, class CtrlView, ReifyMode rm> 
  forceinline ReEq<View0,View1,CtrlView,rm> 
  ::ReEq(Space& home, ReEq& p) : Propagator(home, p) {
    x0.update (home, p.x0);
    x1.update (home, p.x1);
    b .update (home, p.b);
  }

  template<class View0, class View1, class CtrlView, ReifyMode rm> 
  forceinline PropCost
  ReEq<View0,View1,CtrlView,rm> ::cost(const Space&, const ModEventDelta&) const {
    return PropCost::ternary(PropCost::LO);
  }

  template<class View0, class View1, class CtrlView, ReifyMode rm> 
  forceinline void
  ReEq<View0,View1,CtrlView,rm> ::reschedule(Space& home) {
    b .reschedule(home, *this, Gecode::Int::PC_INT_VAL);
    x0.reschedule(home, *this, PC_STRING_ANY);
    x1.reschedule(home, *this, PC_STRING_ANY);
  }

  template<class View0, class View1, class CtrlView, ReifyMode rm> 
  forceinline size_t
  ReEq<View0,View1,CtrlView,rm> ::dispose(Space& home) {
    b .cancel(home, *this, Gecode::Int::PC_INT_VAL);
    x0.cancel(home, *this, PC_STRING_ANY);
    x1.cancel(home, *this, PC_STRING_ANY);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  template<class View0, class View1, class CtrlView, ReifyMode rm> 
  forceinline ExecStatus
  ReEq<View0,View1,CtrlView,rm>::post(Home home, View0 x0, View1 x1, CtrlView b) {
    (void) new (home) ReEq<View0,View1,CtrlView,rm> (home, x0, x1, b);
    return ES_OK;
  }

  template<class View0, class View1, class CtrlView, ReifyMode rm> 
  forceinline Actor*
  ReEq<View0,View1,CtrlView,rm> ::copy(Space& home) {
    return new (home) ReEq<View0,View1,CtrlView,rm> (home, *this);
  }

  template<class View0, class View1, class CtrlView, ReifyMode rm> 
  forceinline ExecStatus
  ReEq<View0,View1,CtrlView,rm> ::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"ReEq::propagate "<<b<<" <> "<<x0<<" = "<<x1<<std::endl;
    if (b.zero()) {
      if (rm == RM_IMP)
        return home.ES_SUBSUMED(*this);
      GECODE_REWRITE(*this, (Nq<View0,View1>::post(home(*this), x0, x1)));
    }
    if (b.one()) {
      if (rm == RM_PMI)
        return home.ES_SUBSUMED(*this);
      GECODE_REWRITE(*this, (Eq<View0,View1>::post(home(*this), x0, x1)));
    }
    if (!check_equate_x(x0,x1) || !check_equate_x(x1,x0)) {
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

}}}
