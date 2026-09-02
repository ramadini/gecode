#include <gecode/int/arithmetic.hh>

namespace Gecode { namespace String {

  forceinline
  GConcat::GConcat(Home home, ViewArray<StringView>& x, StringView y)
  : NaryOnePropagator<StringView, PC_STRING_DOM>(home, x, y) {}

  forceinline
  GConcat::GConcat(Space& home, GConcat& p)
  : NaryOnePropagator<StringView, PC_STRING_DOM>(home, p) {}

  forceinline ExecStatus
  GConcat::post(Home home, ViewArray<StringView>& x, StringView y) {
    // Remove aliases.
    for (int i = 0; i < x.size(); ++i)
      for (int j = i + 1; j < x.size(); ++j)
        if (x[i].varimp() == x[j].varimp()) {
          StringVar z(
            home, x[j].may_chars(), x[j].min_length(), x[j].max_length()
          );
          x[j] = z;
          rel(home, x[i], STRT_EQ, z);
        }
    // Separate an operand aliased with the result. NaryOnePropagator
    // requires distinct subscriptions between its array and extra view.
    for (int i = 0; i < x.size(); ++i)
      if (x[i].same(y)) {
        StringVar z(
          home, y.may_chars(), y.min_length(), y.max_length()
        );
        rel(home, y, STRT_EQ, z);
        y = z;
        break;
      }
    (void) new (home) GConcat(home, x, y);
    // Redundant length-channeling constraint |y| = sum_i |x[i]|.
    IntVarArgs lxs;
    IntArgs ia;
    for (int i = 0; i < x.size(); ++i) {
      IntVar lxi(home, x[i].min_length(), x[i].max_length());
      length(home, StringVar(x[i]), lxi);
      lxs << lxi;
      ia << 1;
    }
    IntVar ly(home, y.min_length(), y.max_length());
    length(home, StringVar(y), ly);
    lxs << ly;
    ia << -1;
    linear(home, ia, lxs, IRT_EQ, 0);
    return ES_OK;
  }

  forceinline Actor*
  GConcat::copy(Space& home) {
    return new (home) GConcat(home, *this);
  }

  forceinline bool
  GConcat::refine_card(Space& home) {

    long lx = 0, ux = 0;
    for (auto& xi : x) {
      lx += xi.min_length();
      ux += xi.max_length();
    }
    if (lx > ux || lx > y.max_length() || ux < y.min_length())
      return false;
    bool again;
    do {



      again = false;
      if (lx > y.min_length()) {
        y.lb(home, lx);
        again = true;
      }
      if (ux < y.max_length()) {
        y.ub(home, ux);
        again = true;
      }
      int i = -1, ly = y.min_length(), uy = y.max_length();
      for (auto& xi : x) {
        i++;
        long lxi = ly - ux + xi.max_length();
        long uxi = max(0, uy - lx + xi.min_length());
        if (lxi > uxi)
          return false;
        if (lxi > xi.min_length()) {
          xi.lb(home, lxi);
          again = true;
        }
        if (uxi < xi.max_length()) {
          xi.ub(home, uxi);
          again = true;
        }
      }
      if (again) {
        lx = 0, ux = 0;
        for (auto& xi : x) {
          lx += xi.min_length();
          ux += xi.max_length();
        }
      }
      if (lx > ux || lx > y.max_length() || ux < y.min_length())
        return false;
    }
    while (again);

    return true;
  }

  forceinline ExecStatus
  GConcat::assigned(Space& home) {
    string val;
    string::size_type length = 0;
    for (auto& s : x)
      length += s.min_length();
    val.reserve(length);
    for (auto& s : x)
      val += s.val();
    if (y.assigned())
      return val == y.val() ? home.ES_SUBSUMED(*this) : ES_FAILED;
    rel(home, y, STRT_EQ, StringVar(home, val));
    return home.ES_SUBSUMED(*this);
  }

  forceinline ExecStatus
  GConcat::propagate(Space& home, const ModEventDelta&) {
    if (x.assigned())
      return assigned(home);
    if (!refine_card(home))
      return ES_FAILED;
    if (x.assigned())
      return assigned(home);
    ModEvent me = y.gconcat(home, x);
    GECODE_ME_CHECK(me);
    if (!refine_card(home))
      return ES_FAILED;
    assert (y.domain().is_normalized());
    for (auto& xi : x)
      assert (xi.domain().is_normalized());

    return x.assigned() ? assigned(home) : ES_FIX;
  }

}}
