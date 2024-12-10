#include <gecode/int/bool.hh>

namespace Gecode { namespace String {

  forceinline
  stringCDFA::stringCDFA(const DFA& d, const NSIntSet& alphabet): trimDFA(d) {
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
      for (int i = 0; i < n_states; ++i)
        std::sort(delta[i].begin(), delta[i].end());
    }
    ua = ur = -1;
  }

  forceinline void
  stringCDFA::negate() {
    if (accepting(0)) {
      final_fst = final_lst + 1;
      final_lst = n_states - 1;
      for (int i = 0; i < n_states; ++i)
        std::sort(delta[i].begin(), delta[i].end());
      int tmp = ur;
      ur = ua;
      ua = tmp;
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

  template <class CtrlView, ReifyMode rm>
  forceinline
  ReReg<CtrlView, rm>::ReReg(Home home, StringView x, stringCDFA* d, CtrlView c)
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
    (void) new (home) ReReg(home, x, new stringCDFA(d, x.may_chars()), b);
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
    // std::cerr<<"ReDFA::propagate "<<b<<" <> "<<x0<<" in dfa "<<*dfa<<std::endl;
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
        stringCDFA* ndfa = new stringCDFA(*dfa);
        ndfa->negate();
        // std::cerr << "Negated DFA: " << *dfa << '\n';
        GECODE_REWRITE(*this, (Reg::post(home(*this), x0, ndfa)));
      }
      else if (rm == RM_PMI)
        return home.ES_SUBSUMED(*this);
      GECODE_REWRITE(*this, (Reg::post(home(*this), x0, dfa)));
    }
    // std::cerr<<"ReDFA::propagated "<<b<<" <> "<<x0<<std::endl;
    dfa->compute_univ(x0.may_chars());
    DashedString* x = x0.pdomain();
    std::vector<std::vector<NSIntSet>> F(x->length());
    NSIntSet Fi(0);
    int n = x->length();
    for (int i = 0; i < n; ++i) {
      F[i] = Reg::reach_fwd(dfa, Fi, x->at(i), true);
      Fi = F[i].back();
      if (dfa->univ_rejected(Fi)) {
        GECODE_ME_CHECK(b.eq(home, 0));
        return propagate(home, m);
      }
      if (dfa->univ_accepted(Fi)) {
        GECODE_ME_CHECK(b.eq(home, 1));
        return propagate(home, m);
      }
    }
    NSIntSet E(F.back().back());
    Fi = dfa->accepting_states();
    if (Fi.contains(E)) {
      GECODE_ME_CHECK(b.eq(home, 1));
      return propagate(home, m);
    }
    if (Fi.comp().contains(E)) {
      GECODE_ME_CHECK(b.eq(home, 0));
      return propagate(home, m);
    }
    if (b.one())
      E.intersect(Fi);
    else if (b.zero())
      E.intersect(Fi.comp());
    else
      return ES_FIX;
    NSBlocks y[n];
    E.intersect(Fi);
    if (E.empty())
      return ES_FAILED;
    // std::cerr << E << '\n';
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
