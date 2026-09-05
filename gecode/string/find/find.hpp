namespace Gecode { namespace String {

  template<class Sequence>
  forceinline int
  find_symbol_sequence(const Sequence& haystack, const StringVal& needle) {
    const int h = static_cast<int>(haystack.size());
    const int n = static_cast<int>(needle.size());
    if (n == 0)
      return 0;
    if (n > h)
      return -1;
    for (int i = 0; i <= h - n; ++i) {
      int j = 0;
      while (j < n &&
             haystack[static_cast<typename Sequence::size_type>(i + j)] ==
             needle[static_cast<StringVal::size_type>(j)])
        ++j;
      if (j == n)
        return i;
    }
    return -1;
  }

  forceinline bool
  find_byte_domain(const DashedString& value) {
    for (int i = 0; i < value.length(); ++i)
      if (!value.at(i).S.empty() && value.at(i).S.max() > 255)
        return false;
    return true;
  }

  forceinline std::vector<StringSymbol>
  find_known_prefix_symbols(const DashedString& value) {
    int end = 0;
    std::vector<StringSymbol>::size_type length = 0;
    for (; end < value.length(); ++end) {
      const DSBlock& block = value.at(end);
      if (block.null())
        continue;
      if (block.S.size() > 1)
        break;
      length += static_cast<std::vector<StringSymbol>::size_type>(block.l);
      if (block.l < block.u) {
        ++end;
        break;
      }
    }
    std::vector<StringSymbol> prefix;
    prefix.reserve(length);
    for (int i = 0; i < end; ++i) {
      const DSBlock& block = value.at(i);
      if (!block.null())
        prefix.insert(prefix.end(), block.l, block.S.min());
    }
    return prefix;
  }

  forceinline
  Find::Find(Home home, StringView x, StringView y, Gecode::Int::IntView n)
  : MixTernaryPropagator<StringView, PC_STRING_DOM,
    StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM>
    (home, x, y, n) {}

  forceinline ExecStatus
  Find::post(Home home, StringView x, StringView y, Gecode::Int::IntView n) {
    if (x.same(y))
      rel(home, n, IRT_EQ, 1);
    else
      (void) new (home) Find(home, x, y, n);
    return ES_OK;
  }

  forceinline
  Find::Find(Space& home, Find& p)
  : MixTernaryPropagator<StringView, PC_STRING_DOM,
    StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM>
    (home, p) {}

  forceinline Actor*
  Find::copy(Space& home) {
    return new (home) Find(home, *this);
  }

  forceinline ExecStatus
  Find::propagate(Space& home, const ModEventDelta&) {
    // std::cerr << "\nFind::propagate: " << x2 << " = find " << x0 << " in " << x1 << "\n";
    int l = x2.min(), u = x2.max();
    // Adjusting x2 upper bound.
    int max_index = std::max(1, x1.max_length());
    if (u > max_index) {
      u = max_index;
      GECODE_ME_CHECK(x2.lq(home, u));
    }
    if (x0.min_length() > x1.max_length()) {
      GECODE_ME_CHECK(x2.eq(home, 0));
      return home.ES_SUBSUMED(*this);
    }
    // The substring x0 is known: we check whether it definitely occurs in x1,
    // possibly updating x2. Keep the legacy byte path unchanged when both
    // domains contain byte-valued symbols; use scalar values otherwise.
    if (x0.assigned()) {
      string s;
      const bool byte_path = x0.domain().try_val_bytes(s) &&
        find_byte_domain(x1.domain());
      if (byte_path) {
        if (s == "") {
          GECODE_ME_CHECK(x2.eq(home, 1));
          return home.ES_SUBSUMED(*this);
        }
        if (x1.assigned()) {
          int i = x1.val().find(s) + 1;
          if (i < l || i > u)
            return ES_FAILED;
          GECODE_ME_CHECK(x2.eq(home, i));
          return home.ES_SUBSUMED(*this);
        }
        const DashedString& p1 = x1.domain();
        string pref = p1.known_pref();
        if (pref.size() > 0) {
          int i = pref.find(s) + 1;
          if (i > 0) {
            if (i < l || i > u)
              return ES_FAILED;
            GECODE_ME_CHECK(x2.eq(home, i));
            return home.ES_SUBSUMED(*this);
          }
        }
        int n = p1.max_length();
        if (n < l)
          return ES_FAILED;
        string curr;
        Position start({0, 0});
        // Checking fixed components.
        for (int i = 0; n > 0 && i < p1.length(); ++i) {
          const DSBlock& b = p1.at(i);
          if (b.S.size() == 1) {
            char c = int2char(b.S.min());
            int fixed = min(b.l, n);
            curr.append(fixed, c);
            int k = curr.find(s);
            if (k != (int) string::npos) {
              GECODE_ME_CHECK(x2.gq(home, 1));
              if (l == 0)
                l = 1;
              int ub = start.off + k + 1;
              for (int j = 0; j < start.idx && ub < u; ++j)
                ub += p1.at(j).u;
              GECODE_ME_CHECK(x2.lq(home, ub));
              if (u > ub)
                u = ub;
              break;
            }
            if (b.u > b.l) {
              curr.assign(fixed, c);
              start = Position({i, b.u - b.l});
            }
          }
          else {
            curr.clear();
            start = Position({i, b.u});
          }
          n -= b.u;
        }
      } else {
        const StringVal pattern = x0.val_symbols();
        if (pattern.empty()) {
          GECODE_ME_CHECK(x2.eq(home, 1));
          return home.ES_SUBSUMED(*this);
        }
        if (x1.assigned()) {
          const int pos = find_symbol_sequence(x1.val_symbols(), pattern);
          const int i = pos + 1;
          if (i < l || i > u)
            return ES_FAILED;
          GECODE_ME_CHECK(x2.eq(home, i));
          return home.ES_SUBSUMED(*this);
        }
        const DashedString& p1 = x1.domain();
        const std::vector<StringSymbol> pref = find_known_prefix_symbols(p1);
        if (!pref.empty()) {
          const int pos = find_symbol_sequence(pref, pattern);
          const int i = pos + 1;
          if (i > 0) {
            if (i < l || i > u)
              return ES_FAILED;
            GECODE_ME_CHECK(x2.eq(home, i));
            return home.ES_SUBSUMED(*this);
          }
        }
        int n = p1.max_length();
        if (n < l)
          return ES_FAILED;
        std::vector<StringSymbol> curr;
        Position start({0, 0});
        // Checking fixed components.
        for (int i = 0; n > 0 && i < p1.length(); ++i) {
          const DSBlock& b = p1.at(i);
          if (b.S.size() == 1) {
            const StringSymbol c = b.S.min();
            const int fixed = min(b.l, n);
            curr.insert(curr.end(), fixed, c);
            const int k = find_symbol_sequence(curr, pattern);
            if (k >= 0) {
              GECODE_ME_CHECK(x2.gq(home, 1));
              if (l == 0)
                l = 1;
              int ub = start.off + k + 1;
              for (int j = 0; j < start.idx && ub < u; ++j)
                ub += p1.at(j).u;
              GECODE_ME_CHECK(x2.lq(home, ub));
              if (u > ub)
                u = ub;
              break;
            }
            if (b.u > b.l) {
              curr.assign(fixed, c);
              start = Position({i, b.u - b.l});
            }
          } else {
            curr.clear();
            start = Position({i, b.u});
          }
          n -= b.u;
        }
      }
    }
    bool mod = (l > 0);
    // x1 = "".
    if (x1.max_length() == 0) {
      if (mod) {
        GECODE_ME_CHECK(x2.eq(home, 1));
        GECODE_ME_CHECK(x0.eq(home, ""));
        return home.ES_SUBSUMED(*this);
      }
      else {
        if (u == 0) {
          rel(home, x0, STRT_NQ, StringVar(home, ""));
          return home.ES_SUBSUMED(*this);
        }
        GECODE_ME_CHECK(x2.lq(home, 1));
        u = 1;
      }
    }
    // x2 = 0.
    if (l == u && u == 0) {
      if (x0.assigned() && x0.min_length() == 1) {
        // Removing a single character from all the bases.
        string pattern_bytes;
        const int c = x0.domain().try_val_bytes(pattern_bytes)
          ? char2int(pattern_bytes[0])
          : x0.val_symbols()[0];
        StringVarImp::DomainState x1_state =
          x1.begin_refinement();
        DashedString& pdom = x1.mutable_domain(x1_state);
        bool changed = false;
        bool norm = false;
        for (int i = 0; i < pdom.length(); ++i) {
          DSBlock& b = pdom.at(i);
          if (b.S.contains(c)) {
            b.S.remove(home, c);
            changed = true;
            if (b.l > 0 && b.S.empty())
              return ES_FAILED;
            norm |= b.S.empty() || (i > 0 && pdom.at(i-1).S == b.S) ||
                    (i < pdom.length()-1 && pdom.at(i+1).S == b.S);
          }
        }
        if (norm)
          pdom.normalize(home);
        if (changed)
          GECODE_ME_CHECK(x1.commit_refinement(home, x1_state));
        assert (pdom.is_normalized());
        if (x1.assigned()) {
          string haystack_bytes;
          if (x0.domain().try_val_bytes(pattern_bytes) &&
              x1.domain().try_val_bytes(haystack_bytes)) {
            if (haystack_bytes.find(pattern_bytes) != string::npos)
              return ES_FAILED;
          } else if (find_symbol_sequence
                     (x1.val_symbols(), x0.val_symbols()) >= 0) {
            return ES_FAILED;
          }
        }
        return home.ES_SUBSUMED(*this);
      }
      return ES_FIX;
    }
    if (mod) {
      NSIntSet ychars = x1.may_chars();
      int n = x0.domain().length();
      NSBlocks v;
      if (u > 1)
        v.push_back(NSBlock(ychars, l - 1, u - 1));
      for (int i = 0; i < n; ++i)
        v.push_back(NSBlock(x0.domain().at(i)));
      int j = x1.max_length() - x0.min_length() - l + 1;
      if (j > 0) {
        int i = max(0, x1.min_length() - x0.max_length() - u + 1);
        v.push_back(NSBlock(ychars, i, j));
      }
      v.normalize();
      GECODE_ME_CHECK(x1.dom(home, v));
    }
    else {
      // l = min(D(x2) - {0}).
      Gecode::Int::ViewValues<Gecode::Int::IntView> i(x2);
      ++i;
      l = i.val();
    }
    // General case.
    GECODE_ME_CHECK(x1.find(home, x0, l, u, mod));
    // std::cerr << "After find: " << x0 << " in " << x1 << " (" << l << ", " << u << ")\n";
    GECODE_ME_CHECK(x2.lq(home, u));
    if (mod) {
      // Can modify x and y.
      GECODE_ME_CHECK(x2.gq(home, l));
      if (x1.assigned()) {
        string haystack_bytes;
        const bool haystack_is_bytes =
          x1.domain().try_val_bytes(haystack_bytes);
        if (x0.assigned()) {
          string pattern_bytes;
          int n;
          if (haystack_is_bytes && x0.domain().try_val_bytes(pattern_bytes))
            n = static_cast<int>(haystack_bytes.find(pattern_bytes)) + 1;
          else
            n = find_symbol_sequence(x1.val_symbols(), x0.val_symbols()) + 1;
          GECODE_ME_CHECK(x2.eq(home, n));
          return home.ES_SUBSUMED(*this);
        }
        if (x2.assigned() && x0.min_length() == x0.max_length()) {
          int n = x2.val(), m = x0.min_length();
          assert (n > 0);
          if (haystack_is_bytes) {
            GECODE_ME_CHECK(x0.eq(home, haystack_bytes.substr(n - 1, m)));
          } else {
            const StringVal haystack = x1.val_symbols();
            std::vector<StringSymbol> symbols;
            symbols.reserve(static_cast<std::vector<StringSymbol>::size_type>(m));
            for (int j = 0; j < m; ++j)
              symbols.push_back(haystack[static_cast<StringVal::size_type>(n - 1 + j)]);
            GECODE_ME_CHECK
              (x0.eq(home, StringVal::from_symbols(std::move(symbols))));
          }
          return home.ES_SUBSUMED(*this);
        }
      }
    }
    else {
      // Can't modify x and y.
      if (u == 0)
        GECODE_ME_CHECK(x2.eq(home, 0));
      if (l > 1) {
        IntSet s(1, l - 1);
        IntSetRanges is(s);
        GECODE_ME_CHECK(x2.minus_r(home, is));
      }
    }
    // std::cerr << "index: " << x2 << "\n";
    assert (x0.domain().is_normalized() && x1.domain().is_normalized());
    return ES_FIX;
  }

}}
