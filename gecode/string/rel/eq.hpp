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
  Eq<View0,View1>::refine_card(Space& home) {
    int l = std::max(x0.min_length(), x1.min_length()),
        u = std::min(x0.max_length(), x1.max_length());
    GECODE_ME_CHECK(x0.bnd_length(home, l, u));
    GECODE_ME_CHECK(x1.bnd_length(home, l, u));
    return ES_OK;
  }

  template<class View0, class View1>
  ExecStatus
  Eq<View0,View1>::propagate(Space& home, const ModEventDelta&) {
    std::cerr<<"\n"<<this<<"::Eq::propagate" << x0 << "  vs  " << x1 << "\n";
    refine_card(home);
    if (x0.assigned()) {
      if (x0.isNull()) {
        GECODE_ME_CHECK(x1.nullify(home));
        return __ES_SUBSUMED; //home.ES_SUBSUMED(*this);
      }
      if (check_equate_x(x1,x0)) {
        if (!x1.assigned())
            x1.update(home, x0);
        return __ES_SUBSUMED; //home.ES_SUBSUMED(*this);
      }
      else
        return ES_FAILED;
    }
    if (x1.assigned()) {
      if (x1.isNull()) {
        GECODE_ME_CHECK(x0.nullify(home));
        return __ES_SUBSUMED; //home.ES_SUBSUMED(*this);
      }
      if (check_equate_x(x0,x1)) {
        x0.update(home, x1);
        return __ES_SUBSUMED; //FIXME: with home.ES_SUBSUMED(*this), seg.fault in SQL 12
      }
      else
        return ES_FAILED;
    }
    if (x0.contains(x1)) {
      if (x0.equals(x1))
        return ES_FIX;
      x0.update(home, x1);
      return ES_FIX;
    }
    if (x1.contains(x0)) {
      x1.update(home, x0);
      return ES_FIX;
    }
    ModEvent me0;
    do {
      me0 = x0.equate(home, x1);
      GECODE_ME_CHECK(me0);
      ModEvent me1 = x1.equate(home, x0);
      GECODE_ME_CHECK(me1);
      if (me0 + me1 != ME_STRING_NONE)
        refine_card(home);
    } while (x0.assigned() + x1.assigned() == 1);
    std::cerr << "Eq::propagated.\n";
    return x0.assigned() ? home.ES_SUBSUMED(*this) : ES_FIX;
  }

}}}

// STATISTICS: string-prop
