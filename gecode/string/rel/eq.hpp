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

    ModEvent me0 = View0::me(med);
    ModEvent me1 = View1::me(med);
    
    return ES_OK;

//    Region r;

//    if (testStringEventLB(me0,me1)) {
//      GlbRanges<View0> x0lb(x0);
//      GlbRanges<View1> x1lb(x1);
//      Iter::Ranges::Union<GlbRanges<View0>,GlbRanges<View1> > lbu(x0lb,x1lb);
//      Iter::Ranges::Cache lbuc(r,lbu);
//      GECODE_ME_CHECK(x0.includeI(home,lbuc));
//      lbuc.reset();
//      GECODE_ME_CHECK(x1.includeI(home,lbuc));
//    }

//    if (testStringEventUB(me0,me1)) {
//      LubRanges<View0> x0ub(x0);
//      LubRanges<View1> x1ub(x1);
//      Iter::Ranges::Inter<LubRanges<View0>,LubRanges<View1> > ubi(x0ub,x1ub);
//      Iter::Ranges::Cache ubic(r,ubi);
//      GECODE_ME_CHECK(x0.intersectI(home,ubic));
//      ubic.reset();
//      GECODE_ME_CHECK(x1.intersectI(home,ubic));
//    }

//    if (testStringEventCard(me0,me1)) {
//      unsigned int max = std::min(x0.cardMax(),x1.cardMax());
//      unsigned int min = std::max(x0.cardMin(),x1.cardMin());
//      GECODE_ME_CHECK(x0.cardMax(home,max));
//      GECODE_ME_CHECK(x1.cardMax(home,max));
//      GECODE_ME_CHECK(x0.cardMin(home,min));
//      GECODE_ME_CHECK(x1.cardMin(home,min));
//    }

//    if (x0.assigned()) {
//      assert (x1.assigned());
//      return home.ES_SUBSUMED(*this);
//    }
//    return shared(x0,x1) ? ES_NOFIX : ES_FIX;
  }

}}}

// STATISTICS: string-prop
