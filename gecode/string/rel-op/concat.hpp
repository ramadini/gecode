#include <gecode/string/rel.hh>

namespace Gecode { namespace String { namespace RelOp {

  template<class View0, class View1, class View2>
  forceinline ExecStatus
  Concat<View0,View1,View2>::post(Home home, View0 x0, View1 x1, View2 x2) {
    if (x0.same(x2))
      GECODE_ME_CHECK(x1.nullify(home));
    else if (x1.same(x2))
      GECODE_ME_CHECK(x0.nullify(home));
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
  Concat<View0,View1,View2>::refine_card(Space& home) {
    ModEvent me0;
    do {
      int l = std::max(x0.min_length(), x2.min_length() - x1.max_length()),
          u = std::min(x0.max_length(), x2.max_length() - x1.min_length());
      me0 = x0.bnd_length(home, l, u);
      GECODE_ME_CHECK(me0);
      l = std::max(x1.min_length(), x2.min_length() - x0.max_length()),
      u = std::min(x1.max_length(), x2.max_length() - x0.min_length());
      ModEvent me1 = x1.bnd_length(home, l, u);
      GECODE_ME_CHECK(me1);
      me0 += me1;
      l = std::max(x2.min_length(), x0.min_length() + x1.min_length()),
      u = std::min(x2.max_length(), x0.max_length() + x1.max_length());
      me1 = x2.bnd_length(home, l, u);
      GECODE_ME_CHECK(me1);
      me0 += me1;
    } while (me0 != ME_STRING_NONE);
    return ES_OK;
  }

  template<class View0, class View1, class View2>
  forceinline ExecStatus
  Concat<View0,View1,View2>::propagate(Space& home, const ModEventDelta&) {
//    std::cerr << "Concat::propagate " <<x0<< " ++ " <<x1<< " = " <<x2<<"\n";
    GECODE_ME_CHECK(refine_card(home));
    if (x2.isNull()) {
      GECODE_ME_CHECK(x0.nullify(home));
      GECODE_ME_CHECK(x1.nullify(home));
      return home.ES_SUBSUMED(*this);
    }
    if (x0.isNull()) {
      Gecode::String::Rel::Eq<View1,View2>::post(home, x1, x2);
      return home.ES_SUBSUMED(*this);
    }
    if (x1.isNull()) {
      Gecode::String::Rel::Eq<View0,View2>::post(home, x0, x2);
      return home.ES_SUBSUMED(*this);
    }
    if (x0.assigned() && x1.assigned()) {
      ConcatView<View0,View1> xy(x0,x1);
      if (check_equate_x(x2, xy) && check_equate_x(xy, x2) && !x2.assigned())
        x2.gets(home, xy.val());
      else
        return ES_FAILED;
      return home.ES_SUBSUMED(*this);
    }
    int a;
    do {
      ConcatView<View0,View1> xy(x0,x1);
      ModEvent me0 = x2.equate(home, xy);
      GECODE_ME_CHECK(me0);      
      ModEvent me1 = xy.equate(home, x2);
      GECODE_ME_CHECK(me1);
      if (me0 + me1 != ME_STRING_NONE)
        GECODE_ME_CHECK(refine_card(home));
      a = x0.assigned() + x1.assigned() + x2.assigned();
    } while (a == 2);
//    std::cerr << "After Concat::propagate " <<x0<< " ++ " <<x1<< " = " <<x2<<"\n";
    return a == 3 ? home.ES_SUBSUMED(*this) : ES_OK;
  }  
  
}}}
