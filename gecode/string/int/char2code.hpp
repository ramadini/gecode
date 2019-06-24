namespace Gecode { namespace String {

  forceinline
  Char2Code::Char2Code(Home home, StringView y0, Gecode::Int::IntView y1)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView,Gecode::Int::PC_INT_BND> 
    (home, y0, y1) {}

  forceinline ExecStatus
  Char2Code::post(Home home, StringView x0, Gecode::Int::IntView x1) {
    (void) new (home) Char2Code(home, x0, x1);
    return ES_OK;
  }

  forceinline
  Char2Code::Char2Code(Space& home, bool share, Char2Code& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView,Gecode::Int::PC_INT_BND>
    (home, share, p) {}

  forceinline Actor*
  Char2Code::copy(Space& home, bool share) {
    return new (home) Char2Code(home,share,*this);
  }

  forceinline ExecStatus
  Char2Code::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"\nChar2Code::propagate char2code("<<x0<<") = "<<x1<<'\n';
    if (x0.max_length() == 0) {
      GECODE_ME_CHECK(x1.eq(home, -1));
      return home.ES_SUBSUMED(*this);
    }
    if (x1.max() == -1) {
      GECODE_ME_CHECK(x1.eq(home, -1));
      return home.ES_SUBSUMED(*this);
    }
    if (x0.min_length() > 0)
      GECODE_ME_CHECK(x1.gq(home, 1));
    if (x1.min() > -1)
      GECODE_ME_CHECK(x0.lb(home, 1));
    IntSet s(x0.pdomain()->at(0).S.toIntSet());
    IntSetRanges is(s);
    GECODE_ME_CHECK(x1.inter_r(home, is));
    Gecode::Int::ViewRanges<Gecode::Int::IntView> d(x1);
    x0.pdomain()->at(0).S.intersectI(home, d);
    // std::cerr<<"\nChar2Code::propagated char2code("<<x0<<") = "<<x1<<'\n';
    assert (x0.pdomain()->is_normalized());
    return ES_FIX;
  }

}}
