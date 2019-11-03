namespace Gecode { namespace String {

  forceinline
  Length::Length(Home home, StringView y0, Gecode::Int::IntView y1)
  : MixBinaryPropagator<StringView, PC_STRING_DOM,
    Gecode::Int::IntView,Gecode::Int::PC_INT_BND> (home, y0, y1) {}

  forceinline
  Length::Length(Space& home, Length& p)
  : MixBinaryPropagator<StringView, PC_STRING_DOM,
    Gecode::Int::IntView,Gecode::Int::PC_INT_BND> (home, p) {}

  forceinline ExecStatus
  Length::post(Home home, StringView x0, Gecode::Int::IntView x1) {
    (void) new (home) Length(home, x0, x1);
    return ES_OK;
  }
  
  forceinline Actor*
  Length::copy(Space& home) {
    return new (home) Length(home, *this);
  }

  forceinline ExecStatus
  Length::propagate(Space& home, const ModEventDelta&) {
    //std::cerr<<"\nLength::propagate "<<x0<<" "<<x1<<'\n';
    int lx0 = x0.min_length(), lx1 = x1.min();
    if (lx0 < lx1)
      GECODE_ME_CHECK(x0.lb(home, lx1));
    else if (lx0 > lx1)
      GECODE_ME_CHECK(x1.gq(home, lx0));
    int ux0 = x0.max_length(), ux1 = x1.max();
    if (ux0 > ux1)
      GECODE_ME_CHECK(x0.ub(home, ux1));
    else if (ux0 < ux1)
      GECODE_ME_CHECK(x1.lq(home, ux0));
    // std::cerr<<"After Length: "<<x0<<" "<<x1<<'\n';
    if (x0.min_length() == x0.max_length()) {
      if (!x1.assigned())
        GECODE_ME_CHECK(x1.eq(home, x0.min_length()));
      return home.ES_SUBSUMED(*this);
    }
    if (x1.assigned()) {
      int x = x1.val();
      if (x0.min_length() != x0.max_length()) {
        GECODE_ME_CHECK(x0.lb(home, x));
        GECODE_ME_CHECK(x0.ub(home, x));
      }
      assert (x0.min_length() == x0.max_length() && x0.max_length() == x && 
              x0.pdomain()->is_normalized());
      return home.ES_SUBSUMED(*this);
    }
    assert (x0.pdomain()->is_normalized());
    return ES_FIX;
  }

}}
