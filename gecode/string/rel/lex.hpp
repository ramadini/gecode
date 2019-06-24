namespace Gecode { namespace String {

  forceinline
  Lex::Lex(Home home, StringView x, StringView y, bool b)
  : MixBinaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>
    (home, x, y), lt(b) {}

  forceinline
  Lex::Lex(Space& home, bool share, Lex& p)
  : MixBinaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>
    (home, share, p), lt(p.lt) {}

  forceinline ExecStatus
  Lex::post(Home home, StringView x, StringView y, bool b) {
    if (x.same(y)) {
      if (b)
        return ES_FAILED;
      else
        return Eq::post(home, x, y);
    }
    (void) new (home) Lex(home, x, y, b);
    return ES_OK;
  }

  forceinline Actor*
  Lex::copy(Space& home, bool share) {
    return new (home) Lex(home, share, *this);
  }

  forceinline ExecStatus
  Lex::propagate(Space& home, const ModEventDelta&) {
    //lt ? std::cerr<<"\nLex::propagate "<<x0<<" < " <<x1<<std::endl
    //   : std::cerr<<"\nLex::propagate "<<x0<<" <= "<<x1<<std::endl;
    if (x0.assigned() && x1.assigned()) {
      return (x0.val() < x1.val() || (!lt && x0.val() == x1.val()))
        ? home.ES_SUBSUMED(*this) : ES_FAILED;
    }
    if (lt)
      GECODE_ME_CHECK(x1.lb(home, 1));
    if (x1.max_length() == 0) {
      GECODE_ME_CHECK(x0.eq(home, ""));
      return home.ES_SUBSUMED(*this);
    }
    if (x0.max_length() == 0)
      return home.ES_SUBSUMED(*this);
    GECODE_ME_CHECK(x0.lex(home, x1, lt));
    if (x0.assigned() && x1.assigned()) {
      assert (x0.val() < x1.val() || (!lt && x0.val() == x1.val()));
      return home.ES_SUBSUMED(*this);
    }
    //std::cerr<<"propagated: "<<x0<<" < (<=) "<<x1<<std::endl;
    assert (x0.pdomain()->is_normalized() && x1.pdomain()->is_normalized());
    return ES_FIX;
  }

}}
