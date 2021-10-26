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
    int l = std::max(0, x2.min_length()-x1.max_length());
    int u = x2.max_length() - x1.min_length();
    if (u < 0)
      return ES_FAILED;
    if (l > x0.min_length()) GECODE_ME_CHECK(x0.min_length(home, l));
    if (u < x0.max_length()) GECODE_ME_CHECK(x0.max_length(home, u));
    l = std::max(0, x2.min_length() - x0.max_length());
    u = x2.max_length() - x0.min_length();
    if (l > x1.min_length()) GECODE_ME_CHECK(x1.min_length(home, l));
    if (u < x1.max_length()) GECODE_ME_CHECK(x1.max_length(home, u));
    long n = x0.min_length() + x1.min_length();
    if (n > MAX_STRING_LENGTH)
      return ES_FAILED;
    l = (int) n;
    u = ubounded_sum(x0.max_length(), x1.max_length());
    if (l > x2.min_length()) GECODE_ME_CHECK(x2.min_length(home, l));
    if (u < x2.max_length()) GECODE_ME_CHECK(x2.max_length(home, u));
//    std::cerr << "After refine_card: " <<x2<<" = "<<x0<< " ++ " <<x1<<"\n";
    return ES_FIX;
  }
  
  template<class View0, class View1, class View2>
  forceinline ExecStatus
  Concat<View0,View1,View2>::propagate(Space& home, const ModEventDelta&) {
//    std::cerr << "\n" << this << "::Concat::propagate " <<x2<<" = "<<x0<< " ++ " <<x1<<"\n";
    GECODE_ME_CHECK(refine_card(home));
    int a;
    do {
      if (x0.isNull()) {
        GECODE_REWRITE(*this, 
          (Gecode::String::Rel::Eq<View1,View2>::post(home(*this), x1, x2)));
        return home.ES_SUBSUMED(*this);
      }
      if (x1.isNull()) {
        GECODE_REWRITE(*this, 
          (Gecode::String::Rel::Eq<View0,View2>::post(home(*this), x0, x2)));
        return home.ES_SUBSUMED(*this);
      }
      if (x2.isNull()) {
        GECODE_ME_CHECK(x0.nullify(home));
        GECODE_ME_CHECK(x1.nullify(home));
        return home.ES_SUBSUMED(*this);
      }
      ConcatView xy(x0,x1);
      if (xy.assigned()) {
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
      ModEvent me0 = ME_STRING_NONE;
      if (!x2.assigned()) {
        me0 = x2.equate(home, xy);
        GECODE_ME_CHECK(me0);
//        std::cerr << "me0: " << me0 << ", x2: " << x2 << '\n';
      }
      ModEvent me1 = xy.equate(home, x2);
//      std::cerr << "me1: " << me1 << ", xy: " << xy << '\n';
      GECODE_ME_CHECK(me1);
      if (me0 == ME_STRING_VAL || me0 == ME_STRING_CARD
      ||  me1 == ME_STRING_VAL || me1 == ME_STRING_CARD)
        GECODE_ME_CHECK(refine_card(home));
      a = x0.assigned() + x1.assigned() + x2.assigned();
    } while (a == 2);
//    std::cerr << "After Concat::propagate " <<x2<<" = "<<x0<< " ++ " <<x1<<"\n";
    return a == 3 ? home.ES_SUBSUMED(*this) : ES_FIX;
  }
  
  
}}}
