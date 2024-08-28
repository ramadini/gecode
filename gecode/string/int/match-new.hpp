namespace Gecode { namespace String {

  forceinline
  MatchNew::MatchNew(Home home, StringView x, Gecode::Int::IntView i, stringDFA* R, 
    stringDFA* R1, int r)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM>
    (home, x, i), sRsC(nullptr), sRs(R), Rs(R1), minR(r) {}

  forceinline ExecStatus
  MatchNew::post(Home home, StringView x, string re, Gecode::Int::IntView i) {  
    // TODO
    return ES_OK;
  }

  forceinline
  MatchNew::MatchNew(Space& home, MatchNew& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM> 
    (home, p), sRsC(p.sRsC), sRs(p.sRs), Rs(p.Rs), minR(p.minR) {}

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
        for (auto yi : y) {
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
    //TODO
//   std::cerr << "\nMatchNew::propagated: " << x1 << " = MatchNew " << x0 << "\n";
    return ES_FIX;
  }
  
  forceinline
  MatchNew::~MatchNew() {
    delete Rs;
    delete sRs;
    delete sRsC;
  }

}}
