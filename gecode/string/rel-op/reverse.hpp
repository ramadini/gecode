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
  forceinline ExecStatus
  Reverse<View0,View1>::propagate(Space& home, const ModEventDelta&) {
    GECODE_NEVER;
  }
  
  
}}}
