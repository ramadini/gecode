namespace Gecode { namespace String {

  forceinline
  StrToNat::StrToNat(Home home, StringView x, Gecode::Int::IntView y)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>
    (home, x, y) {}

  forceinline
  StrToNat::StrToNat(Space& home, StrToNat& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>
    (home, p) {}
      
  forceinline ExecStatus
  StrToNat::post(Home home, StringView x, Gecode::Int::IntView y) {
    GECODE_ME_CHECK(y.gq(home, -1));
    NSBlocks domain({
      NSBlock(NSIntSet('0', '9'), 1, DashedString::_MAX_STR_LENGTH)
    });
    if (x.pdomain()->check_equate(domain))
      (void) new (home) StrToNat(home, x, y);
    else
      GECODE_ME_CHECK(y.eq(home, -1));
    return ES_OK;
  }

  forceinline Actor*
  StrToNat::copy(Space& home) {
    return new (home) StrToNat(home, *this);
  }

  forceinline NSBlocks
  StrToNat::strdom() const {
    int lb = max(0, x1.min()), ub = x1.max();
    string ls = std::to_string(lb), us = std::to_string(ub);
    int lsize = ls.size(), usize = us.size();
    NSBlocks dom(1, NSBlock(NSIntSet('0'), 0, x0.max_length() - 1));
    int n = usize - lsize;
    if (n > 0) {
      dom.push_back(NSBlock(NSIntSet('1', us[0]), 0, 1));
      if (n > 1)
        dom.push_back(NSBlock(NSIntSet('0', '9'), 0, n - 1));
      else
        dom.push_back(NSBlock(NSIntSet('0', '9'), lsize, lsize + n - 1));
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
    // std::cerr << dom << '\n';
    return dom;
  }

  forceinline string
  StrToNat::min_str() const {
    DashedString* p = x0.pdomain();
    string n = "";
    if (int2char(p->at(0).min()) != '0')
      n = string(p->at(0).l, int2char(p->at(0).S.min()));
    for (int i = 1; i < p->length(); ++i) {
      const DSBlock& b = p->at(i);
      if (b.l > 0)
        n += string(b.l, int2char(b.S.min()));
    }
    return n == "" ? "0" : n;
  }

  forceinline string
  StrToNat::max_str() const {
    DashedString* p = x0.pdomain();
    string n = "";
    if (int2char(p->at(0).max()) != '0')
      n = string(p->at(0).u, int2char(p->at(0).S.max()));
    for (int i = 1; i < p->length(); ++i) {
      const DSBlock& b = p->at(i);
      n += string(b.u, int2char(b.S.max()));
    }
    return n == "" ? "0" : n;
  }

  forceinline ExecStatus
  StrToNat::refine_int(Space& home) {
    int lx = x1.min(), ux = x1.max(), n = x0.pdomain()->length();
    string lb = "", ub = "";
    bool num_only = true;
    const NSIntSet num('0', '9');
	  for (int i = 0; i < n; ++i) {
	    const DSBlock& b = x0.pdomain()->at(i);	    
      if (num.disjoint(b.S)) {
        if (b.l > 0)
          GECODE_ME_CHECK(x1.eq(home, -1));
        num_only = false;
        break;
      }
      else {
        NSIntSet N(b.S);
        N.intersect(num);
        ub += string(b.u, N.max());
    	  if (num_only && b.l > 0 && b.S.min() >= '0' && b.S.max() <= '9')
          lb += string(b.l, N.min());
       	else
          num_only = false;
      }
	  }
	  if (lb != "") {
      long l = std::stol(lb);
      if (num_only && l > lx)
        GECODE_ME_CHECK(x1.gq(home, (long long) l));
	  }
	  if (ub != "") {
      try {
        long u = std::stol(ub);
        if (u < ux)
          GECODE_ME_CHECK(x1.lq(home, (long long) u));
      }
      catch (...) {};
	  }
    return ES_OK;
  }

  forceinline ExecStatus
  StrToNat::propagate(Space& home, const ModEventDelta& m) {
    // std::cerr<<"\nStrToNat::propagate "<<x0<<" => "<<x1<<std::endl;
    if (x0.assigned()) {
      try {
        GECODE_ME_CHECK(x1.eq(home, (int) max(-1, std::stoi(x0.val()))));
      }
      catch (...) {
        GECODE_ME_CHECK(x1.eq(home, -1));
      }
      // std::cerr << "x1 = " << x1 << "\n";
      return home.ES_SUBSUMED(*this);
    }
    if (x1.assigned()) {
      if (x1.val() > -1) {
        NSBlocks val({
          NSBlock(NSIntSet('0'), 0, x0.max_length())
        });
        string s = std::to_string(x1.val());
        val.extend(NSBlocks(s));
        int n = s.size();
        rel(home, x0, STRT_DOM, val, n, n + x0.max_length());
      }
      else
        extensional(home, x0, "[0-9][0-9]*", BoolVar(home, 0, 0), RM_EQV);
      // std::cerr << "x0 = " << x0 << "\n";
      return home.ES_SUBSUMED(*this);
    }
    NSBlocks dom = strdom();
    // std::cerr << dom << '\n';
    if (x1.min() > -1
    || dom.contains<DSBlock, DSBlocks>(x0.pdomain()->blocks())) {
      if (x1.min() > 0)
        GECODE_ME_CHECK(x0.lb(home, (int) ceil(std::log10(x1.min()))));
      GECODE_ME_CHECK(x0.dom(home, dom));
      try {
        GECODE_ME_CHECK(x1.gq(home, std::stoi(min_str())));
        GECODE_ME_CHECK(x1.lq(home, std::stoi(max_str())));
      } 
      catch (const std::out_of_range&) {}
    }
    else if (x1.max() == -1 || !x0.pdomain()->check_equate(dom)) {
      extensional(home, x0, "[0-9][0-9]*", BoolVar(home, 0, 0), RM_EQV);
      // std::cerr << x0 << " => -1 (subsumed)\n";
      return home.ES_SUBSUMED(*this);
    }
    GECODE_ES_CHECK(refine_int(home));
    // std::cerr<<"\nStrToNat::propagated "<<x0<<" => "<<x1<<std::endl;
    assert (x0.pdomain()->is_normalized());
    return x0.assigned() || x1.assigned() ? propagate(home, m) : ES_FIX;
  }

}}
