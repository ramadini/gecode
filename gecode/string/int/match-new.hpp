namespace Gecode { namespace String {

  forceinline
  MatchNew::MatchNew(Home home, StringView x, Gecode::Int::IntView i, int r, 
             stringDFA* Rp, stringDFA* Rf, matchNFA* Rn)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM>
    (home, x, i), minR(r), Rpref(Rp), Rfull(Rf), Rcomp(nullptr), Rnfa(Rn) {
    NSIntSet may_chars = x.may_chars();
    Rcomp = new stringDFA(*Rfull);
    Rcomp->negate(may_chars);         
  }

  forceinline ExecStatus
  MatchNew::post(Home home, StringView x, string re, Gecode::Int::IntView i) {  
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
    GECODE_ME_CHECK(i.gr(home, -1));
    GECODE_ME_CHECK(i.le(home, x.max_length() - r));
    GECODE_ME_CHECK(x.lb(home, r));
//    std::cerr << "RE: " << re << ", minlen: " << r << ", i: " << i << '\n';
    stringDFA* Rp = new stringDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* Rn = new matchNFA(Rp->toMatchNFA(x.may_chars()));
    (void) new (home) MatchNew(home, x, i, r, Rp, R, Rn);    
    return ES_OK;
  }

  forceinline
  MatchNew::MatchNew(Space& home, MatchNew& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM> 
    (home, p), minR(p.minR), Rpref(p.Rpref), Rfull(p.Rfull), Rcomp(p.Rcomp),
                                                             Rnfa(p.Rnfa) {}

  forceinline Actor*
  MatchNew::copy(Space& home) {
    return new (home) MatchNew(home, *this);
  }

  forceinline NSBlocks
  MatchNew::prefix(int idx, int off) const {
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
  MatchNew::suffix(int idx, int off) const {
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
  
  forceinline std::vector<NSIntSet>
  MatchNew::reachFwd(const DSBlock& b, const NSIntSet& F) const {
    int l = b.l;
    std::vector<NSIntSet> Q(l + 2);
    Q[0] = F;
    // Mandatory region.
    for (int i = 0; i < l; ++i) {
      NSIntSet qi;
      for (NSIntSet::iterator it(Q[i]); it(); ++it) {
        NSIntSet qj = Rnfa->neighbours(*it, b.S);
        qi.include(qj);
      }
      assert (!qi.empty());
      if ((qi.size() == 1 && qi.min() == qi.max()) || qi == Q[i]) {        
        for (int j = i + 1; j <= l + 1; ++j)
          Q[j] = qi;
        return Q;
      }
      Q[i + 1] = qi;
    }
    Q[l + 1] = Q[l];
    int dist[Rnfa->n_states];
    for (int q = 0; q < Rnfa->n_states; ++q)
      dist[q] = Q[l].contains(q) ? l : DashedString::_MAX_STR_LENGTH;
    std::list<int> Q_bfs;
    for (NSIntSet::iterator i(Q[l]); i(); ++i)
      Q_bfs.push_back(*i);
    // BFS over optional region.
    while (!Q_bfs.empty()) {
      int q = Q_bfs.front(), d = dist[q];
      Q_bfs.pop_front();
      if (d < DashedString::_MAX_STR_LENGTH)
        ++d;
      if (d <= b.u) {
        NSIntSet nq;
        nq = Rnfa->neighbours(q, b.S);
        for (NSIntSet::iterator j(nq); j(); ++j) {
          int q1 = *j;
          if (dist[q1] > d) {
            Q_bfs.push_back(q1);
            Q[l + 1].include(q1);
            dist[q1] = d;
          }
        }
      }
    }
    return Q;
  }
  
  forceinline void
  MatchNew::reachBwd(int i, NSIntSet& B, const std::vector<NSIntSet>& Q,
                     int& j, int& k) const {
//    std::cerr<<"\nMatchNew::reachBwd: X["<<i<<"] = "<<x0.pdomain()->at(i)<<", B=" <<B.toString()<<'\n';
    const DSBlock& b = x0.pdomain()->at(i);
    int l = b.l;
    std::vector<NSIntSet> delta_bwd(Rnfa->n_states);
    for (int q = 0; q < Rnfa->n_states; ++q)
      for (auto& x : Rnfa->delta[q])
        if (b.S.in(x.first))
          if (x.second < 0) {
            delta_bwd[-x.second].add(q);
            delta_bwd[Rnfa->bot].add(q);
          }
          else
            delta_bwd[x.second].add(q);
//    for (int q = 0; q < Rnfa->n_states; ++q) std::cerr << "\tdelta_bwd[q" << q << "] = " << delta_bwd[q].toString() << "\n";
    int dist[Rnfa->n_states];
    for (int q = 0; q < Rnfa->n_states; ++q)
      dist[q] = B.contains(q) ? 0 : DashedString::_MAX_STR_LENGTH;
    NSIntSet Q1(B);
    std::list<int> Q_bfs;
    for (NSIntSet::iterator i(Q1); i(); ++i)
      Q_bfs.push_back(*i);
    // Optional region
    while (!Q_bfs.empty()) {
      int q = Q_bfs.front(), d = dist[q];
      Q_bfs.pop_front();
      if (d < DashedString::_MAX_STR_LENGTH)
        ++d;
      if (d <= b.u - b.l) {
        for (NSIntSet::iterator it(delta_bwd[q]); it(); ++it) {
          int q1 = *it;          
          if (dist[q1] > d && Q[l + 1].contains(q1)) {
//            std::cerr << "q = "<<q<<", q'= " << q1 << ", dist = "<<dist[q1]<<" > d = "<<d<<", Q[l+1] = " << Q[l+1].toString() << "\n";
            Q_bfs.push_back(q1);
            if (Q[l].in(q1))
              B.include(q1);
            dist[q1] = d;
            if (q1 == Rnfa->bot && k == 0)
              k = b.u - d + 1;
          }
        }
      }
    }    
    B.intersect(Q[l]);
//    std::cerr<<"After opt. B = "<< B.toString() << ", k = " << k << ".\n------\n";
    if (B.size() == 1 && B.in(Rnfa->bot)) {
      j = l+1;
      return;
    }
    // Mandatory region
    for (int i = l; i > 0; --i) {
//      std::cerr<<"i = "<<i<<"\n";
      NSIntSet B1;
      for (NSIntSet::iterator it(B); it(); ++it) {
        int q = *it;
//        std::cerr << "\tq = "<<q<<", delta_bwd[q]=" << delta_bwd[q].toString() << "\n";
        for (NSIntSet::iterator it(delta_bwd[q]); it(); ++it) {
          int q1 = *it;
          if (Q[i - 1].contains(q1)) {
//            std::cerr << "\tq'= " << q1 << ", Q["<<i-1<<"] = " << Q[i-1].toString() << "\n";
            B1.add(q1);
            if (k == 0 && q1 == Rnfa->bot)
              k = i;
          }
        }        
      }
      if (j == 0 && B1.size() == 1 && B1.in(Rnfa->bot))
        j = i;
//      std::cerr << "\tB' = " << B1.toString() << ", j = " << j << ", k = " << k << "\n";
      B = B1;
//      std::cerr << "B = " << B.toString() << "\n";
    }
//    std::cerr<<"After mand. B = "<< B.toString() << ", j = " << j << ", k = " << k << "\n";
  }
  
  forceinline ExecStatus
  MatchNew::propagateReg(Space& home, NSBlocks& x, stringDFA* d) {
//    std::cerr << "\npropagateReg: "<<x<<" in "<<*d<<std::endl;
    // Returns ES_FAILED, ES_FIX (no changes) or ES_NOFIX (x changed).
    if (x.known())
      return d->accepted(x.val()) ? ES_FIX : ES_FAILED;    
    bool changed, nofix = false;
    do {
//      d->compute_univ(x.may_chars());
      int n = x.length();
      std::vector<std::vector<NSIntSet>> F(n);
      NSIntSet Fi(0);
      for (int i = 0; i < n; ++i) {
        F[i] = Reg::reach_fwd(d, Fi, DSBlock(home,x[i]));
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
        y[i] = Reg::reach_bwd(d, F[i], E, DSBlock(home,x[i]), changed);
        k += y[i].size();
      }
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

  forceinline ExecStatus
  MatchNew::refine_idx(Space& home, int& i_lb, int& j_lb) {
    DashedString& x = *x0.pdomain();
    std::vector<std::vector<NSIntSet>> F(x.length());
    NSIntSet Fi(0);
    int n = x.length();
    // Fwd pass.
    for (int i = 0; i < n; ++i) {
      F[i] = reachFwd(x.at(i), Fi);
      assert (!F[i].empty());
      Fi = F[i].back();
//      std::cerr << "last(F[" << i << "]): " << Fi.toString() << "\n";
      if (Fi.size() == 1 && Fi.in(1)) {
        GECODE_ME_CHECK(x1.gq(home, 1));
        n = i + 1;
        break;
      }
    }
    n--;
    assert (!F[n].back().empty());
    if (!F[n].back().in(1)) {
      GECODE_ME_CHECK(x1.eq(home, 0));
      return ES_OK;
    }
    // Bwd pass.
    NSIntSet B(1);
    i_lb = j_lb = 0;
    int i_ub = 0, j_ub = 0;
    for (int i = n, j = 0, k = 0; i >= 0; --i) {      
      reachBwd(i, B, F[i], j, k);
//      std::cerr << "Bwd pass after " << x.at(i) << ": last(F[" << i << "]) = " 
//        << DSIntSet(home, F[i].back()).toIntSet() << ", B = " 
//        << DSIntSet(home, B).toIntSet() << ", j = " << j << ", k = " << k << "\n";      
      if (j > 0) {
        i_lb = i;
        j_lb = j;
      }
      if (k > 0 && i_ub == 0 && j_ub == 0) {
        i_ub = i;
        j_ub = k;
      }
    }
//    std::cerr << "(i_lb,j_lb)=("<<i_lb<<","<<j_lb<<"), (i_ub,j_ub)=("<<i_ub<<","<<j_ub<<")\n";
    int u = j_ub;
    for (int i = 0; i < i_ub; ++i)
      u += x.at(i).u;
    GECODE_ME_CHECK(x1.lq(home, u));    
    return ES_OK;
  };
  
  forceinline NSIntSet
  MatchNew::reachFwdLazy(const DSBlock& b, const NSIntSet& Q_in) const {
//    std::cerr << "reachFwdLazy " << b << ' '<< Q_in.toString() << ", Rfull: " << *Rfull << '\n';
    int l = b.l, j = 0;
    NSIntSet Q_prev = Q_in;
    // Mandatory region.
    for (int i = 0; i < l; ++i) {
      NSIntSet Qi;
      for (NSIntSet::iterator it(Q_prev); it(); ++it)
        Qi.include(Rfull->neighbours(*it, b.S));
//      std::cerr << "Qi after block " << b << ": " << Qi.toString() << '\n';
      if (Qi.size() == 1 && Qi.in(1))
        return Qi;
      if (Qi == Q_prev)
        // Fixpoint.
        return Qi;
      Q_prev = Qi;
    }
    // BFS over optional region.
    int dist[Rfull->n_states];
    for (int q = 0; q < Rfull->n_states; ++q)
      dist[q] = Q_prev.contains(q) ? l : DashedString::_MAX_STR_LENGTH + 1;
    std::list<int> U;
    for (NSIntSet::iterator i(Q_prev); i(); ++i)
      U.push_back(*i);    
    while (!U.empty()) {
      int q = U.front(), d = dist[q] + 1;
      U.pop_front();
      if (d <= b.u) {
        NSIntSet Nq = Rfull->neighbours(q, b.S);
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
    for (int q = 0; q < Rfull->n_states; ++q)
      if (dist[q] <= DashedString::_MAX_STR_LENGTH)
        Qf.add(q);
//    std::cerr << "Qf: " << Qf.toString() << '\n';
    return Qf;
  }
  
  forceinline bool
  MatchNew::must_match(void) const {
    DashedString& px = *x0.pdomain();
    NSIntSet Q(0);
    for (int i = 0; i < px.length(); ++i) {
      Q = reachFwdLazy(px.at(i), Q);
      if (Q.size() == 1 && Q.in(1))
        return true;
    }
    return false;
  };

  forceinline ExecStatus
  MatchNew::propagate(Space& home, const ModEventDelta& med) {
//    std::cerr << "\nMatchNew::propagate: Var " << x1.varimp() << ": " << x1 << " = MatchNew " << x0 << " in " << *Rnfa << "\n";
    GECODE_ME_CHECK(x1.lq(home, x0.max_length() - minR + 1));
    do {
      // x1 fixed and val(x1) in {0,1}.
      if (x1.assigned() && x1.val() <= 1) {
        if (x1.val() == 0) {
          if (Rcomp == nullptr) {
            Rcomp = Rfull;
            NSIntSet may_chars = x0.may_chars();
            Rcomp->negate(may_chars);
          }
          GECODE_REWRITE(*this, Reg::post(home, x0, Rcomp));
        }
        GECODE_REWRITE(*this, Reg::post(home, x0, Rpref));
      }
      DashedString& X = *x0.pdomain();
      std::string w = X.known_pref();
//      std::cerr << "w: " << w << '\n';
      int k = w.size();
      if (k > 0) {
        if (Rfull->accepted(w)) {
          for (int i = 0; i < k; ++i)
            if (Rpref->accepted(w.substr(i))) {
              GECODE_ME_CHECK(x1.eq(home, i+1));
              return home.ES_SUBSUMED(*this);            
            }
        }
        if (X.known())  {
          GECODE_ME_CHECK(x1.eq(home, 0));
          return home.ES_SUBSUMED(*this);            
        }  
      }
      int h = 0;
      GECODE_ES_CHECK(refine_idx(home, h, k));
//      std::cerr << "After refine: " << x1 << ", " << x0 << ", (h,k)=" << "("<<h<<","<<k<<")\n";
      int l = k;
      for (int i = 0; i < h; ++i)
        l += X.at(i).l;
      if (l > 1 && l > x1.min()) {
        IntSet s(1, l-1);
        IntSetRanges is(s);  
        GECODE_ME_CHECK(x1.minus_r(home, is));
//        std::cerr << "Refined i = " << x1 << '\n';
      }
      if (x1.in(0)) {
        if (must_match()) {
          GECODE_ME_CHECK(x1.gq(home,1));
          l = 1;
        }
        else
          return ES_FIX;
      }
//      std::cerr << "x: " << x0 << ", l = " <<l<<' '<<x1<<"\n";
      
      // General case.
      NSBlocks pref, suff;
      int es_pref = ES_FIX;  
      if (l < x1.min()) {
        // Updating (h,k) from the lower bound of x1.
        h = 0, k = x1.min() - 1;
        while (h < X.length() && k >= X.at(h).u) {      
          k -= X.at(h).u;
          h++;
        }
//        std::cerr << "(h,k)=" << "("<<h<<","<<k<<")\n";
        pref = prefix(h, k);
//        std::cerr << "Pref: " << pref << "\n"; 
        if (Rcomp == nullptr) {
          NSIntSet may_chars = x0.may_chars();
          Rcomp = new stringDFA(*Rfull);
          Rcomp->negate(may_chars);
        }        
        es_pref = propagateReg(home, pref, Rcomp);
//        std::cerr << "Rpref: " << *Rpref << '\n';
//        std::cerr << "Rfull: " << *Rfull << '\n';
//        std::cerr << "Rcomp: " << *Rcomp << '\n';
//        std::cerr << "Pref: " << pref << "\n";
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
      else {
        // x1 fixed and greater than 1.
        h = 0, k = x1.min() - 1;
        while (h < X.length() && k >= X.at(h).u) {      
          k -= X.at(h).u;
          h++;
        }
        pref = prefix(h, k);
      }
//      std::cerr << "Pref: " << pref << "\n";
      suff = suffix(h, k);
//      std::cerr << "Suff: " << suff << ' ' << x1 << "\n";
      int es_suff = x1.assigned() ? propagateReg(home, suff, Rpref) 
                                  : ES_FIX;
      if (es_suff == ES_FAILED)
        return ES_FAILED;
//      std::cerr << "New suff: " << suff << "\n";
      if (es_pref == ES_NOFIX || es_suff == ES_NOFIX) {
        // Udpating x0.
        NSBlocks x_new;
        pref.concat(suff, x_new);
        x_new.normalize();
//        std::cerr << "x_new: " << x_new << "\n";
        StringDelta d(true);
        X.update(home, x_new);
        GECODE_ME_CHECK(x0.varimp()->notify(
          home, x0.assigned() ? ME_STRING_VAL : ME_STRING_DOM, d
        ));
      }
      GECODE_ME_CHECK(x1.lq(home, x0.max_length() - minR + 1));
      assert (X.is_normalized());
    } while (x1.assigned() && x1.val() <= 1);
//    std::cerr << "\nMatchNew::propagated: " << x1 << " = MatchNew " << x0 << "\n";
    return ES_FIX;
  }
  
  forceinline
  MatchNew::~MatchNew() {
    delete Rpref;
    delete Rfull;
    delete Rcomp;
    delete Rnfa;
  }

}}
