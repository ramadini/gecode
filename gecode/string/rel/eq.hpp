namespace Gecode { namespace String { namespace Rel {

  template<class View0, class View1>
  forceinline
  Eq<View0,View1>::Eq(Home home, View0 x, View1 y)
    : MixBinaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY>(home,x,y) {}

  template<class View0, class View1>
  forceinline
  Eq<View0,View1>::Eq(Space& home, Eq& p)
    : MixBinaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY>(home,p) {}

  template<class View0, class View1>
  ExecStatus
  Eq<View0,View1>::post(Home home, View0 x, View1 y) {
    if (!x.same(y))
      (void) new (home) Eq(home,x,y);
    return ES_OK;
  }

  template<class View0, class View1>
  Actor*
  Eq<View0,View1>::copy(Space& home) {
    return new (home) Eq(home,*this);
  }

  template<class View0, class View1>
  ExecStatus
  Eq<View0,View1>::propagate(Space& home, const ModEventDelta& med) {
    GECODE_ME_CHECK(equate_x(home, x0, x1));
    GECODE_ME_CHECK(equate_x(home, x1, x0));
//    if (!refine_card_eq(h, that))
//      return false;
//    if (known()) {
//      string s = val();
//      if (that.known())
//        return s == that.val();
//      else {
//        if (check_equate_x(s, x1)) {
//          that.update(h, s);
//          return true;
//        }
//        else
//          return false;
//      }
//    }
//    if (that.known()) {
//      string s = that.val();
//      if (check_sweep<DSBlock, DSBlocks, char, string>(_blocks, s)) {
//        update(h, s);
//        return true;
//      }
//      else
//        return false;
//    }
//    if (contains(that)) {
//      if (*this == that)
//        return true;
//      update(h, that);
//      return true;
//    }
//    if (that.contains(*this)) {
//      that.update(h, *this);
//      return true;
//    }
//    // std::cerr<<"refined: "<<*this<<' '<<that<<std::endl;
//    if (!sweep_equate(h, *this, that))
//      return false;
//    if ((_changed || that._changed) && !refine_card_eq(h, that))
//      return false;
//    // std::cerr<<"DashedString::equated "<<*this<<' '<<that<<std::endl;
//    return true;
  }

}}}

// STATISTICS: string-prop
