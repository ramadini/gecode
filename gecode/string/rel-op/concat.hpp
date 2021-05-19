#include <gecode/string/rel.hh>

namespace Gecode { namespace String { namespace RelOp {

  template<class View0, class View1, class View2>
  forceinline ExecStatus
  Concat<View0,View1,View2>::post(Home home, View0 x0, View1 x1, View2 x2) {
    if (x0.same(x2))
      eq(home, x1, std::vector<int>());
    else if (x1.same(x2))
      eq(home, x0, std::vector<int>());
    // FIXME: if x0.same(x1) then x2 = x0^2?
    else
      (void) new (home) Concat(home, x0, x1, x2);
    return ES_OK;
  }
  
  template<class View0, class View1, class View2>
  forceinline Actor*
  Concat<View0,View1,View2>::copy(Space& home) {
    return new (home) Concat(home, *this);
  }

  template<class View0, class View1, class View2> 
  forceinline Concat<View0,View1,View2>::Concat(Home home, View0 y0, View1 y1, 
                                                                     View2 y2)
  : MixTernaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY,
                         View2,PC_STRING_ANY>(home, y0, y1, y2) {}

  template<class View0, class View1, class View2>
  forceinline Concat<View0,View1,View2>::Concat(Space& home, Concat& p)
  : MixTernaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY, 
                                             View2,PC_STRING_ANY>(home, p) {}

  template<class View0, class View1, class View2>
  forceinline ExecStatus
  Concat<View0,View1,View2>::propagate(Space& home, const ModEventDelta& m) {
    //TODO:
    return ES_FIX;
  }
  
}}}
