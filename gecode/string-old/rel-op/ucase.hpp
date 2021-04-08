namespace Gecode { namespace String {

  forceinline ExecStatus
  UpperCase::post(Home home, StringView x0, StringView x1) {
    if (x0.same(x1)) {
      StringVar y(home, x0.may_chars(), x0.min_length(), x0.max_length());
      (void) new (home) UpperCase(home, x0, y);
      rel (home, x1, STRT_EQ, y);
    }
    else
      (void) new (home) UpperCase(home, x0, x1);
    return ES_OK;
  }

  forceinline Actor*
  UpperCase::copy(Space& home) {
    return new (home) UpperCase(home, *this);
  }

  forceinline
  UpperCase::UpperCase(Home home, StringView y0, StringView y1)
  : MixBinaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>
    (home, y0, y1) {}

  forceinline
  UpperCase::UpperCase(Space& home, UpperCase& p)
  : MixBinaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>
    (home, p) {}

  forceinline ExecStatus
  UpperCase::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"UpperCase::propagate "<<x1<<" = ucase("<<x0<<")\n";
    int n = x0.pdomain()->length();
    NSBlocks ux(n);
    bool changed = false;
    for (int i = 0; i < n; ++i) {
      const DSBlock& bi = x0.pdomain()->at(i);
      NSIntSet si(bi.S);
      if (!(bi.S.disjoint(_LCASE_SET))) {
        NSIntSet ti(si);
        ti.intersect(_LCASE_SET);
        si.exclude(ti);
        ti.shift(-32);
        si.include(ti);
        changed = true;
      }
      ux[i] = NSBlock(si, bi.l, bi.u);
    }
    if (changed)
      ux.normalize();
    GECODE_ME_CHECK(x1.dom(home, ux));
    changed = false;
    n = x1.pdomain()->length();
    NSBlocks ly(n);
    for (int i = 0; i < n; ++i) {
      const DSBlock& bi = x1.pdomain()->at(i);
      NSIntSet si(bi.S);
      if (!(bi.S.disjoint(_UCASE_SET))) {
        NSIntSet ti(si);
        ti.intersect(_UCASE_SET);
        ti.shift(32);
        si.include(ti);
        changed = true;
      }
      ly[i] = NSBlock(si, bi.l, bi.u);
    }
    if (changed)
      ly.normalize();
    GECODE_ME_CHECK(x0.dom(home, ly));
    // std::cerr<<"UpperCase::propagated "<<x1<<" = ucase("<<x0<<")\n";
    assert (x0.pdomain()->is_normalized() && x1.pdomain()->is_normalized());
    return ES_FIX;
  }

}}
