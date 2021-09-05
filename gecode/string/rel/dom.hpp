namespace Gecode { namespace String { namespace Rel {

  template<class View0, class View1>
  forceinline
  Dom<View0,View1>::Dom(Home home, View0 x, View1 y)
    : MixBinaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY>(home,x,y) {}

  template<class View0, class View1>
  forceinline
  Dom<View0,View1>::Dom(Space& home, Dom& p)
    : MixBinaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY>(home,p) {}

  template<class View0, class View1>
  ExecStatus
  Dom<View0,View1>::post(Home home, View0 x, View1 y) {
    if (!x.same(y))
      (void) new (home) Dom(home,x,y);
    return ES_OK;
  }

  template<class View0, class View1>
  Actor*
  Dom<View0,View1>::copy(Space& home) {
    return new (home) Dom(home,*this);
  }

  template<class View0, class View1>
  ExecStatus
  Dom<View0,View1>::propagate(Space& home, const ModEventDelta&) {
    std::cerr<<"\n"<<this<<"::Dom::propagate" << x0 << "  vs  " << x1 << "\n";
    GECODE_ME_CHECK(x0.bnd_length(home, 
      std::max(x0.min_length(), x1.min_length()),
      std::min(x0.max_length(), x1.max_length())
    ));
    if (x1.contains(x0))
      return home.ES_SUBSUMED(*this);
    if (x0.assigned())
      return check_equate_x(x1,x0) ? home.ES_SUBSUMED(*this): ES_FAILED;
    if (x1.assigned()) {
      if (!check_equate_x(x0,x1))
        return ES_FAILED;
      x0.update(home, x1);
      return home.ES_SUBSUMED(*this);
    }
    ModEvent me = x0.equate(home, x1);
    GECODE_ME_CHECK(me);
    if (me != ME_STRING_NONE) {
      GECODE_ME_CHECK(x0.bnd_length(home, 
        std::max(x0.min_length(), x1.min_length()),
        std::min(x0.max_length(), x1.max_length())
      ));
    }
    if (!check_equate_x(x1,x0))
      return ES_FAILED;
     std::cerr<<"Dom::propagated" << x0 << "  vs  " << x1 << "\n";
    return x0.assigned() ? home.ES_SUBSUMED(*this) : ES_FIX;
  }

}}}

// STATISTICS: string-prop
