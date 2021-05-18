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
//    std::cerr << "Prop. Eq " << x0 << "  vs  " << x1 << "\n";
    int l0 = x0.min_length(), l1 = x1.min_length(), 
        u0 = x0.max_length(), u1 = x1.max_length(),
        l = std::max(l0, l1), u = std::min(u0, u1);
    ModEvent me0 = x0.bnd_length(home, l, u);
    GECODE_ME_CHECK(me0);
    ModEvent me1 = x1.bnd_length(home, l, u);
    GECODE_ME_CHECK(me1);
    if (x0.assigned()) {
      if (check_equate_x(x0,x1) && check_equate_x(x1,x0)) {
        if (!x1.assigned()) {
          x1.update(home, x0);
          StringDelta d;
          x1.varimp()->notify(home, ME_STRING_VAL, d);
        }
        return home.ES_SUBSUMED(*this);      
      }
      else
        return ES_FAILED;
    }
    if (x1.assigned()) {
      if (check_equate_x(x0,x1) && check_equate_x(x1,x0)) {
        x0.update(home, x1);
        if (!x0.assigned()) {
          x0.update(home, x1);
          StringDelta d;
          x0.varimp()->notify(home, ME_STRING_VAL, d);
        }
        return home.ES_SUBSUMED(*this);      
      }
      else
        return ES_FAILED;
    }
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
      me0 = StringVarImp::me_combine(me0, equate_x(home, x0, x1));
      GECODE_ME_CHECK(me0);
      me1 = StringVarImp::me_combine(me1, equate_x(home, x1, x0));
      GECODE_ME_CHECK(me1);
      int l = std::max(x0.min_length(), x1.min_length()),
          u = std::min(x0.max_length(), x1.max_length());
      me0 = StringVarImp::me_combine(me0, x0.bnd_length(home, l, u));
      GECODE_ME_CHECK(me0);
      me1 = StringVarImp::me_combine(me1, x1.bnd_length(home, l, u));
      GECODE_ME_CHECK(me1);
    } while (x0.assigned() + x1.assigned() == 1);    
    if (me0 != ME_STRING_NONE) {
      StringDelta d;
      x0.varimp()->notify(home, me0, d);
    }
    if (me1 != ME_STRING_NONE) {
      StringDelta d;
      x1.varimp()->notify(home, me1, d);
    }
    // std::cerr << "Eq::propagated.\n";
    return x0.assigned() ? home.ES_SUBSUMED(*this) : ES_OK;
  }

}}}

// STATISTICS: string-prop
