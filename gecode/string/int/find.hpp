#include <gecode/int.hh>

namespace Gecode { namespace String { namespace Int {

  // x2 is the index of the first occurrence of x1 in x0.

  template <class View0, class View1>
  forceinline
  Find<View0,View1>::Find(Home home, View0 x, View1 y, Gecode::Int::IntView n)
  : MixTernaryPropagator<View0, PC_STRING_CARD, View1, PC_STRING_CARD, 
    Gecode::Int::IntView, Gecode::Int::PC_INT_BND>(home, x, y, n) {}

  template <class View0, class View1>
  forceinline ExecStatus
  Find<View0,View1>::post(Home home, View0 x, View1 y, Gecode::Int::IntView n) {
    if (x.same(y))
      rel(home, n, IRT_EQ, 1);
    else {
      GECODE_ME_CHECK(n.gq(home,0));
      GECODE_ME_CHECK(n.lq(home, std::max(0, x.max_length()-y.min_length()+1)));
      (void) new (home) Find(home, x, y, n);
    }
    return ES_OK;
  }

  template <class View0, class View1>
  forceinline
  Find<View0,View1>::Find(Space& home, Find& p)
  : MixTernaryPropagator<View0, PC_STRING_CARD, 
    View1, PC_STRING_CARD, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> 
    (home, p) {}

  template <class View0, class View1>
  forceinline Actor*
  Find<View0,View1>::copy(Space& home) {
    return new (home) Find(home, *this);
  }

  template <class View0, class View1>
  forceinline ExecStatus
  Find<View0,View1>::propagate(Space& home, const ModEventDelta&) {
    int lx = x0.min_length(), ly = x1.min_length(),
        ux = x0.max_length(), uy = x1.max_length();
    if (lx > uy) {
      GECODE_ME_CHECK(x2.eq(home, 0));
      return home.ES_SUBSUMED(*this);
    }
    if (ux < x2.min())
      return ES_FAILED;
    GECODE_ME_CHECK(x2.lq(home, std::max(0, ux-ly+1)));
    int ln = x2.min(), un = x2.max();
    // The query string is known: we check if x0 definitely occurs in x1, and
    // possibly update the index variable.
    if (x1.assigned()) {
//      std::vector<int> w = x0.val();
      if (uy == 0) {
        GECODE_ME_CHECK(x2.eq(home, 1));
        return home.ES_SUBSUMED(*this);
      }
      if (x0.assigned()) {
        int i = 0; //FIXME: no need to retrieve the values, int i = find_fixed(x0,x1);
        if (i < ln || i > un)
          return ES_FAILED;
        GECODE_ME_CHECK(x2.eq(home, i));
        return home.ES_SUBSUMED(*this);
      }
      int n = x0.size();
      std::vector<int> pref = x0.fixed_pref(Position(0,0), Position(n,0), n);
      n = pref.size();
      if (n > 0 && n >= uy) {
        int i = 0; //FIXME: find_fixed(ConstStringView(pref,pref.size()), x[1]);
        if (i > 0) {
          GECODE_ME_CHECK(x2.eq(home, i));
          return home.ES_SUBSUMED(*this);
        }
      }
      // FIXME: Check fixed components in an auxiliary function.
//      string curr = "";
//      Position start(0,0);
//      for (int i = 0; n > 0 && i < p1->length(); ++i) {
//        const DSBlock& b = p1->at(i);
//        if (b.S.size() == 1) {
//          char c = b.S.min();
//          string t(min(b.l, n), c);
//          curr += t;
//          int k = curr.find(s);
//          if (k != (int) string::npos) {
//            GECODE_ME_CHECK(x2.gq(home, 1));
//            if (l == 0)
//              l = 1;
//            int ub = start.off + k + 1;
//            for (int j = 0; j < start.idx && ub < u; ++j)
//              ub += p1->at(j).u;
//            GECODE_ME_CHECK(x2.lq(home, ub));
//            if (u > ub)
//              u = ub;
//            break;
//          }
//          if (b.u > b.l) {
//            curr = t;
//            start = Position({i, b.u - b.l});
//          }
//        }
//        else {
//          curr = "";
//          start = Position({i, b.u});
//        }
//        n -= b.u;
//      }
    }
    bool mod = (ln > 0);
    // Target string empty.
    if (ux == 0) {
      if (mod) {
        GECODE_ME_CHECK(x2.eq(home, 1));
        GECODE_ME_CHECK(x0.nullify(home));
        return home.ES_SUBSUMED(*this);
      }
      else {
        if (un == 0) {
          nq(home, x1, std::vector<int>());
          return home.ES_SUBSUMED(*this);
        }
        GECODE_ME_CHECK(x2.lq(home, 1));
        un = 1;
      }
    }
    // The query string does not occur.
    if (ln == un && un == 0) {
      if (x0.assigned() && x0.val().size() == 1) {
        // TODO:Removing a single character from all the bases.
//        int c = char2int(x0.val()[0]);
//        DashedString* pdom = x1.pdomain();
//        bool norm = false;
//        for (int i = 0; i < pdom->length(); ++i) {
//          DSBlock& b = pdom->at(i);
//          if (b.S.contains(c)) {
//            b.S.remove(home, c);
//            if (b.l > 0 && b.S.empty())
//              return ES_FAILED;
//            norm |= b.S.empty() || (i > 0 && pdom->at(i-1).S == b.S) || 
//                    (i < pdom->length()-1 && pdom->at(i+1).S == b.S);
//          }
//        }
//        if (norm)
//          pdom->normalize(home);
//        assert (pdom->is_normalized());
//        if (x1.assigned() && x1.val().find(x0.val()))
//          return ES_FAILED;
//        return home.ES_SUBSUMED(*this);
      }
      return ES_FIX;
    }
    if (mod) {
//TODO with a ConstDashedView
//      NSIntSet ychars = x1.may_chars();
//      int n = x0.pdomain()->length();
//      NSBlocks v;
//      if (u > 1)
//        v.push_back(NSBlock(ychars, l - 1, u - 1));
//      for (int i = 0; i < n; ++i)
//        v.push_back(NSBlock(x0.pdomain()->at(i)));
//      int j = x1.max_length() - x0.min_length() - l + 1;
//      if (j > 0) {
//        int i = max(0, x1.min_length() - x0.max_length() - u + 1);
//        v.push_back(NSBlock(ychars, i, j));
//      }
//      v.normalize();
//      GECODE_ME_CHECK(x1.dom(home, v));
    }
    else {
      // l = min(D(x2) - {0}).
      Gecode::Int::ViewValues<Gecode::Int::IntView> i(x2);
      ++i;
      ln = i.val();
    }
    // General case.
//    GECODE_ME_CHECK(x1.find(home, x0, l, u, mod));
//    // std::cerr << "After find: " << x0 << " in " << x1 << " (" << l << ", " << u << ")\n";
    GECODE_ME_CHECK(x2.lq(home, un));
    if (mod) {
      // Can modify x and y.
      GECODE_ME_CHECK(x2.gq(home, ln));
      if (x0.assigned()) {
        if (x1.assigned()) {
          int n = 0; // FIXME: (int) x1.val().find(x0.val()) + 1;
          GECODE_ME_CHECK(x2.eq(home, n));
          return home.ES_SUBSUMED(*this);
        }
        if (x2.assigned() && x1.min_length() == x1.max_length()) {
          int n = x2.val(), m = x0.min_length();
          assert (n > 0);
//FIXME:  GECODE_ME_CHECK(x0.eq(home, x1.val().substr(n-1, m)));
          return home.ES_SUBSUMED(*this); 
        }
      }
    }
    else {
      // Can't modify neither x nor y.
      if (un == 0)
        GECODE_ME_CHECK(x2.eq(home, 0));
      if (ln > 1) {
        IntSet s(1, ln - 1);
        IntSetRanges is(s);
        GECODE_ME_CHECK(x2.minus_r(home, is));
      }
    }
    return ES_FIX;
  }

}}}
