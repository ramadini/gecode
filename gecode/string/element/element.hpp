namespace Gecode { namespace String {

  forceinline
  Element::Element(Home home, ViewArray<StringView> v, IntView i)
  : MixNaryOnePropagator
  <StringView, PC_STRING_DOM, IntView, Gecode::Int::PC_INT_DOM>(home, v, i) {}

  forceinline
  Element::Element(Space& home, Element& p) : MixNaryOnePropagator 
  <StringView, PC_STRING_DOM, IntView, Gecode::Int::PC_INT_DOM>
    (home, p) {}

  forceinline ExecStatus
  Element::post(Home home, ViewArray<StringView> a, IntView i, StringView x) {
    if (a.size() == 0)
      return ES_FAILED;
    GECODE_ME_CHECK(i.gq(home, 1));
    GECODE_ME_CHECK(i.lq(home, a.size()));
    if (i.assigned()) {
      rel(home, x, STRT_EQ, a[i.val() - 1]);
      return ES_OK;
    }
    ViewArray<StringView> v(home, a.size() + 1);
    v[0] = x;
    for (int j = 0; j < a.size(); ++j)
      v[j + 1] = a[j];
    (void) new (home) Element(home, v, i);
    return ES_OK;
  }

  forceinline Actor*
  Element::copy(Space& home) {
    return new (home) Element(home, *this);
  }

  forceinline ExecStatus
  Element::propagate(Space& home, const ModEventDelta&) {
    // std::cerr << "Element::propagate "<<x<<"[" << y << "] = "<<x[0]<< "\n";
    if (y.assigned()) {
      rel(home, x[0], STRT_EQ, x[y.val()]);
      return home.ES_SUBSUMED(*this);
    }
    StringView& v = x[0];
    NSIntSet s;
    int l = DashedString::_MAX_STR_LENGTH, u = 0;
    Region region;
    int* removed = region.alloc<int>(y.size());
    int removed_size = 0;
    for (Gecode::Int::ViewValues<IntView> i(y); i(); ++i) {
      StringView& candidate = x[i.val()];
      // std::cerr << "x["<< i.val() << "] = " << vi << "\n";
      if (v.check_equate(candidate)) {
        s.include(candidate.may_chars());
        int li = candidate.min_length(), ui = candidate.max_length();
        if (li < l)
          l = li;
        if (ui > u)
          u = ui;
      }
      else
        removed[removed_size++] = i.val();
    }
    if (removed_size > 0) {
      IntSet removed_set(removed, removed_size);
      IntSetRanges removed_ranges(removed_set);
      GECODE_ME_CHECK(y.minus_r(home, removed_ranges));
    }
    if (y.assigned()) {
      rel(home, x[0], STRT_EQ, x[y.val()]);
      return home.ES_SUBSUMED(*this);
    }
    NSBlocks dom(1, NSBlock(s, l, u));
    ModEvent me = v.dom(home, dom);
    GECODE_ME_CHECK(me);
    // std::cerr << "Element::propagated " << x << "[" << y << "] = " << x[0] << "\n";
    for (auto& v : x)
      assert (v.pdomain()->is_normalized());
    return me_modified(me) ? ES_NOFIX : ES_FIX;
  }

}}
