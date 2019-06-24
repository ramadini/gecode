#include <gecode/string.hh>

namespace Gecode  {

  // FIXME: Assuming indexes 1-based.
  void
  replace(
    Home home, StringVar x, StringVar x1, StringVar y, StringVar y1, VarArgs& va
  ) {
    GECODE_POST;
    String::NSIntSet ychars(y.may_chars());
    ychars.intersect(y1.may_chars());
    int ymax = std::min(y.max_length(), y1.max_length());
    StringVar pref(home, ychars, 0, ymax);
    StringVar suff(home, ychars, 0, ymax);
    int u = std::min(String::DashedString::_MAX_STR_LENGTH,  ymax);
    StringVar aux(home, 0, u), aux1(home, 0, u);
    StringVar z (home,  x.may_chars(), 0,  x.max_length());
    StringVar z1(home, x1.may_chars(), 0, x1.max_length());

    // y = pref ++ z ++ suff.
    StringVarArgs pzs, pz1s;
    pzs  << pref << z  << suff;
    pz1s << pref << z1 << suff;
    gconcat(home, pzs, y);
    gconcat(home, pz1s, y1);
    va.sva << pref << suff << z << z1;

    // z = [eps,x][i] /\ z1 = [eps,x1][i] /\ find(x, y) = n /\ i = [n > 0] + 1.
    IntVar i0(home, 0, 1);
    IntVar n(home, 0, std::max(0, y.max_length() - x.min_length() + 1));
    BoolVar b0(home, 0, 1);
    StringVarArgs ex, ex1;
    ex  << StringVar(home, "") << x;
    ex1 << StringVar(home, "") << x1;
    IntVar i(home, 1, 2);
    IntArgs vc;
    IntVarArgs vx;
    vc << 1 << -1;
    vx << i << i0;
    rel(home, n, IRT_GR, 0, Reify(b0));
    channel(home, b0, i0);
    element(home, ex,  i, z);
    element(home, ex1, i, z1);
    linear(home, vc, vx, IRT_EQ, 1);
    va.iva << i << n;

    // |pref| = max(0, n - 1) /\ find(x, pref) = j /\ j = [x = eps].
    IntVar n1(home, -1, n.max() - 1), lpref(home, 0, ymax), j(home, 0, 1);
    BoolVar b1(home, 0, 1);
    IntArgs a;
    a << 1 << -1;
    IntVarArgs v;
    v << n1 << n;
    // n1 - n = -1 ==> n1 = n - 1
    find(home, x, y, n);
    length(home, pref, lpref);
    linear(home, a, v, IRT_EQ, -1);
    max(home, IntVar(home, 0, 0), n1, lpref);
    find(home, x, pref, j);
    rel(home, x, STRT_EQ, StringVar(home, ""), Reify(b1));
    channel(home, b1, j);
    va.iva << j;

  }

}
