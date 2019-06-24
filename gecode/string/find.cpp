#include <gecode/string.hh>
#include <gecode/string/find.hh>
#include <gecode/int/arithmetic.hh>

namespace Gecode {

  void
  find(Home home, StringVar x, StringVar y, IntVar n) {
    GECODE_POST;
    rel(home, n, IRT_GQ, 0);
    GECODE_ES_FAIL(String::Find::post(home, x, y, n));
  }

  // rfind(x, y) = n <=> find(rev(x), rev(y)) = m /\ n = [m > 0] * (|y| + 1 - m)
  void
  rfind(Home home, StringVar x, StringVar y, IntVar n) {
    GECODE_POST;
    int lx = x.min_length(), ux = x.max_length(),
        ly = y.min_length(), uy = y.max_length();
    rel(home, n, IRT_LQ, uy);
    rel(home, n, IRT_GQ, 0);
    // x1 = rev(x), y1 = rev(y), m = find(x1, y1).
    StringVar x1(home, lx, ux);
    rel(home, x, STRT_REV, x1);
    StringVar y1(home, ly, uy);
    rel(home, y, STRT_REV, y1);
    IntVar m(home, 0, uy);
    find(home, x1, y1, m);
    // e - |y| + m = 1 => e = |y| - m + 1.
    IntVar l(home, ly, uy);
    IntVar e(home, std::min(1, ly - m.max()), uy - m.min());
    length(home, y, l);
    IntArgs a;
    a << 1 << -1 << 1;
    IntVarArgs v;
    v << e << l << m;
    linear(home, a, v, IRT_EQ, 1);
    // r = [m > 0]
    BoolVar b(home, 0, 1);
    rel(home, m, IRT_GR, 0, b);
    IntVar r(home, 0, 1);
    channel(home, b, r);
    // n = e * r.
    Int::Arithmetic::MultBnd::post(home, e, r, n);
  }

}
