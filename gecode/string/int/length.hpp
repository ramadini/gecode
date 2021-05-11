#include <gecode/string.hh>
#include <gecode/int.hh>

namespace Gecode { namespace String { namespace Int {

  template<class View>
  forceinline
  Length<View>::Length(Home home, View y0, Gecode::Int::IntView y1)
    : MixBinaryPropagator<View,PC_STRING_CARD,
      Gecode::Int::IntView,Gecode::Int::PC_INT_BND> (home, y0, y1) {}

  template<class View>
  forceinline ExecStatus
  Length<View>::post(Home home, View x0, Gecode::Int::IntView x1) {
    GECODE_ME_CHECK(x1.gq(home,0));
    GECODE_ME_CHECK(x0.cardMax(home, Gecode::Int::Limits::max));
    (void) new (home) Length(home,x0,x1);
    return ES_OK;
  }

  template<class View>
  forceinline
  Length<View>::Length(Space& home, Length& p)
    : MixBinaryPropagator<View,PC_STRING_CARD,
      Gecode::Int::IntView,Gecode::Int::PC_INT_BND> (home, p) {}

  template<class View>
  Actor*
  Length<View>::copy(Space& home) {
   return new (home) Length(home,*this);
  }

  template<class View>
  ExecStatus
  Length<View>::propagate(Space& home, const ModEventDelta&) {
//   int x1min, x1max;
//   do {
//     x1min = x1.min();
//     x1max = x1.max();
//     GECODE_ME_CHECK(x0.cardMin(home,static_cast<unsigned int>(x1min)));
//     GECODE_ME_CHECK(x0.cardMax(home,static_cast<unsigned int>(x1max)));
//     GECODE_ME_CHECK(x1.gq(home,static_cast<int>(x0.cardMin())));
//     GECODE_ME_CHECK(x1.lq(home,static_cast<int>(x0.cardMax())));
//   } while (x1.min() > x1min || x1.max() < x1max);
//   if (x1.assigned())
//     return home.ES_SUBSUMED(*this);
   return ES_FIX;
  }

}}}
