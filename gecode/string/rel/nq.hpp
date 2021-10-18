#include <gecode/string/int.hh>

namespace Gecode { namespace String { namespace Rel {

  template<class View0, class View1>
  forceinline
  Nq<View0,View1>::Nq(Home home, View0 x, View1 y)
    : MixBinaryPropagator<View0,PC_STRING_VAL,View1,PC_STRING_VAL>(home,x,y) {}

  template<class View0, class View1>
  forceinline
  Nq<View0,View1>::Nq(Space& home, Nq& p)
    : MixBinaryPropagator<View0,PC_STRING_VAL,View1,PC_STRING_VAL>(home,p) {}

  template<class View0, class View1>
  forceinline ExecStatus
  Nq<View0,View1>::post(Home home, View0 x, View1 y) {
    if (!x.same(y))
      (void) new (home) Nq(home,x,y);
    return ES_OK;
  }

  template<class View0, class View1>
  forceinline Actor*
  Nq<View0,View1>::copy(Space& home) {
    return new (home) Nq(home,*this);
  }

  template<class View0, class View1>
  forceinline ExecStatus
  Nq<View0,View1>::propagate(Space& home, const ModEventDelta&) {
//    std::cerr << "Prop. Nq " << x0 << "  vs  " << x1 << "\n";
    if (x0.min_length() > x1.max_length() || x0.max_length() < x1.min_length())
      return home.ES_SUBSUMED(*this);
    if (x1.assigned()) {
      if (x0.assigned())
        return check_equate_x(x0, x1) ? ES_FAILED : home.ES_SUBSUMED(*this);
      if (x1.val().empty()) {
        IntVar n(home, 1, std::min(Gecode::Int::Limits::max, x0.max_length()));
        Gecode::String::Int::Length<View0>::post(home, x0, n);
        return home.ES_SUBSUMED(*this);
      }
    }
    if (x0.assigned() && x0.val().empty()) {
      IntVar n(home, 1, x1.max_length());
      Gecode::String::Int::Length<View1>::post(home, x1, n);
      return home.ES_SUBSUMED(*this);
    }
    if (!check_equate_x(x0,x1) || !check_equate_x(x1,x0))
      return home.ES_SUBSUMED(*this);
//     std::cerr << "Nq::propagated.\n";
    return ES_FIX;
  }

}}}

// STATISTICS: string-prop
