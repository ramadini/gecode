namespace Gecode { namespace String {

  forceinline
  Eq::Eq(Home home, StringView x, StringView y)
  : MixBinaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>
    (home, x, y) {}

  forceinline
  Eq::Eq(Space& home, Eq& p)
  : MixBinaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>
    (home, p) {}

  forceinline ExecStatus
  Eq::post(Home home, StringView x, StringView y) {
    if (!x.same(y))
      (void) new (home) Eq(home, x, y);
    return ES_OK;
  }

  forceinline Actor*
  Eq::copy(Space& home) {
    return new (home) Eq(home, *this);
  }

  forceinline ExecStatus
  Eq::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"\nEq::propagate "<<x0<<" = "<<x1<<std::endl;
    GECODE_ME_CHECK(x0.eq(home, x1));
    if (home.failed())
      return ES_FAILED;
    // std::cerr<<"propagated: "<<x0<<" = "<<x1<<std::endl;
    assert (x0.domain().is_normalized() && x1.domain().is_normalized());
    if (x0.assigned()) {
      if (!x1.assigned())
        GECODE_ME_CHECK(x1.eq(home, x0.val()));
      assert (x0.val() == x1.val());
      return home.ES_SUBSUMED(*this);
    }
    if (x1.assigned()) {
      GECODE_ME_CHECK(x0.eq(home, x1.val()));
      assert (x0.val() == x1.val());
      return home.ES_SUBSUMED(*this);
    }
    return ES_FIX;
  }

}}
