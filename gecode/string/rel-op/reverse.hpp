#include <gecode/string/rel.hh>

namespace Gecode { namespace String { namespace RelOp {

  template<class View0, class View1>
  forceinline ExecStatus
  Reverse<View0,View1>::post(Home home, View0 x0, View1 x1) {
    if (x0.same(x1)) {
      StringVar y(home, x1);
      (void) new (home) Gecode::String::Rel::Eq<View0,ReverseView>
                                               (home, x0, ReverseView(y));
      rel(home, x1, STRT_EQ, y);
    }
    else
      (void) new (home) Reverse(home, x0, ReverseView(x1));
    return ES_OK;
  }
  
  template<class View0, class View1>
  forceinline Actor*
  Reverse<View0,View1>::copy(Space& home) {
    return new (home) Reverse(home, *this);
  }

  template<class View0, class View1> 
  forceinline Reverse<View0,View1>::Reverse(Home home, View0 y0, View1 y1)
  : MixBinaryPropagator<View0,PC_STRING_ANY,
                        View1,PC_STRING_ANY>(home, y0, y1) {}

  template<class View0, class View1>
  forceinline Reverse<View0,View1>::Reverse(Space& home, Reverse& p)
  : MixBinaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY>(home, p) {}

  template<class View0, class View1>
  ExecStatus
  Reverse<View0,View1>::refine_card(Space& home) {
    int l = std::max(x0.min_length(), x1.min_length()),
        u = std::min(x0.max_length(), x1.max_length());
    GECODE_ME_CHECK(x0.bnd_length(home, l, u));
    GECODE_ME_CHECK(x1.bnd_length(home, l, u));
    return ES_OK;
  }

  template<class View0, class View1>
  forceinline ExecStatus
  Reverse<View0,View1>::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"\n"<<this<<"::Rev::propagate" << x0 << "  vs  " << x1 << "\n";
    refine_card(home);
    if (x0.assigned()) {
      if (x0.isNull()) {
        GECODE_ME_CHECK(x1.nullify(home));
        return home.ES_SUBSUMED(*this);
      }
      if (check_equate_x(x1,ReverseView(x0))) {
        if (!x1.assigned())
          x1.gets_rev(home, x0);
        return home.ES_SUBSUMED(*this);
      }
      else
        return ES_FAILED;
    }
    if (x1.assigned()) {
      if (x1.isNull()) {
        GECODE_ME_CHECK(x0.nullify(home));
        return home.ES_SUBSUMED(*this);
      }
      if (check_equate_x(x0, ReverseView(x1))) {
        x0.gets_rev(home, x1);
        return home.ES_SUBSUMED(*this);
      }
      else
        return ES_FAILED;
    }
    if (x0.contains_rev(x1)) {
      if (x0.equals_rev(x1))
        return ES_FIX;
      x0.gets_rev(home, x1);
      return ES_FIX;
    }
    if (x1.contains_rev(x0)) {
      x1.gets_rev(home, x0);
      return ES_FIX;
    }
    ModEvent me0;
    do {
      me0 = x0.equate(home, ReverseView(x1));
      GECODE_ME_CHECK(me0);
      ModEvent me1 = x1.equate(home, ReverseView(x0));
      GECODE_ME_CHECK(me1);
      if (me0 + me1 != ME_STRING_NONE)
        refine_card(home);
    } while (x0.assigned() + x1.assigned() == 1);
//    std::cerr << "Rev::propagated.\n";
    return x0.assigned() ? home.ES_SUBSUMED(*this) : ES_FIX;
  }
  
  
}}}
