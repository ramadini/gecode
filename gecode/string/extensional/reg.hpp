namespace Gecode { namespace String {

  template<class Char, class Traits>
  forceinline std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const trimDFA& d) {
    os << '[';
    for (int i = 0; i < d.n_states; i++)
      for (auto& x : d.delta[i])
        os << "(q" << i << ", " << x.first << ", q" << x.second << "), ";
    return os << "], F: " << d.accepting_states().toString();
  }

  forceinline
  trimDFA::trimDFA(const DFA& d) : stringDFA(d), delta(d.n_states()) {
    for (DFA::Transitions t(d); t(); ++t) {
      const int source = t.i_state();
      const int target = t.o_state();
      std::vector<std::pair<NSIntSet, int>>& outgoing = delta[source];
      std::vector<std::pair<NSIntSet, int>>::iterator edge = outgoing.begin();
      while (edge != outgoing.end() && edge->second != target)
        ++edge;
      if (edge == outgoing.end()) {
        outgoing.emplace_back(NSIntSet(t.symbol()), target);
      } else {
        edge->first.add(t.symbol());
      }
    }
  }

  forceinline
  trimDFA::trimDFA(const DFA& d, const std::vector<NSIntSet>& labels)
    : stringDFA(d), delta(d.n_states()) {
    for (DFA::Transitions t(d); t(); ++t) {
      const int symbol = t.symbol();
      if (symbol < 0 || static_cast<std::size_t>(symbol) >= labels.size())
        throw std::out_of_range("Symbolic DFA transition label is out of range");
      if (labels[symbol].empty())
        continue;
      const int source = t.i_state();
      const int target = t.o_state();
      std::vector<std::pair<NSIntSet, int>>& outgoing = delta[source];
      std::vector<std::pair<NSIntSet, int>>::iterator edge = outgoing.begin();
      while (edge != outgoing.end() && edge->second != target)
        ++edge;
      if (edge == outgoing.end()) {
        outgoing.emplace_back(labels[symbol], target);
      } else {
        edge->first.include(labels[symbol]);
      }
    }
  }

  forceinline int
  trimDFA::search(int q, int c) const {
    for (const auto& transition : delta[q])
      if (transition.first.in(c))
        return transition.second;
    return -1;
  }

  template<class CharacterSet>
  forceinline trimDFA::delta_t
  trimDFA::reverse_transitions(const CharacterSet& characters) const {
    delta_t reverse(n_states);
    for (int source = 0; source < n_states; ++source)
      for (auto& transition : delta[source])
        if (!characters.disjoint(transition.first)) {
          NSIntSet label(characters);
          label.intersect(transition.first);
          reverse[transition.second].emplace_back(
            std::move(label), source);
        }
    return reverse;
  }

  forceinline NSIntSet
  trimDFA::alphabet() const {
    NSIntSet s;
    for (int i = 0; i < n_states; i++)
      for (auto& x : delta[i])
        s.include(x.first);
    return s;
  }

  forceinline int
  trimDFA::min_word_length() const {
    std::vector<int> dist(n_states, DashedString::_MAX_STR_LENGTH);
    std::vector<int> queue;
    queue.reserve(n_states);
    queue.push_back(0);
    dist[0] = 0;
    for (unsigned int head = 0; head < queue.size(); ++head) {
      int q = queue[head];
      if (accepting(q))
        return dist[q];
      int next_distance = dist[q] + 1;
      for (const auto& transition : delta[q]) {
        int next = transition.second;
        if (next_distance < dist[next]) {
          dist[next] = next_distance;
          queue.push_back(next);
        }
      }
    }
    return 0;
  }

  forceinline NSIntSet
  trimDFA::neighbours(int q) const {
    NSIntSet s;
    for (auto& x : delta[q])
      s.add(x.second);
    return s;
  }

  template<class CharacterSet, class Visitor>
  forceinline void
  trimDFA::visit_neighbours(int q, const CharacterSet& characters,
                            Visitor visitor) const {
    for (auto& transition : delta[q])
      if (!characters.disjoint(transition.first))
        visitor(transition.second);
  }

  template<class CharacterSet>
  forceinline void
  trimDFA::include_neighbours(NSIntSet& states, int q,
                              const CharacterSet& characters) const {
    visit_neighbours(q, characters,
      [&states](int state) { states.add(state); });
  }

  template<class CharacterSet>
  forceinline bool
  trimDFA::include_all_neighbours(NSIntSet& states, int q,
                                  const CharacterSet& characters) const {
    NSIntSet required(characters);
    NSIntSet covered;
    for (auto& transition : delta[q]) {
      if (!characters.disjoint(transition.first)) {
        states.add(transition.second);
        NSIntSet label(transition.first);
        label.intersect(characters);
        covered.include(label);
      }
    }
    return !required.empty() && covered == required;
  }

  forceinline NSIntSet
  trimDFA::neighbot(int q, const DSIntSet& S) const {
    NSIntSet s;
    return include_all_neighbours(s, q, S) ? s : NSIntSet();
  }

  forceinline NSIntSet
  trimDFA::reach_all(const DSBlock& b, const NSIntSet& initial) const {
    NSIntSet current = initial;
    for (int i = 0; i < b.l; ++i) {
      NSIntSet next;
      for (NSIntSet::iterator state(current); state(); ++state)
        if (!include_all_neighbours(next, *state, b.S))
          return NSIntSet();
      if (next == current)
        return next;
      current = std::move(next);
    }

    Region region;
    int* distance = region.alloc<int>(n_states);
    for (int state = 0; state < n_states; ++state)
      distance[state] = current.contains(state)
        ? b.l : DashedString::_MAX_STR_LENGTH + 1;
    std::vector<int> queue;
    queue.reserve(n_states);
    for (NSIntSet::iterator state(current); state(); ++state)
      queue.push_back(*state);
    NSIntSet reachable = current;
    for (unsigned int head = 0; head < queue.size(); ++head) {
      int state = queue[head], next_distance = distance[state] + 1;
      if (next_distance <= b.u) {
        NSIntSet next;
        if (!include_all_neighbours(next, state, b.S))
          return NSIntSet();
        for (NSIntSet::iterator neighbour(next); neighbour(); ++neighbour) {
          int next_state = *neighbour;
          if (distance[next_state] > next_distance) {
            queue.push_back(next_state);
            reachable.add(next_state);
            distance[next_state] = next_distance;
          }
        }
      }
    }
    return reachable;
  }

  forceinline
  Reg::Reg(Home home, StringView x, trimDFA* p)
  : UnaryPropagator<StringView, PC_STRING_DOM>(home, x), dfa(p) {
    home.notice(*this, AP_DISPOSE);
  }

  forceinline NSBlocks
  Reg::dom(trimDFA* dfa) {
    NSIntSet S = dfa->alphabet();
    int l = 0, u = 0, n_states = dfa->n_states;
    std::vector<int> dist(n_states);
    if (DashedString::_DECOMP_REGEX)
      u = DashedString::_MAX_STR_LENGTH;
    else {
      l = dfa->min_word_length();
      if (l > 0 && S.empty())
        return NSBlocks();
      std::vector<int> sorted;
      sorted.reserve(n_states);
      std::vector<std::pair<int,bool>> stack;
      stack.reserve(n_states);
      stack.push_back(std::pair<int,bool>(0, 0));
      // DFS for u.
      for (int i = 0; i < n_states; ++i)
        dist[i] = 0;
      while (u != DashedString::_MAX_STR_LENGTH && !stack.empty()) {
        int q = stack.back().first, open = stack.back().second;
        stack.pop_back();
        if (open) {
          dist[q] = 2;
          sorted.push_back(q);
        }
        else {
          dist[q] = 1;
          // push again q in "exit" mode.
          stack.push_back(std::pair<int,bool>(q,1));
          for (const auto& transition : dfa->delta[q]) {
            int qi = transition.second;
            if (dist[qi] == 1) {
              u = DashedString::_MAX_STR_LENGTH;
              break;
            }
            else if (dist[qi] == 0)
              stack.push_back(std::pair<int,bool>(qi, 0));
          }
        }
      }
      if (u != DashedString::_MAX_STR_LENGTH) {
        dist[0] = 0;
        for (int i = 1; i < n_states; ++i)
          dist[i] = -1;
        for (auto i = sorted.rbegin(); i != sorted.rend(); ++i) {
          int q = *i;
          if (dist[q] != -1) {
            for (const auto& transition : dfa->delta[q]) {
              int qi = transition.second;
              if (dist[qi] < dist[q] + 1) {
                int d = dist[q] + 1;
                if (d > u)
                  u = d;
                dist[qi] = d;
              }
            }
          }
        }
      }
    }
    assert (l <= u);
    return NSBlocks(1, NSBlock(S, l, u));
  }

  forceinline
  Reg::Reg(Space& home, Reg& p)
  : UnaryPropagator<StringView, PC_STRING_DOM>(home, p), dfa(p.dfa) {}

  forceinline ExecStatus
  Reg::post(Home home, StringView x, const DFA& d) {
    if (d.final_fst() >= d.final_lst())
      return ES_FAILED;
    if (x.assigned()) {
      string bytes;
      if (x.domain().try_val_bytes(bytes)) {
        int q = 0;
        for (int i = 0; i < (int) bytes.size(); ++i) {
          int c = char2int(bytes[i]), qi = -1;
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
      const StringVal value = x.val_symbols();
      int q = 0;
      for (StringVal::size_type i = 0; i < value.size(); ++i) {
        int qi = -1;
        for (DFA::Transitions t(d, value[i]); t(); ++t)
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
    rel(home, x, STRT_DOM, dx, x.min_length(), x.max_length());
    (void) new (home) Reg(home, x, dfa);
    return ES_OK;
  }

  forceinline ExecStatus
  Reg::post(Home home, StringView x, trimDFA* p) {
    (void) new (home) Reg(home, x, p);
    return ES_OK;
  }

  forceinline Actor*
  Reg::copy(Space& home) {
    return new (home) Reg(home, *this);
  }

  forceinline size_t
  Reg::dispose(Space& home) {
    home.ignore(*this, AP_DISPOSE);
    dfa.~TrimDFAHandle();
    (void) UnaryPropagator<StringView, PC_STRING_DOM>::dispose(home);
    return sizeof(*this);
  }

  template <typename Block>
  forceinline std::vector<NSIntSet>
  Reg::reach_fwd(
    trimDFA* dfa, const NSIntSet& Qf, const Block& b, bool rev
  ) {
    int l = b.l;
    std::vector<NSIntSet> Q(l + 2);
    Q[0] = Qf;
    trimDFA::delta_t delta_rev;
    if (rev)
      delta_rev = dfa->reverse_transitions(b.S);
    // Mandatory region.
    for (int i = 0; i < l; ++i) {
      NSIntSet qi;
      if (rev) {
        for (NSIntSet::iterator it(Q[i]); it(); ++it)
          for (auto& transition : delta_rev[*it])
            qi.include(transition.second);
      }
      else {
        for (NSIntSet::iterator it(Q[i]); it(); ++it)
          dfa->include_neighbours(qi, *it, b.S);
      }
      if (qi.empty())
        return std::vector<NSIntSet>();
      if (qi == Q[i]) {
        // Fixpoint.
        for (int j = i + 1; j <= l + 1; ++j)
          Q[j] = qi;
        return Q;
      }
      Q[i + 1] = std::move(qi);
    }
    Q[l + 1] = Q[l];
    Region region;
    int* dist = region.alloc<int>(dfa->n_states);
    for (int q = 0; q < dfa->n_states; ++q)
      if (Q[l].contains(q))
        dist[q] = l;
      else
        dist[q] = DashedString::_MAX_STR_LENGTH;
    std::vector<int> Q_bfs;
    Q_bfs.reserve(dfa->n_states);
    for (NSIntSet::iterator i(Q[l]); i(); ++i)
      Q_bfs.push_back(*i);
    // BFS over optional region.
    for (unsigned int head = 0; head < Q_bfs.size(); ++head) {
      int q = Q_bfs[head], d = dist[q];
      if (d < DashedString::_MAX_STR_LENGTH)
        ++d;
      if (d <= b.u) {
        if (rev) {
          for (auto& transition : delta_rev[q]) {
            int q1 = transition.second;
            if (dist[q1] > d) {
              Q_bfs.push_back(q1);
              Q[l + 1].include(q1);
              dist[q1] = d;
            }
          }
        }
        else
          dfa->visit_neighbours(q, b.S, [&](int q1) {
            if (dist[q1] > d) {
              Q_bfs.push_back(q1);
              Q[l + 1].include(q1);
              dist[q1] = d;
            }
          });
      }
    }
    return Q;
  }

  template <typename Block>
  forceinline NSBlocks
  Reg::reach_bwd(
    trimDFA* dfa, const std::vector<NSIntSet>& Q, NSIntSet& Qe,
    const Block& b, bool& changed, bool rev
  ) {
    trimDFA::delta_t delta_bwd;
    if (!rev)
      delta_bwd = dfa->reverse_transitions(b.S);
    int l = b.l, l1 = DashedString::_MAX_STR_LENGTH;
    NSIntSet E(std::move(Qe));
    Region region;
    int* dist = region.alloc<int>(dfa->n_states);
    for (int q = 0; q < dfa->n_states; ++q)
      if (E.contains(q))
        dist[q] = 0;
      else
        dist[q] = DashedString::_MAX_STR_LENGTH;
    std::vector<int> Q_bfs;
    Q_bfs.reserve(dfa->n_states);
    for (NSIntSet::iterator i(E); i(); ++i)
      Q_bfs.push_back(*i);
    NSIntSet S_opt;
    for (unsigned int head = 0; head < Q_bfs.size(); ++head) {
      int q = Q_bfs[head], d = dist[q];
      if (Q[l].contains(q))
        l1 = min(l1, dist[q]);
      if (d < DashedString::_MAX_STR_LENGTH)
        ++d;
      if (d <= b.u - b.l) {
        const std::vector<std::pair<NSIntSet, int>>& dx =
          rev ? dfa->delta[q] : delta_bwd[q];
        for (const auto& x : dx) {
          NSIntSet characters(x.first);
          int q1 = x.second;
          if (rev)
            characters.intersect(b.S);
          if (characters.empty())
            continue;
          if (Q[l + 1].contains(q1)) {
            S_opt.include(characters);
            if (dist[q1] > d) {
              Q_bfs.push_back(q1);
              if (Q[l].in(q1))
                E.include(q1);
              dist[q1] = d;
            }
          }
        }
      }
    }
    NSBlocks y(l + 1);
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
        const std::vector<std::pair<NSIntSet, int>>& dx =
          rev ? dfa->delta[q] : delta_bwd[q];
        for (const auto& x : dx) {
          NSIntSet characters(x.first);
          int q1 = x.second;
          if (rev)
            characters.intersect(b.S);
          if (characters.empty())
            continue;
          S_opt.include(characters);
          if (Q[i - 1].contains(q1)) {
            S_man.include(characters);
            B1.add(q1);
          }
        }
      }
      E = std::move(B1);
      if (S_man.size() < (int) b.S.size())
        changed = true;
      y[i - 1] = NSBlock(S_man, 1, 1);
    }
    y.normalize();
    Qe = std::move(E);
    return y;
  }

  template <typename Block>
  forceinline NSBlocks
  Reg::reach_bwd(
    compDFA* dfa, const std::vector<NSIntSet>& Q, NSIntSet& Qe,
    const Block& b, bool& changed
  ) {
    compDFA::Delta_t delta_bwd = dfa->reverse_transitions(b.S);
    int l = b.l, l1 = DashedString::_MAX_STR_LENGTH;
    NSIntSet E(std::move(Qe));
    Region region;
    int* dist = region.alloc<int>(dfa->n_states);
    for (int q = 0; q < dfa->n_states; ++q)
      if (E.contains(q))
        dist[q] = 0;
      else
        dist[q] = DashedString::_MAX_STR_LENGTH;
    std::vector<int> Q_bfs;
    Q_bfs.reserve(dfa->n_states);
    for (NSIntSet::iterator i(E); i(); ++i)
      Q_bfs.push_back(*i);
    NSIntSet S_opt;
    for (unsigned int head = 0; head < Q_bfs.size(); ++head) {
      int q = Q_bfs[head], d = dist[q];
      if (Q[l].contains(q))
        l1 = min(l1, dist[q]);
      if (d < DashedString::_MAX_STR_LENGTH)
        ++d;
      if (d <= b.u - b.l) {
        const std::vector<std::pair<NSIntSet, int>>& dx = delta_bwd[q];
        for (const auto& x : dx) {
          const NSIntSet& s = x.first;
          int q1 = x.second;
          if (Q[l + 1].contains(q1)) {
            S_opt.include(s);
            if (dist[q1] > d) {
              Q_bfs.push_back(q1);
              if (Q[l].in(q1))
                E.include(q1);
              dist[q1] = d;
            }
          }
        }
      }
    }
    NSBlocks y(l + 1);
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
        const std::vector<std::pair<NSIntSet, int>>& dx = delta_bwd[q];
        for (const auto& x : dx) {
          const NSIntSet& s = x.first;
          int q1 = x.second;
          S_opt.include(s);
          if (Q[i - 1].contains(q1)) {
            S_man.include(s);
            B1.add(q1);
          }
        }
      }
      E = std::move(B1);
      if (S_man.size() < (int) b.S.size())
        changed = true;
      y[i - 1] = NSBlock(S_man, 1, 1);
    }
    y.normalize();
    Qe = std::move(E);
    return y;
  }

  template <typename Block>
  forceinline std::vector<NSIntSet>
  Reg::reach_fwd(compDFA* dfa, const NSIntSet& Qf, const Block& b) {
    int l = b.l;
    std::vector<NSIntSet> Q(l + 2);
    Q[0] = Qf;
    // Mandatory region.
    for (int i = 0; i < l; ++i) {
      NSIntSet qi;
      for (NSIntSet::iterator it(Q[i]); it(); ++it)
        dfa->include_neighbours(qi, *it, b.S);
      if (qi.empty())
        return std::vector<NSIntSet>();
      if (qi == Q[i]) {
        // Fixpoint.
        for (int j = i + 1; j <= l + 1; ++j)
          Q[j] = qi;
        return Q;
      }
      Q[i + 1] = std::move(qi);
    }
    Q[l + 1] = Q[l];
    Region region;
    int* dist = region.alloc<int>(dfa->n_states);
    for (int q = 0; q < dfa->n_states; ++q)
      if (Q[l].contains(q))
        dist[q] = l;
      else
        dist[q] = DashedString::_MAX_STR_LENGTH;
    std::vector<int> Q_bfs;
    Q_bfs.reserve(dfa->n_states);
    for (NSIntSet::iterator i(Q[l]); i(); ++i)
      Q_bfs.push_back(*i);
    // BFS over optional region.
    for (unsigned int head = 0; head < Q_bfs.size(); ++head) {
      int q = Q_bfs[head], d = dist[q];
      if (d < DashedString::_MAX_STR_LENGTH)
        ++d;
      if (d <= b.u)
        dfa->visit_neighbours(q, b.S, [&](int q1) {
          if (dist[q1] > d) {
            Q_bfs.push_back(q1);
            Q[l + 1].include(q1);
            dist[q1] = d;
          }
        });
    }
    return Q;
  }

  forceinline NSBlocks
  merge_refined_blocks(std::vector<NSBlocks>&& refined) {
    NSBlocks domain;
    for (auto& blocks : refined)
      for (auto& block : blocks) {
        if (block.null())
          continue;
        if (!domain.empty() && domain.back().S == block.S) {
          domain.back().l += block.l;
          domain.back().u += block.u;
        }
        else
          domain.push_back(std::move(block));
      }
    return domain;
  }

  forceinline ModEvent
  commit_refined_blocks(
    Space& home, StringView x, std::vector<NSBlocks>&& refined
  ) {
    NSBlocks domain = merge_refined_blocks(std::move(refined));
    return x.dom(home, domain);
  }

  template <typename DFA_t>
  forceinline ExecStatus
  Reg::propagate_blocks(Space&, NSBlocks& x, DFA_t* dfa) {
    if (x.known()) {
      string bytes;
      const bool accepted = x.try_val_bytes(bytes)
        ? dfa->accepted(bytes)
        : dfa->accepted(x.val_symbols());
      return accepted ? ES_FIX : ES_FAILED;
    }
    bool changed, nofix = false;
    do {
      int n = x.length();
      std::vector<std::vector<NSIntSet>> forward(n);
      NSIntSet initial(0);
      const NSIntSet* states = &initial;
      for (int i = 0; i < n; ++i) {
        forward[i] = reach_fwd(dfa, *states, x[i]);
        if (forward[i].empty())
          return ES_FAILED;
        states = &forward[i].back();
      }
      NSIntSet endings(forward.back().back());
      endings.intersect(dfa->accepting_states());
      if (endings.empty())
        return ES_FAILED;
      std::vector<NSBlocks> refined(n);
      changed = false;
      for (int i = n - 1; i >= 0; --i)
        refined[i] = reach_bwd(dfa, forward[i], endings, x[i], changed);
      if (changed) {
        nofix = true;
        x = merge_refined_blocks(std::move(refined));
        assert(x.is_normalized());
      }
    } while (changed);
    return nofix ? ES_NOFIX : ES_FIX;
  }

  forceinline ExecStatus
  Reg::propagate(Space& home, const ModEventDelta&) {
    while (true) {
      if (x0.assigned()) {
        string bytes;
        const bool accepted = x0.domain().try_val_bytes(bytes)
          ? dfa->accepted(bytes)
          : dfa->accepted(x0.val_symbols());
        return accepted ? home.ES_SUBSUMED(*this) : ES_FAILED;
      }
      const DashedString& x = x0.domain();
      std::vector<std::vector<NSIntSet>> F(x.length());
      NSIntSet initial(0);
      const NSIntSet* states = &initial;
      int n = x.length();
      for (int i = 0; i < n; ++i) {
        F[i] = reach_fwd(dfa.get(), *states, x.at(i));
        if (F[i].empty())
          return ES_FAILED;
        states = &F[i].back();
      }
      NSIntSet E(*states);
      std::vector<NSBlocks> y(n);
      NSIntSet accepting = dfa->accepting_states();
      E.intersect(accepting);
      if (E.empty())
        return ES_FAILED;
      bool changed = false;
      for (int i = n - 1; i >= 0; --i)
        y[i] = reach_bwd(dfa.get(), F[i], E, x.at(i), changed);
      if (changed) {
        ModEvent me = commit_refined_blocks(home, x0, std::move(y));
        GECODE_ME_CHECK(me);
        assert (x0.domain().is_normalized());
        if (x0.assigned())
          return home.ES_SUBSUMED(*this);
        if (me_modified(me))
          continue;
      }
      // Reverse run.
      if (DashedString::_REVERSE_REGEX) {
        states = &accepting;
        for (int i = 0; i < n; ++i) {
          F[i] = reach_fwd(dfa.get(), *states, x.at(n - i - 1), true);
          if (F[i].empty())
            return ES_FAILED;
          states = &F[i].back();
        }
        E = F.back().back();
        if (E.contains(0))
          E = NSIntSet(0);
        else
          return ES_FAILED;
        changed = false;
        for (int i = 0; i < n; ++i) {
          y[i] = reach_bwd(
            dfa.get(), F[n - i - 1], E, x.at(i), changed, true);
          std::reverse(y[i].begin(), y[i].end());
        }
        if (changed) {
          ModEvent me = commit_refined_blocks(home, x0, std::move(y));
          GECODE_ME_CHECK(me);
          assert (x0.domain().is_normalized());
          if (x0.assigned())
            return home.ES_SUBSUMED(*this);
          if (me_modified(me))
            continue;
        }
      }
      assert (x0.domain().is_normalized());
      return ES_FIX;
    }
  }

}}

// STATISTICS: string-prop
