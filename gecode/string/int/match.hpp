namespace Gecode { namespace String {

  forceinline
  Match::Match(Home home, StringView x, Gecode::Int::IntView i, trimDFA* R, 
    trimDFA* R1, int r)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM>
    (home, x, i), sRsC(nullptr), sRs(R), Rs(R1), minR(r) {}

  forceinline ExecStatus
  Match::post(Home home, StringView x, string re, Gecode::Int::IntView i) {  
    String::RegEx* regex = RegExParser(".*(" + re + ").*").parse();
    if (regex->has_empty()) {
      rel(home, i, IRT_EQ, 1);
      return ES_OK;
    }
    GECODE_ME_CHECK(x.lb(home, 1));
    trimDFA* R = new trimDFA(regex->dfa());
    assert (R->accepting(1) && R->accepting_states().size() == 1);
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
      if (R->accepting(q)) {
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
    GECODE_ME_CHECK(i.gq(home, 0));
    GECODE_ME_CHECK(i.lq(home, x.max_length() - r + 1));
    if (!i.in(0))
      GECODE_ME_CHECK(x.lb(home, r));
//    std::cerr << "RE: " << re << ", minlen: " << r << ", i: " << i << '\n';
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    (void) new (home) Match(home, x, i, R, R1, r);
    return ES_OK;
  }

  forceinline
  Match::Match(Space& home, Match& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM> 
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

  forceinline NSIntSet
  Match::checkBlock(const DSBlock& b, const NSIntSet& Q_in) const {
//    std::cerr << "checkBlock " << b << '\n';
    int l = b.l;
    NSIntSet Q_prev = Q_in;
    // Mandatory region.
    for (int i = 0; i < l; ++i) {
      NSIntSet Qi;
      for (NSIntSet::iterator it(Q_prev); it(); ++it)
        Qi.include(sRs->neighbours(*it, b.S));
      if (Qi.in(1) || Qi == Q_prev)
        // Final state q1 belongs to Qi, or fixpoint reached.
        return Qi;
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
        for (NSIntSet::iterator j(Nq); j(); ++j) {
          int q1 = *j;
          if (dist[q1] > d) {
            U.push_back(q1);
            dist[q1] = d;
          }
        }
        if (Nq.in(1))
          // Final state in the neighbourhood of q.
          break;
      }
    }
    NSIntSet Qf;
    for (int q = 0; q < sRs->n_states; ++q)
      if (dist[q] <= DashedString::_MAX_STR_LENGTH)
        Qf.add(q);
    return Qf;
  }
  
  template <typename DFA_t>
  forceinline ExecStatus
  Match::propagateReg(Space& home, NSBlocks& x, DFA_t* d) {
//    std::cerr << "\npropagateReg: "<<x<<" in "<<*d<<std::endl;
    // Returns ES_FAILED, ES_FIX (no changes) or ES_NOFIX (x changed).
    if (x.known())
      return d->accepted(x.val()) ? ES_FIX : ES_FAILED;    
    bool changed, nofix = false;
    do {
      int n = x.length();
      std::vector<std::vector<NSIntSet>> F(n);
      NSIntSet Fi(0);
      for (int i = 0; i < n; ++i) {
        F[i] = Reg::reach_fwd(d, Fi, DSBlock(home,x[i]));
        if (F[i].empty())
          return ES_FAILED;
        Fi = F[i].back();
      }
      NSIntSet E(F.back().back());
      NSBlocks y[n];
      Fi = d->accepting_states();
      E.intersect(Fi);
      if (E.empty())
        return ES_FAILED;
      changed = false;
      for (int i = n - 1; i >= 0; --i)
        y[i] = Reg::reach_bwd(d, F[i], E, DSBlock(home,x[i]), changed);
      if (changed) {
        nofix = true;
        NSBlocks z;
        for (auto& yi : y) {
          for (auto& yij: yi) {
            if (yij.null())
              continue;
            if (!z.empty() && z.back().S == yij.S) {
              z.back().l += yij.l;
              z.back().u += yij.u;
            }
            else
              z.push_back(yij);
          }
        }
        x = z.empty() ? NSBlocks() : z;
//        std::cerr << "changed: " << x << '\n';
        assert (x.is_normalized());
      }
    } while (changed);
    return nofix ? ES_NOFIX : ES_FIX;
  }
  
  template <typename DFA_t>
  forceinline int
  Match::checkReg(Space& home, int i, const NSBlocks& x, DFA_t* d) const {
//    std::cerr << "\ncheckReg: "<<x<<" in "<<*d<<std::endl;
    NSIntSet Fj(0);
    for (int j = 0; j < x.length(); ++j) {
      std::vector<NSIntSet> F = Reg::reach_fwd(d, Fj, DSBlock(home,x[j]));
      if (F.empty())
        return -1;
//      for (auto x : F) std::cerr<<x.toString()<<'\n';
      Fj = F.back();
//      std::cerr << "After x[" << j << "] = " << x[j] << ": " << Fj.toString() << "\n";
      if (Fj.in(1)) {
        if (Fj.size() > 1)
          return 0;
        int idx = must_idx();
        if (idx == -1)
          return 0;
        int u = 0;
        for (int k = 0; k <= idx; ++k)
          u += x0.pdomain()->at(k).u;
//        std::cerr << "x0: "<< x0 << ", x:" << x << ", i: " << i << ", u: " << u << ", minR: " << minR << "\n";
        return u;
      }
    }
    return -1;
  }

  forceinline NSIntSet
  Match::reachMust(const DSBlock& b, const NSIntSet& Q_in) const {
//    std::cerr << "reachMust " << b << ' '<< Q_in.toString() << ", Rfull: " << *Rfull << '\n';
    int l = b.l, j = 0;
    NSIntSet Q_prev = Q_in;
    // Mandatory region.
    for (int i = 0; i < l; ++i) {
      NSIntSet Qi;
      for (NSIntSet::iterator it(Q_prev); it(); ++it) {
        NSIntSet Q = sRs->neighbot(*it, b.S);
        if (Q.empty())
          return Q;
        Qi.include(Q);
      }
//      std::cerr << "Qi after block " << b << ": " << Qi.toString() << '\n';
      if ((Qi.size() == 1 && Qi.in(1)) || Qi == Q_prev)
        // Fixpoint.
        return Qi;
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
        NSIntSet Nq = sRs->neighbot(q, b.S);
        if (Nq.empty())
          return Nq;
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
//    std::cerr << "Qf: " << Qf.toString() << '\n';
    return Qf;
  }

  forceinline int
  Match::must_idx(void) const {
    DashedString& px = *x0.pdomain();
    NSIntSet Q(0);
    for (int i = 0; i < px.length(); ++i) {
//      std::cerr << i << ", Q before:" << Q.toString() << "\n";
      Q = reachMust(px.at(i), Q);
//      std::cerr << i << ", Q after:" << Q.toString() << "\n";
      if (Q.empty())
        return -1;
      if (Q.size() == 1 && Q.in(1))
        return i;
    }
    return -1;
  };

  forceinline ExecStatus
  Match::propagate(Space& home, const ModEventDelta& med) {
//    std::cerr << "\nMatch::propagate: Var " << x1.varimp() << ": " << x1 << " = Match " << x0 << " in\n " << *sRs << "\n";    
    GECODE_ME_CHECK(x1.lq(home, std::max(0, x0.max_length() - minR + 1)));
    do {
      // x1 fixed and val(x1) in {0,1}.
      if (x1.assigned() && x1.val() <= 1) {
        if (x1.val() == 0) {
          BoolVar b(home, 0, 0);
          GECODE_REWRITE(*this, (ReReg<Gecode::Int::BoolView,RM_EQV>::post(home, x0, new compDFA(*sRs, x0.may_chars()), b)));
        }
        else
          GECODE_REWRITE(*this, Reg::post(home, x0, Rs));
      }
      DashedString& px = *x0.pdomain();
      
      std::string w = px.known_pref();
//      std::cerr << "w: " << w << '\n';
      int z = w.size();
      if (z > 0) {
        if (sRs->accepted(w)) {
          for (int i = 0; i < z; ++i)
            if (Rs->accepted(w.substr(i))) {
//              std::cerr << "\nMatch::propagated: i = " << i+1 << '\n';
              GECODE_ME_CHECK(x1.eq(home, i+1));
              return home.ES_SUBSUMED(*this);            
            }
        }
        if (px.known())  {
          GECODE_ME_CHECK(x1.eq(home, 0));
          return home.ES_SUBSUMED(*this);            
        }  
      }
      
      NSIntSet Q(0);
      int i = 0, j = 0, n = px.length();
      while (i < n && !Q.in(1))
        //FIXME: Positions are 0-based, not 1-based.
        Q = checkBlock(px.at(i++), Q);
//      std::cerr << "Q = " << Q.toString() << "\n";
      
      // No match.
      if (!Q.in(1)) 
        return me_failed(x1.eq(home,0)) ? ES_FAILED : ES_FIX;
        
      // Surely a match: possibly refining lower and upper bound of x1.
      if (Q.size() == 1 && Q.in(1)) {
        GECODE_ME_CHECK(x1.gq(home, 1));
        if (i < n) {
          int u = -minR + 1;
          for (int j = 0; j < i; ++j)
            u += px.at(j).u;
//          std::cerr << u << '\n';
          GECODE_ME_CHECK(x1.lq(home, u));
        }
      }
      
      // Compute l as the leftmost position for a match.
      int l = 1, k = 0, h = n;
//      std::cerr << *Rs << '\n';
      for (int i = 0; i < n; ++i) {
        NSBlocks x_suff = suffix(i, 0);
        if (x_suff[0].l > 1)
          x_suff[0].l = 1;
//        std::cerr << i << ": " << x_suff << "\n";
        int u = checkReg(home, i, x_suff, Rs);
        if (u >= 0) {
          h = i;
          k = 0;
//          std::cerr << u << '\n';
          if (u > 0)
            GECODE_ME_CHECK(x1.lq(home, u));
//          std::cerr << "[" << x0.varimp() << "] updated l and (h,k)\n";
          break;
        }
        else
          l += px.at(i).l;
      }
      assert (h < n && k == 0);
      
//      std::cerr << "l: " << l << ", h: " << h << "\n";
      bool updatedI = false;
      if (l > 1 && l > x1.min()) {
        IntSet s(1, l-1);
        IntSetRanges is(s);  
        GECODE_ME_CHECK(x1.minus_r(home, is));
//        std::cerr << "Refined i = " << x1 << '\n';
        updatedI = true;
      }
//      std::cerr << "i: " << x1 << "\n";
      // Can't refine x1.
      if (x1.in(0)) {
//        std::cerr << "\nMatch::propagated: " << x1 << ' ' << x0 << "\n";
        return ES_FIX;
      }      
      // General case.
      NSBlocks pref, suff;
      int es_pref = ES_FIX;
      if (updatedI || l < x1.min()) {
        // Updating (h,k) from the lower bound of x1.
        if (!updatedI) {
          h = 0, k = x1.min() - 1;
          while (h < n && k >= px.at(h).u) {      
            k -= px.at(h).u;
            h++;
          }
        }
//        std::cerr << "Updated (h,k) = ("<<h<<", "<<k<<") from position " << x1.min() << "\n";
        pref = prefix(h, k);
//        std::cerr << "Pref: " << pref << "\n"; 
        if (sRsC == nullptr) {
          sRsC = new compDFA(*sRs, x0.may_chars());
          sRsC->negate();
        }
        es_pref = propagateReg(home, pref, sRsC);
//        std::cerr << "Rfull: " << *sRs << '\n';
//        std::cerr << "Rcomp: " << *sRsC << '\n';
        if (es_pref == ES_FAILED)
          return ES_FAILED; 
      }
      // We don't propagate the suffix if l >= x1.min() and x1 is not fixed.
      else if (!x1.assigned()) {
//        std::cerr << "\nMatch::propagated: " << x1 << " = Match " << x0 << "\n";
        return ES_FIX;
      }
      else if (x1.val() <= 1)
        continue;
        
      if (pref.size() == 0)
        pref = prefix(h, k);
//        std::cerr << "Pref: " << pref << "\n";
      suff = suffix(h, k);
//      std::cerr << "Suff: " << suff << "\n";
      int es_suff = x1.assigned() ? propagateReg(home, suff, Rs) 
                                  : propagateReg(home, suff, sRs);
      if (es_suff == ES_FAILED)
        return ES_FAILED;
      if (es_pref == ES_NOFIX || es_suff == ES_NOFIX) {
        // Udpating x0.
        NSBlocks x_new;
        pref.concat(suff, x_new);
        x_new.normalize();
//        std::cerr << "x_new: " << x_new << "\n";
        StringDelta d(true);
        px.update(home, x_new);
        GECODE_ME_CHECK(x0.varimp()->notify(
          home, x0.assigned() ? ME_STRING_VAL : ME_STRING_DOM, d
        ));
      }
      GECODE_ME_CHECK(x1.lq(home, std::max(0, x0.max_length() - minR + 1)));
      assert (px.is_normalized());
    } while (x1.assigned() && x1.val() <= 1);
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
