namespace Gecode { namespace String {

  /// False proves every ASCII-digit member fits native int; true is uncertain.
  forceinline bool
  str2nat_may_overflow(const DashedString& domain) {
    int value = 0;
    const NSIntSet digits('0', '9');
    for (int i = 0; i < domain.length(); ++i) {
      const DSBlock& block = domain.at(i);
      NSIntSet allowed(block.S);
      allowed.intersect(digits);
      if (allowed.empty()) {
        if (block.l > 0)
          return false;
        continue;
      }
      const int digit = allowed.max() - '0';
      if ((value == 0) && (digit == 0))
        continue;
      // Maximum digits and repetitions bound every member from above,
      // even when aggregate length bounds exclude this particular word.
      for (int j = 0; j < block.u; ++j) {
        if (value > (INT_MAX - digit) / 10)
          return true;
        value = 10 * value + digit;
      }
    }
    return false;
  }

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
    if (x.domain().check_equate(domain))
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
    if (usize > lsize) {
      // Shorter values may start with any digit, even above us[0].
      dom.push_back(NSBlock(NSIntSet('0', '9'), 1, usize));
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
    return dom;
  }

  forceinline string
  StrToNat::min_str() const {
    const DashedString& p = x0.domain();
    bool include_first = int2char(p.at(0).min()) != '0';
    string::size_type length = include_first ? p.at(0).l : 0;
    for (int i = 1; i < p.length(); ++i)
      length += p.at(i).l;
    string n;
    n.reserve(length);
    if (include_first)
      n.append(p.at(0).l, int2char(p.at(0).S.min()));
    for (int i = 1; i < p.length(); ++i) {
      const DSBlock& b = p.at(i);
      if (b.l > 0)
        n.append(b.l, int2char(b.S.min()));
    }
    return n == "" ? "0" : n;
  }

  forceinline string
  StrToNat::max_str() const {
    const DashedString& p = x0.domain();
    bool include_first = int2char(p.at(0).max()) != '0';
    string::size_type length = include_first ? p.at(0).u : 0;
    for (int i = 1; i < p.length(); ++i)
      length += p.at(i).u;
    string n;
    n.reserve(length);
    if (include_first)
      n.append(p.at(0).u, int2char(p.at(0).S.max()));
    for (int i = 1; i < p.length(); ++i) {
      const DSBlock& b = p.at(i);
      n.append(b.u, int2char(b.S.max()));
    }
    return n == "" ? "0" : n;
  }

  forceinline ExecStatus
  StrToNat::refine_int(Space& home) {
    int lx = x1.min(), ux = x1.max(), n = x0.domain().length();
    string lb = "", ub = "";
    bool num_only = true;
    const NSIntSet num('0', '9');
	  for (int i = 0; i < n; ++i) {
	    const DSBlock& b = x0.domain().at(i);
      if (num.disjoint(b.S)) {
        if (b.l > 0)
          GECODE_ME_CHECK(x1.eq(home, -1));
        num_only = false;
        break;
      }
      else {
        NSIntSet N(b.S);
        N.intersect(num);
        ub.append(b.u, N.max());
	  if (num_only && b.l > 0 && b.S.min() >= '0' && b.S.max() <= '9')
          lb.append(b.l, N.min());
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
  StrToNat::propagate(Space& home, const ModEventDelta&) {
    while (true) {
      if (x0.assigned()) {
        int value = -1;
        string s;
        bool digits = x0.domain().try_val_bytes(s) && !s.empty();
        for (string::const_iterator c = s.begin(); digits && c != s.end(); ++c)
          digits = *c >= '0' && *c <= '9';
        if (digits) {
          try {
            value = std::stoi(s);
          }
          catch (const std::invalid_argument&) {}
          catch (const std::out_of_range&) {}
        }
        GECODE_ME_CHECK(x1.eq(home, value));
        return home.ES_SUBSUMED(*this);
      }
      if ((x1.min() == -1) && str2nat_may_overflow(x0.domain())) {
        if (!x1.assigned() && !x0.domain().check_equate(strdom()))
          GECODE_ME_CHECK(x1.eq(home, -1));
        // Possible overflow is not entailment. Retain both subscriptions
        // until assignment or removal of the sentinel resolves the choice.
        return ES_FIX;
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
        return home.ES_SUBSUMED(*this);
      }
      NSBlocks dom = strdom();
      if (x1.min() > -1
      || dom.contains<DSBlock, DSBlocks>(x0.domain().blocks())) {
        if (x1.min() > 0)
          GECODE_ME_CHECK(x0.lb(home, (int) ceil(std::log10(x1.min()))));
        GECODE_ME_CHECK(x0.dom(home, dom));
        try {
          GECODE_ME_CHECK(x1.gq(home, std::stoi(min_str())));
          GECODE_ME_CHECK(x1.lq(home, std::stoi(max_str())));
        }
        catch (const std::out_of_range&) {}
      }
      else if (x1.max() == -1 || !x0.domain().check_equate(dom)) {
        extensional(home, x0, "[0-9][0-9]*", BoolVar(home, 0, 0), RM_EQV);
        return home.ES_SUBSUMED(*this);
      }
      GECODE_ES_CHECK(refine_int(home));
      assert (x0.domain().is_normalized());
      if (!x0.assigned() && !x1.assigned())
        return ES_FIX;
    }
  }

}}

// STATISTICS: string-prop
