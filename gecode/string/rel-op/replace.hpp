namespace Gecode { namespace String {

  // x[3] is the string resulting from replacing the first occurrence of x[0] 
  // with x[1] in the target string x[2].

  forceinline ExecStatus
  Replace::post(Home home, ViewArray<StringView>& x) {
    if (x[0].same(x[1])) {
      rel(home, x[2], STRT_EQ, x[3]);
      return ES_OK;
    }
    else if (x[0].same(x[2])) {
      rel(home, x[1], STRT_EQ, x[3]);
      return ES_OK;
    }
    else if (x[0].same(x[3]))
      rel(home, x[0], STRT_SUB, x[1]);
    (void) new (home) Replace(home, x);
    return ES_OK;
  }

  forceinline Actor*
  Replace::copy(Space& home) {
    return new (home) Replace(home, *this);
  }

  forceinline
  Replace::Replace(Home home, ViewArray<StringView>& x)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, x) {}

  forceinline
  Replace::Replace(Space& home, Replace& p)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, p) {}

  forceinline ExecStatus
  Replace::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"\nReplace::propagate: "<< x <<"\n";
    assert(x[0].pdomain()->is_normalized() && x[1].pdomain()->is_normalized() &&
           x[2].pdomain()->is_normalized() && x[3].pdomain()->is_normalized());
    if (x[0].assigned()) {
      string sx = x[0].val();
      if (sx == "") {
        rel(home, x[1], x[2], STRT_CAT, x[3]);
        return home.ES_SUBSUMED(*this);
      }
      if (x[2].assigned()) {
        string sy = x[2].val();
        int n = sy.find(sx);
        string pref = sy.substr(0, n);
        string suff = sy.substr(n + sx.size());
        if (x[1].assigned())
          GECODE_ME_CHECK(x[3].eq(home, pref + x[1].val() + suff));
        else {
          StringVar z(home);
          rel(home, StringVar(home, pref), x[1], STRT_CAT, z);
          rel(home, z, StringVar(home, suff), STRT_CAT, x[3]);
        }
        return home.ES_SUBSUMED(*this);
      }
      // Compute fixed components of x[2] to see if x[0] occurs in it.
    }
    // Check if find(x, y) = 0
    // Check if find(x', y') = 0
    // Equate on y and y'    
    return x[0].assigned() && x[1].assigned() ? ES_NOFIX : ES_FIX;
    // std::cerr<<"After replace: "<< x <<"\n";
    return ES_FIX;
  }

}}
