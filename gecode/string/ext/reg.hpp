namespace Gecode { namespace String {

  template<class Char, class Traits>
  forceinline std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const trimDFA& d) {
    os << '[';
    for (int i = 0; i < d.n_states; i++) {
      std::vector<NSIntSet> qi(d.n_states);
      for (auto& x : d.delta[i])
        qi[x.second].add(x.first);
      for (unsigned j = 0; j < qi.size(); ++j)
        if (!qi[j].empty())
          os << "(q" << i << ", " << qi[j] << ", q" << j << "), ";
    }
    return os << "], F: " << d.accepting_states().toString();
  }
  
  forceinline
  trimDFA::trimDFA(const DFA& d) : stringDFA(d), delta(d.n_states()) {
    NSIntSet S;
    for (DFA::Transitions t(d); t(); ++t) {
      S.add(t.symbol());
      delta[t.i_state()].push_back(std::pair<int,int>(t.symbol(), t.o_state()));
    }
    for (int i = 0; i < n_states; ++i)
      std::sort(delta[i].begin(), delta[i].end());
  }
  
  forceinline int
  trimDFA::search(int q, int c) const {
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

  forceinline NSIntSet
  trimDFA::alphabet() const {
    NSIntSet s;
    for (int i = 0; i < n_states; i++)
      for (auto& x : delta[i])
        s.add(x.first);
    return s;
  }

  forceinline NSIntSet
  trimDFA::neighbours(int q) const {
    NSIntSet s;
    for (auto& x : delta[q])
      s.add(x.second);
    return s;
  }

  forceinline NSIntSet
  trimDFA::neighbours(int q, const DSIntSet& S) const {
    NSIntSet s;
    for (auto& x : delta[q])
      if (S.in(x.first))
        s.add(x.second);
    return s;
  }
  
  forceinline NSIntSet
  trimDFA::neighbot(int q, const DSIntSet& S) const {
    NSIntSet s, s_delta;
    for (auto& x : delta[q]) {
      if (S.in(x.first))
        s.add(x.second);
      s_delta.add(x.first);
    }
    return s_delta.contains(s) ? s : NSIntSet();
  }

  forceinline
  Reg::Reg(Home home, StringView x, trimDFA* p)
  : UnaryPropagator<StringView, PC_STRING_DOM>(home, x), dfa(p) {}

  forceinline NSBlocks
  Reg::dom(trimDFA* dfa) {
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
        if (dfa->accepting(q)) {
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
    trimDFA* dfa = new trimDFA(d);
    NSBlocks dx(dom(dfa));
    // std::cerr << "ExtDFA dom: " << dx << '\n';
    rel(home, x, STRT_DOM, dx, x.min_length(), x.max_length());
    (void) new (home) Reg(home, x, dfa);
    return ES_OK;
  }

  forceinline ExecStatus
  Reg::post(Home home, StringView x, trimDFA* p) {
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
    trimDFA* dfa, const NSIntSet& Qf, const DSBlock& b, bool rev
  ) {
    int l = b.l;
    std::vector<NSIntSet> Q(l + 2);
    Q[0] = Qf;
    trimDFA::delta_t delta_rev;
    if (rev) {
      delta_rev = trimDFA::delta_t(dfa->n_states);
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
    trimDFA* dfa, const std::vector<NSIntSet>& Q, NSIntSet& Qe, 
    const DSBlock& b, bool& changed, bool rev
  ) {
    trimDFA::delta_t delta_bwd(dfa->n_states);
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

  forceinline NSBlocks
  Reg::reach_bwd(
    compDFA* dfa, const std::vector<NSIntSet>& Q, NSIntSet& Qe, 
    const DSBlock& b, bool& changed
  ) {
    compDFA::Delta_t delta_bwd(dfa->n_states);
    for (int q = 0; q < dfa->n_states; ++q)
      for (auto& x : dfa->delta[q])
        if (!b.S.disjoint(x.first)) {
          NSIntSet s(b.S);
          s.intersect(x.first);
          delta_bwd[x.second].push_back(std::pair<NSIntSet, int>(s, q));
        }
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
        std::vector<std::pair<NSIntSet, int>> dx;
        dx = delta_bwd[q];
        for (auto& x : dx) {
          NSIntSet s = x.first;
          int q1 = x.second;
          if (Q[l + 1].contains(q1)) {
            S_opt.include(s);
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
        std::vector<std::pair<NSIntSet, int>> dx;
        dx = delta_bwd[q];
        for (auto& x : dx) {
          NSIntSet s = x.first; 
          int q1 = x.second;
          S_opt.include(s);
          if (Q[i - 1].contains(q1)) {
            S_man.include(s);
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
  
  forceinline std::vector<NSIntSet>
  Reg::reach_fwd(compDFA* dfa, const NSIntSet& Qf, const DSBlock& b) {
    int l = b.l;
    std::vector<NSIntSet> Q(l + 2);
    Q[0] = Qf;
    // Mandatory region.
    for (int i = 0; i < l; ++i) {
      NSIntSet qi;
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

  forceinline ExecStatus
  Reg::propagate(Space& home, const ModEventDelta& m) {
    // std::cerr<<"\nExtDFA<StringView>::propagate "<<x0<<" in dfa "<<*dfa<<std::endl;
    if (x0.assigned()) {
      // std::cerr << dfa->accepted(x0.val()) << std::endl;
      return dfa->accepted(x0.val()) ? home.ES_SUBSUMED(*this) : ES_FAILED;
    }
    DashedString* x = x0.pdomain();
    std::vector<std::vector<NSIntSet>> F(x->length());
    NSIntSet Fi(0);
    int n = x->length();
    for (int i = 0; i < n; ++i) {
      F[i] = reach_fwd(dfa, Fi, x->at(i));
      if (F[i].empty())
        return ES_FAILED;
      Fi = F[i].back();
    }
    NSIntSet E(F.back().back());
    NSBlocks y[n];
    Fi = dfa->accepting_states();
    E.intersect(Fi);
    if (E.empty())
      return ES_FAILED;
    bool changed = false;
    for (int i = n - 1; i >= 0; --i)
      y[i] = reach_bwd(dfa, F[i], E, x->at(i), changed);
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
        F[i] = reach_fwd(dfa, Fi, x->at(n - i - 1), true);
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
      for (int i = 0; i < n; ++i) {
        y[i] = reach_bwd(dfa, F[n - i - 1], E, x->at(i), changed, true);
        std::reverse(y[i].begin(), y[i].end());
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
