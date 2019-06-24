#include <gecode/int/bool.hh>

namespace Gecode { namespace String {

  template<class CtrlView, ReifyMode rm>
  forceinline
  ReLex<CtrlView, rm>::ReLex(
    Home home, StringView y0, StringView y1, CtrlView y2, bool strict
  ) : Propagator(home), lt(strict), x0(y0), x1(y1), b(y2)  {
    b .subscribe (home, *this, Gecode::Int::PC_INT_VAL);
    x0.subscribe (home, *this, PC_STRING_DOM);
    x1.subscribe (home, *this, PC_STRING_DOM);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline
  ReLex<CtrlView, rm>::ReLex(Space& home, bool share, ReLex& p)
  : Propagator(home, share, p), lt(p.lt) {
    x0.update (home, share, p.x0);
    x1.update (home, share, p.x1);
    b .update (home, share, p.b);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline PropCost
  ReLex<CtrlView, rm>::cost(const Space&, const ModEventDelta&)
  const {
    return PropCost::ternary(PropCost::LO);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline void
  ReLex<CtrlView, rm>::reschedule(Space& home) {
    b .reschedule(home, *this, Gecode::Int::PC_INT_VAL);
    x0.reschedule(home, *this, PC_STRING_DOM);
    x1.reschedule(home, *this, PC_STRING_DOM);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline size_t
  ReLex<CtrlView, rm>::dispose(Space& home) {
    b .cancel(home, *this, Gecode::Int::PC_INT_VAL);
    x0.cancel(home, *this, PC_STRING_DOM);
    x1.cancel(home, *this, PC_STRING_DOM);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline ExecStatus
  ReLex<CtrlView, rm>::post(
    Home home, StringView x0, StringView x1, CtrlView b, bool s
  ) {
    (void) new (home) ReLex<CtrlView, rm>(home, x0, x1, b, s);
    return ES_OK;
  }

  template<class CtrlView, ReifyMode rm>
  forceinline Actor*
  ReLex<CtrlView, rm>::copy(Space& home, bool share) {
    return new (home) ReLex<CtrlView, rm>(home, share, *this);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline ExecStatus
  ReLex<CtrlView, rm>::propagate(
    Space& home, const ModEventDelta&
  ) {
    // std::cerr<<"ReLex::propagate "<<b<<" <> "<<x0<<" lex "<<x1<<std::endl;
    if (b.zero()) {
      if (rm == RM_IMP)
        return home.ES_SUBSUMED(*this);
      GECODE_REWRITE(*this, (Lex::post(home(*this), x1, x0, !lt)));
    }
    if (b.one()) {
      if (rm == RM_PMI)
        return home.ES_SUBSUMED(*this);
      GECODE_REWRITE(*this, (Lex::post(home(*this), x0, x1, lt)));
    }
    if (!x0.pdomain()->check_lex(*x1.pdomain(), lt)) {
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
    // std::cerr<<"propagated: "<<b<<" lex "<<x0<<" = "<<x1<<std::endl;
    return ES_FIX;
  }

}}
