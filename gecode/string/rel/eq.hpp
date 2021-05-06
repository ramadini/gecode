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
  Eq<View0,View1>::propagate(Space& home, const ModEventDelta&) {
    int l0 = x0.min_length(), l1 = x1.min_length(), 
        u0 = x0.min_length(), u1 = x1.min_length(),
        l = std::max(l0, l1), u = std::min(u0, u1);
    ModEvent me = x0.bnd_length(home, l, u);
    GECODE_ME_CHECK(me);
    me = StringVarImp::me_combine(me, x1.bnd_length(home, l, u));
    GECODE_ME_CHECK(me);
    if (x0.contains(x1)) {
      if (x0.equals(x1))
        return ES_OK;
      x0.update(home, x1);
      StringDelta d;
      if (x0.assigned()) {
        x0.varimp()->notify(home, ME_STRING_VAL, d);
        return home.ES_SUBSUMED(*this);  
      }
      x0.varimp()->notify(home, x0.min_length() > l0 || x0.max_length() < u0 ?
                          ME_STRING_CARD : ME_STRING_BASE, d);
      return ES_OK;
    }
    if (x1.contains(x0)) {
      x1.update(home, x0);
      StringDelta d;
      if (x1.assigned()) {
        x1.varimp()->notify(home, ME_STRING_VAL, d);
        return home.ES_SUBSUMED(*this);  
      }
      x1.varimp()->notify(home, x1.min_length() > l1 || x1.max_length() < u1 ?
                          ME_STRING_CARD : ME_STRING_BASE, d);
      return ES_OK;
    }
    do {
      me = StringVarImp::me_combine(me, equate_x(home, x0, x1));
      GECODE_ME_CHECK(me);
      me = StringVarImp::me_combine(me, equate_x(home, x1, x0));
      GECODE_ME_CHECK(me);
      int l = std::max(x0.min_length(), x1.min_length()),
          u = std::min(x0.max_length(), x1.max_length());
      me = StringVarImp::me_combine(me, x0.bnd_length(home, l, u));
      GECODE_ME_CHECK(me);
      me = StringVarImp::me_combine(me, x1.bnd_length(home, l, u));
      GECODE_ME_CHECK(me);
    } while (x0.assigned() + x1.assigned() == 1);
    return x0.assigned() ? home.ES_SUBSUMED(*this) : ES_OK;
  }

}}}

// STATISTICS: string-prop
