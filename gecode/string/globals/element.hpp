#include <gecode/string/globals.hh>

namespace Gecode { namespace String { namespace Globals {

  template<class View>
  forceinline
  Element<View>::Element(Home home, ViewArray<View> v, Gecode::Int::IntView i)
  : MixNaryOnePropagator <View, PC_STRING_CARD, 
    Gecode::Int::IntView, Gecode::Int::PC_INT_BND> (home, v, i) {}

  template<class View>
  forceinline ExecStatus
  Element<View>::post(Home home, ViewArray<View> x, Gecode::Int::IntView n, 
                                                                    View y) {
    //TODO
    return ES_OK;
  }

  template<class View>
  forceinline
  Element<View>::Element(Space& home, Element& p)
  : MixNaryOnePropagator <View, PC_STRING_CARD, 
    Gecode::Int::IntView, Gecode::Int::PC_INT_BND> (home, p) {}

  template<class View>
  forceinline Actor*
  Element<View>::copy(Space& home) {
    return new (home) Element(home,*this);
  }

  template<class View>
  forceinline ExecStatus
  Element<View>::propagate(Space& home, const ModEventDelta&) {
//    std::cerr << TODO

//    std::cerr << TODO
    return ES_FIX;
  }

}}}
