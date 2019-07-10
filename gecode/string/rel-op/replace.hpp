namespace Gecode { namespace String {

  // x[3] is the string resulting from replacing the first occurrence of x[0] 
  // with x[1] in the target string x[2].

  forceinline ExecStatus
  Replace::post(Home home, ViewArray<StringView>& x, bool replace_all) {
    if (x[0].same(x[1])) {
      rel(home, x[2], STRT_EQ, x[3]);
      return ES_OK;
    }
    else if (x[0].same(x[2])) {
      rel(home, x[1], STRT_EQ, x[3]);
      return ES_OK;
    }
    else if (x[0].same(x[3]))
      rel(home, x[0], STRT_SUB, x[1]);
    (void) new (home) Replace(home, x, replace_all);
    return ES_OK;
  }

  forceinline Actor*
  Replace::copy(Space& home) {
    return new (home) Replace(home, *this);
  }

  forceinline
  Replace::Replace(Home home, ViewArray<StringView>& x, bool replace_all)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, x), all(replace_all) {}

  forceinline
  Replace::Replace(Space& home, Replace& p)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, p), all(p.all) {}


  // Crush x[k][p : q] into a single block.
  forceinline NSBlock
  Replace::crush(int k, const Position& p, const Position& q) const {
    NSBlock block;
    if (p == q)
      return block;
    assert (Fwd::lt(p, q));
    DashedString* px = x[k].pdomain();
    for (int i = p.idx; i <= q.idx; ++i) {
      const DSBlock& b = px->at(i);
      block.S.include(b.S);
      block.u += b.u;
    }
    return block;
  }

  // Returns the prefix of x[k] until position p.
  forceinline NSBlocks
  Replace::prefix(int k, const Position& p) const {
    NSBlocks pref;
    DashedString* px = x[k].pdomain();
    for (int i = 0; i < p.idx; ++i)
      pref.push_back(NSBlock(px->at(i)));
    int off = p.off;
    if (off > 0) {
      const DSBlock& b = px->at(p.idx);
      if (off < b.l)
        pref.push_back(NSBlock(b.S, off, off));
      else
        pref.push_back(NSBlock(b.S, b.l, off));
    }
    return pref;
  }
  
  // Returns the suffix of x[k] from position p.
  forceinline NSBlocks
  Replace::suffix(int k, const Position& p) const {
    NSBlocks suff;
    DashedString* px = x[k].pdomain();
    int off = p.off;
    if (off < px->at(p.idx).u) {
      const DSBlock& b = px->at(p.idx);
      suff.push_back(NSBlock(b.S, max(0, b.l - off), b.u - off));
    }
    for (int i = p.idx + 1; i < px->length(); ++i)
      suff.push_back(NSBlock(px->at(i)));
    return suff;
  }
  
  // Decomposes replaceAll into basic constraints.
  forceinline ExecStatus
  Replace::replace_all(Space& home) {
    // std::cerr << "replace_all\n";
    string sx = x[0].val(), sy = x[2].val();
    if (x[1].assigned()) {
      string sx1 = x[1].val();
      if (sx == "") {
        string sz = sx1;
        for (auto c : sy)
          sz += c + sx1;
        GECODE_ME_CHECK(x[3].eq(home, sz));
      }
      else {
        size_t pos = sy.find(sx), n = sx.size(), n1 = sx1.size();
        while (pos != string::npos) {
          sy.replace(pos, n, sx1);
          pos = sy.find(sx, pos + n1);
        }
        GECODE_ME_CHECK(x[3].eq(home, sy));
      }
    }
    else {
      if (sx == "") {
        if (sy == "") {
          rel(home, x[1], STRT_EQ, x[3]);
          return home.ES_SUBSUMED(*this);
        }
        int n = sy.size();
        StringVarArray vy(home, n);
        rel(home, x[1], StringVar(home, string(1, sy[0])), STRT_CAT, vy[0]);
        for (int i = 1; i < n; ++i) {
          StringVar z(home);
          rel(home, x[1], StringVar(home, string(1, sy[i])), STRT_CAT, z);
          rel(home, vy[i - 1], z, STRT_CAT, vy[i]);
        }
        rel(home, vy[n - 1], x[1], STRT_CAT, x[3]);
      }
      else {
        size_t pos = sy.find(sx);
        if (pos == string::npos) {
          rel(home, x[2], STRT_EQ, x[3]);
          return home.ES_SUBSUMED(*this);
        }
        std::vector<StringVar> vy(1, StringVar(home));
        rel(home, StringVar(home, sy.substr(0, pos)), x[1], STRT_CAT, vy[0]);
        size_t pos1 = pos + 1;
        pos = sy.find(sx, pos1);        
        while (pos != string::npos) {        
          StringVar last = vy.back();
          vy.push_back(StringVar(home));
          if (pos > pos1) {
            StringVar z(home);
            rel(home,
              StringVar(home, sy.substr(pos1, pos - pos1)), x[1], STRT_CAT, z
            );
            rel(home, last, z, STRT_CAT, vy.back());
          }
          else
            rel(home, last, x[1], STRT_CAT, vy.back());
          pos1 = pos + 1;
          pos = sy.find(sx, pos1);          
        }
        if (pos1 != string::npos)
          rel(home, vy.back(), StringVar(home,sy.substr(pos1)), STRT_CAT, x[3]);
      }
    }    
    // std::cerr << "After replace_all: " << x << "\n";
    return home.ES_SUBSUMED(*this);
  }

  // Propagating |y'| = |y| + |x'| - |x|, knowing that x occurs in y.
  forceinline ModEvent
  Replace::refine_card(Space& home) {
    long lx  = x[0].min_length(), ux  = x[0].max_length(),
         lx1 = x[1].min_length(), ux1 = x[1].max_length(),
         ly  = x[2].min_length(), uy  = x[2].max_length(),
         ly1 = x[3].min_length(), uy1 = x[3].max_length();
    bool again, cx, cx1, cy, cy1;
    do {
      again = cx = cx1 = cy = cy1 = false;
      int llx = ly + lx1 - uy1, uux = uy + ux1 - ly1,
          llx1 = ly1 + lx - uy, uux1 = uy1 + ux - ly,
          lly = ly1 + lx - ux1, uuy = uy1 + ux - ux1,
          lly1 = ly + lx1 - ux, uuy1 = uy + ux1 - lx;
      if (llx > lx)   { lx  = llx;  cx  = again = true; }
      if (uux < ux)   { ux  = uux;  cx  = again = true; }
      if (llx1 > lx1) { lx1 = llx1; cx1 = again = true; }
      if (uux1 < ux1) { ux1 = uux1; cx1 = again = true; }
      if (lly > ly)   { ly  = lly;  cy  = again = true; }
      if (uuy < uy)   { uy  = uuy;  cy  = again = true; }
      if (lly1 > ly1) { ly1 = lly1; cy1 = again = true; }
      if (uuy1 < uy1) { uy1 = uuy1; cy1 = again = true; }
      if (ux < lx || ux1 < lx1 || uy < ly || uy1 < ly1)
        return ME_STRING_FAILED;
    } while (again);
    if ((cx  && !x[0].pdomain()->refine_card(home, lx, ux))   ||
        (cx1 && !x[1].pdomain()->refine_card(home, lx1, ux1)) ||
        (cy  && !x[2].pdomain()->refine_card(home, ly, uy))   ||
        (cy1 && !x[3].pdomain()->refine_card(home, ly1, uy1)))
      return ME_STRING_FAILED;      
    StringDelta d(true);
    if (cx)
      return x[0].varimp()->notify(
        home, x[0].assigned() ? ME_STRING_VAL : ME_STRING_DOM, d
      );
    if (cx1)
      return x[1].varimp()->notify(
        home, x[1].assigned() ? ME_STRING_VAL : ME_STRING_DOM, d
      );
    if (cy)
      return x[2].varimp()->notify(
        home, x[2].assigned() ? ME_STRING_VAL : ME_STRING_DOM, d
      );
    if (cy1)
      return x[3].varimp()->notify(
        home, x[3].assigned() ? ME_STRING_VAL : ME_STRING_DOM, d
      );
    return ME_STRING_NONE;
  }
  
  forceinline ExecStatus
  Replace::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"\nReplace" << (all ? "All" : "") << "::propagate: "<< x <<"\n";
    assert(x[0].pdomain()->is_normalized() && x[1].pdomain()->is_normalized() &&
           x[2].pdomain()->is_normalized() && x[3].pdomain()->is_normalized());                     
    bool occur = false;
    if (x[0].assigned()) {
      string sx = x[0].val();
      if (sx == "" && !all) {
        rel(home, x[1], x[2], STRT_CAT, x[3]);
        return home.ES_SUBSUMED(*this);
      }
      if (x[2].assigned()) {
        if (all)
          return replace_all(home);
        string sy = x[2].val();
        size_t n = sy.find(sx);
        if (n == string::npos)
          rel(home, x[2], STRT_EQ, x[3]);
        else {
          string pref = sy.substr(0, n);
          string suff = sy.substr(n + sx.size());
          if (x[1].assigned())
            GECODE_ME_CHECK(x[3].eq(home, pref + x[1].val() + suff));
          else {
            StringVar z(home);
            rel(home, StringVar(home, pref), x[1], STRT_CAT, z);
            rel(home, z, StringVar(home, suff), STRT_CAT, x[3]);
          }
        }
        return home.ES_SUBSUMED(*this);
      }
      // Compute fixed components of x[2] to see if x[0] must occur in it, i.e.,
      // if we actually replace x[0] with x[1] in x[2].
      string curr = "";
      DashedString* p = x[2].pdomain();
      for (int i = 0; i < p->length(); ++i) {
        const DSBlock& b = p->at(i);
        if (b.S.size() == 1) {
          string w(b.l, b.S.min());
          curr += w;
          if (curr.find(sx) != string::npos) {
            occur = true;
            break;
          }
          if (b.u > b.l)
            curr = w;
        }
        else
          curr = "";
      }
    }
    // x[2] != x[3] => x[0] occur in x[2] /\ x[1] occur in x[3].
    if (!occur && !x[2].pdomain()->check_equate(*x[3].pdomain())) {
      occur = true;
      NSBlocks d0(1, NSBlock(x[2].may_chars(), 0, x[2].max_length()));      
      NSBlocks d1(1, NSBlock(x[3].may_chars(), 0, x[3].max_length()));
      GECODE_ME_CHECK(x[0].dom(home, d0));
      GECODE_ME_CHECK(x[1].dom(home, d1));
      GECODE_ME_CHECK(x[2].lb(home, x[0].min_length()));
      GECODE_ME_CHECK(x[3].lb(home, x[1].min_length()));
    };
    // If x[0] must not occur in x[2], then x[2] = x[3]. Otherwise, we use the 
    // earliest/latest start/end positions of x[0] in x[2] to possibly refine 
    // x[3] via equation.
    Position pos[2];
    // std::cerr << "occur: " << occur << "\n";
    if (sweep_replace(*x[0].pdomain(), *x[2].pdomain(), pos)) {
      // Prefix: x[2][: es]
      NSBlocks v;
      Position es = pos[0], le = pos[1];
      if (es != Position({0, 0}))
        v = prefix(2, es);
      // Crush x[2][es : le] into a single block.
      if (es != le) {
        NSBlock b = crush(2, es, le);
        v.push_back(b);
        if (occur) {
          DashedString* px = x[1].pdomain();
          for (int i = 0; i < px->length(); ++i)
            v.push_back(NSBlock(px->at(i)));
          v.push_back(b);
        }
      }
      // Suffix: x[2][le :]
      if (le != Position({x[2].pdomain()->length(), 0}))
        v.extend(suffix(2, le));      
      v.normalize();
      // std::cerr << "Equating y' = " << x[3] << " with " << v << "\n";
      GECODE_ME_CHECK(x[3].dom(home, v));
    }
    else {
      rel(home, x[2], STRT_EQ, x[3]);
      return home.ES_SUBSUMED(*this);
    }    
    // If x[1] must not occur in x[3], then find(x[0], x[2]) = 0 /\ x[2] = x[3].
    // Otherwise, we use the earliest/latest start/end positions of x[1] in x[3]
    // to possibly refine x[2] via equation.
    if (sweep_replace(*x[1].pdomain(), *x[3].pdomain(), pos)) {
      // Prefix: x[3][: es].
      NSBlocks v;
      Position es = pos[0], le = pos[1];
      if (es != Position({0, 0}))
        v = prefix(3, es);
      // Crush x[3][es : ls] into a single block.
      if (es != le) {
        NSBlock b = crush(3, es, le);
        v.push_back(b);
        if (occur) {
          DashedString* px = x[0].pdomain();
          for (int i = 0; i < px->length(); ++i)
            v.push_back(NSBlock(px->at(i)));
          v.push_back(b);
        }
      }
      // Suffix: x[3][le :]
      if (le != Position({x[3].pdomain()->length(), 0}))
        v.extend(suffix(3, le));
      v.normalize();
      // std::cerr << "Equating y = " << x[2] << " with " << v << "\n";
      GECODE_ME_CHECK(x[2].dom(home, v));
    }
    else {
      find(home, x[0], x[2], IntVar(home, 0, 0));
      rel(home, x[2], STRT_EQ, x[3]);
      return home.ES_SUBSUMED(*this);
    }
    //if (occur && !all)
      //GECODE_ME_CHECK(refine_card(home));
    // std::cerr<<"After replace: "<< x <<"\n";
    return x[0].assigned() && x[2].assigned() ? ES_NOFIX : ES_FIX;
  }

}}
