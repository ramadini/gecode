namespace Gecode { namespace String {

  
  forceinline
  Inc::Inc(Home home, StringView x, bool b)
    : UnaryPropagator<StringView, PC_STRING_DOM>
      (home, x), strict(b) {}

  forceinline
  Inc::Inc(Space& home, bool share, Inc& p)
    : UnaryPropagator<StringView, PC_STRING_DOM>
      (home, share, p), strict(p.strict) {}

  forceinline ExecStatus
  Inc::post(Home home, StringView x, bool b) {
    (void) new (home) Inc(home, x, b);
    return ES_OK;
  }

  forceinline Actor*
  Inc::copy(Space& home, bool share) {
    return new (home) Inc(home, share, *this);
  }

  forceinline ExecStatus
  Inc::propagate(Space& home, const ModEventDelta&) {
    //  strict ? std::cerr<<"\nInc lt ::propagate "<<x0<<std::endl
    //         : std::cerr<<"\nInc le ::propagate "<<x0<<std::endl;
    if (x0.assigned()) {
      string s = x0.val();
      for (unsigned i = 0; i < s.size() - 1; ++i)
        if (s[i] > s[i + 1] || (strict && s[i] == s[i + 1]))
          return ES_FAILED;
      return home.ES_SUBSUMED(*this);
    }
    if (strict) {
      NSIntSet s(x0.may_chars());
      int m = s.size(), n = x0.min_length();
      if (m < n)
        return ES_FAILED;
      else if (m == n) {
        NSBlocks v;
        for (NSIntSet::iterator it = s.begin(); it(); ++it)
          v.push_back(NSBlock(*it, 1, 1));
        GECODE_ME_CHECK(x0.dom(home, v));
        return home.ES_SUBSUMED(*this);
      }
      else if (x0.max_length() < x0.must_chars().size())
        return ES_FAILED;
    }
    GECODE_ME_CHECK(x0.inc(home, strict));
    if (x0.assigned()) {
      string s = x0.val();
      for (unsigned i = 0; i < s.size() - 1; ++i)
        assert (s[i] < s[i + 1] || (!strict && s[i] == s[i + 1]));
      return home.ES_SUBSUMED(*this);
    }
    assert (x0.pdomain()->is_normalized());
    // std::cerr<<"propagated: "<<x0<<std::endl;
    return ES_FIX;
  }

}}
