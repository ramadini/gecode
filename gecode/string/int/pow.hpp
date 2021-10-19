#include <gecode/int.hh>
#include <gecode/string/rel.hh>

namespace Gecode { namespace String { namespace Int {

  template<class View0,class View1>
  forceinline
  Pow<View0,View1>::Pow(Home home, View0 x, Gecode::Int::IntView n, View1 y)
  : MixTernaryPropagator<View0, PC_STRING_CARD, Gecode::Int::IntView,
  Gecode::Int::PC_INT_BND, View1, PC_STRING_CARD> (home, x, n, y) {}

  template<class View0,class View1>
  forceinline ExecStatus
  Pow<View0,View1>::post(Home home, View0 x, Gecode::Int::IntView n, View1 y) {
    if (x.same(y)) {
      GECODE_ME_CHECK(n.gq(home, 0));
      GECODE_ME_CHECK(n.lq(home, 1));
    }
    else
      (void) new (home) Pow(home, x, n, y);
    return ES_OK;
  }

  template<class View0,class View1>
  forceinline
  Pow<View0,View1>::Pow(Space& home, Pow& p)
  : MixTernaryPropagator<View0, PC_STRING_CARD, Gecode::Int::IntView,
  Gecode::Int::PC_INT_BND, View1, PC_STRING_CARD> (home, p) {}

  template<class View0,class View1>
  forceinline Actor*
  Pow<View0,View1>::copy(Space& home) {
    return new (home) Pow(home,*this);
  }
  
  template<class View0, class View1>
  forceinline long
  Pow<View0,View1>::div_l(long a, long b) {
    return b == 0 ? 0 : a / b;
  }
  template<class View0, class View1>
  forceinline long
  Pow<View0,View1>::div_u(long a, long b) {
    return b == 0 ? MAX_STRING_LENGTH : ceil(a / b);
  }

  template<class View0, class View1>
  forceinline ExecStatus
  Pow<View0,View1>::refine_card(Space& home) {
    int n1 = x1.min(), n2 = x1.max();
    long lx = x0.min_length(), ly = std::max(lx*n1, long(x2.min_length())),
         ux = x0.max_length(), uy = std::min(ux*n2, long(x2.max_length()));
    ly = std::max(ly, lx * n1), uy = std::min(uy, ux * n2),
    lx = std::max(lx, div_l(ly, n2)), ux = std::min(ux, div_u(uy, n1)),
    n1 = std::max(n1, int(div_l(ly,ux))), n2 = std::min(n2, int(div_u(uy,lx)));
    if (n1 > n2 || lx > ux || ly > uy)
      return ES_FAILED;
    if (lx > x0.min_length()) GECODE_ME_CHECK(x0.min_length(home, lx));
    if (ux < x0.max_length()) GECODE_ME_CHECK(x0.max_length(home, ux));
    if (n1 > x1.min())        GECODE_ME_CHECK(x1.gq(home, n1));
    if (n2 < x1.max())        GECODE_ME_CHECK(x1.lq(home, n2));
    if (ly > x2.min_length()) GECODE_ME_CHECK(x2.min_length(home, ly));
    if (uy < x2.max_length()) GECODE_ME_CHECK(x2.max_length(home, uy));
    return ES_OK;
  }

  template<class View0,class View1>
  forceinline ExecStatus
  Pow<View0,View1>::propagate(Space& home, const ModEventDelta&) {
    std::cerr <<"\n"<< this << "::Pow::propagate "<< x2 << " = " << x0 << " ^(" << x1 << ")\n";
    refine_card(home);
    int a;
    do {
      int l = x1.min(), u = x1.max();
      if (l == u && u == 1)
        GECODE_REWRITE(*this, (Gecode::String::Rel::Eq<View0,View1>
          ::post(home(*this), x0, x2)));
      // n == 0 \/ epsilon^n.
      if (u == 0 || x0.isNull()) {
        if (x2.isNull())
          return home.ES_SUBSUMED(*this);
        if (x2.min_length() > 0)
          return ES_FAILED;
        x2.nullify(home);
        return home.ES_SUBSUMED(*this);
      }
      // x^n = epsilon
      if (x2.isNull()) {
        if (l > 0) {
          if (x0.min_length() > 0)
            return ES_FAILED;
          x0.nullify(home);
          return home.ES_SUBSUMED(*this);
        }
        else if (x0.min_length() > 0) {
          x1.lq(home, 0);
          return home.ES_SUBSUMED(*this);  
        }
        return ES_FIX;
      }
      // General case
      int n0 = x0.size();
      bool norm = x0[0].equals(x0[n0-1]);
      int m0 = n0 < 2 ? 0 : l*(n0-2*norm);
      if (m0 == 0) {
        if (!x0[0].isUniverse()) {
          Block bx;
          bx.update(home, x0[0]);
          bx.updateCard(home, 0, MAX_STRING_LENGTH);
          if (x2.assigned()) {
            if (!check_equate_x(x2, ConstDashedView(bx, 1)))
              return ES_FAILED;
          }
          else
            GECODE_ME_CHECK(x2.equate(home, ConstDashedView(bx, 1)));
        }
      }
      else {
        Region r1;
        Block* d0 = r1.alloc<Block>(m0+1);
        Set::GLBndSet s;
        for (int i = 0; i < n0; ++i) {
          d0[i].update(home, x0[i]);
          x0[i].includeBaseIn(home, s);
        }
        int l0, u0;
        if (norm) {
          l0 = ubounded_sum(x0[0].lb(), x0[n0-1].lb()); 
          u0 = ubounded_sum(x0[0].ub(), x0[n0-1].ub());
        }
        for (int i = 1; i < l; ++i) {
          for (int j = norm; j < n0; ++j)
            d0[i*n0+j].update(home, x0[j]);
          if (norm)
            d0[i*n0].updateCard(home, l0, u0);
        }
        u0 = std::min(long(MAX_STRING_LENGTH), x0.max_length()*long(u-l));
        if (d0[m0-1].baseEquals(s)) {
          m0--;
          d0[m0].updateCard(home, d0[m0].lb(), ubounded_sum(d0[m0].ub(), u0));
        }
        else if (u0 > 0)
          d0[m0].update(home, Block(home, CharSet(home, s), 0, u0));
        if (x2.assigned()) {
          if (!check_equate_x(x2, ConstDashedView(d0[0], m0)))
            return ES_FAILED;
        }
        else
          GECODE_ME_CHECK(x2.equate(home, ConstDashedView(d0[0], m0)));
      }
      if (l > 0) {
        Set::GLBndSet s;
        for (int i = 0; i < x2.size(); ++i)
          x2[i].includeBaseIn(home, s);
        Block by(home, CharSet(home, s), x2.min_length(), x2.max_length());
        if (x0.assigned()) {
          if (!check_equate_x(x0, ConstDashedView(by, 1)))
            return ES_FAILED;
        }
        else
          GECODE_ME_CHECK(x0.equate(home, ConstDashedView(by, 1)));
      }
      GECODE_ME_CHECK(refine_card(home));
      a = x0.assigned() + x1.assigned() + x2.assigned();
      switch (a) {
      case 3:
        return home.ES_SUBSUMED(*this);
      case 2:
        if ((x1.assigned() && x1.val() == 0) || 
            (x0.max_length() == 0 && x2.max_length() == 0))
          return ES_FIX;
        break;
      }
    } while (a == 2);
    std::cerr << "After pow: " << x2 << " = " << x0 << " ^(" << x1 << ")\n";
    return ES_FIX;
  }

}}}
