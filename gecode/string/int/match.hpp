namespace Gecode { namespace String {

  forceinline
  Match::Match(Home home, StringView x, Gecode::Int::IntView i, stringDFA* R, 
    stringDFA* R1, int r)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>
    (home, x, i), sRs(R), Rs(R1), minR(r)  {}

  forceinline ExecStatus
  Match::post(Home home, StringView x, string re, Gecode::Int::IntView i) {    
    String::RegEx* regex = RegExParser(".*(" + re + ").*").parse();
    if (regex->has_empty()) {
      rel(home, i, IRT_EQ, 1);
      return ES_OK;
    }
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
    assert (r > 0);
    if (i.gr(home, x.max_length() - r + 1) == Int::ME_INT_FAILED)
      return ES_FAILED;    
    stringDFA* R1 = new stringDFA(RegExParser(re + ".*").parse()->dfa());
    (void) new (home) Match(home, x, i, R, R1, r);
    return ES_OK;
  }

  forceinline
  Match::Match(Space& home, Match& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> 
    (home, p), sRs(p.sRs), Rs(p.Rs), minR(p.minR) {}

  forceinline Actor*
  Match::copy(Space& home) {
    return new (home) Match(home, *this);
  }

  forceinline std::pair<NSIntSet,int>
  Match::checkBlock(const DSBlock& b, const NSIntSet& Q) {
    int l = b.l, j = 0;
    NSIntSet Q_prev = Q;
    // Mandatory region.
    for (int i = 0; i < l; ++i) {
      NSIntSet Qi;
      for (NSIntSet::iterator it(Q_prev); it(); ++it)
        Qi.include(sRs->neighbours(*it, b.S));
      if (Qi.max() == 0)
        // All paths lead to q0.
        j = i;
      else if (Qi.in(1))
        // Final state q1 belongs to Qi.
        return std::make_pair(Qi, j);
      if (Qi == Q_prev)
        // Fixpoint.
        return std::make_pair(Qi, Qi.max() == 0 ? l : j);
      Q_prev = Qi;
    }
    // BFS over optional region.
    int dist[sRs->n_states];
    for (int q = 0; q < sRs->n_states; ++q)
      dist[q] = Q_prev.contains(q) ? l : DashedString::_MAX_STR_LENGTH + 1;
    std::list<int> U;
    for (NSIntSet::iterator i(Q_prev); i(); ++i)
      U.push_back(*i);    
    while (!U.empty()) {
      int q = U.front(), d = dist[q] + 1;
      U.pop_front();
      if (d <= b.u) {
        NSIntSet Nq = sRs->neighbours(q, b.S);
        if (Nq.in(1))
          // Final state in the neighbourhood of q.
          return std::make_pair(Nq, j);
        for (NSIntSet::iterator j(Nq); j(); ++j) {
          int q1 = *j;
          if (dist[q1] > d) {
            U.push_back(q1);
            dist[q1] = d;
          }
        }
      }
    }
    NSIntSet Qf;
    for (int q = 0; q < sRs->n_states; ++q)
      if (dist[q] <= DashedString::_MAX_STR_LENGTH)
        Qf.add(q);
    return std::make_pair(Qf, j);
  }

  forceinline ExecStatus
  Match::propagate(Space& home, const ModEventDelta& med) {
    //FIXME: To handle the case where x0 is fixed, we need to keep track of the 
    // original regex and take into account C++ regex syntax/semantics.
    if (x1.assigned()) {
      int k = x1.val();
      sRs->negate(x0.may_chars());
      if (k == 0)
        GECODE_REWRITE(*this, Reg::post(home, x0, sRs));
      // Rewrite into x = yz, |y| = k-1, ¬regular(y, sRs), regular(z, Rs)
      StringVar y(home), z(home);
      rel(home, y, z, STRT_CAT, x0);
      k--;
      length(home, y, IntVar(home, k, k));      
      Reg::post(home, y, sRs);
      Reg::post(home, z, Rs);
      return home.ES_SUBSUMED(*this);
    }
    // TODO

    
//    class RegProp : public Reg {
//    public:
//      RegProp(Home home, StringView x, stringDFA* d) :
//        Reg(home, x, d) {};
//    };
//    RegProp p1(home, x0, sRs);
//    RegProp p2(home, x0, Rs);
//    p1.propagate(home, med);
//    p2.propagate(home, med);
//  FIXME: home.ES_SUBSUMED(p1); home.ES_SUBSUMED(p2); ???
    assert (x0.pdomain()->is_normalized());
    return ES_FIX;
  }
  
  forceinline
  Match::~Match() {
    delete sRs;
    delete Rs;
  }

}}
