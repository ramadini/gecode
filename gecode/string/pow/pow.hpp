#include <gecode/int/arithmetic.hh>

namespace Gecode { namespace String {

  forceinline
  Pow::Pow(Home home, StringView x, Gecode::Int::IntView n, StringView y)
  : MixTernaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView,
    Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> (home, x, n, y) {}

  forceinline
  Pow::Pow(Space& home, Pow& p)
  : MixTernaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView,
    Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> (home, p) {}

  Actor*
  Pow::copy(Space& home) {
    return new (home) Pow(home, *this);
  }

  forceinline ExecStatus
  Pow::post(Home home, StringView x, Gecode::Int::IntView n, StringView y) {
    if (x.same(y)) {
      if (n.assigned() && (n.val() == 1))
        return ES_OK;
      if (n.min() > 1)
        GECODE_ME_CHECK(x.eq(home, ""));
      else {
        StringVar x0(home, x.may_chars(), x.min_length(), x.max_length());
        (void) new (home) Pow(home, x, n, x0);
        rel(home, x, STRT_EQ, x0);
        // Redundant length-channeling constraint |x0| = |x| * n.
        IntVar lx(home, x.min_length(), x.max_length());
        IntVar lx0(home, x0.min_length(), x0.max_length());
        length(home, StringVar(x), lx);
        length(home, x0, lx0);
        Gecode::mult(home, lx, IntVar(n), lx0);
      }
    }
    else {
      (void) new (home) Pow(home, x, n, y);
      // Redundant length-channeling constraint |y| = |x| * n.
      IntVar lx(home, x.min_length(), x.max_length());
      IntVar ly(home, y.min_length(), y.max_length());
      length(home, StringVar(x), lx);
      length(home, StringVar(y), ly);
      Gecode::mult(home, lx, IntVar(n), ly);
    }
    return ES_OK;
  }

  ExecStatus
  Pow::propagate(Space& home, const ModEventDelta&) {
    // std::cerr << "\nPow::propagate: " << x2 << " = " << x0 << " ** " << x1 << '\n';
    while (true) {
      if (x2.min_length() > 0)
        GECODE_ME_CHECK(x1.gq(home, 1));
      else
        GECODE_ME_CHECK(x1.gq(home, 0));
      GECODE_ME_CHECK(x1.lq(home, DashedString::_MAX_STR_LENGTH));
      int l = x1.min(), u = x1.max();
      GECODE_ME_CHECK(x2.pow(home, x0, l, u));
      GECODE_ME_CHECK(x1.gq(home, l));
      GECODE_ME_CHECK(x1.lq(home, u));
      // std::cerr << "After pow: " << x2 << " = " << x0 << " ** " << x1 << '\n';
      assert (x0.domain().is_normalized() && x2.domain().is_normalized());
      int assigned = x0.assigned() + x1.assigned() + x2.assigned();
      if (assigned == 3) {
        string x = x0.val(), xn;
        int n = x1.val();
        xn.reserve(x2.min_length());
        for (int i = 0; i < n; ++i)
          xn += x;
        assert (x2.val() == xn);
        return home.ES_SUBSUMED(*this);
      }
      if (assigned == 2) {
        if ((x1.assigned() && x1.val() == 0) ||
            (x0.max_length() == 0 && x2.max_length() == 0))
          return ES_FIX;
      }
      else
        return ES_FIX;
    }
  }

}}
