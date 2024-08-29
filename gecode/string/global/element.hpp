namespace Gecode { namespace String {

  forceinline
  Element::Element(Home home, ViewArray<StringView> v, IntView i)
  : MixNaryOnePropagator
  <StringView, PC_STRING_DOM, IntView, Gecode::Int::PC_INT_BND>(home, v, i) {}

  forceinline
  Element::Element(Space& home, Element& p) : MixNaryOnePropagator 
  <StringView, PC_STRING_DOM, IntView, Gecode::Int::PC_INT_BND>
    (home, p) {}

  forceinline ExecStatus
  Element::post(Home home, ViewArray<StringView> a, IntView i, StringView x) {
    ViewArray<StringView> v(home, a.size() + 1);
    v[0] = x;
    for (int j = 0; j < a.size(); ++j)
      v[j + 1] = a[j];
    rel(home, i, IRT_GQ, 1);
    rel(home, i, IRT_LQ, a.size());
    (void) new (home) Element(home, v, i);
    return ES_OK;
  }

  forceinline Actor*
  Element::copy(Space& home) {
    return new (home) Element(home, *this);
  }

  forceinline ExecStatus
  Element::propagate(Space& home, const ModEventDelta& m) {
    // std::cerr << "Element::propagate "<<x<<"[" << y << "] = "<<x[0]<< "\n";
    if (y.assigned()) {
      rel(home, x[0], STRT_EQ, x[y.val()]);
      return home.ES_SUBSUMED(*this);
    }
    StringView& v = x[0];
    NSIntSet s;
    int l = DashedString::_MAX_STR_LENGTH, u = 0;
    std::vector<int> remove;
    for (IntVarValues i(y); i(); ++i) {
      StringView& vi = x[i.val()];
      // std::cerr << "x["<< i.val() << "] = " << vi << "\n";
      if (v.check_equate(vi)) {
        s.include(vi.may_chars());
        int li = vi.min_length(), ui = vi.max_length();
        if (li < l)
          l = li;
        if (ui > u)
          u = ui;
      }
      else
        remove.push_back(i.val());
    }
    // std::cerr<<"remove = ["; for(auto& x:remove) std::cerr<<"\""<<x<< "\", "; std::cerr<<"]\n";
    IntSet is(IntSet(&remove[0], remove.size()));
    IntSetRanges r(is);
    GECODE_ME_CHECK(y.minus_r(home, r));
    NSBlocks dom(1, NSBlock(s, l, u));
    GECODE_ME_CHECK(v.dom(home, dom));
    // std::cerr << "Element::propagated " << x << "[" << y << "] = " << x[0] << "\n";
    for (auto& v : x)
      assert (v.pdomain()->is_normalized());
    return y.assigned() ? propagate(home, m) : ES_FIX;
  }

}}
