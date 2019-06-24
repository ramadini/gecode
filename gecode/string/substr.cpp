#include <gecode/string.hh>

namespace Gecode {

  /*
   * y = x[i..j] <==> y = x[i] x[i + 1] ... x[j].
   * lx = |x| /\ n = max(1, i) /\ m = min(lx, j) /\ ly = |y| /\
   * ly = min(lx, max(0, m - n + 1)) /\ x = y' ++ y ++ y'' /\
   * y' :: X^{min(lx, n - 1)} /\ y'' :: X^{max(0, lx - m)} /\ X = chars(x)
   */
  void
  substr(Home home, StringVar x, IntVar i, IntVar j, StringVar y) {
    GECODE_POST;
    int x1 = x.min_length(),
        x2 = x.max_length(),
        n1 = std::max(1, i.min()),
        n2 = std::max(1, i.max()),
        m1 = std::max(0, std::min(x1, j.min())),
        m2 = std::max(0, std::min(x2, j.max()));
    if (x2 == 0 || m1 > m2 || n1 > n2 || n1 > m2 || n1 > x2) {
      y.varimp()->eq(home, "");
      return;
    }
    IntVar n(home, n1, n2),
         n_1(home, n1 - 1, n2 - 1),
           m(home, m1, m2),
           p(home, m1 - n2 + 1, m2 - n1 + 1),
       l(home, 0, p.max()),
           lx(home, x1, x2),
       lx_m(home, x1 - m2, x2 - m1),
           ly (home, 0, x2),
       ly1(home, 0, x2),
       ly2(home, 0, x2);

    min(home, lx, j, m);
    max(home, IntVar(home, 1, 1), i, n);
    IntArgs a;
    a << -1 << 1 << 1;
    IntVarArgs v;
    v << m << n << p;
    linear(home, a, v, IRT_EQ, 1); // -m + n + p = 1 ==> p = m - n + 1
    max(home, IntVar(home, 0, 0), p, l); // l = max(0, p) = max(0, m - n + 1)
    min(home, l, lx, ly);
    length(home, x, lx);
    length(home, y, ly);

    StringVar y1(home), y1y(home);
    rel(home, y1, y,  STRT_CAT, y1y);
    IntArgs a1;
    a1 << -1 << 1;
    IntVarArgs v1;
    v1 << n_1 << n;
    linear(home, a1, v1, IRT_EQ, 1);
    min(home, lx, n_1, ly1);
    length(home, y1,  ly1);

    StringVar y2(home);
    rel(home, y1y, y2, STRT_CAT, x);
    IntArgs a2;
    a2 << 1 << -1 << -1;
    IntVarArgs v2;
    v2 << lx << m << lx_m;
    linear(home, a2, v2, IRT_EQ, 0);
    max(home, IntVar(home, 0, 0), lx_m, ly2);
    length(home, y2, ly2);
  }

  // x contains y <==> find(y, x) = n /\ n > 0.
  void
  contains(Home home, StringVar x, StringVar y) {
    GECODE_POST;
    IntVar n(home, 1, x.max_length() - y.min_length() + 1);
    find(home, y, x, n);
  }

  // b = (x contains y) <==> find(y, x) = n /\ b = [n > 0]
  void
  contains(Home home, StringVar x, StringVar y, BoolVar b) {
    GECODE_POST;
    IntVar n(home, 0, std::max(0, x.max_length() - y.min_length() + 1));
    find(home, y, x, n);
    rel(home, n, IRT_GR, 0, b);
  }

  // x startswith y <==> x = y ++ z.
  void
  startswith(Home home, StringVar x, StringVar y) {
  StringVar z(home);
  rel(home, y, z, STRT_CAT, x);
  }

  // x endswith y <==> x = z ++ y.
  void
  endswith(Home home, StringVar x, StringVar y) {
    StringVar z(home);
    rel(home, z, y, STRT_CAT, x);
  }

  // b = (x startswith y) <==> b = [find(y, x) = 1].
  void
  startswith(Home home, StringVar x, StringVar y, BoolVar b) {
    IntVar i(home, 0, x.max_length());
    find(home, y, x, i);
    IntVar lx(home, x.min_length(), x.max_length());
    IntVar ly(home, y.min_length(), y.max_length());
    length(home, x, lx);
    length(home, y, ly);
    rel(home, ly, IRT_LQ, lx);
    rel(home, i, IRT_EQ, 1, Reify(b));
  }

  // b = (x endswith y) <==> b = [find(rev(y), rev(x)) = 1].
  void
  endswith(Home home, StringVar x, StringVar y, BoolVar b) {
    IntVar i(home, 0, x.max_length());
    StringVar x1(home), y1(home);
    rel(home, x, STRT_REV, x1);
    rel(home, y, STRT_REV, y1);
    IntVar lx(home, x.min_length(), x.max_length());
    IntVar ly(home, y.min_length(), y.max_length());
    length(home, x, lx);
    length(home, y, ly);
    rel(home, ly, IRT_LQ, lx);
    find(home, y1, x1, i);
    rel(home, i, IRT_EQ, 1, Reify(b));
  }


}
