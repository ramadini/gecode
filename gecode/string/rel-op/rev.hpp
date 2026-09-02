namespace Gecode { namespace String {

  forceinline ExecStatus
  Rev::post(Home home, StringView x0, StringView x1) {
    if (x0.same(x1)) {
      StringVar y(home, x0.may_chars(), x0.min_length(), x0.max_length());
      (void) new (home) Rev(home, x0, y);
      rel(home, x1, STRT_EQ, y);
      // Redundant length-channeling constraint |x0| = |y|.
      IntVar lx0(home, x0.min_length(), x0.max_length());
      IntVar ly(home, y.min_length(), y.max_length());
      length(home, StringVar(x0), lx0);
      length(home, y, ly);
      rel(home, lx0, IRT_EQ, ly);
    }
    else {
      (void) new (home) Rev(home, x0, x1);
      // Redundant length-channeling constraint |x0| = |x1|.
      IntVar lx0(home, x0.min_length(), x0.max_length());
      IntVar lx1(home, x1.min_length(), x1.max_length());
      length(home, StringVar(x0), lx0);
      length(home, StringVar(x1), lx1);
      rel(home, lx0, IRT_EQ, lx1);
    }
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
    while (true) {
      GECODE_ME_CHECK(x1.rev(home, x0));
      // std::cerr<<"Rev::propagated "<<x1<<" = "<<x0<<"^-1"<<std::endl;
      assert (x0.domain().is_normalized() && x1.domain().is_normalized());
      int assigned = x0.assigned() + x1.assigned();
      if (assigned == 2) {
        string x0r = x0.val();
        std::reverse(x0r.begin(), x0r.end());
        assert (x0r == x1.val());
        return home.ES_SUBSUMED(*this);
      }
      if (assigned != 1)
        return ES_FIX;
    }
  }

}}
