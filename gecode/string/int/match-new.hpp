namespace Gecode { namespace String {

  forceinline
  MatchNew::MatchNew(Home home, StringView x, Gecode::Int::IntView i, int r, 
             stringDFA* Rp, stringDFA* Rf, matchNFA* Rn)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM>
    (home, x, i), minR(r), Rpref(Rp), Rfull(Rf), Rcomp(nullptr), Rnfa(Rn) {}

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
    matchNFA* Rn = new matchNFA(Rp->toMatchNFA());
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

  
  forceinline ExecStatus
  MatchNew::propagateReg(Space& home, NSBlocks& x, stringDFA* d) {
//    std::cerr << "\npropagateReg: "<<x<<" in "<<*d<<std::endl;
    // Returns ES_FAILED, ES_FIX (no changes) or ES_NOFIX (x changed).
    if (x.known())
      return d->accepted(x.val()) ? ES_FIX : ES_FAILED;    
    bool changed, nofix = false;
    do {
      d->compute_univ(x.may_chars());
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
  MatchNew::propagate(Space& home, const ModEventDelta& med) {
//    std::cerr << "\nMatchNew::propagate: Var " << x1.varimp() << ": " << x1 << " = MatchNew " << x0 << " in " << *sRs << "\n";
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
      DashedString& px = *x0.pdomain();
      std::string w = px.known_pref();
      int k = w.size();
      if (k > 0) {
        if (Rfull->accepted(w)) {
          for (int i = 0; i < k; ++i)
            if (Rpref->accepted(w.substr(i))) {
              GECODE_ME_CHECK(x1.eq(home, i+1));
              return home.ES_SUBSUMED(*this);            
            }
        }
        // i = 0 \/ i > k.
        IntSet s(1, k);
        IntSetRanges is(s);
        GECODE_ME_CHECK(x1.minus_r(home, is));
      }
      // lb = min(I - {0})
      int lb = x1.min();
      if (lb == 0) {
        Gecode::IntVarValues r(x1);
        ++r;
        assert (r());
        lb = r.val();
      }
      
      //TODO
      // check_refine
      // ...
      
    } while (x1.assigned() && x1.val() <= 1);
//   std::cerr << "\nMatchNew::propagated: " << x1 << " = MatchNew " << x0 << "\n";
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
