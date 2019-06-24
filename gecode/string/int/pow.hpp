namespace Gecode { namespace String {

  forceinline
  Pow::Pow(Home home, StringView x, Gecode::Int::IntView n, StringView y)
  : MixTernaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView,
    Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> (home, x, n, y) {}

  forceinline
  Pow::Pow(Space& home, bool share, Pow& p)
  : MixTernaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView,
    Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> (home, share, p) {}

  Actor*
  Pow::copy(Space& home, bool share) {
    return new (home) Pow(home, share, *this);
  }
  
  forceinline ExecStatus
  Pow::post(Home home, StringView x, Gecode::Int::IntView n, StringView y) {
    if (x.same(y)) {
      if (n.min() > 0)
        rel(home, x, STRT_EQ, StringVar(home, ""));
      else {
        StringVar x0(home, x.may_chars(), x.min_length(), x.max_length());
        (void) new (home) Pow(home, x, n, x0);
        rel(home, x, STRT_EQ, x0);
      }
    }
    else
      (void) new (home) Pow(home, x, n, y);
    return ES_OK;
  }

  ExecStatus
  Pow::propagate(Space& home, const ModEventDelta&) {
    //std::cerr << "\nPow::propagate: " << x2 << " = " << x0 << " ** " << x1 << '\n';
    if (x2.min_length() > 0)
      GECODE_ME_CHECK(x1.gq(home, 1));
    else
      GECODE_ME_CHECK(x1.gq(home, 0));
    GECODE_ME_CHECK(x1.lq(home, DashedString::_MAX_STR_LENGTH));
    int l = x1.min(), u = x1.max();
    GECODE_ME_CHECK(x2.pow(home, x0, l, u));
    GECODE_ME_CHECK(x1.gq(home, l));
    GECODE_ME_CHECK(x1.lq(home, u));
    //std::cerr << "After pow: " << x2 << " = " << x0 << " ** " << x1 << '\n';    
    assert (x0.pdomain()->is_normalized() && x2.pdomain()->is_normalized());
    switch (x0.assigned() + x1.assigned() + x2.assigned()) {
      case 3: {
        string x = x0.val(), xn;
        for (int i = 0; i < x1.val(); ++i)
          xn += x;
        assert (x2.val() == xn);
        return home.ES_SUBSUMED(*this);
      }
      case 2:
        return ES_NOFIX;
      default:
        return ES_FIX;
    }
  }

}}
