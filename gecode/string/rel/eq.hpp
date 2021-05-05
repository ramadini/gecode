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
    int l0 = x0.min_length(), l1 = x1.min_length(), 
        u0 = x0.min_length(), u1 = x1.min_length();
    if (l0 > u1 || l1 > u0)
      return ES_FAILED;
    int l = std::max(l0, l1), u = std::min(u0, u1);
    ModEvent me = ME_STRING_NONE;
    if (l0 > l1)
      me = x1.min_length(home, l0);
    else if (l1 > l0)
      me = x0.min_length(home, l1);
    GECODE_ME_CHECK(me);
    if (u0 < u1)
      me = StringVarImp::me_combine(me, x1.max_length(home, u0));
    else if (u1 < u0)
      me = StringVarImp::me_combine(me, x0.max_length(home, u1));
    GECODE_ME_CHECK(me);
    if (x0.assigned()) {
      if (x1.assigned())
        return x0.val() == x1.val() ? home.ES_SUBSUMED(*this) : ES_FAILED;
    }
    if (x1.assigned()) {
    
    }
    
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
    return ES_OK;
  }

}}}

// STATISTICS: string-prop
