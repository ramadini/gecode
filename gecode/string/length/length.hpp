namespace Gecode { namespace String {

  forceinline
  Length::Length(Home home, StringView y0, Gecode::Int::IntView y1)
  : MixBinaryPropagator<StringView, PC_STRING_LEN,
    Gecode::Int::IntView,Gecode::Int::PC_INT_BND> (home, y0, y1) {}

  forceinline
  Length::Length(Space& home, Length& p)
  : MixBinaryPropagator<StringView, PC_STRING_LEN,
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
    do {
      if (x0.min_length() < x1.min())
        GECODE_ME_CHECK(x0.lb(home, x1.min()));
      if (x0.max_length() > x1.max())
        GECODE_ME_CHECK(x0.ub(home, x1.max()));
      if (x1.min() < x0.min_length())
        GECODE_ME_CHECK(x1.gq(home, x0.min_length()));
      if (x1.max() > x0.max_length())
        GECODE_ME_CHECK(x1.lq(home, x0.max_length()));
    } while ((x0.min_length() < x1.min()) ||
             (x0.max_length() > x1.max()));
    if (x1.assigned())
      return home.ES_SUBSUMED(*this);
    return ES_FIX;
  }

}}
