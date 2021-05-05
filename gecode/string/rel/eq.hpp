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
    // TODO: This must be a method x.same_length(home, y) delegating corresponding varimp method:
    int l0 = x0.min_length(), l1 = x1.min_length(), 
        u0 = x0.min_length(), u1 = x1.min_length();
    if (l0 > u1 || l1 > u0)
      return ES_FAILED;
    int l = std::max(l0, l1), u = std::min(u0, u1);
    ModEvent me = ME_STRING_NONE;
    if (l0 > l1) {
      me = x1.min_length(home, l0);
      l1 = l0;
      GECODE_ME_CHECK(me);
    }
    else if (l1 > l0) {
      me = x0.min_length(home, l1);
      l0 = l1;
      GECODE_ME_CHECK(me);
    }
    if (u0 < u1) {
      me = StringVarImp::me_combine(me, x1.max_length(home, u0));
      u1 = u0;
      GECODE_ME_CHECK(me);
    }
    else if (u1 < u0) {
      me = StringVarImp::me_combine(me, x0.max_length(home, u1));
      u0 = u1;
      GECODE_ME_CHECK(me);  
    }
    GECODE_ME_CHECK(me);
    // TODO: GECODE_ME_CHECK(x.same_length(home, y));
    bool eq = false;
    if (0) { // TODO: x0.contains(x1, eq)) {
      if (eq) // eq set to true if x0 == x1
        return ES_OK;
      //FIXME: contains must also check when *this = S^l,u and l <= y.min_length and u >= y.max_length
      x0.update(home, x1);
      StringDelta d;
      if (x0.assigned())
        x0.varimp()->notify(home, ME_STRING_VAL, d);
      else if (x0.min_length() > l0 || x0.max_length() < u0)
        x0.varimp()->notify(home, ME_STRING_CARD, d);
      else
        x0.varimp()->notify(home, ME_STRING_BASE, d);
      return ES_OK;
    }
    if (1) { // TODO: x1.contains(x0, eq)) {
      x1.update(home, x0);
      StringDelta d;
      if (x1.assigned())
        x1.varimp()->notify(home, ME_STRING_VAL, d);
      else if (x1.min_length() > l1 || x1.max_length() < u1)
        x1.varimp()->notify(home, ME_STRING_CARD, d);
      else
        x1.varimp()->notify(home, ME_STRING_BASE, d);
      return ES_OK;
    }
    do {
      me = StringVarImp::me_combine(me, equate_x(home, x0, x1));
      GECODE_ME_CHECK(me);
      me = StringVarImp::me_combine(me, equate_x(home, x1, x0));
      GECODE_ME_CHECK(me);
//TODO:      me = me_combine(me, x.same_length(home, y));
    } while (x0.assigned() + x1.assigned() == 1);
    return x0.assigned() ? home.ES_SUBSUMED(*this) : ES_OK;
  }

}}}

// STATISTICS: string-prop
