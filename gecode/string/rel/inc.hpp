namespace Gecode { namespace String {


  forceinline
  Inc::Inc(Home home, StringView x, bool b)
    : UnaryPropagator<StringView, PC_STRING_DOM>
      (home, x), strict(b) {}

  forceinline
  Inc::Inc(Space& home, Inc& p)
    : UnaryPropagator<StringView, PC_STRING_DOM>
      (home, p), strict(p.strict) {}

  forceinline ExecStatus
  Inc::post(Home home, StringView x, bool b) {
    (void) new (home) Inc(home, x, b);
    return ES_OK;
  }

  forceinline Actor*
  Inc::copy(Space& home) {
    return new (home) Inc(home, *this);
  }

  forceinline bool
  Inc::assigned_ok(void) const {
    string bytes;
    if (x0.domain().try_val_bytes(bytes)) {
      for (unsigned int i = 0; i + 1 < bytes.size(); ++i)
        if (bytes[i] > bytes[i + 1] ||
            (strict && bytes[i] == bytes[i + 1]))
          return false;
      return true;
    }

    const StringVal value = x0.domain().val_symbols();
    for (StringVal::size_type i = 0; i + 1 < value.size(); ++i)
      if (value[i] > value[i + 1] ||
          (strict && value[i] == value[i + 1]))
        return false;
    return true;
  }

  forceinline ExecStatus
  Inc::propagate(Space& home, const ModEventDelta&) {
    //  strict ? std::cerr<<"\nInc lt ::propagate "<<x0<<std::endl
    //         : std::cerr<<"\nInc le ::propagate "<<x0<<std::endl;
    if (x0.assigned())
      return assigned_ok() ? home.ES_SUBSUMED(*this) : ES_FAILED;
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
      assert(assigned_ok());
      return home.ES_SUBSUMED(*this);
    }
    assert (x0.domain().is_normalized());
    // std::cerr<<"propagated: "<<x0<<std::endl;
    return ES_FIX;
  }

}}
