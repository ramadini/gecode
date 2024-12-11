#include <gecode/int/bool.hh>

namespace Gecode { namespace String {

  template<class Char, class Traits>
  forceinline std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const compDFA& d) {
    os << '[';
    for (int i = 0; i < d.n_states; i++) {
      for (auto& x : d.delta[i]){
        NSIntSet S = x.first;
        assert (!S.empty());
        os << "(q" << i << ", " << S.toString() << ", q" << x.second << "), ";
      }
    }
    return os << "], |Q| = " << d.n_states << ", F: " << 
                                d.accepting_states().toString();
  }

  forceinline
  compDFA::compDFA(const trimDFA& d, const NSIntSet& alphabet) : q_bot(-1),
  stringDFA(d), delta(d.n_states) {
    bool add_bot = false;
    for (int q = 0; q < n_states; ++q) {
      std::vector<std::pair<int, int>> delta_q = d.delta[q];
      add_bot |= delta_q.size() < alphabet.size();
      NSIntSet a(alphabet);
      for (auto& x : delta_q) {
        delta[q].push_back(std::pair<NSIntSet,int>(NSIntSet(x.first),x.second));
        if (add_bot)
          a.remove(x.first);
      }
      if (add_bot) {
        assert (!a.empty());
        delta[q].push_back(std::pair<NSIntSet,int>(a, n_states));
      }
    }
    if (add_bot)
      q_bot = n_states++;
  }
  
  forceinline
  compDFA::compDFA(const DFA& d, const NSIntSet& alphabet) : q_bot(-1),
  stringDFA(d), delta(d.n_states()) {
    // std::cerr << d << "\n";
    int chars_count[n_states];
    for (auto& q : chars_count)
      q = 0;
    for (DFA::Transitions t(d); t(); ++t) {
      int q_in = t.i_state(), a = t.symbol(), q_out = t.o_state();
      chars_count[q_in]++;
      std::vector<std::pair<NSIntSet, int>>& delta_q = delta[q_in];
      bool new_state = true;
      for (auto& x : delta_q)
        if (x.second == q_out) {
          x.first.add(a);
          new_state = false;
          break;
        }
      if (new_state)
        delta_q.push_back(std::pair<NSIntSet,int>(NSIntSet(a), q_out));
    }
    bool complete = true;
    for (int i = 0; complete && i < n_states; ++i)
      complete &= chars_count[i] == alphabet.size();
    // Adding a "bottom" state if DFA is not complete.
    if (!complete) {
      delta.push_back(std::vector<std::pair<NSIntSet, int>>());
      NSIntSet s;
      delta[n_states].push_back(std::make_pair(alphabet, n_states));
      for (int i = 0; i < n_states; ++i)
        if (chars_count[i] < alphabet.size()) {
          NSIntSet s(alphabet);
          for (auto& x : delta[i])
            s.exclude(x.first);
          delta[i].push_back(std::pair<NSIntSet,int>(s, n_states));
        }
      q_bot = n_states++;
    }
    // std::cerr << *this << '\n';
  }
  
  forceinline int
  compDFA::nstate(int q) const {
    if (q < final_fst)
      return q;
    else if (q > final_lst)
      return q + final_fst - final_lst - 1;
    else
      return q + n_states - final_lst - 1;
  }
  
  forceinline NSIntSet
  compDFA::alphabet() const {
    NSIntSet s;
    for (int i = 0; i < n_states; i++)
      for (auto& x : delta[i])
        s.include(x.first);
    return s;
  }
  
  forceinline int
  compDFA::search(int q, int c) const {
//    std::cerr << "search(q" << q << ", " << c << ")\n";
    for (auto& x : delta[q])
      if (x.first.in(c))
        return x.second;
    return -1;
  };
  
  forceinline NSIntSet
  compDFA::neighbours(int q) const {
    NSIntSet s;
    for (auto& x : delta[q])
      s.add(x.second);
    return s;
  };
  
  forceinline NSIntSet 
  compDFA::neighbours(int q, const DSIntSet& S) const {
    NSIntSet s;
    for (auto& x : delta[q])
      if (!S.disjoint(x.first))
        s.add(x.second);
    return s;
  };

  forceinline void
  compDFA::negate() {
    if (accepting(0)) {
      final_fst = final_lst + 1;
      final_lst = n_states - 1;
      return;
    }
    Delta_t rdelta(n_states);
    for (int i = 0; i < n_states; i++)
      for (auto& x : delta[i])
        rdelta[nstate(i)].push_back(std::make_pair(x.first, nstate(x.second)));
    delta = rdelta;
    final_lst = n_states - final_lst + final_fst - 2;
    final_fst = 0;
  }

  template <class CtrlView, ReifyMode rm>
  forceinline
  ReReg<CtrlView, rm>::ReReg(Home home, StringView x, compDFA* d, CtrlView c)
  : Propagator(home), x0(x), b(c), dfa(d) {
    b .subscribe (home, *this, Gecode::Int::PC_INT_VAL);
    x0.subscribe (home, *this, PC_STRING_DOM);
  }

  template <class CtrlView, ReifyMode rm>
  forceinline
  ReReg<CtrlView, rm>::ReReg(Space& home, ReReg& p)
  : Propagator(home, p), dfa(p.dfa) {
    x0.update (home, p.x0);
    b .update (home, p.b);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline PropCost
  ReReg<CtrlView, rm>::cost(const Space&, const ModEventDelta&)
  const {
    return PropCost::ternary(PropCost::HI);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline void
  ReReg<CtrlView, rm>::reschedule(Space& home) {
    b .reschedule(home, *this, Gecode::Int::PC_INT_VAL);
    x0.reschedule(home, *this, PC_STRING_DOM);
  }

  template<class CtrlView, ReifyMode rm>
  forceinline size_t
  ReReg<CtrlView, rm>::dispose(Space& home) {
    b .cancel(home, *this, Gecode::Int::PC_INT_VAL);
    x0.cancel(home, *this, PC_STRING_DOM);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }
  
  template <class CtrlView, ReifyMode rm>
  forceinline ExecStatus
  ReReg<CtrlView, rm>::post(Home home, StringView x, compDFA* d, CtrlView b) {
    (void) new (home) ReReg(home, x, d, b);
    return ES_OK;
  }

  template <class CtrlView, ReifyMode rm>
  forceinline ExecStatus
  ReReg<CtrlView, rm>::post(Home home, StringView x, const DFA& d, CtrlView b) {
    NSIntSet S;
    if (d.final_fst() >= d.final_lst()) {
      if (rm != RM_PMI)
        GECODE_ME_CHECK(b.eq(home, 0));
      return ES_OK;
    }
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
        if (qi == -1) {
          GECODE_ME_CHECK(b.eq(home, 0));
          return ES_OK;
        }
        q = qi;
      }
      if (q < d.final_fst() || q >= d.final_lst())
        GECODE_ME_CHECK(b.eq(home, 0));
      else
        GECODE_ME_CHECK(b.eq(home, 1));
      return ES_OK;
    }
    (void) new (home) ReReg(home, x, new compDFA(d, x.may_chars()), b);
    return ES_OK;
  }

  template <class CtrlView, ReifyMode rm>
  Actor*
  ReReg<CtrlView, rm>::copy(Space& home) {
    return new (home) ReReg(home, *this);
  }
  
  template<class CtrlView, ReifyMode rm>
  forceinline ExecStatus
  ReReg<CtrlView, rm>::propagate(Space& home, const ModEventDelta& m) {
//    std::cerr<<"ReDFA::propagate "<<b<<" <> "<<x0<<" in dfa "<<*dfa<<std::endl;
    if (x0.assigned()) {
      if (dfa->accepted(x0.val())) {
        if (rm != RM_IMP)
          GECODE_ME_CHECK(b.eq(home, 1));
        return home.ES_SUBSUMED(*this);
      }
      else {
        if (rm != RM_PMI)
          GECODE_ME_CHECK(b.eq(home, 0));
        return home.ES_SUBSUMED(*this);
      }
    }
    if (b.assigned()) {
      if (b.zero()) {
        if (rm == RM_IMP)
          return home.ES_SUBSUMED(*this);
      }
      else if (rm == RM_PMI)
        return home.ES_SUBSUMED(*this);
    }
    DashedString* x = x0.pdomain();
    std::vector<std::vector<NSIntSet>> F(x->length());
    NSIntSet Fi(0);
    int n = x->length();
    for (int i = 0; i < n; ++i) {
      F[i] = Reg::reach_fwd(dfa, Fi, x->at(i));
      Fi = F[i].back();
//      std::cerr << Fi.toString() << ' ' << dfa->q_bot << "\n";
      if (Fi.size() == 1 && Fi.min() == dfa->q_bot) {
        n = i + 1;
        break;
      }
    }
    NSIntSet E(Fi);
    Fi = dfa->accepting_states();
//    std::cerr << "E = " << E.toString() << ", Fi = " << Fi.toString() << '\n';
    if (Fi.contains(E))
      GECODE_ME_CHECK(b.eq(home, 1));
    if (Fi.comp().contains(E))
      GECODE_ME_CHECK(b.eq(home, 0));
    if (b.one()) {
      if (rm == RM_PMI)
        return home.ES_SUBSUMED(*this);
      E.intersect(Fi);
    }
    else if (b.zero()) {
      if (rm == RM_IMP)
        return home.ES_SUBSUMED(*this);
      E.intersect(Fi.comp());
    }
    else
      return ES_FIX;
    NSBlocks y[n];
//    std::cerr << "E = " << E.toString() << ", comp(Fi) = " << Fi.comp().toString() << '\n';    
    if (E.empty())
      return ES_FAILED;
    bool changed = false;
    int k = 0;
    for (int i = n - 1; i >= 0; --i) {
      y[i] = Reg::reach_bwd(dfa, F[i], E, x->at(i), changed);
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
      // std::cerr<<"ExtDFA<View>::propagated (changed) "<<x0<<"\n\n";
      assert (x0.pdomain()->is_normalized());
      return x0.assigned() ? home.ES_SUBSUMED(*this) : propagate(home, m);
    }
    // std::cerr<<"ExtDFA<View>::propagated (no change) "<<x0<<"\n\n";
    assert (x0.pdomain()->is_normalized());
    return ES_FIX;
  }

  template<class CtrlView, ReifyMode rm>
  forceinline
  ReReg<CtrlView, rm>::~ReReg() {
    delete dfa;
  }

}}
