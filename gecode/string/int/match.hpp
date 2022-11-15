namespace Gecode { namespace String {

  forceinline
  Match::Match(Home home, StringView x, Gecode::Int::IntView i, stringDFA* R, 
    stringDFA* R1, int r)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>
    (home, x, i), sRs(R), Rs(R1), minR(r)  {}

  forceinline ExecStatus
  Match::post(Home home, StringView x, string re, Gecode::Int::IntView i) {    
    String::RegEx* regex = RegExParser(".*(" + re + ").*").parse();
    if (regex->has_empty()) {
      rel(home, i, IRT_EQ, 1);
      return ES_OK;
    }
    stringDFA* R = new stringDFA(regex->dfa());
    assert (R->final(1) && R->final_lst == 1);
    assert (R->neighbours(1) == NSIntSet(1));
    // BFS to find minimal-length word accepted by R.
    int r = 0, n = R->n_states;
    std::vector<int> dist(n);    
    for (int i = 0; i < n; ++i)
      dist[i] = DashedString::_MAX_STR_LENGTH;        
    std::list<int> s;
    s.push_back(0);
    dist[0] = 0;
    while (!s.empty()) {
      int q = s.front();
      s.pop_front();
      if (R->final(q)) {
        r = dist[q];
        break;
      }
      NSIntSet nq = R->neighbours(q);
      for (NSIntSet::iterator it(nq); it(); ++it) {
        int a = dist[q] + 1, x = *it;
        if (a < dist[x]) {
          dist[x] = a;
          s.push_back(x);
        }
      }
    }
    assert (r > 0);
    if (i.gr(home, x.max_length() - r + 1) == Int::ME_INT_FAILED)
      return ES_FAILED;    
    stringDFA* R1 = new stringDFA(RegExParser(re + ".*").parse()->dfa());
    (void) new (home) Match(home, x, i, R, R1, r);
    return ES_OK;
  }

  forceinline
  Match::Match(Space& home, Match& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> 
    (home, p), sRs(p.sRs), Rs(p.Rs), minR(p.minR) {}

  forceinline Actor*
  Match::copy(Space& home) {
    return new (home) Match(home, *this);
  }

  forceinline NSBlocks
  Match::prefix(int idx, int off) const {
    NSBlocks pref;
    DashedString& px = *x0.pdomain();
    for (int i = 0; i < idx; ++i)
      pref.push_back(NSBlock(px.at(i)));
    if (off > 0) {
      const DSBlock& b = px.at(idx);
      pref.push_back(NSBlock(b.S, off < b.l ? off : b.l, off));
    }
    return pref;
  }
  
  forceinline NSBlocks
  Match::suffix(int idx, int off) const {
    NSBlocks suff;
    DashedString& px = *x0.pdomain();
    if (off < px.at(idx).u) {
      const DSBlock& b = px.at(idx);
      suff.push_back(NSBlock(b.S, max(0, b.l - off), b.u - off));
    }
    for (int i = idx + 1; i < px.length(); ++i)
      suff.push_back(NSBlock(px.at(i)));
    return suff;
  }

  forceinline std::pair<NSIntSet,int>
  Match::checkBlock(const DSBlock& b, const NSIntSet& Q) const {
    int l = b.l, j = 0;
    NSIntSet Q_prev = Q;
    // Mandatory region.
    for (int i = 0; i < l; ++i) {
      NSIntSet Qi;
      for (NSIntSet::iterator it(Q_prev); it(); ++it)
        Qi.include(sRs->neighbours(*it, b.S));
      if (Qi.max() == 0)
        // All paths lead to q0.
        j = i;
      else if (Qi.in(1))
        // Final state q1 belongs to Qi.
        return std::make_pair(Qi, j);
      if (Qi == Q_prev)
        // Fixpoint.
        return std::make_pair(Qi, Qi.max() == 0 ? l : j);
      Q_prev = Qi;
    }
    // BFS over optional region.
    int dist[sRs->n_states];
    for (int q = 0; q < sRs->n_states; ++q)
      dist[q] = Q_prev.contains(q) ? l : DashedString::_MAX_STR_LENGTH + 1;
    std::list<int> U;
    for (NSIntSet::iterator i(Q_prev); i(); ++i)
      U.push_back(*i);    
    while (!U.empty()) {
      int q = U.front(), d = dist[q] + 1;
      U.pop_front();
      if (d <= b.u) {
        NSIntSet Nq = sRs->neighbours(q, b.S);
        if (Nq.in(1))
          // Final state in the neighbourhood of q.
          return std::make_pair(Nq, j);
        for (NSIntSet::iterator j(Nq); j(); ++j) {
          int q1 = *j;
          if (dist[q1] > d) {
            U.push_back(q1);
            dist[q1] = d;
          }
        }
      }
    }
    NSIntSet Qf;
    for (int q = 0; q < sRs->n_states; ++q)
      if (dist[q] <= DashedString::_MAX_STR_LENGTH)
        Qf.add(q);
    return std::make_pair(Qf, j);
  }

  forceinline ExecStatus
  Match::propagate(Space& home, const ModEventDelta& med) {
    //FIXME: To handle the case where x0 is fixed, we need to keep track of the 
    // original regex and take into account C++ regex syntax/semantics.
    do {
      DashedString& px = *x0.pdomain();
      if (x1.assigned()) {
        int k = x1.val();
        sRs->negate(x0.may_chars());
        if (k == 0)
          GECODE_REWRITE(*this, Reg::post(home, x0, sRs));
        // Rewrite into x = yz, |y| = k-1, ¬regular(y, sRs), regular(z, Rs)
        StringVar y(home), z(home);
        rel(home, y, z, STRT_CAT, x0);
        k--;
        length(home, y, IntVar(home, k, k));      
        Reg::post(home, y, sRs);
        Reg::post(home, z, Rs);
        return home.ES_SUBSUMED(*this);
      }
      int i = 0, j = 0, k = 0, h = 0, n = px.length();
      NSIntSet Q(0);
      while (i < n && !Q.in(1)) {
        //FIXME: Positions are implemented as 0-based, not 1-based.
        std::pair<NSIntSet,int> p = checkBlock(px.at(i), Q);
        Q = p.first;
        j = p.second;
        if (Q.size() == 1 && Q.max() == 0) {
          h = i;
          k = j;
        }
        ++i;
      }
      if (!Q.in(1))
        // No match.
        return me_failed(x1.eq(home,0)) ? ES_FAILED : ES_OK;
      if (Q.size() == 1 && Q.max() == 1) {
        // Surely a match: possibly refining lower and upper bound of x1.
        GECODE_ME_CHECK(x1.gq(home, 1));
        if (i < n) {
          int u;
          for (int j = 0, u = -minR + 1; j < i; ++j)
            u += px.at(j).u;
          GECODE_ME_CHECK(x1.lq(home, u));
        }
      }
      // FIXME: Check positions/offsets!!!
      int l, m = x1.min();
      for (int j = 0, l = k + 1; j < h && l <= m; ++j)
        l += px.at(j).l;
      if (l > m)
        l = m;
      if (Q.in(0)) {
        // Can't refine x1.
        if (l > 1) {
          IntSet s(1, l-1);
          IntSetRanges is(s);
          GECODE_ME_CHECK(x1.minus_r(home, is));
        }
        return ES_OK;
      }
      // General case:
      class RegProp : public Reg {
      public:
        RegProp(Home home, StringView x, stringDFA* d) : Reg(home, x, d) {};
      };
      StringVar x_pref(home), x_suff(home);
      double x_dim = px.logdim();
      int maxl = x0.max_length();
      NSBlocks pref, suff = suffix(h, k);
      if (l > x1.min()) {
        // Lower bound improved.
        GECODE_ME_CHECK(x1.gq(home, l));
        if (x1.size() == 1)
          continue;
      }
      else if (l > x1.min()) {
        // Updating (h,k) from the lower bound of x1.
        h = 0, k = x1.min() - 1;
        while (h < n && k >= px.at(h).u) {
          h++;
          k -= px.at(h).u;
        }
        NSBlocks pref = prefix(h, k);
        stringDFA* CsRs = new stringDFA(*sRs);
        CsRs->negate(x0.may_chars());
        RegProp p_pref(home, x_pref, CsRs);
        p_pref.propagate(home, med);
      }
      else
        pref = prefix(h, k);
      RegProp p_suff(home, x_suff, Rs);
      p_suff.propagate(home, med);        
      if (x0.max_length() < maxl) {
        int r = x0.max_length() - minR + 1;
        GECODE_ME_CHECK(x1.lq(home, r));
      }
      if (px.logdim() < x_dim) {
        // Udpating x0.
        NSBlocks x_new;
        pref.concat(suff, x_new);
        if (pref.back() == suff.front())
          x_new.normalize();
        StringDelta d(true);
        px.update(home, x_new);
        GECODE_ME_CHECK(x0.varimp()->notify(
          home, x0.assigned() ? ME_STRING_VAL : ME_STRING_DOM, d)
        );
      }
      assert (px.is_normalized());
    } while (x0.assigned() || x1.assigned());
    return ES_FIX;
  }
  
  forceinline
  Match::~Match() {
    delete sRs;
    delete Rs;
  }

}}
