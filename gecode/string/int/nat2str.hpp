namespace Gecode { namespace String {

  forceinline
  NatToStr::NatToStr(Home home, Gecode::Int::IntView x, StringView y)
  : MixBinaryPropagator
  <Gecode::Int::IntView, Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM>
    (home, x, y) {}

  forceinline
  NatToStr::NatToStr(Space& home, NatToStr& p)
  : MixBinaryPropagator
  <Gecode::Int::IntView, Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM>
    (home, p) {}

  forceinline ExecStatus
  NatToStr::post(Home home, Gecode::Int::IntView x, StringView y) {
    // std::cerr << "NatToStr::post " << x << " <-> " << y << '\n';
    extensional(home, y, "(()|0|[1-9][0-9]*)");
    if (x.max() < 0)
      y.eq(home, "");
    else
      (void) new (home) NatToStr(home, x, y);
    return ES_OK;
  }
  
  forceinline Actor*
  NatToStr::copy(Space& home) {
    return new (home) NatToStr(home, *this);
  }

  forceinline NSBlocks
  NatToStr::strdom() const {
    int lb = max(0, x0.min()), ub = x0.max();
    string ls = std::to_string(lb), us = std::to_string(ub);
    int lsize = ls.size(), usize = us.size();
    int n = usize - lsize;
    NSBlocks dom;
    bool eps = x1.min_length() == 0;
    if (n > 0) {
      dom.push_back(NSBlock(NSIntSet('1', us[0]), 0, 1));
      if (n > 1) {
        dom.push_back(NSBlock(NSIntSet('0', '9'), 0, n - 1));
        if (eps)
          dom.back().u += lsize;
        else {
          dom.back().l += lsize;
          dom.back().u += lsize;
        }
      }
      else if (eps)
        dom.push_back(NSBlock(NSIntSet('0', '9'), 0, lsize));
      else
        dom.push_back(NSBlock(NSIntSet('0', '9'), lsize, lsize));
    }
    else {
      if (eps) {
        dom.push_back(NSBlock(NSIntSet(ls[0], us[0]), 0, 1));
        if (lsize > 1)
          dom.push_back(NSBlock(NSIntSet('0', '9'), 0, lsize - 1));
      }
      else {
        dom.push_back(NSBlock(NSIntSet(ls[0], us[0]), 1, 1));
        if (lsize > 1) {
          int m = lsize - 1;
          if (ls[0] != '0' || us[0] != '9')
            dom.push_back(NSBlock(NSIntSet('0', '9'), m, m));
          else {
            dom.back().l += m;
            dom.back().u += m;
          }
        }
      }
    }
    // std::cerr << dom << '\n';
    return dom;
  }

  forceinline string
  NatToStr::min_str() const {
    DashedString* p = x1.pdomain();
    string n = string(p->at(0).l, int2char(p->at(0).S.min()));
    for (int i = 1; i < p->length(); ++i) {
      const DSBlock& b = p->at(i);
      if (b.l > 0)
        n += string(b.l, int2char(b.S.min()));
    }
    return n == "" ? std::to_string(Gecode::Int::Limits::min) : n;
  }
  
  forceinline string
  NatToStr::max_str() const {
    DashedString* p = x1.pdomain();
    string n = string(p->at(0).u, int2char(p->at(0).S.max()));
    for (int i = 1; i < p->length(); ++i) {
      const DSBlock& b = p->at(i);
      n += string(b.u, int2char(b.S.max()));
    }
    return n == "" ? "-1" : n;
  }

  forceinline ExecStatus
  NatToStr::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"\nNatToStr::propagate "<<x0<<" => "<<x1<<std::endl;
    if (x0.assigned()) {
      int n = x0.val();
      if (n < 0)
        GECODE_ME_CHECK(x1.eq(home, ""));
      else
    	  GECODE_ME_CHECK(x1.eq(home, std::to_string(n)));
      return home.ES_SUBSUMED(*this);
    }
    if (x1.assigned()) {
      string s = x1.val();
      if (s.empty())
        GECODE_ME_CHECK(x0.lq(home, -1));
      else
        GECODE_ME_CHECK(x0.eq(home, std::stoi(s)));
      return home.ES_SUBSUMED(*this);
    }
    if (x0.max() <= -1) {
      x1.eq(home, "");
      return home.ES_SUBSUMED(*this);
    }
    NSBlocks dom = strdom();
    GECODE_ME_CHECK(x1.dom(home, dom));
    if (x0.min() > -1 || x1.min_length() > 0) {
      GECODE_ME_CHECK(x0.gq(home, 0));
      if (x0.min() == 0)
        GECODE_ME_CHECK(x1.lb(home, 1));
      else {
        GECODE_ME_CHECK(x1.lb(home, (int) std::log10(x0.min()) + 1));
        rel(home, x1, STRT_NQ, StringVar(home, "0"));
      }
      GECODE_ME_CHECK(x1.ub(home, ((int) std::log10(x0.max())) + 1));
      try {
        GECODE_ME_CHECK(x0.gq(home, std::stoi(min_str())));
        GECODE_ME_CHECK(x0.lq(home, std::stoi(max_str())));
      } catch (const std::out_of_range&) {}
      if (x1.pdomain()->length() == 1 && x1.pdomain()->at(0).S.size() == 1) {
        const DSBlock& b = x1.pdomain()->at(0);
        char a = b.S.min();
        IntArgs ia(b.u - b.l - 1);
        for (int i = b.l; i <= b.u; ++i)
          ia[i] = std::stoi(string(i, a));
        IntSet is(ia);
        IntSetRanges isr(is);
        x0.inter_r(home, isr);
      }
    }
    else {
      if (x0.max() > 0)
        GECODE_ME_CHECK(x1.ub(home, ((int) std::log10(x0.max())) + 1));
      else {
        NSBlocks d({NSBlock(NSIntSet('0'), 0, 1)});
        rel(home, x1, STRT_DOM, d, 0, 1);
      }
      try {
        GECODE_ME_CHECK(x0.lq(home, std::stoi(max_str())));
      } 
      catch (const std::out_of_range&) {}
    }
    // std::cerr<<"\nNatToStr::propagated "<<x0<<" => "<<x1<<std::endl;
    assert (x1.pdomain()->is_normalized());
    return x0.assigned() || x1.assigned() ? ES_NOFIX : ES_FIX;
  }

}}
