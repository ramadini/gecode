namespace Gecode { namespace String {

  forceinline
  Match::Match(Home home, StringView x, Gecode::Int::IntView i, trimDFA* R, 
    trimDFA* R1, int r)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM>
    (home, x, i), Rs(R1), sRs(R), sRsC(nullptr), minR(r) {
    home.notice(*this, AP_DISPOSE);
  }

  forceinline ExecStatus
  Match::post(Home home, StringView x, string re, Gecode::Int::IntView i) {  
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    if (regex->has_empty()) {
      rel(home, i, IRT_EQ, 1);
      return ES_OK;
    }
    GECODE_ME_CHECK(x.lb(home, 1));
    TrimDFAHandle R(new trimDFA(regex->dfa()));
    assert (R->accepting(1) && R->accepting_states().size() == 1);
    assert (R->neighbours(1) == NSIntSet(1));
    int r = R->min_word_length();
    GECODE_ME_CHECK(i.gq(home, 0));
    GECODE_ME_CHECK(i.lq(home, x.max_length() - r + 1));
    if (!i.in(0))
      GECODE_ME_CHECK(x.lb(home, r));
//    std::cerr << "RE: " << re << ", minlen: " << r << ", i: " << i << '\n';
    TrimDFAHandle R1(
      new trimDFA(RegExParser("(" + re + ").*").parse()->dfa()));
    (void) new (home) Match(home, x, i, R.get(), R1.get(), r);
    return ES_OK;
  }

  forceinline
  Match::Match(Space& home, Match& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM> 
    (home, p), Rs(p.Rs), sRs(p.sRs), sRsC(p.sRsC), minR(p.minR) {}

  forceinline Actor*
  Match::copy(Space& home) {
    return new (home) Match(home, *this);
  }

  forceinline size_t
  Match::dispose(Space& home) {
    home.ignore(*this, AP_DISPOSE);
    Rs.~TrimDFAHandle();
    sRs.~TrimDFAHandle();
    sRsC.~CompDFAHandle();
    (void) MixBinaryPropagator
      <StringView, PC_STRING_DOM, Gecode::Int::IntView,
       Gecode::Int::PC_INT_DOM>::dispose(home);
    return sizeof(*this);
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
        sRs->include_neighbours(Qi, *it, b.S);
      if (Qi.in(1) || Qi == Q_prev)
        // Final state q1 belongs to Qi, or fixpoint reached.
        return Qi;
      Q_prev = Qi;
    }
    // BFS over optional region.
    Region region;
    int* dist = region.alloc<int>(sRs->n_states);
    for (int q = 0; q < sRs->n_states; ++q)
      dist[q] = Q_prev.contains(q) ? l : DashedString::_MAX_STR_LENGTH + 1;
    std::vector<int> U;
    U.reserve(sRs->n_states);
    for (NSIntSet::iterator i(Q_prev); i(); ++i)
      U.push_back(*i);
    NSIntSet Qf(Q_prev);
    for (unsigned int head = 0; head < U.size(); ++head) {
      int q = U[head], d = dist[q] + 1;
      if (d <= b.u) {
        bool accepting_neighbour = false;
        sRs->visit_neighbours(q, b.S, [&](int q1) {
          if (dist[q1] > d) {
            U.push_back(q1);
            Qf.add(q1);
            dist[q1] = d;
          }
          if (q1 == 1)
            accepting_neighbour = true;
        });
        if (accepting_neighbour)
          // Final state in the neighbourhood of q.
          break;
      }
    }
    return Qf;
  }
  
  template <typename DFA_t>
  forceinline int
  Match::checkReg(Space& home, const NSBlocks& x, DFA_t* d) const {
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
          u += x0.domain().at(k).u;
//        std::cerr << "x0: "<< x0 << ", x:" << x << ", i: " << i << ", u: " << u << ", minR: " << minR << "\n";
        return u;
      }
    }
    return -1;
  }

  forceinline int
  Match::must_idx(void) const {
    const DashedString& px = x0.domain();
    NSIntSet Q(0);
    for (int i = 0; i < px.length(); ++i) {
//      std::cerr << i << ", Q before:" << Q.toString() << "\n";
      Q = sRs->reach_all(px.at(i), Q);
//      std::cerr << i << ", Q after:" << Q.toString() << "\n";
      if (Q.empty())
        return -1;
      if (Q.size() == 1 && Q.in(1))
        return i;
    }
    return -1;
  };

  forceinline ExecStatus
  Match::propagate(Space& home, const ModEventDelta&) {
//    std::cerr << "\nMatch::propagate: Var " << x1.varimp() << ": " << x1 << " = Match " << x0 << " in\n " << *sRs << "\n";    
    GECODE_ME_CHECK(x1.lq(home, std::max(0, x0.max_length() - minR + 1)));
    do {
      // x1 fixed and val(x1) in {0,1}.
      if (x1.assigned() && x1.val() <= 1) {
        if (x1.val() == 0) {
          BoolVar b(home, 0, 0);
          CompDFAHandle dfa(new compDFA(*sRs, x0.may_chars()));
          GECODE_REWRITE(*this, (ReReg<Gecode::Int::BoolView,RM_EQV>::post(home, x0, dfa.get(), b)));
        }
        else {
          TrimDFAHandle dfa(Rs);
          GECODE_REWRITE(*this, Reg::post(home, x0, dfa.get()));
        }
      }
      const DashedString& px = x0.domain();
      
      std::string w = px.known_pref();
//      std::cerr << "w: " << w << '\n';
      int z = w.size();
      if (z > 0) {
        // A match starting at the first position is already final: extending
        // the string cannot introduce an earlier match. Matches at later
        // positions are final only when the complete string is known, since
        // an extension can still complete an earlier match.
        if (Rs->accepted(w, 0)) {
          GECODE_ME_CHECK(x1.eq(home, 1));
          return home.ES_SUBSUMED(*this);
        }
        if (px.known())  {
          for (int i = 1; i < z; ++i)
            if (Rs->accepted(w, i)) {
//              std::cerr << "\nMatch::propagated: i = " << i+1 << '\n';
              GECODE_ME_CHECK(x1.eq(home, i+1));
              return home.ES_SUBSUMED(*this);
            }
          GECODE_ME_CHECK(x1.eq(home, 0));
          return home.ES_SUBSUMED(*this);            
        }  
      }
      
      NSIntSet Q(0);
      int i = 0, n = px.length();
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
        NSBlocks x_suff = px.suffix(i, 0);
        if (x_suff[0].l > 1)
          x_suff[0].l = 1;
//        std::cerr << i << ": " << x_suff << "\n";
        int u = checkReg(home, x_suff, Rs.get());
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
        pref = px.prefix(h, k);
//        std::cerr << "Pref: " << pref << "\n"; 
        if (!sRsC) {
          sRsC = new compDFA(*sRs, x0.may_chars());
          sRsC->negate();
        }
        es_pref = Reg::propagate_blocks(home, pref, sRsC.get());
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
        pref = px.prefix(h, k);
//        std::cerr << "Pref: " << pref << "\n";
      suff = px.suffix(h, k);
//      std::cerr << "Suff: " << suff << "\n";
      int es_suff = x1.assigned()
        ? Reg::propagate_blocks(home, suff, Rs.get())
        : Reg::propagate_blocks(home, suff, sRs.get());
      if (es_suff == ES_FAILED)
        return ES_FAILED;
      if (es_pref == ES_NOFIX || es_suff == ES_NOFIX) {
        // Udpating x0.
        NSBlocks x_new;
        pref.concat(suff, x_new);
        x_new.normalize();
//        std::cerr << "x_new: " << x_new << "\n";
        GECODE_ME_CHECK(x0.varimp()->refine(home, x_new));
      }
      GECODE_ME_CHECK(x1.lq(home, std::max(0, x0.max_length() - minR + 1)));
      assert (px.is_normalized());
    } while (x1.assigned() && x1.val() <= 1);
//      std::cerr << "\nMatch::propagated: " << x1 << " = Match " << x0 << "\n";
    return ES_FIX;
  }
  
}}
