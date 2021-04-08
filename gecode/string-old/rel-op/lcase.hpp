namespace Gecode { namespace String {

  forceinline ExecStatus
  LowerCase::post(Home home, StringView x0, StringView x1) {
    if (x0.same(x1)) {
      StringVar y(home, x0.may_chars(), x0.min_length(), x0.max_length());
      (void) new (home) LowerCase(home, x0, y);
      rel (home, x1, STRT_EQ, y);
    }
    else
      (void) new (home) LowerCase(home, x0, x1);
    return ES_OK;
  }

  forceinline Actor*
  LowerCase::copy(Space& home) {
    return new (home) LowerCase(home, *this);
  }

  forceinline
  LowerCase::LowerCase(Home home, StringView y0, StringView y1)
  : MixBinaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>
    (home, y0, y1) {}

  forceinline
  LowerCase::LowerCase(Space& home, LowerCase& p)
  : MixBinaryPropagator<StringView, PC_STRING_DOM,
    StringView, PC_STRING_DOM>(home, p) {}

  forceinline ExecStatus
  LowerCase::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"LowerCase::propagate "<<x1<<" = lcase("<<x0<<")\n";
    int n = x0.pdomain()->length();
    NSBlocks lx(n);
    bool changed = false;
    for (int i = 0; i < n; ++i) {
      const DSBlock& bi = x0.pdomain()->at(i);
	    NSIntSet si(bi.S);
      if (!(bi.S.disjoint(String::UpperCase::_UCASE_SET))) {
        NSIntSet ti(si);
        ti.intersect(String::UpperCase::_UCASE_SET);
        si.exclude(ti);
        ti.shift(32);
        si.include(ti);
        changed = true;
      }
      lx[i] = NSBlock(si, bi.l, bi.u);
	  }
    if (changed)
      lx.normalize();
    GECODE_ME_CHECK(x1.dom(home, lx));
    changed = false;
    n = x1.pdomain()->length();
    NSBlocks uy(n);
    for (int i = 0; i < n; ++i) {
      const DSBlock& bi = x1.pdomain()->at(i);
      NSIntSet si(bi.S);
      if (!(bi.S.disjoint(String::UpperCase::_LCASE_SET))) {
        NSIntSet ti(si);
        ti.intersect(String::UpperCase::_LCASE_SET);
        ti.shift(-32);
        si.include(ti);
        changed = true;
      }
      uy[i] = NSBlock(si, bi.l, bi.u);
    }
    if (changed)
      uy.normalize();
    GECODE_ME_CHECK(x0.dom(home, uy));
    // std::cerr<<"LowerCase::propagated "<<x1<<" = lcase("<<x0<<")\n";
    assert (x0.pdomain()->is_normalized() && x1.pdomain()->is_normalized());
    return ES_FIX;
  }

}}
