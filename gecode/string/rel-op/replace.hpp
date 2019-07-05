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
    DashedString* px = x[2].pdomain();
    if (sweep_replace(*x[0].pdomain(), *px, pos)) {
      NSBlocks v;
      // Prefix of x[2].
      for (int i = 0; i < pos[0].idx; ++i)
        v.push_back(NSBlock(px->at(i)));
      int off = pos[0].off;
      if (off > 0) {
        const DSBlock& b = px->at(pos[0].idx);
        if (off < b.l)
          v.push_back(NSBlock(b.S, off, off));
        else
          v.push_back(NSBlock(b.S, b.l, off));
      }
      //crush(x[2][pos[0] : pos[1]]);
      if (occur) {
        px = x[1].pdomain();
        for (int i = 0; i < px->length(); ++i)
          v.push_back(NSBlock(px->at(i)));
      }
      //crush(x[2][pos[2] : pos[3]]);
      // Suffix of x[2] [pos[3] : ];
      v.normalize();
      GECODE_ME_CHECK(x[3].dom(home, v));
    }
    else {
      rel(home, x[2], STRT_EQ, x[3]);
      return home.ES_SUBSUMED(*this);
    }
    // If x[1] must not occur in x[3], then find(x[0], x[2]) = 0 /\ x[2] = x[3].
    // Otherwise, we use the earliest/latest start/end positions of x[1] in x[3]
    // to possibly refine x[2] via equation.
    px = x[3].pdomain();
    if (sweep_replace(*x[1].pdomain(), *px, pos)) {
      NSBlocks v;
      // Prefix of x[3].
      for (int i = 0; i < pos[0].idx; ++i)
        v.push_back(NSBlock(px->at(i)));
      int off = pos[0].off;
      if (off > 0) {
        const DSBlock& b = px->at(pos[0].idx);
        if (off < b.l)
          v.push_back(NSBlock(b.S, off, off));
        else
          v.push_back(NSBlock(b.S, b.l, off));
      }
      //crush(x[3][pos[0] : pos[1]]);
      if (occur) {
        px = x[0].pdomain();
        for (int i = 0; i < px->length(); ++i)
          v.push_back(NSBlock(px->at(i)));
      }
      //crush(x[3][pos[2] : pos[3]]);
      // Suffix of x[3] [pos[3] : ];
      v.normalize();
      GECODE_ME_CHECK(x[3].dom(home, v));
    }
    else {
      find(home, x[0], x[2], IntVar(home, 0, 0));
      rel(home, x[2], STRT_EQ, x[3]);      
      return home.ES_SUBSUMED(*this);
    }    
    return x[0].assigned() && x[2].assigned() ? ES_NOFIX : ES_FIX;
    // std::cerr<<"After replace: "<< x <<"\n";
    return ES_FIX;
  }

}}
