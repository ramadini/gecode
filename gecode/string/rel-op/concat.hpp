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
//    std::cerr << "Before refine_card: " <<x2<<" = "<<x0<< " ++ " <<x1<<"\n";
    int l = std::max(x0.min_length(), x2.min_length() - x1.max_length()),
        u = std::min(x0.max_length(), x2.max_length() - x1.min_length());
//    std::cerr << "x0) " << l << " " << u << "\n";
    GECODE_ME_CHECK(x0.bnd_length(home, l, u));
    l = std::max(x1.min_length(), x2.min_length() - x0.max_length()),
    u = std::min(x1.max_length(), x2.max_length() - x0.min_length());
//    std::cerr << "x1) " << l << " " << u << "\n";
    GECODE_ME_CHECK(x1.bnd_length(home, l, u));
    l = std::max(x2.min_length(), x0.min_length() + x1.min_length()),
    u = std::min(x2.max_length(), x0.max_length() + x1.max_length());
//    std::cerr << "x2) " << l << " " << u << "\n";
    GECODE_ME_CHECK(x2.bnd_length(home, l, u));
//    std::cerr << "After refine_card: " <<x2<<" = "<<x0<< " ++ " <<x1<<"\n";
    return ES_FIX;
  }

  template<class View0, class View1, class View2>
  forceinline ExecStatus
  Concat<View0,View1,View2>::propagate(Space& home, const ModEventDelta&) {
//    std::cerr << "\n" << this << "::Concat::propagate " <<x2<<" = "<<x0<< " ++ " <<x1<<"\n";   
    if (x0.isNull()) {
      GECODE_REWRITE(*this, 
        (Gecode::String::Rel::Eq<View1,View2>::post(home(*this), x1, x2)));
      return home.ES_SUBSUMED(*this);
    }
    if (x1.isNull()) {
      GECODE_REWRITE(*this, 
        (Gecode::String::Rel::Eq<View1,View2>::post(home(*this), x0, x2)));
      return home.ES_SUBSUMED(*this);
    }
    if (x2.isNull()) {
      GECODE_ME_CHECK(x0.nullify(home));
      GECODE_ME_CHECK(x1.nullify(home));
      return home.ES_SUBSUMED(*this);
    }
    GECODE_ME_CHECK(refine_card(home));
    if (x0.assigned() && x1.assigned()) {
      ConcatView xy(x0,x1);
      if (check_equate_x(x2, xy)) {
        if (!x2.assigned()) {
          if (check_equate_x(xy, x2))
            x2.gets(home, xy.val());
          else
            return ES_FAILED;
        }
        return home.ES_SUBSUMED(*this);
      }
      else
        return ES_FAILED;
    }
    int a;
    do {
      ConcatView xy(x0,x1);
      ModEvent me0 = ME_STRING_NONE;
      if (!x2.assigned()) {
        me0 = x2.equate(home, xy);
        GECODE_ME_CHECK(me0);
      }
      ModEvent me1 = xy.equate(home, x2);
      GECODE_ME_CHECK(me1);
      if (me0 == ME_STRING_VAL || me0 == ME_STRING_CARD
      ||  me1 == ME_STRING_VAL || me1 == ME_STRING_CARD)
        GECODE_ME_CHECK(refine_card(home));
//      std::cerr << me0 << ' ' << me1 << "\n";
      a = x0.assigned() + x1.assigned() + x2.assigned();
    } while (a == 2);
//    std::cerr << "After Concat::propagate " <<x2<<" = "<<x0<< " ++ " <<x1<<"\n";
    return a == 3 ? home.ES_SUBSUMED(*this) : ES_FIX;
  }
  
  
}}}
