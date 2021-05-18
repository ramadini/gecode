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
    GECODE_ME_CHECK(x1.lq(home, String::Limits::MAX_STRING_LENGTH));
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
    int x1min, x1max;
    ModEvent me0 = ME_STRING_NONE;
    ModEvent me1 = Gecode::Int::ME_INT_NONE;
    do {
      x1min = x1.min();
      x1max = x1.max();
      GECODE_ME_CHECK(combine<StringVarImp>(
        x0.bnd_length(home, x1min, x1max), me0
      ));
      GECODE_ME_CHECK(combine<Gecode::Int::IntVarImp>(
        x1.gq(home, x0.min_length()), me1
      ));
      GECODE_ME_CHECK(combine<Gecode::Int::IntVarImp>(
        x1.lq(home, x0.max_length()), me1
      ));
    } while (x1.min() > x1min || x1.max() < x1max);
    if (me0 != ME_STRING_NONE) {
      StringDelta d;
      x0.varimp()->notify(home, me0, d);
      if (x1.assigned())
        return home.ES_SUBSUMED(*this);
    }
    if (me1 != Gecode::Int::ME_INT_NONE) {
      Gecode::Int::IntDelta d; //FIXME
      x1.varimp()->notify(home, me1, d);
    }
    return ES_FIX;
  }

}}}
