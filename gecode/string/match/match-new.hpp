namespace Gecode { namespace String {
  
  template<class Char, class Traits>
  forceinline std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const matchNFA& d) {
    os << '[';
    for (int i = 0; i < d.n_states; i++) {
      for (auto& x : d.delta[i]){
        NSIntSet S = x.first;
        os << "(q" << i << ", " << S.toString();
        if (x.second < 0)
          os << ", {q" << -x.second << ", q" << d.q_bot << "}), ";
        else
          os << ", q" << x.second << "), ";
      }
    }
    return os << "], |Q| = " << d.n_states << ", q_bot: q" << d.q_bot << ", F: "
                             << d.accepting_states().toString();
  }

  forceinline
  matchNFA::matchNFA(const trimDFA& dfa, const NSIntSet& domain) : 
  compDFA(dfa, domain), reverse() {
    // Proper pattern: not empty, not containing empty string.
    assert (dfa.n_states > 0 && !dfa.accepted(""));
    if (q_bot < 0) {
      q_bot = n_states++;
      delta.emplace_back();
    }
    delta[q_bot].clear();
    delta[q_bot].reserve(delta[0].size());
    // q_0 preserves the earliest start and q_F records a completed match;
    // only ordinary destinations also need the alternative q_bot branch.
    for (const auto& transition : delta[0]) {
      int next = transition.second;
      if (next != q_bot && next > 1)
        next = -next;
      delta[q_bot].emplace_back(transition.first, next);
    }
    for (std::size_t i = 0; i < delta.size(); ++i) {
      if (i != static_cast<std::size_t>(q_bot)) {
        for (auto& transition : delta[i]) {
          int q = transition.second;
          if (q > 1 && q != q_bot)
            transition.second = -q;
        }
      }
    }
    reverse.resize(n_states);
    for (int source = 0; source < n_states; ++source) {
      for (std::size_t edge = 0; edge < delta[source].size(); ++edge) {
        int target = delta[source][edge].second;
        ReverseTransition transition(source, static_cast<int>(edge));
        if (target < 0) {
          reverse[-target].push_back(transition);
          reverse[q_bot].push_back(transition);
        } else {
          reverse[target].push_back(transition);
        }
      }
    }
  }

  forceinline void
  matchNFA::Predecessors::skip_disjoint(void) {
    while (current != finish &&
           characters.disjoint(
             nfa.delta[current->first][current->second].first))
      ++current;
  }

  forceinline
  matchNFA::Predecessors::Predecessors(const matchNFA& automaton, int state,
                                       const DSIntSet& allowed)
    : nfa(automaton), characters(allowed),
      current(automaton.reverse[state].begin()),
      finish(automaton.reverse[state].end()) {
    skip_disjoint();
  }

  forceinline bool
  matchNFA::Predecessors::operator ()(void) const {
    return current != finish;
  }

  forceinline void
  matchNFA::Predecessors::operator ++(void) {
    ++current;
    skip_disjoint();
  }

  forceinline int
  matchNFA::Predecessors::operator *(void) const {
    return current->first;
  }

  forceinline void
  matchNFA::include_neighbours(NSIntSet& states, int q,
                               const DSIntSet& characters) const {
    for (auto& x : delta[q]) {
      if (!characters.disjoint(x.first)) {
        if (x.second < 0) {
          states.add(-x.second);
          states.add(q_bot);
        }
        else
          states.add(x.second);
      }
    }
  }

  forceinline NSIntSet
  matchNFA::neighbours(int q, const DSIntSet& characters) const {
    NSIntSet states;
    include_neighbours(states, q, characters);
    return states;
  };

  forceinline
  MatchNew::MatchNew(Home home, StringView x, Gecode::Int::IntView i, int r, 
             trimDFA* Rp, trimDFA* Rf, matchNFA* Rn)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM>
    (home, x, i), Rpref(Rp), Rfull(Rf), Rcomp(nullptr), Rnfa(Rn), minR(r) {
    home.notice(*this, AP_DISPOSE);
  }

  forceinline ExecStatus
  MatchNew::post(Home home, StringView x, string re, Gecode::Int::IntView i) {  
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
    TrimDFAHandle Rp(
      new trimDFA(RegExParser("(" + re + ").*").parse()->dfa()));
    MatchNFAHandle Rn(new matchNFA(*Rp, x.may_chars()));
    (void) new (home) MatchNew(
      home, x, i, r, Rp.get(), R.get(), Rn.get());
    return ES_OK;
  }

  forceinline
  MatchNew::MatchNew(Space& home, MatchNew& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM> 
    (home, p), Rpref(p.Rpref), Rfull(p.Rfull), Rcomp(p.Rcomp),
    Rnfa(p.Rnfa), minR(p.minR) {}

  forceinline Actor*
  MatchNew::copy(Space& home) {
    return new (home) MatchNew(home, *this);
  }

  forceinline size_t
  MatchNew::dispose(Space& home) {
    home.ignore(*this, AP_DISPOSE);
    Rpref.~TrimDFAHandle();
    Rfull.~TrimDFAHandle();
    Rcomp.~CompDFAHandle();
    Rnfa.~MatchNFAHandle();
    (void) MixBinaryPropagator
      <StringView, PC_STRING_DOM, Gecode::Int::IntView,
       Gecode::Int::PC_INT_DOM>::dispose(home);
    return sizeof(*this);
  }

  forceinline compDFA*
  MatchNew::full_complement(void) {
    if (!Rcomp) {
      Rcomp = new compDFA(*Rfull, x0.may_chars());
      Rcomp->negate();
    }
    return Rcomp.get();
  }

  forceinline std::vector<NSIntSet>
  MatchNew::reachFwd(const DSBlock& b, const NSIntSet& F) const {
    int l = b.l;
    std::vector<NSIntSet> Q(l + 2);
    Q[0] = F;
    // Mandatory region.
    for (int i = 0; i < l; ++i) {
      NSIntSet qi;
      for (NSIntSet::iterator it(Q[i]); it(); ++it)
        Rnfa->include_neighbours(qi, *it, b.S);
      assert (!qi.empty());
      if ((qi.size() == 1 && qi.in(1)) || qi == Q[i]) {        
        for (int j = i + 1; j <= l + 1; ++j)
          Q[j] = qi;
        return Q;
      }
      Q[i + 1] = std::move(qi);
    }
    Q[l + 1] = Q[l];
    Region region;
    int* dist = region.alloc<int>(Rnfa->n_states);
    for (int q = 0; q < Rnfa->n_states; ++q)
      dist[q] = Q[l].contains(q) ? l : DashedString::_MAX_STR_LENGTH;
    std::vector<int> Q_bfs;
    Q_bfs.reserve(Rnfa->n_states);
    for (NSIntSet::iterator i(Q[l]); i(); ++i)
      Q_bfs.push_back(*i);
    // BFS over optional region.
    for (unsigned int head = 0; head < Q_bfs.size(); ++head) {
      int q = Q_bfs[head], d = dist[q];
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
    const DSBlock& b = x0.pdomain()->at(i);
    int l = b.l;
    Region region;
    int* dist = region.alloc<int>(Rnfa->n_states);
    for (int q = 0; q < Rnfa->n_states; ++q)
      dist[q] = B.contains(q) ? 0 : DashedString::_MAX_STR_LENGTH;
    std::vector<int> Q_bfs;
    Q_bfs.reserve(Rnfa->n_states);
    for (NSIntSet::iterator i(B); i(); ++i)
      Q_bfs.push_back(*i);
    // Optional region
    for (unsigned int head = 0; head < Q_bfs.size(); ++head) {
      int q = Q_bfs[head], d = dist[q];
      if (d < DashedString::_MAX_STR_LENGTH)
        ++d;
      if (d <= b.u - b.l) {
        for (matchNFA::Predecessors predecessor(*Rnfa, q, b.S);
             predecessor(); ++predecessor) {
          int q1 = *predecessor;
          if (dist[q1] > d && Q[l + 1].contains(q1)) {
            Q_bfs.push_back(q1);
            if (Q[l].in(q1))
              B.include(q1);
            dist[q1] = d;
            if (q1 == Rnfa->q_bot && k == 0)
              k = b.u - d + 1;
          }
        }
      }
    }    
    B.intersect(Q[l]);
    if (B.size() == 1 && B.in(Rnfa->q_bot)) {
      j = l+1;
      return;
    }
    // Mandatory region
    for (int i = l; i > 0; --i) {
      NSIntSet B1;
      for (NSIntSet::iterator it(B); it(); ++it) {
        int q = *it;
        for (matchNFA::Predecessors predecessor(*Rnfa, q, b.S);
             predecessor(); ++predecessor) {
          int q1 = *predecessor;
          if (Q[i - 1].contains(q1)) {
            B1.add(q1);
            if (k == 0 && q1 == Rnfa->q_bot)
              k = i;
          }
        }        
      }
      if (j == 0 && B1.size() == 1 && B1.in(Rnfa->q_bot))
        j = i;
      B = std::move(B1);
    }
  }
  
  forceinline ExecStatus
  MatchNew::refine_idx(Space& home, int& i_lb, int& j_lb) {
    DashedString& x = *x0.pdomain();
    std::vector<std::vector<NSIntSet>> F(x.length());
    NSIntSet initial(0);
    const NSIntSet* Fi = &initial;
    int n = x.length();
    // Fwd pass.
    for (int i = 0; i < n; ++i) {
      F[i] = reachFwd(x.at(i), *Fi);
      assert (!F[i].empty());
      Fi = &F[i].back();
      if (Fi->size() == 1 && Fi->in(1)) {
        GECODE_ME_CHECK(x1.gq(home, 1));
        n = i + 1;
        break;
      }
    }
    assert (!F[n-1].back().empty());
    if (!F[n-1].back().in(1)) {
      GECODE_ME_CHECK(x1.eq(home, 0));
      return ES_OK;
    }
    // Bwd pass.
    NSIntSet B(1);
    i_lb = j_lb = 0;
    int i_ub = n, j_ub = 0;
    for (int i = n-1, j = 0, k = 0; i >= 0; --i) {      
      reachBwd(i, B, F[i], j, k);
      assert (!B.empty());
      if (k > 0 && i_ub == n && j_ub == 0) {
        i_ub = i;
        j_ub = k;
      }
      if (j > 0) {
        i_lb = i;
        j_lb = j;
        assert (i_ub < n);
        break;
      }
    }
    int u = j_ub;
    for (int i = 0; i < i_ub; ++i)
      u += x.at(i).u;
    GECODE_ME_CHECK(x1.lq(home, u));    
    return ES_OK;
  };
  
  forceinline bool
  MatchNew::must_match(void) const {
    DashedString& px = *x0.pdomain();
    NSIntSet Q(0);
    for (int i = 0; i < px.length(); ++i) {
      Q = Rfull->reach_all(px.at(i), Q);
      if (Q.empty())
        return false;
      if (Q.size() == 1 && Q.in(1))
        return true;
    }
    return false;
  };

  forceinline ExecStatus
  MatchNew::propagate(Space& home, const ModEventDelta&) {
    GECODE_ME_CHECK(x1.lq(home, std::max(0, x0.max_length() - minR + 1)));
    do {
      // x1 fixed and val(x1) in {0,1}.
      if (x1.assigned() && x1.val() <= 1) {
        if (x1.val() == 0) {
          BoolVar b(home, 1, 1);
          CompDFAHandle dfa(full_complement());
          GECODE_REWRITE(*this, (ReReg<Gecode::Int::BoolView,RM_EQV>::post(home, x0, dfa.get(), b)));
        }
        else {
          TrimDFAHandle dfa(Rpref);
          GECODE_REWRITE(*this, Reg::post(home, x0, dfa.get()));
        }
      }
      DashedString& X = *x0.pdomain();
      std::string w = X.known_pref();
      int k = w.size();
      if (k > 0) {
        if (Rfull->accepted(w)) {
          for (int i = 0; i < k; ++i)
            if (Rpref->accepted(w, i)) {
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
      int l = max(1, k);
      for (int i = 0; i < h; ++i)
        l += X.at(i).l;
      bool updatedI = false;
      if (l > 1 && l > x1.min()) {
        IntSet s(1, l-1);
        IntSetRanges is(s);  
        GECODE_ME_CHECK(x1.minus_r(home, is));
        updatedI = true;
      }
      if (x1.in(0)) {
        if (must_match()) {
          GECODE_ME_CHECK(x1.gq(home,1));
          l = 1;
        }
        else
          return ES_FIX;
      }
      
      // General case.
      NSBlocks pref, suff;
      int es_pref = ES_FIX;  
      if (updatedI || l < x1.min()) {
        // Updating (h,k) from the lower bound of x1.
        h = 0, k = x1.min() - 1;
        while (h < X.length() && k >= X.at(h).u) {      
          k -= X.at(h).u;
          h++;
        }
        pref = X.prefix(h, k);
        es_pref = Reg::propagate_blocks(home, pref, full_complement());
        if (es_pref == ES_FAILED)
          return ES_FAILED;
      }
      // We don't propagate the suffix if l >= x1.min() and x1 is not fixed.
      else if (!x1.assigned())
        return ES_FIX;
      else if (x1.val() <= 1)
        continue;
      else {
        // x1 fixed and greater than 1.
        h = 0, k = x1.min() - 1;
        while (h < X.length() && k >= X.at(h).u) {      
          k -= X.at(h).u;
          h++;
        }
        pref = X.prefix(h, k);
      }
      suff = X.suffix(h, k);
      int es_suff = x1.assigned()
        ? Reg::propagate_blocks(home, suff, Rpref.get())
        : Reg::propagate_blocks(home, suff, Rfull.get());
      if (es_suff == ES_FAILED)
        return ES_FAILED;
      if (es_pref == ES_NOFIX || es_suff == ES_NOFIX) {
        // Updating x0.
        NSBlocks x_new;
        pref.concat(suff, x_new);
        x_new.normalize();
        int old_min_length = x0.min_length();
        int old_max_length = x0.max_length();
        X.update(home, x_new);
        GECODE_ME_CHECK(x0.varimp()->notify(
          home, old_min_length, old_max_length));
      }
      GECODE_ME_CHECK(x1.lq(home, std::max(0, x0.max_length() - minR + 1)));
      assert (X.is_normalized());
    } while (x1.assigned() && x1.val() <= 1);
    return ES_FIX;
  }
  
}}
