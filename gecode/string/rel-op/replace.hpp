namespace Gecode { namespace String {

  // x[3] is the string resulting from replacing the first occurrence of x[0] 
  // with x[1] in the target string x[2].

  forceinline ExecStatus
  Replace::post(Home home, ViewArray<StringView>& x) {
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
    (void) new (home) Replace(home, x);
    return ES_OK;
  }

  forceinline Actor*
  Replace::copy(Space& home) {
    return new (home) Replace(home, *this);
  }

  forceinline
  Replace::Replace(Home home, ViewArray<StringView>& x)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, x) {}

  forceinline
  Replace::Replace(Space& home, Replace& p)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, p) {}


  // Crush x[k][p : q] into a single block.
  forceinline NSBlock
  Replace::crush(int k, const Position& p, const Position& q) {
    NSBlock block;
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
  Replace::prefix(int k, const Position& p) {
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
  Replace::suffix(int k, const Position& p) {
    NSBlocks suff;
    DashedString* px = x[k].pdomain();
    int off = p.off;
    if (off > 0) {
      const DSBlock& b = px->at(p.idx);
      suff.push_back(NSBlock(b.S, max(0, off - b.u + b.l), b.u));
    }
    for (int i = p.idx + 1; i < px->length(); ++i)
      suff.push_back(NSBlock(px->at(i)));
    return suff;
  }

  forceinline ExecStatus
  Replace::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"\nReplace::propagate: "<< x <<"\n";
    assert(x[0].pdomain()->is_normalized() && x[1].pdomain()->is_normalized() &&
           x[2].pdomain()->is_normalized() && x[3].pdomain()->is_normalized());
    bool occur = false;
    if (x[0].assigned()) {
      string sx = x[0].val();
      if (sx == "") {
        rel(home, x[1], x[2], STRT_CAT, x[3]);
        return home.ES_SUBSUMED(*this);
      }
      if (x[2].assigned()) {
        string sy = x[2].val();
        int n = sy.find(sx);
        string pref = sy.substr(0, n);
        string suff = sy.substr(n + sx.size());
        if (x[1].assigned())
          GECODE_ME_CHECK(x[3].eq(home, pref + x[1].val() + suff));
        else {
          StringVar z(home);
          rel(home, StringVar(home, pref), x[1], STRT_CAT, z);
          rel(home, z, StringVar(home, suff), STRT_CAT, x[3]);
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
    // If x[0] must not occur in x[2], then x[2] = x[3]. Otherwise, we use the 
    // earliest/latest start/end positions of x[0] in x[2] to possibly refine 
    // x[3] via equation.
    Position pos[4];
    if (sweep_replace(*x[0].pdomain(), *x[2].pdomain(), pos)) {      
      // Prefix: x[2][: es]
      NSBlocks v = prefix(2, pos[0]);
      // Crush x[2][ee : le] into a single block.  
      v.push_back(crush(2, pos[0], pos[1]));
      // If x[0] surely occcurs in x[2], then x[1] replaces x[0] in x[3].
      if (occur) {
        DashedString* px = x[1].pdomain();
        for (int i = 0; i < px->length(); ++i)
          v.push_back(NSBlock(px->at(i)));
      }
      // Crush x[2][ee : le] into a single block.
      v.push_back(crush(2, pos[2], pos[3]));
      // Suffix: x[2][le :]
      v.extend(suffix(2, pos[3]));
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
      NSBlocks v = prefix(3, pos[0]);
      // Crush x[3][ee : le] into a single block.
      v.push_back(crush(3, pos[0], pos[1]));      
      if (occur) {
        DashedString* px = x[0].pdomain();
        for (int i = 0; i < px->length(); ++i)
          v.push_back(NSBlock(px->at(i)));
      }
      // Crush x[3][ee : le] into a single block.
      v.push_back(crush(3, pos[2], pos[3]));
      // Suffix: x[3][le :]
      v.extend(suffix(3, pos[3]));
      v.normalize();
      // std::cerr << "Equating y' = " << x[3] << " with " << v << "\n";
      GECODE_ME_CHECK(x[2].dom(home, v));
    }
    else {
      find(home, x[0], x[2], IntVar(home, 0, 0));
      rel(home, x[2], STRT_EQ, x[3]);      
      return home.ES_SUBSUMED(*this);
    }
    // std::cerr<<"After replace: "<< x <<"\n";
    return x[0].assigned() && x[2].assigned() ? ES_NOFIX : ES_FIX;    
  }

}}
