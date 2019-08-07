namespace Gecode { namespace String {

  forceinline
  Find::Find(Home home, StringView x, StringView y, Gecode::Int::IntView n)
  : MixTernaryPropagator<StringView, PC_STRING_DOM, 
    StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>
    (home, x, y, n) {}

  forceinline ExecStatus
  Find::post(Home home, StringView x, StringView y, Gecode::Int::IntView n) {
    if (x.same(y))
      rel(home, n, IRT_EQ, 1);
    else
      (void) new (home) Find(home, x, y, n);
    return ES_OK;
  }

  forceinline
  Find::Find(Space& home, Find& p)
  : MixTernaryPropagator<StringView, PC_STRING_DOM, 
    StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> 
    (home, p) {}

  forceinline Actor*
  Find::copy(Space& home) {
    return new (home) Find(home, *this);
  }

  forceinline ExecStatus
  Find::propagate(Space& home, const ModEventDelta&) {
    // std::cerr << "\nFind::propagate: " << x2 << " = find " << x0 << " in " << x1 << "\n";
    int l = x2.min(), u = x2.max();
    // Adjusting x2 upper bound.
    if (u > x1.max_length()) {
      u = x1.max_length();
      GECODE_ME_CHECK(x2.lq(home, u));
    }
    if (x0.min_length() > x1.max_length()) {
      GECODE_ME_CHECK(x2.eq(home, 0));
      return home.ES_SUBSUMED(*this);
    }
    // The substring x0 is known: we checking if x0 definitely occurs in x1, by 
    // possibly updating x2.
    if (x0.assigned()) {
      string s = x0.val();
      if (s == "") {
        GECODE_ME_CHECK(x2.eq(home, 1));
        return home.ES_SUBSUMED(*this);
      }
      if (x1.assigned()) {
        int i = x1.val().find(s) + 1;
        if (i < l || i > u)
          return ES_FAILED;
        GECODE_ME_CHECK(x2.eq(home, i));
        return home.ES_SUBSUMED(*this);
      }
      DashedString* p1 = x1.pdomain();
      string pref = p1->known_pref();
      if (pref.size() > 0) {
        int i = pref.find(s) + 1;
        if (i > 0) {
          if (i < l || i > u)
            return ES_FAILED;
          GECODE_ME_CHECK(x2.eq(home, i));
          return home.ES_SUBSUMED(*this);
        }
      }
      int n = p1->max_length();
      if (n < l)
        return ES_FAILED;
      string curr = "";
      Position start({0, 0});
      // Checking fixed components.
      for (int i = 0; n > 0 && i < p1->length(); ++i) {
        const DSBlock& b = p1->at(i);
        if (b.S.size() == 1) {
          char c = b.S.min();
          string t(min(b.l, n), c);
          curr += t;
          int k = curr.find(s);
          if (k != (int) string::npos) {
            GECODE_ME_CHECK(x2.gq(home, 1));
            if (l == 0)
              l = 1;
            int ub = start.off + k + 1;
            for (int j = 0; j < start.idx && ub < u; ++j)
              ub += p1->at(j).u;
            GECODE_ME_CHECK(x2.lq(home, ub));
            if (u > ub)
              u = ub;
            break;
          }
          if (b.u > b.l) {
            curr = t;
            start = Position({i, b.u - b.l});
          }
        }
        else {
          curr = "";
          start = Position({i, b.u});
        }
        n -= b.u;
      }
    }
    bool mod = (l > 0);
    // x1 = "".
    if (x1.max_length() == 0) {
      if (mod) {
        GECODE_ME_CHECK(x2.eq(home, 1));
        GECODE_ME_CHECK(x0.eq(home, ""));
        return home.ES_SUBSUMED(*this);
      }
      else {
        if (u == 0) {
          rel(home, x0, STRT_NQ, StringVar(home, ""));
          return home.ES_SUBSUMED(*this);
        }
        GECODE_ME_CHECK(x2.lq(home, 1));
        u = 1;
      }
    }
    // x2 = 0.
    if (l == u && u == 0) {
      if (x0.assigned() && x0.val().size() == 1) {
        // Removing a single character from all the bases.
        int c = char2int(x0.val()[0]);
        DashedString* pdom = x1.pdomain();
        bool norm = false;
        for (int i = 0; i < pdom->length(); ++i) {
          DSBlock& b = pdom->at(i);
          if (b.S.contains(c)) {
            b.S.remove(home, c);
            if (b.l > 0 && b.S.empty())
              return ES_FAILED;
            norm |= b.S.empty() || (i > 0 && pdom->at(i-1).S == b.S) || 
                    (i < pdom->length()-1 && pdom->at(i+1).S == b.S);
          }
        }
        if (norm)
          pdom->normalize(home);
        assert (pdom->is_normalized());
        return home.ES_SUBSUMED(*this);
      }
      return ES_FIX;
    }
    if (mod) {
      NSIntSet ychars = x1.may_chars();
      int n = x0.pdomain()->length();
      NSBlocks v;
      if (u > 1)
        v.push_back(NSBlock(ychars, l - 1, u - 1));
      for (int i = 0; i < n; ++i)
        v.push_back(NSBlock(x0.pdomain()->at(i)));
      int j = x1.max_length() - x0.min_length() - l + 1;
      if (j > 0) {
        int i = max(0, x1.min_length() - x0.max_length() - u + 1);
        v.push_back(NSBlock(ychars, i, j));
      }
      v.normalize();
      GECODE_ME_CHECK(x1.dom(home, v));
    }
    else {
      // l = min(D(x2) - {0}).
      Gecode::Int::ViewValues<Gecode::Int::IntView> i(x2);
      ++i;
      l = i.val();
    }
    // General case.
    GECODE_ME_CHECK(x1.find(home, x0, l, u, mod));
    // std::cerr << "After find: " << x0 << " in " << x1 << " (" << l << ", " << u << ")\n";
    GECODE_ME_CHECK(x2.lq(home, u));
    if (mod) {
      // Can modify x and y.
      GECODE_ME_CHECK(x2.gq(home, l));
      if (x1.assigned()) {
        if (x0.assigned()) {
          int n = (int) x1.val().find(x0.val()) + 1;
          GECODE_ME_CHECK(x2.eq(home, n));
          return home.ES_SUBSUMED(*this);
        }
        if (x2.assigned() && x0.min_length() == x0.max_length()) {
          int n = x2.val(), m = x0.min_length();
          assert (n > 0);
          GECODE_ME_CHECK(x0.eq(home, x1.val().substr(n, m)));
          return home.ES_SUBSUMED(*this); 
        }
      }
    }
    else {
      // Can't modify x and y.
      if (u == 0) {
        GECODE_ME_CHECK(x2.eq(home, 0));
        // std::cerr << "After find: x2 " << x2 << "\n";
        return home.ES_SUBSUMED(*this);
      }
      if (l > 1) {
        IntSet s(1, l - 1);
        IntSetRanges is(s);
        GECODE_ME_CHECK(x2.minus_r(home, is));
      }
    }
    // std::cerr << "index: " << x2 << "\n";
    assert (x0.pdomain()->is_normalized() && x1.pdomain()->is_normalized());
    return ES_FIX;
  }

}}
