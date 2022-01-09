#include <gecode/int.hh>
#include <gecode/string/var-imp/ds-find.hpp>

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
      GECODE_ME_CHECK(n.lq(home, std::max(0, 
                                 std::min(x.max_length(), 
                                          x.max_length()-y.min_length()+1))));
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
  Find<View0,View1>::refine_card(Space& home) {    
    // |x0| >= |x1| + x2 - 1.
    int k = x1.min_length() + x2.min() - 1;    
    if (x0.min_length() < k)
      GECODE_ME_CHECK(x0.min_length(home, k));
    // |x1| <= |x0| - x2 + 1.
    k = x0.max_length() - x2.min() + 1;
    if (x1.max_length() > k)
      GECODE_ME_CHECK(x1.max_length(home, k));
    // x2 <= |x0| - |x1| + 1;
    k = x0.max_length() - x1.min_length() + 1;
    if (x2.max() > k)
      GECODE_ME_CHECK(x2.lq(home, k));
    return ES_OK;
  }

  template <class View0, class View1>
  forceinline ExecStatus
  Find<View0,View1>::propagate(Space& home, const ModEventDelta&) {
//    std::cerr <<"\n"<< this << "::Find::propagate "<<x0<<".find( "<<x1<<" ) = "<<x2<<" \n";
    int l1 = x1.min_length(), u0 = x0.max_length(), u1 = x1.max_length();
    if (u0 < l1) {
      GECODE_ME_CHECK(x2.eq(home, 0));
      return home.ES_SUBSUMED(*this);
    }
    if (u0 < x2.min())
      return ES_FAILED;
    GECODE_ME_CHECK(x2.lq(home, std::max(0, std::min(u0, u0-l1+1))));
    if (x2.min() > 0)
      refine_card(home);
    int ln = x2.min(), un = x2.max();
    
    // The query string x1 is known: we check if x0 definitely occurs in x1, and
    // possibly update the index variable.
    if (x1.assigned()) {
      if (u1 == 0) {
        GECODE_ME_CHECK(x2.eq(home, 1));
        return home.ES_SUBSUMED(*this);
      }
      if (x0.assigned()) {
      x0_x1_fixed:
        int i = find_fixed(x0,x1);        
        if (i < ln || i > un)
          return ES_FAILED;
        GECODE_ME_CHECK(x2.eq(home, i));
//        std::cerr << this << " subsumed: "<<x0<<".find( "<<x1<<" ) = "<<x2<<"\n\n";
        return home.ES_SUBSUMED(*this);
      }
      int n = x0.size();
      // Checking fixed prefix.
      std::vector<int> vp = x0.fixed_pref(Position(0,0), Position(n,0), n);
//      std::cerr << "Pref: " << vec2str(vp) << "\n";
      n = vp.size();
      assert (n % 2 == 0);
      if (n/2 >= u1) {
        Region r;
        Block* pref = r.alloc<Block>(n/2);
        for (int i=0, j=0; i < n-1; i += 2, ++j)
          pref[j].update(home, Block(vp[i], vp[i+1]));
        int i = find_fixed(ConstDashedView(*pref,n/2), x1);
//        std::cerr << ConstDashedView(*pref,n/2) << ' ' << i << '\n';
        if (i > 0) {
          GECODE_ME_CHECK(x2.eq(home, i));
          return home.ES_SUBSUMED(*this);
        }
      }
      // Checking if x1 occurs in the fixed components of x0.
      GECODE_ME_CHECK(fixed_comp(home, x0, x1, x2));
//      std::cerr << "After fixed_comp: " << x2 << '\n';
      if (ln < x2.min())
        ln = x2.min();
      if (un > x2.max())
        un = x2.max();
    }
    
    // occ is true iff x1 must occur in x0.
    bool occ = (ln > 0);
    // Target string x0 empty.
    if (u0 == 0) {
      if (occ) {
        GECODE_ME_CHECK(x2.eq(home, 1));
        GECODE_ME_CHECK(x1.nullify(home));
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
    
    // The query string x1 does not occur.
    if (ln == un && un == 0) {
      if (x1.assigned() && x1.max_length() == 1) {
        // Removing a single character from all the bases of x0.
        bool norm = false;
        int c = x1.val()[0];
        for (int i = 0; i < x0.size(); ++i) {
          const Block& x_i = x0[i];
          if (x_i.baseSize() == 1 && x_i.baseMin() == c) {
            if (x_i.lb() > 0)
              return ES_FAILED;
            x0.nullifyAt(home, i);
            norm = true;
          }
          else if (x_i.baseContains(c)) {
            x0.baseRemoveAt(home, i, c);
            norm |= (i > 0 && x0[i-1].baseEquals(x_i)) ||
                    (i < x0.size()-1 && x0[i+1].baseEquals(x_i));
          }
        }
        if (norm)
          x0.normalize(home);
//        std::cerr << this << "propagated "<<x0<<".find( "<<x1<<" ) = "<<x2<<"\n";
        return home.ES_SUBSUMED(*this);
      }
      return ES_OK;
    }
    
    // x0, x2 fixed and x1 with fixed length.
    if (x0.assigned() && x2.assigned() && x1.min_length() == x1.max_length()) {
    x0_x2_fixed:
      std::vector<int> w0 = x0.val();
      ConstStringView w1(home, (&w0[0])+x2.val()-1, x1.max_length());
      if (check_equate_x(x1, w1))
        x1.gets(home, w1);
      else
        return ES_FAILED;
//      std::cerr << this << "propagated "<<x0<<".find( "<<x1<<" ) = "<<x2<<"\n";
      return home.ES_SUBSUMED(*this);
    }
    
    // General case.
    if (occ) {
      Set::GLBndSet s;
      for (int i = 0; i < x0.size(); ++i)
        x0[i].includeBaseIn(home, s);
      CharSet S(home, s);
      int bp = (un > 1), j = x0.max_length()-x1.min_length()-ln+1, bs = (j > 0);
      Region r;
      int n = bp + x1.size() + bs;
      Block* dom = r.alloc<Block>(n);
      int k = 0;
      if (bp)
        dom[k++].update(home, Block(home, S, ln-1, un-1));
      for (int i = 0; i < x1.size(); ++i)
        dom[k++].update(home, x1[i]);      
      if (bs) {
        int i = std::max(0,x0.min_length()-x1.max_length()-un+1);
        dom[k++].update(home, Block(home, S, i, j));
      }
      ConstDashedView d(*dom,k);
//      std::cerr << "d: " << d << '\n';
      if (x0.assigned()) {
        if (!check_equate_x(x0,d))
          return ES_FAILED;
      }
      else
        GECODE_ME_CHECK(x0.equate(home,d));
//      std::cerr << "x0: " << x0 << '\n';
      if (x0.assigned()) {
        if (x1.assigned())
          goto x0_x1_fixed;
        if (x2.assigned() && x1.min_length() == x1.max_length())
          goto x0_x2_fixed;
      }
    }
    else {
      // ln = min(D(x2) \ {0}).
      Gecode::Int::ViewValues<Gecode::Int::IntView> i(x2);
      ++i;
      ln = i.val();
    }    
    int ll = ln;
//    std::cerr<<"Before: "<<x0<<".find( "<<x1<<" ) = "<<ln<<".."<<un<<" ("<<occ<<") \n";
    GECODE_ME_CHECK(x0.find(home, x1, ln, un, occ));
//    std::cerr<<"After: "<<x0<<".find( "<<x1<<" ) = "<<ln<<".."<<un<<" ("<<occ<<") \n";
    GECODE_ME_CHECK(x2.lq(home, un));
    if (occ) {
      // Can modify x and y.
      GECODE_ME_CHECK(x2.gq(home, ln));
      refine_card(home);
      if (x0.assigned()) {
        if (x1.assigned())
          goto x0_x1_fixed;
        if (x2.assigned() && x1.min_length() == x1.max_length())
          goto x0_x2_fixed;
      }
    }
    else {
      // Can't modify neither x0 nor x1.
      if (un == 0)
        GECODE_ME_CHECK(x2.eq(home, 0));
      if (ln > ll) {
        IntSet s(ll,ln-1);
        IntSetRanges is(s);
        GECODE_ME_CHECK(x2.minus_r(home, is));
      }
    }
//    std::cerr << this << "::propagated "<<x0<<".find( "<<x1<<" ) = "<<x2<<"\n";
    return ES_OK;
  }

}}}
