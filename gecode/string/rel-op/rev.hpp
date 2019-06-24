namespace Gecode { namespace String {

  forceinline ExecStatus
  Rev::post(Home home, StringView x0, StringView x1) {
    if (x0.same(x1)) {
      StringVar y(home, x0.may_chars(), x0.min_length(), x0.max_length());
      (void) new (home) Rev(home, x0, y);
      rel(home, x1, STRT_EQ, y);
    }
    else
      (void) new (home) Rev(home, x0, x1);
    return ES_OK;
  }
  
  forceinline Actor*
  Rev::copy(Space& home) {
    return new (home) Rev(home, *this);
  }

  forceinline
  Rev::Rev(Home home, StringView y0, StringView y1)
  : MixBinaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>
    (home, y0, y1) {}

  forceinline
  Rev::Rev(Space& home, Rev& p)
  : MixBinaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>
    (home, p) {}

  forceinline ExecStatus
  Rev::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"Rev::propagate "<<x1<<" = "<<x0<<"^-1"<<std::endl;
    GECODE_ME_CHECK(x1.rev(home, x0));
    // std::cerr<<"Rev::propagated "<<x1<<" = "<<x0<<"^-1"<<std::endl;
    assert (x0.pdomain()->is_normalized() && x1.pdomain()->is_normalized());
    switch (x0.assigned() + x1.assigned()) {
      case 2: {
        string x0r = x0.val();
        std::reverse(x0r.begin(), x0r.end());
        assert (x0r == x1.val());
        return home.ES_SUBSUMED(*this);
      }
      case 1:
        return ES_NOFIX;
      default:
        return ES_FIX;
    }
  }

}}
