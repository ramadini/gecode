#include <gecode/int.hh>

namespace Gecode { namespace String { namespace Int {

  // x[2] is the index of the first occurrence of x[1] in x[0].

  template <class View0, class View1>
  forceinline
  Find<View0,View1>::Find(Home home, View0 x, View1 y, Gecode::Int::IntView n)
  : MixTernaryPropagator<View0, PC_STRING_CARD, View1, PC_STRING_CARD, 
    Gecode::Int::IntView, Gecode::Int::PC_INT_BND>(home, x, y, n) {}

  template <class View0, class View1>
  forceinline ExecStatus
  Find<View0,View1>::post(Home home, View0 x, View1 y, Gecode::Int::IntView n) {
    if (x.same(y))
      rel(home, n, IRT_EQ, 1);
    else
      (void) new (home) Find(home, x, y, n);
    return ES_OK;
  }

  template <class View0, class View1>
  forceinline
  Find<View0,View1>::Find(Space& home, Find& p)
  : MixTernaryPropagator<View0, PC_STRING_CARD, 
    View1, PC_STRING_CARD, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> 
    (home, p) {}

  template <class View0, class View1>
  forceinline Actor*
  Find<View0,View1>::copy(Space& home) {
    return new (home) Find(home, *this);
  }

  template <class View0, class View1>
  forceinline ExecStatus
  Find<View0,View1>::propagate(Space& home, const ModEventDelta&) {
    //TODO:
    return ES_FIX;
  }

}}}
