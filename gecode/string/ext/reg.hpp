namespace Gecode { namespace String {

  template<class Char, class Traits>
  forceinline std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const stringDFA& d) {
    os << '[';
    for (int i = 0; i < d.n_states; i++) {
      std::vector<NSIntSet> qi(d.n_states);
      for (auto& x : d.delta[i])
        qi[x.second].add(x.first);
      for (unsigned j = 0; j < qi.size(); ++j)
        if (!qi[j].empty())
          os << "(q" << i << ", " << qi[j] << ", q" << j << "), ";
    }
    return os << "F: " << d.final_fst << ".." << d.final_lst << ", ua = "
          << d.ua << ", ur = " << d.ur << "]";
  }
  template<class Char, class Traits>
  forceinline std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const matchNFA& R) {
    os << '[';
    for (int i = 0; i < R.n_states; i++) {
      std::vector<NSIntSet> Si(R.n_states);
      for (auto& x : R.delta[i]) {
        if (x.second < 0) {
          Si[-x.second].add(x.first);
          Si[R.bot].add(x.first);
        }
        else
          Si[x.second].add(x.first);
      }
      for (unsigned j = 0; j < Si.size(); ++j)
        if (!Si[j].empty())
          os << "(q" << i << ", " << Si[j] << ", q" << j << "), ";
    }
    return os << "qbot = " << R.bot << "]";
  }

  forceinline void
  stringDFA::compute_univ(const NSIntSet& domain) {
  	// For a minimal DFA, there is at most 1 universal accepted/rejected state.
	  ua = -1;
    ur = -1;
    for (int i = 0; i < final_fst; ++i) {
      ur = i;
      for (auto& x : delta[i])
        if (x.second != i) {
          ur = -1;
          break;
        }
      if (ur == i)
        break;
    }
    for (int i = final_fst; i <= final_lst; ++i) {
      ua = i;
      if ((int) delta[i].size() < domain.size()) {
        ua = -1;
        continue;
      }
      for (NSIntSet::iterator it(domain); it(); ++it) {
        if (search(i, *it) != i) {
          ua = -1;
          break;
        }
      }
    }
    for (int i = final_lst + 1; i < n_states; ++i) {
      ur = i;
      for (auto& x : delta[i])
        if (x.second != i) {
          ur = -1;
          break;
        }
      if (ur == i)
        break;
    }
  }

  forceinline
  stringDFA::stringDFA(const DFA& d) : n_states(d.n_states()),
  final_fst(d.final_fst()), final_lst(d.final_lst() - 1), delta(d.n_states()) {
    NSIntSet S;
    for (DFA::Transitions t(d); t(); ++t) {
      S.add(t.symbol());
      delta[t.i_state()].push_back(std::pair<int,int>(t.symbol(), t.o_state()));
    }
    for (int i = 0; i < n_states; ++i)
      std::sort(delta[i].begin(), delta[i].end());
    ua = ur = -1;
  }

  forceinline bool
  stringDFA::final(int q) const {
    return final_fst <= q && q <= final_lst;
  }

  forceinline int
  stringDFA::search(int q, int c) const {
    std::vector<std::pair<int, int>> d = delta[q];
    int l = 0, u = d.size() - 1;
    // Binary search, assuming delta[q] is lexicographically sorted.
    while (l <= u) {
      int m = l + (u - l) / 2, dm = d[m].first;
      if (dm == c)
        return d[m].second;
      if (dm < c)
        l = m + 1;
      else
        u = m - 1;
    }
    return -1;
  }

  forceinline bool
  stringDFA::univ_accepted(const NSIntSet& Q) const {
    return Q.size() <= 1 && (Q.empty() || Q.min() == ua);
  }

  forceinline bool
  stringDFA::univ_rejected(const NSIntSet& Q) const {
    return Q.size() <= 1 && (Q.empty() || Q.min() == ur);
  }

  forceinline bool
  stringDFA::accepted(string s) const {
    int q = 0;
    for (auto& c : s) {
      q = search(q, char2int(c));
      if (q == -1)
        return false;
    }
    return final(q);
  }

  forceinline NSIntSet
  stringDFA::alphabet() const {
    NSIntSet s;
    for (int i = 0; i < n_states; i++)
      for (auto& x : delta[i])
        s.add(x.first);
    return s;
  }

  forceinline NSIntSet
  stringDFA::neighbours(int q) const {
    NSIntSet s;
    for (auto& x : delta[q])
      s.add(x.second);
    return s;
  }

  forceinline NSIntSet
  stringDFA::neighbours(int q, const DSIntSet& S) const {
    NSIntSet s;
    for (auto& x : delta[q])
      if (S.in(x.first))
        s.add(x.second);
    return s;
  }

  forceinline void
  stringDFA::negate(const NSIntSet& alphabet) {
     std::cerr << "stringDFA::negate: " << *this << ' ' << alphabet << '\n';
    bool complete = true;
    for (int i = 0; i < n_states; i++) {
      NSIntSet a(alphabet);
      for (int j = 0; j < (int) delta[i].size(); ++j)
        a.remove(delta[i][j].first);
      complete &= a.empty();
      for (NSIntSet::iterator it(a); it(); ++it)
        delta[i].push_back(std::make_pair(*it, n_states));
    }
    // Adding the "bottom" state before negating, if DFA is not complete.
    if (!complete) {
      delta.push_back(std::vector<std::pair<int, int>>());
      for (NSIntSet::iterator it(alphabet); it(); ++it)
        delta[n_states].push_back(std::make_pair(*it, n_states));
      n_states++;
    }
    if (final(0)) {
      final_fst = final_lst + 1;
      final_lst = n_states - 1;
      for (int i = 0; i < n_states; ++i)
        std::sort(delta[i].begin(), delta[i].end());
      return;
    }
    delta_t rdelta(n_states);
    for (int i = 0; i < n_states; i++)
      for (auto& x : delta[i])
        rdelta[nstate(i)].push_back(std::make_pair(x.first, nstate(x.second)));
    delta = rdelta;
    final_lst = n_states - final_lst + final_fst - 2;
    final_fst = 0;
    for (int i = 0; i < n_states; ++i)
      std::sort(delta[i].begin(), delta[i].end());
  }

  forceinline int
  stringDFA::nstate(int q) const {
    if (q < final_fst)
      return q;
    else if (q > final_lst)
      return q + final_fst - final_lst - 1;
    else
      return q + n_states - final_lst - 1;
  }
  
  forceinline matchNFA
  stringDFA::toMatchNFA(const NSIntSet& domain) {
    int qbot;
    matchNFA R;
    compute_univ(domain);
    // Proper pattern: not empty, not containing empty string.
    assert (ur == -1);
//    std::cerr << "stringDFA::toMatchNFA of " << *this << "\n";
    R.delta = delta;
    R.n_states = n_states + 1;
    qbot = delta.size();
    R.delta.push_back(std::vector<std::pair<int, int>>());
    R.bot = qbot;
    NSIntSet S = alphabet();
    S.include(domain);
    for (NSIntSet::iterator it = S.begin(); it(); ++it)
      R.delta[qbot].push_back(std::make_pair(*it, qbot));
    for (auto& x : delta[0])
      R.delta[qbot][x.first].second = x.second;
//    std::cerr << "matchNFA: " << R << "\n";
    for (int i = 0; i < delta.size(); ++i) {      
      if (i != qbot) {
        NSIntSet T(S);      
        for (int j = 0; j < delta[i].size(); ++j) {
          T.remove(delta[i][j].first);
          int q = delta[i][j].second;
          if (q > 1 && q != qbot)
            R.delta[i][j].second = -q;
        }
        for (NSIntSet::iterator it = T.begin(); it(); ++it)
          R.delta[i].push_back(std::make_pair(*it, qbot));
      }
    }
//    std::cerr << "matchNFA: " << R << "\n";
    return R;
  }
  
  forceinline NSIntSet
  matchNFA::neighbours(int q) const {
    NSIntSet Q;
    for (auto& x : delta[q]) {
      if (x.second < 0) {
        Q.add(-x.second);
        Q.add(bot);
      }
      else
        Q.add(x.second);
    }
    return Q;
  };
  
  forceinline NSIntSet
  matchNFA::neighbours(int q, const DSIntSet& S) const {
    NSIntSet Q;
    for (auto& x : delta[q]) {
      if (S.in(x.first))
        if (x.second < 0) {
          Q.add(-x.second);
          Q.add(bot);
        }
        else
          Q.add(x.second);
    }
    return Q;
  };

  forceinline
  Reg::Reg(Home home, StringView x, stringDFA* p)
  : UnaryPropagator<StringView, PC_STRING_DOM>(home, x), dfa(p) {}

  forceinline NSBlocks
  Reg::dom(stringDFA* dfa) {
    // std::cerr << "dom: " << dfa << '\n';    
    NSIntSet S = dfa->alphabet();
    int l = 0, u = 0, n_states = dfa->n_states;
    std::vector<int> dist(n_states);    
    for (int i = 0; i < n_states; ++i)
      dist[i] = DashedString::_MAX_STR_LENGTH;
    if (DashedString::_DECOMP_REGEX)
      u = DashedString::_MAX_STR_LENGTH;
    else {
      std::list<int> s;
      s.push_back(0);
      dist[0] = 0;
      // BFS for l.
      while (!s.empty()) {
        int q = s.front();
        s.pop_front();
        if (dfa->final(q)) {
          l = dist[q];
          break;
        }
        NSIntSet nq = dfa->neighbours(q);
        for (NSIntSet::iterator it(nq); it(); ++it) {
          int a = dist[q] + 1, x = *it;
          if (a < dist[x]) {
            dist[x] = a;
            s.push_back(x);
          }
        }
      }
      if (l > 0 && S.empty())
        return NSBlocks();
      std::list<std::pair<int,bool>> stack;
      s.clear();
      stack.push_front(std::pair<int,bool>(0, 0));
      // DFS for u.
      for (int i = 0; i < n_states; ++i)
        dist[i] = 0;
      while (u != DashedString::_MAX_STR_LENGTH && !stack.empty()) {
        int q = stack.front().first, open = stack.front().second;
        stack.pop_front();
        if (open) {
//          std::cerr << "Closing " << q << '\n';
          dist[q] = 2;
          s.push_front(q);
        }
        else {
//          std::cerr << "Opening " << q << "\n";
          dist[q] = 1;
          // push again q in "exit" mode.
          stack.push_front(std::pair<int,bool>(q,1));
          NSIntSet nq = dfa->neighbours(q);
          for (NSIntSet::iterator it(nq); it(); ++it) {
            int qi = *it;
            if (dist[qi] == 1) {
//              std::cerr << "Loop! " << qi << "\n";
              u = DashedString::_MAX_STR_LENGTH;
              break;
            }
            else if (dist[qi] == 0)
              stack.push_front(std::pair<int,bool>(qi, 0));
          }
        }        
      }
      if (u != DashedString::_MAX_STR_LENGTH) {
//        std::cerr << "Topo. sort: "; for (auto& x : s) std::cerr << x << " "; std::cerr <<"\n";
        dist[0] = 0;
        for (int i = 1; i < n_states; ++i)
          dist[i] = -1;
        while (!s.empty()) {
          int q = s.front();
          s.pop_front();
          if (dist[q] != -1) {
            NSIntSet nq = dfa->neighbours(q);
            for (NSIntSet::iterator it(nq); it(); ++it) {
              int qi = *it;
              if (dist[qi] < dist[q] + 1) {
                int d = dist[q] + 1;
                if (d > u)
                  u = d;
                dist[qi] = d;
              }
            }
          }
        }
//        std::cerr << "dist: "; for (auto& x : dist) std::cerr << x << " "; std::cerr <<"\n";
      }  
    }
//    std::cerr << l << ' ' << u << '\n';
    assert (l <= u);
    return NSBlocks(1, NSBlock(S, l, u));
  }

  forceinline
  Reg::Reg(Space& home, Reg& p)
  : UnaryPropagator<StringView, PC_STRING_DOM>(home, p), dfa(p.dfa) {}

  forceinline ExecStatus
  Reg::post(Home home, StringView x, const DFA& d) {
    // std::cerr << "Reg::post" << x << ' ' << d << '\n';
    if (d.final_fst() >= d.final_lst())
      return ES_FAILED;
    if (x.assigned()) {
      string s = x.val();
      int q = 0;
      for (int i = 0; i < (int) s.size(); ++i) {
        int c = char2int(s[i]), qi = -1;
        for (DFA::Transitions t(d, c); t(); ++t)
          if (t.i_state() == q) {
            qi = t.o_state();
            break;
          }
        if (qi == -1)
          return ES_FAILED;
        q = qi;
      }
      return q < d.final_fst() || q >= d.final_lst() ? ES_FAILED : ES_OK;
    }
    stringDFA* dfa = new stringDFA(d);
    NSBlocks dx(dom(dfa));
    // std::cerr << "ExtDFA dom: " << dx << '\n';
    rel(home, x, STRT_DOM, dx, x.min_length(), x.max_length());
    (void) new (home) Reg(home, x, dfa);
    return ES_OK;
  }

  forceinline ExecStatus
  Reg::post(Home home, StringView x, stringDFA* p) {
    // std::cerr << "Reg::post" << x << ' ' << *p << '\n';
    (void) new (home) Reg(home, x, p);
    return ES_OK;
  }

  forceinline Actor*
  Reg::copy(Space& home) {
    return new (home) Reg(home, *this);
  }

  forceinline std::vector<NSIntSet>
  Reg::reach_fwd(
    stringDFA* dfa, const NSIntSet& Qf, const DSBlock& b, bool reif, bool rev
  ) {
    int l = b.l;
    std::vector<NSIntSet> Q(l + 2);
    Q[0] = Qf;
    stringDFA::delta_t delta_rev;
    if (rev) {
      delta_rev = stringDFA::delta_t(dfa->n_states);
      for (int q = 0; q < dfa->n_states; ++q)
        for (auto& x : dfa->delta[q])
          if (b.S.in(x.first))
            delta_rev[x.second].push_back(std::pair<int, int>(x.first, q));
    }
    // Mandatory region.
    for (int i = 0; i < l; ++i) {
      NSIntSet qi;
      if (rev)
        for (NSIntSet::iterator it(Q[i]); it(); ++it)
          for (auto& x : delta_rev[*it])
            qi.include(x.second);
      else
        for (NSIntSet::iterator it(Q[i]); it(); ++it)
          qi.include(dfa->neighbours(*it, b.S));
      if (qi.empty())
        return std::vector<NSIntSet>();
      if (qi == Q[i]) {
        // Fixpoint.
        for (int j = i + 1; j <= l + 1; ++j)
          Q[j] = qi;
        return Q;
      }
      if (reif && (dfa->univ_accepted(qi) || dfa->univ_rejected(qi)))
        return std::vector<NSIntSet>(1, qi);
      Q[i + 1] = qi;
    }
    Q[l + 1] = Q[l];
    int dist[dfa->n_states];
    for (int q = 0; q < dfa->n_states; ++q)
      if (Q[l].contains(q))
        dist[q] = l;
      else
        dist[q] = DashedString::_MAX_STR_LENGTH;
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
        if (rev)
          for (auto& x : delta_rev[q])
            nq.include(x.second);
        else
          nq = dfa->neighbours(q, b.S);
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

  forceinline NSBlocks
  Reg::reach_bwd(
    stringDFA* dfa, const std::vector<NSIntSet>& Q, NSIntSet& Qe, 
    const DSBlock& b, bool& changed, bool rev
  ) {
    stringDFA::delta_t delta_bwd(dfa->n_states);
    if (!rev)
      for (int q = 0; q < dfa->n_states; ++q)
        for (auto& x : dfa->delta[q])
          if (b.S.in(x.first))
            delta_bwd[x.second].push_back(std::pair<int, int>(x.first, q));
    int l = b.l, l1 = DashedString::_MAX_STR_LENGTH;
    NSIntSet Q1(Qe);
    int dist[dfa->n_states];
    for (int q = 0; q < dfa->n_states; ++q)
      if (Qe.contains(q))
        dist[q] = 0;
      else
        dist[q] = DashedString::_MAX_STR_LENGTH;
    std::list<int> Q_bfs;
    for (NSIntSet::iterator i(Q1); i(); ++i)
      Q_bfs.push_back(*i);
    NSIntSet S_opt;
    while (!Q_bfs.empty()) {
      int q = Q_bfs.front(), d = dist[q];
      Q_bfs.pop_front();
      if (Q[l].contains(q))
        l1 = min(l1, dist[q]);
      if (d < DashedString::_MAX_STR_LENGTH)
        ++d;
      if (d <= b.u - b.l) {
        std::vector<std::pair<int, int>> dx;
        if (rev) {
          for (auto& x : dfa->delta[q])
            if (b.S.in(x.first))
              dx.push_back(x);
        }
        else
          dx = delta_bwd[q];
        for (auto& x : dx) {
          int c = x.first, q1 = x.second;
          if (Q[l + 1].contains(q1)) {
            S_opt.add(c);
            if (dist[q1] > d) {
              Q_bfs.push_back(q1);
              if (Q[l].in(q1))
                Q1.include(q1);
              dist[q1] = d;
            }
          }
        }
      }
    }
    NSBlocks y(l + 1);
    NSIntSet E(Q1);
    E.intersect(Q[l]);
    if (l1 > l) {
      changed = true;
      y[l] = NSBlock(S_opt, l1, b.u - l);
    }
    else {
      if (b.u > l && S_opt.size() < (int) b.S.size())
        changed = true;
      y[l] = NSBlock(S_opt, 0, b.u - l);
    }
    for (int i = l; i > 0; --i) {
      NSIntSet S_man, B1;
      for (NSIntSet::iterator it(E); it(); ++it) {
        int q = *it;
        std::vector<std::pair<int, int>> dx;
        if (rev) {
          for (auto& x : dfa->delta[q])
            if (b.S.in(x.first))
              dx.push_back(x);
        }
        else
         dx = delta_bwd[q];
        for (auto& x : dx) {
          int c = x.first, q1 = x.second;
          S_opt.add(c);
          if (Q[i - 1].contains(q1)) {
            S_man.add(c);
            B1.add(q1);
          }
        }
      }
      E = B1;
      if (S_man.size() < (int) b.S.size())
        changed = true;
      y[i - 1] = NSBlock(S_man, 1, 1);
    }
    y.normalize();
    Qe = E;
    return y;
  }

  forceinline ExecStatus
  Reg::propagate(Space& home, const ModEventDelta& m) {
    // std::cerr<<"\nExtDFA<StringView>::propagate "<<x0<<" in dfa "<<*dfa<<std::endl;
    if (x0.assigned()) {
      // std::cerr << dfa->accepted(x0.val()) << std::endl;
      return dfa->accepted(x0.val()) ? home.ES_SUBSUMED(*this) : ES_FAILED;
    }
    dfa->compute_univ(x0.may_chars());
    DashedString* x = x0.pdomain();
    std::vector<std::vector<NSIntSet>> F(x->length());
    NSIntSet Fi(0);
    int n = x->length();
    for (int i = 0; i < n; ++i) {
      F[i] = reach_fwd(dfa, Fi, x->at(i));
      if (F[i].empty())
        return ES_FAILED;
      Fi = F[i].back();
      if (dfa->univ_rejected(Fi))
        return ES_FAILED;
    }
    NSIntSet E(F.back().back());
    NSBlocks y[n];
    Fi = NSIntSet(dfa->final_fst, dfa->final_lst);
    E.intersect(Fi);
    if (E.empty())
      return ES_FAILED;
    bool changed = false;
    int k = 0;
    for (int i = n - 1; i >= 0; --i) {
      y[i] = reach_bwd(dfa, F[i], E, x->at(i), changed);
      k += y[i].size();
    }
    if (changed) {
      StringDelta d(true);
      NSBlocks z;
      for (auto& yi : y)
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
      z.empty() ? x->set_null(home) : x->update(home, z);
      GECODE_ME_CHECK(x0.varimp()->notify(
        home, x0.assigned() ? ME_STRING_VAL : ME_STRING_DOM, d)
      );
      // std::cerr<<"ExtDFA<StringView>::propagated (changed) "<<x0<<"\n\n";
      assert (x0.pdomain()->is_normalized());
      return x0.assigned() ? home.ES_SUBSUMED(*this) : propagate(home, m);
    }
    // Reverse run.
    if (DashedString::_REVERSE_REGEX) {
      // std::cerr << "Reverse propagation\n";
      for (int i = 0; i < n; ++i) {
        F[i] = reach_fwd(dfa, Fi, x->at(n - i - 1), false, true);
        if (F[i].empty())
          return ES_FAILED;
        Fi = F[i].back();
      }
      E = F.back().back();
      if (E.contains(0))
        E = NSIntSet(0);
      else
        return ES_FAILED;
      changed = false;
      k = 0;
      for (int i = 0; i < n; ++i) {
        y[i] = reach_bwd(dfa, F[n - i - 1], E, x->at(i), changed, true);
        std::reverse(y[i].begin(), y[i].end());
        k += y[i].size();
      }
      if (changed) {
        StringDelta d(true);
        NSBlocks z;
        for (auto& yi : y)
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
        z.empty() ? x->set_null(home) : x->update(home, z);
        GECODE_ME_CHECK(x0.varimp()->notify(
          home, x0.assigned() ? ME_STRING_VAL : ME_STRING_DOM, d)
        );
        // std::cerr<<"ExtDFA<StringView>::propagated (changed) "<<x0<<"\n\n";
        assert (x0.pdomain()->is_normalized());
        return x0.assigned() ? home.ES_SUBSUMED(*this) : propagate(home, m);
      }
    }
    // std::cerr<<"ExtDFA<StringView>::propagated (no change) "<<x0<<"\n\n";
    assert (x0.pdomain()->is_normalized());
    return ES_FIX;
  }

  forceinline
  Reg::~Reg() {
    delete dfa;
  }

}}
