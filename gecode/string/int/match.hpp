namespace Gecode { namespace String {

  forceinline
  Match::Match(Home home, StringView x, Gecode::Int::IntView i, stringDFA* R, 
    stringDFA* R1, int r)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>
    (home, x, i), sRsC(nullptr), sRs(R), Rs(R1), minR(r) {}

  forceinline ExecStatus
  Match::post(Home home, StringView x, string re, Gecode::Int::IntView i) {  
    String::RegEx* regex = RegExParser(".*(" + re + ").*").parse();
    if (regex->has_empty()) {
      rel(home, i, IRT_EQ, 1);
      return ES_OK;
    }
    GECODE_ME_CHECK(x.lb(home, 1));
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
        assert (q == 1);
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
//    std::cerr << "RE: " << re << ", minlen: " << r << '\n';
    GECODE_ME_CHECK(i.gr(home, -1));
    GECODE_ME_CHECK(i.le(home, x.max_length() - r + 1));
    GECODE_ME_CHECK(x.lb(home, r));
    stringDFA* R1 = new stringDFA(RegExParser("(" + re + ").*").parse()->dfa());
    (void) new (home) Match(home, x, i, R, R1, r);
    return ES_OK;
  }

  forceinline
  Match::Match(Space& home, Match& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> 
    (home, p), sRsC(p.sRsC), sRs(p.sRs), Rs(p.Rs), minR(p.minR) {}

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
  Match::propagateReg(Space& home, StringView& x, stringDFA* d) {
//    std::cerr << "\npropagateReg: "<<x<<" in "<<*d<<std::endl;
    if (x.assigned())
      return d->accepted(x.val()) ? ES_FIX : ES_FAILED;
    bool changed, nofix = false;
    do {
      d->compute_univ(x.may_chars());
      DashedString* px = x.pdomain();
      std::vector<std::vector<NSIntSet>> F(px->length());
      NSIntSet Fi(0);
      int n = px->length();
      for (int i = 0; i < n; ++i) {
        F[i] = Reg::reach_fwd(d, Fi, px->at(i));
        if (F[i].empty())
          return ES_FAILED;
        Fi = F[i].back();
        if (d->univ_rejected(Fi))
          return ES_FAILED;
      }
      NSIntSet E(F.back().back());
      NSBlocks y[n];
      Fi = NSIntSet(d->final_fst, d->final_lst);
      E.intersect(Fi);
      if (E.empty())
        return ES_FAILED;
      changed = false;
      int k = 0;
      for (int i = n - 1; i >= 0; --i) {
        y[i] = Reg::reach_bwd(d, F[i], E, px->at(i), changed);
        k += y[i].size();
      }
      if (changed) {
        nofix = true;
        NSBlocks z;
        for (auto yi : y)
          for (auto yij: yi) {
            if (yij.null())
              continue;
            if (!z.empty() && z.back().S == yij.S) {
              z.back().l += yij.l;
              z.back().u += yij.u;
            }
            else
              z.push_back(yij);
          }
        z.empty() ? px->set_null(home) : px->update(home, z);
//        std::cerr << "changed: " << x << '\n';
        assert (x.pdomain()->is_normalized());
      }
    } while (changed);
    return nofix ? ES_NOFIX : ES_FIX;
  }

  forceinline ExecStatus
  Match::propagate(Space& home, const ModEventDelta& med) {
//    std::cerr << "\nMatch::propagate: " << x1 << " = Match " << x0 << " in " << *sRs << "\n";
    GECODE_ME_CHECK(x1.lq(home, x0.max_length() - minR + 1));
    do {
      DashedString& px = *x0.pdomain();
      if (x0.assigned()) {
        string val = x0.val();
        for (int i = max(0,x1.min()-1); i < val.size(); i++)
          if (Rs->accepted(val.substr(i))) {
//            std::cerr << "Rewriting into i = " << i+1 << "\n";
            GECODE_ME_CHECK(x1.eq(home, i+1));
            return home.ES_SUBSUMED(*this);
          }
        GECODE_ME_CHECK(x1.eq(home, 0));
        return home.ES_SUBSUMED(*this);
      }
      if (x1.assigned() && x1.val() <= 1) {
        if (x1.val() == 0) {
          if (sRsC == nullptr) {
            sRsC = sRs;
            NSIntSet may_chars = x0.may_chars();
            sRsC->negate(may_chars);
          }
          GECODE_REWRITE(*this, Reg::post(home, x0, sRsC));
        }
        GECODE_REWRITE(*this, Reg::post(home, x0, Rs));
      }
      int i = 0, j = 0, k = 0, h = 0, n = px.length();
      NSIntSet Q(0);
      while (i < n && !Q.in(1)) {
        //FIXME: Positions are 0-based, not 1-based.
        std::pair<NSIntSet,int> p = checkBlock(px.at(i), Q);
        Q = p.first; j = p.second;
        if (Q.size() == 1 && Q.max() == 0) {
          // Q = {q0}.
          h = i; k = j;
        }
        ++i;
      }
//      std::cerr << "(h,k) = (" << h << "," << k << ")\n";
      if (!Q.in(1))
        // No match.
        return me_failed(x1.eq(home,0)) ? ES_FAILED : ES_FIX;
      if (Q.size() == 1 && Q.max() == 1) {
        // Surely a match: possibly refining lower and upper bound of x1.
        GECODE_ME_CHECK(x1.gq(home, 1));
        if (i < n) {
          int u = -minR + 1;
          for (int j = 0; j < i; ++j)
            u += px.at(j).u;
          GECODE_ME_CHECK(x1.lq(home, u));
        }
      }
      int l = k + 1, m = x1.min();
      for (int j = 0; j < h; ++j)
        l += px.at(j).l;
      assert (l > 0);
//      std::cerr << "l = " << l << '\n';
      if (x1.in(0)) {
        // Can't refine x1.
        if (l > 1) {
          IntSet s(1, l-1);
          IntSetRanges is(s);
          GECODE_ME_CHECK(x1.minus_r(home, is));
        }
  //      std::cerr << "\nMatch::propagated: " << x1 << ' ' << x0 << "\n";
        return ES_FIX;
      }
      // General case.
      StringView x_pref(home), x_suff(home);
      NSBlocks pref;
      int es_pref = ES_FIX;
      if (l > x1.min())
        // Lower bound improved.
        GECODE_ME_CHECK(x1.gq(home, l));
      else if (l < x1.min()) {
        // Updating (h,k) from the lower bound of x1.
        h = 0, k = x1.min() - 1;
        while (h < n && k >= px.at(h).u) {          
          k -= px.at(h).u;
          h++;
        }        
//        std::cerr << "Updated (h,k) = ("<<h<<", "<<k<<") from position " << x1.min() << "\n";
        NSBlocks pref = prefix(h, k);
//        std::cerr << "Pref: " << pref << "\n";        
        if (sRsC == nullptr) {
          sRsC = new stringDFA(*sRs);
          NSIntSet may_chars = x0.may_chars();
          sRsC->negate(may_chars);
        }
        x_pref.pdomain()->update(home, pref);
        es_pref = propagateReg(home, x_pref, sRsC);
        if (es_pref == ES_FAILED)
          return ES_FAILED;
        
      }
      if (x1.assigned()) {
        if (x1.val() <= 1)
          continue;
      }
      else
        // std::cerr << "\nMatch::propagated: " << x1 << " = Match " << x0 << "\n";
        return ES_FIX;
      pref = prefix(h, k);
      NSBlocks suff = suffix(h, k);
//      std::cerr << "Pref: " << pref << "\n";
//      std::cerr << "Suff: " << suff << "\n";
      x_suff.pdomain()->update(home, suff);
      int es_suff = propagateReg(home, x_suff, Rs);
      if (es_suff == ES_FAILED)
        return ES_FAILED;
      if (es_pref == ES_NOFIX || es_suff == ES_NOFIX) {
        // Udpating x0.
        NSBlocks x_new;
        (es_pref == ES_NOFIX ? NSBlocks(*x_pref.pdomain()) : pref).concat
        (es_suff == ES_NOFIX ? NSBlocks(*x_suff.pdomain()) : pref, x_new);
        x_new.normalize();
//        std::cerr << "x_new: " << x_new << "\n";
        StringDelta d(true);
        px.update(home, x_new);
        GECODE_ME_CHECK(x0.varimp()->notify(
          home, x0.assigned() ? ME_STRING_VAL : ME_STRING_DOM, d
        ));
      }
      GECODE_ME_CHECK(x1.lq(home, x0.max_length() - minR + 1));
      assert (px.is_normalized());
    } while (x0.assigned() || (x1.assigned() && x1.val() <= 1));
//      std::cerr << "\nMatch::propagated: " << x1 << " = Match " << x0 << "\n";
    return ES_FIX;
  }
  
  forceinline
  Match::~Match() {
    delete Rs;
    delete sRs;
    delete sRsC;
  }

}}
