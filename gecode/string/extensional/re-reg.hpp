#include <gecode/int/bool.hh>

namespace Gecode { namespace String {

  template<class Char, class Traits>
  forceinline std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const compDFA& d) {
    os << '[';
    for (int i = 0; i < d.n_states; i++) {
      for (auto& x : d.delta[i]){
        NSIntSet S = x.first;
        os << "(q" << i << ", " << S.toString() << ", q" << x.second << "), ";
      }
    }
    return os << "], |Q| = " << d.n_states << ", q_bot: " << d.q_bot << ", F: "
                             << d.accepting_states().toString();
  }

  forceinline
  compDFA::compDFA(const trimDFA& d, const NSIntSet& alphabet)
    : stringDFA(d), q_bot(-1), delta(d.n_states) {
//    std::cerr << d << "\n";
    bool add_bot = false;
    std::vector<NSIntSet> labels(n_states);
    for (int q = 0; q < d.n_states; ++q) {
      const std::vector<std::pair<int, int>>& delta_q = d.delta[q];
      for (auto& x : delta_q)
        if (alphabet.in(x.first))
          labels[x.second].add(x.first);
      NSIntSet missing(alphabet);
      for (int i = 0; i < n_states; ++i)
        missing.exclude(labels[i]);
      for (int i = 0; i < d.n_states; ++i)
        if (!labels[i].empty())
          delta[q].emplace_back(std::move(labels[i]), i);
      if (!missing.empty()) {
        add_bot = true;
        delta[q].emplace_back(std::move(missing), n_states);
      }
    }
    if (add_bot) {
      delta.emplace_back();
      delta.back().emplace_back(alphabet, n_states);
      q_bot = n_states++;
    }
//    std::cerr << *this << '\n';
  }
  
  forceinline
  compDFA::compDFA(const DFA& d, const NSIntSet& alphabet)
    : stringDFA(d), q_bot(-1), delta(d.n_states()) {
//    std::cerr << d << "\n";
    for (DFA::Transitions t(d); t(); ++t) {
      int q_in = t.i_state(), a = t.symbol(), q_out = t.o_state();
      if (!alphabet.in(a))
        continue;
      std::vector<std::pair<NSIntSet, int>>& delta_q = delta[q_in];
      bool new_state = true;
      for (auto& x : delta_q)
        if (x.second == q_out) {
          x.first.add(a);
          new_state = false;
          break;
        }
      if (new_state)
        delta_q.emplace_back(NSIntSet(a), q_out);
    }
    bool add_bot = false;
    for (int q = 0; q < n_states; ++q) {
      NSIntSet missing(alphabet);
      for (auto& x : delta[q])
        missing.exclude(x.first);
      if (!missing.empty()) {
        add_bot = true;
        delta[q].emplace_back(std::move(missing), n_states);
      }
    }
    if (add_bot) {
      delta.emplace_back();
      delta.back().emplace_back(alphabet, n_states);
      q_bot = n_states++;
    }
//    std::cerr << *this << '\n';
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
        rdelta[nstate(i)].emplace_back(
          std::move(x.first), nstate(x.second));
    if (q_bot >= 0)
      q_bot = nstate(q_bot);
    delta = std::move(rdelta);
    final_lst = n_states - final_lst + final_fst - 2;
    final_fst = 0;
  }

  template <class CtrlView, ReifyMode rm>
  forceinline
  ReReg<CtrlView, rm>::ReReg(Home home, StringView x, compDFA* d, CtrlView c)
  : Propagator(home), x0(x), b(c), dfa(d) {
    home.notice(*this, AP_DISPOSE);
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
    home.ignore(*this, AP_DISPOSE);
    b .cancel(home, *this, Gecode::Int::PC_INT_VAL);
    x0.cancel(home, *this, PC_STRING_DOM);
    dfa.~CompDFAHandle();
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
        //FIXME: Transform into trimDFA if corresponding delta is smaller?
      }
      else if (rm == RM_PMI)
        return home.ES_SUBSUMED(*this);
      //FIXME: Transform into trimDFA if corresponding delta is smaller?
    }
    DashedString* x = x0.pdomain();
    std::vector<std::vector<NSIntSet>> F(x->length());
    NSIntSet initial(0);
    const NSIntSet* states = &initial;
    int n = x->length();
    for (int i = 0; i < n; ++i) {
      F[i] = Reg::reach_fwd(dfa.get(), *states, x->at(i));
      if (F[i].empty()) {
        GECODE_ME_CHECK(b.eq(home, 0));
        return home.ES_SUBSUMED(*this);
      }
      states = &F[i].back();
      if (states->size() == 1 && states->min() == dfa->q_bot) {
        n = i + 1;
        break;
      }
    }
    NSIntSet E(*states);
    NSIntSet accepting = dfa->accepting_states();
    NSIntSet rejecting;
    bool have_rejecting = false;
    if (accepting.contains(E))
      GECODE_ME_CHECK(b.eq(home, 1));
    else {
      rejecting = accepting.comp();
      have_rejecting = true;
      if (rejecting.contains(E))
        GECODE_ME_CHECK(b.eq(home, 0));
    }
    if (b.one()) {
      if (rm == RM_PMI)
        return home.ES_SUBSUMED(*this);
      E.intersect(accepting);
    }
    else if (b.zero()) {
      if (rm == RM_IMP)
        return home.ES_SUBSUMED(*this);
      if (!have_rejecting)
        rejecting = accepting.comp();
      E.intersect(rejecting);
    }
    else
      return ES_FIX;
    std::vector<NSBlocks> y(n);
    if (E.empty())
      return ES_FAILED;
    bool changed = false;
    for (int i = n - 1; i >= 0; --i)
      y[i] = Reg::reach_bwd(dfa.get(), F[i], E, x->at(i), changed);
    if (changed) {
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
      int old_min_length = x0.min_length();
      int old_max_length = x0.max_length();
      z.empty() ? x->set_null(home) : x->update(home, z);
      GECODE_ME_CHECK(x0.varimp()->notify(
        home, old_min_length, old_max_length));
      assert (x0.pdomain()->is_normalized());
      return x0.assigned() ? home.ES_SUBSUMED(*this) : propagate(home, m);
    }
    assert (x0.pdomain()->is_normalized());
    return ES_FIX;
  }

}}
