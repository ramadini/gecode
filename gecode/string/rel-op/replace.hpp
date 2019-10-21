namespace Gecode { namespace String {

  // x[3] is the string resulting from x[0] by replacing the first occurrence of 
  // x[1] with x[2].

  forceinline ExecStatus
  Replace::post(Home home, ViewArray<StringView>& x, bool a, bool l) {
    if (x[1].same(x[2])) {
      rel(home, x[0], STRT_EQ, x[3]);
      return ES_OK;
    }
    else if (x[0].same(x[1])) {
      rel(home, x[2], STRT_EQ, x[3]);
      return ES_OK;
    }
    else if (x[1].same(x[3]))
      rel(home, x[1], STRT_SUB, x[0]);
    (void) new (home) Replace(home, x, a, l);
    return ES_OK;
  }

  forceinline Actor*
  Replace::copy(Space& home) {
    return new (home) Replace(home, *this);
  }

  forceinline
  Replace::Replace(Home home, ViewArray<StringView>& x, bool a, bool l)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, x), all(a), last(l) {}

  forceinline
  Replace::Replace(Space& home, Replace& p)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, p), all(p.all), last(p.last) 
  {}

  forceinline int
  Replace::lb_card(int k, const Position& a, const Position& b) const {  
    if (a == b)
      return 0;
    int l = 0;
    assert (Fwd::lt(a, b));
    const DashedString* px = x[k].pdomain();
    const DSBlock& xa = px->at(a.idx);
    if (a.off < xa.l)
      l += xa.l - a.off;    
    for (int i = a.idx + 1; i < b.idx; ++i)
      l += px->at(i).l;
    return l + min(b.off, px->at(b.idx).l);
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
  
  // Decomposes decomp_all into basic constraints.
  forceinline ExecStatus
  Replace::decomp_all(Space& home) {
    // std::cerr << "decomp_all\n";
    string sx = x[0].val(), sq = x[1].val();
    if (x[2].assigned()) {
      string sq1 = x[2].val();
      if (sq == "") {
        if (sq1 == "") {
          rel(home, x[0], STRT_EQ, x[3]);
          return home.ES_SUBSUMED(*this);
        }
        string sz = sq1;
        for (auto c : sx)
          sz += c + sq1;
        GECODE_ME_CHECK(x[3].eq(home, sz));
      }
      else {
        size_t pos = sx.find(sq), n = sq.size(), n1 = sq1.size();
        while (pos != string::npos) {
          sx.replace(pos, n, sq1);
          pos = sx.find(sq, pos + n1);
        }
        GECODE_ME_CHECK(x[3].eq(home, sx));
      }
    }
    else {
      if (sq == "") {
        if (sx == "") {
          rel(home, x[2], STRT_EQ, x[3]);
          return home.ES_SUBSUMED(*this);
        }
        int n = sx.size();
        StringVarArray vx(home, n);
        rel(home, x[2], StringVar(home, string(1, sx[0])), STRT_CAT, vx[0]);
        for (int i = 1; i < n; ++i) {
          StringVar z(home);
          rel(home, x[2], StringVar(home, string(1, sx[i])), STRT_CAT, z);
          rel(home, vx[i - 1], z, STRT_CAT, vx[i]);
        }
        rel(home, vx[n - 1], x[2], STRT_CAT, x[3]);
      }
      else {
        size_t pos = sx.find(sq);
        if (pos == string::npos) {
          rel(home, x[0], STRT_EQ, x[3]);
          return home.ES_SUBSUMED(*this);
        }
        std::vector<StringVar> vx(1, StringVar(home));
        string s = sx.substr(0, pos);
        if (s == "")
          rel(home, x[2], STRT_EQ, vx[0]);
        else
          rel(home, StringVar(home, s), x[2], STRT_CAT, vx[0]);
        size_t nx = sq.size(), pos1 = pos + nx;
        pos = sx.find(sq, pos1);
        while (pos != string::npos) {
          StringVar last = vx.back();
          vx.push_back(StringVar(home));
          if (pos > pos1) {
            StringVar z(home);
            rel(home,
              StringVar(home, sx.substr(pos1, pos - pos1)), x[2], STRT_CAT, z
            );
            rel(home, last, z, STRT_CAT, vx.back());
          }
          else
            rel(home, last, x[2], STRT_CAT, vx.back());
          pos1 = pos + nx;
          pos = sx.find(sq, pos1);          
        }
        if (pos1 < sx.size())
          rel(home, vx.back(), StringVar(home,sx.substr(pos1)), STRT_CAT, x[3]);
        else
          rel(home, vx.back(), STRT_EQ, x[3]);
      }
    }
    // std::cerr << "After decomp_all: " << x << "\n";
    return home.ES_SUBSUMED(*this);
  }

  // Checking if |y| = |x| + |q'| - |q| is consistent.
  forceinline bool
  Replace::check_card() const {  
   int lx  = x[0].min_length(), ux  = x[0].max_length(),
       lq  = x[1].min_length(), uq  = x[1].max_length(),
       lq1 = x[2].min_length(), uq1 = x[2].max_length(),       
       ly  = x[3].min_length(), uy  = x[3].max_length();
     return ly <= ux + uq1 - lq && uy >= lx + lq1 - uq;
  }
  
  // For replace, it returns 1 if q must occur in x[0], 0 otherwise.
  // For decomp_all, it returns the minimum number of occurrences of q in x[0].
  forceinline int
  Replace::occur(string q) const {
    int min_occur = 0;
    string curr = "";
    DashedString* p = x[0].pdomain();
    for (int i = 0; i < p->length(); ++i) {
      const DSBlock& b = p->at(i);
      if (b.S.size() == 1) {
        string w(b.l, b.S.min());
        curr += w;
        size_t i = curr.find(q);
        while (i != string::npos) {
          min_occur++;
          if (!all)
            return min_occur;
          curr = curr.substr(i + q.size());
          i = curr.find(q);
        }
        if (b.l < b.u)
          curr = w;
      }
      else
        curr = "";
    }
    return min_occur;
  }
  
  forceinline ExecStatus
  Replace::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"\nReplace" << (all ? "All" : "") << "::propagate: "<< x <<"\n";
    assert(x[0].pdomain()->is_normalized() && x[1].pdomain()->is_normalized() &&
           x[2].pdomain()->is_normalized() && x[3].pdomain()->is_normalized());
    if (!all && !check_card()) {
      find(home, x[1], x[0], IntVar(home, 0, 0));
      rel(home, x[0], STRT_EQ, x[3]);
      return home.ES_SUBSUMED(*this);
    }
    int min_occur = 0;
    if (x[1].assigned()) {
      string sq = x[1].val();
      if (x[2].assigned() && sq == x[2].val()) {
        rel(home, x[0], STRT_EQ, x[3]);
        return home.ES_SUBSUMED(*this);
      }
      if (sq == "" && !all) {
        last ? rel(home, x[0], x[2], STRT_CAT, x[3])
             : rel(home, x[2], x[0], STRT_CAT, x[3]);
        return home.ES_SUBSUMED(*this);
      }
      if (x[0].assigned()) {
        if (all)
          return decomp_all(home);
        string sx = x[0].val();
        size_t n = last ? sx.rfind(sq) : sx.find(sq);
        if (n == string::npos)
          rel(home, x[0], STRT_EQ, x[3]);
        else {
          string pref = sx.substr(0, n);
          string suff = sx.substr(n + sq.size());
          if (x[2].assigned())
            GECODE_ME_CHECK(x[3].eq(home, pref + x[2].val() + suff));
          else {
            StringVar z(home);
            rel(home, StringVar(home, pref), x[2], STRT_CAT, z);
            rel(home, z, StringVar(home, suff), STRT_CAT, x[3]);
          }
        }
        return home.ES_SUBSUMED(*this);
      }
      min_occur = occur(sq);      
    }
    // x[0] != x[3] => x[1] occur in x[0] /\ x[2] occur in x[3].
    DashedString* px  = x[0].pdomain();
    DashedString* pq  = x[1].pdomain();
    DashedString* pq1 = x[2].pdomain();
    DashedString* py  = x[3].pdomain();    
    if (min_occur == 0 && !px->check_equate(*py))
      min_occur = 1;
    // If x[1] must not occur in x[0], then x[0] = x[3]. Otherwise, we use the 
    // earliest/latest start/end positions of x[1] in x[0] to possibly refine 
    // x[3] via equation.
    Position pos[2];
    // std::cerr << "min_occur: " << min_occur << "\n";
    if (check_find(*pq, *px, pos)) {
      // Prefix: x[0][: es]
      NSBlocks v;
      Position es = pos[0], le = pos[1];
      if (es != Position({0, 0}))
        v = prefix(0, es);
      // Crush x[0][es : le], possibly adding x[2].
      int u = x[3].max_length() - lb_card(0, Position({0, 0}), es)
                                - lb_card(0, le, last_fwd(px->blocks()));     
      if (u > 0) {
        NSBlock b;
        b.S.include(x[0].may_chars());
        b.S.include(x[2].may_chars());
        b.u = u;
        v.push_back(b);
        for (int i = 0; i < min_occur; ++i) {
          for (int j = 0; j < pq1->length(); ++j)
            v.push_back(NSBlock(pq1->at(j)));
          v.push_back(b);
        }
      }
      else
        for (int i = 0; i < min_occur; ++i)
          for (int j = 0; j < pq1->length(); ++j)
            v.push_back(NSBlock(pq1->at(j)));
      // Suffix: x[0][le :]
      if (le != Position({px->length(), 0}))
        v.extend(suffix(0, le));
      v.normalize();
      // std::cerr << "1) Equating " << x[3] << " with " << v << " => \n";
      GECODE_ME_CHECK(x[3].dom(home, v));
      // std::cerr << x[3] << "\n";
    }
    else {
      if (min_occur > 0)
        return ES_FAILED;
      rel(home, x[0], STRT_EQ, x[3]);
      return home.ES_SUBSUMED(*this);
    }    
    // If x[2] must not occur in x[3], then find(x[1], x[0]) = 0 /\ x[0] = x[3].
    // Otherwise, we use the earliest/latest start/end positions of x[2] in x[3]
    // to possibly refine x[0] via equation.
    if (check_find(*pq1, *py, pos)) {
      // Prefix: x[3][: es].
      NSBlocks v;
      Position es = pos[0], le = pos[1];
      if (es != Position({0, 0}))
        v = prefix(3, es);
      // Crush x[3][es : ls], possibly adding x[1].
      int u = x[0].max_length() - lb_card(3, Position({0, 0}), es) 
                                - lb_card(3, le, last_fwd(py->blocks()));
      if (u > 0) {
        NSBlock b;
        b.S.include(x[1].may_chars());
        b.S.include(x[3].may_chars());
        b.u = u;
        v.push_back(b);
        for (int i = 0; i < min_occur; ++i) {
          for (int j = 0; j < pq->length(); ++j)
            v.push_back(NSBlock(pq->at(j)));
          v.push_back(b);
        }
      }
      else {
        for (int i = 0; i < min_occur; ++i)
          for (int j = 0; j < pq->length(); ++j)
            v.push_back(NSBlock(pq->at(j)));
      }
      // Suffix: x[3][le :]
      if (le != Position({py->length(), 0}))
        v.extend(suffix(3, le));
      v.normalize();
      // std::cerr << "2) Equating " << x[0] << " with " << v << " => \n";
      GECODE_ME_CHECK(x[0].dom(home, v));
      // std::cerr << x[0] << "\n";
    }
    else {
      if (min_occur > 0)
        return ES_FAILED;
      find(home, x[1], x[0], IntVar(home, 0, 0));
      rel(home, x[0], STRT_EQ, x[3]);
      return home.ES_SUBSUMED(*this);
    }
    if (!all && !check_card()) {
      rel(home, x[0], STRT_EQ, x[3]);
      return home.ES_SUBSUMED(*this);
    }
    // std::cerr<<"After replace: "<< x <<"\n";
    assert (px->is_normalized() && pq->is_normalized() 
        && pq1->is_normalized() && py->is_normalized());
    switch (
      x[1].assigned() + x[2].assigned() + x[0].assigned() + x[3].assigned()
    ) {
      case 4:
      case 3:
        return ES_NOFIX;
      case 2:
        return x[1].assigned() && x[0].assigned() ? ES_NOFIX : ES_FIX;
      default:
        return ES_FIX;
    }
  }

}}
