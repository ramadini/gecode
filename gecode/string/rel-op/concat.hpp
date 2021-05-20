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
  Concat<View0,View1,View2>::propagate(Space& home, const ModEventDelta&) {
    if (x2.isNull()) {
      GECODE_ME_CHECK(x0.nullify(home));
      GECODE_ME_CHECK(x1.nullify(home));
      return home.ES_SUBSUMED(*this);
    }
    if (x0.isNull()) {
      eq(home, x1, x2);
      return home.ES_SUBSUMED(*this);
    }
    if (x1.isNull()) {
      eq(home, x0, x2);
      return home.ES_SUBSUMED(*this);
    }
    if (x0.assigned() && x1.assigned()) {
      ConcatView<View0,View1> xy(x0,x1);
      if (check_equate_x(x2, xy) && check_equate_x(xy, x2) && !x2.assigned())
        GECODE_ME_CHECK(x2.update(home, xy.val()));
      else
        return ES_FAILED;
      return home.ES_SUBSUMED(*this);
    }
// TODO:
//    if (x._min_length > 0 && y._min_length > 0 && contains(x, y)) {
//      // std::cerr << *this << " contains " << x << "  ++  " << y << "\n";
//      int norm = x.blocks().back().S == y.at(0).S ? 1 : 0;
//      double d = logdim();
//      NSBlocks xy(x.length() + y.length() - norm);
//      for (int i = 0; i < x.length(); ++i)
//        xy[i] = NSBlock(x.at(i));
//      if (norm) {
//        xy[x.length() - 1].l += y.at(0).l;
//        xy[x.length() - 1].u += y.at(0).u;
//      }
//      for (int i = norm; i < y.length(); ++i)
//        xy[i + x.length() - norm] = NSBlock(y.at(i));
//      if (d != xy.logdim()) {
//        update(h, xy);
//        _changed = true;
//      }
//    }
//    else {
//      ConcatView xy(x, y);
//      if (!sweep_concat(h, xy, x, y, *this))
//        return false;
//    }
//    if (!refine_card_cat(h, x, y))
//      return false;
    return ES_FIX;
  }
  
}}}
