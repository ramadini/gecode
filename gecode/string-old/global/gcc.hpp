namespace Gecode { namespace String {

  forceinline
  GCC::GCC(
    Home home, StringView x, const vec2& cov, ViewArray<Gecode::Int::IntView>& c
  ) : MixNaryOnePropagator
  <Gecode::Int::IntView, Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM>
  (home, c, x), A(cov) {
    for (unsigned i = 0; i < A.size(); ++i)
      C.add(A[i].first);
  }

  forceinline
  GCC::GCC(Space& home, GCC& p)
  : MixNaryOnePropagator
  <Gecode::Int::IntView, Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> 
    (home, p), A(p.A), C(p.C) {}

  forceinline ExecStatus
  GCC::post(
    Home home, StringView x, const vec2& cov, ViewArray<Gecode::Int::IntView> c
  ) {
    for (int i = 0; i < c.size(); ++i) {
      GECODE_ME_CHECK(c[i].gq(home, 0));
      GECODE_ME_CHECK(c[i].lq(home, x.max_length()));
    }
    (void) new (home) GCC(home, x, cov, c);
    return ES_OK;
  }

  forceinline Actor*
  GCC::copy(Space& home) {
    return new (home) GCC(home, *this);
  }

  forceinline ExecStatus
  GCC::propagate(Space& home, const ModEventDelta& m) {
    //std::cerr<<"\nGCC::propagate GCC("<<y<<", "<<x<<") -- chars = "<<C<<"\n";
    int n = x.size();
    // y is a knwon string.
    if (y.assigned()) {
      string s = y.val();
      for (int i = 0; i < n; ++i)
        GECODE_ME_CHECK(x[A[i].second].eq(home,
          (int) std::count(s.begin(), s.end(), int2char(A[i].first)))
        );
      return home.ES_SUBSUMED(*this);
    }
    int sl = 0, su = 0, mal = y.max_length();
    const NSIntSet& dom = y.may_chars();
    // Refining upper bounds of count variables and possibly narrow C.
    for (int i = 0; i < x.size(); ++i) {
      Gecode::Int::IntView& xi = x[i];
      int mxi = x[i].min();
      GECODE_ME_CHECK(xi.lq(home, mal));
      sl += mxi;
      su += xi.max();
      if (sl > mal || (mxi > 0 && !dom.contains(A[i].first)))
        return ES_FAILED;
      if (su > DashedString::_MAX_STR_LENGTH)
        su = DashedString::_MAX_STR_LENGTH;
      if (xi.assigned())
        C.remove(xi.val());
    }
    // Refining upper bound of count variables.
    for (int i = 0; i < n; ++i)
      if (sl - x[i].min() + x[i].max() > mal)
        GECODE_ME_CHECK(x[i].lq(home, mal - sl + x[i].min()));
    bool changed = false;
    DashedString* pd = y.pdomain();
    if (mal <= sl) {
      changed = true;
      for (int i = 0; i < pd->length(); ++i) {
        DSBlock& b = pd->at(i);
        b.S.intersect(home, C);
        if (b.S.empty()) {
          if (b.l > 0)
            return ES_FAILED;
          else
            b.u = 0;
        }
      }
    }
    vec2 S(n, tpl2(0, 0));
    // S[k] = (a, b) iff a <= #{occurrences of A[k].first in string y} <= b
    for (int i = 0; i < pd->length(); ++i) {
      const DSBlock& b = pd->at(i);
      int j = 0;
      for (RangeList* c = b.S.ranges(); j < n && c != NULL; ) {
        int k = A[j].first;
        int l = c->min(), u = c->max();
        if (l <= k && k <= u) {
          if (b.S.size() == 1)
            S[j].first  += b.l;
          S[j].second += b.u;
          ++j;
        }
        else if (k < l)
          ++j;
        else
          c = c->next();
      }
    }
    for (int i = 0; i < pd->length(); ++i) {
      DSBlock& b = pd->at(i);
      if (!b.S.disjoint(C)) {
        // Refining lower bound of the cardinalities of y.
        int l = 0;
        for (int j = 0; j < n; ++j) {
          int k = A[j].second;
          // Possibly refining the bound of count variables.
          if (b.S.size() == 1) {
            if (S[j].first > x[k].min())
              GECODE_ME_CHECK(x[k].gq(home, S[j].first));
            if (S[j].second < x[k].max())
              GECODE_ME_CHECK(x[k].lq(home, S[j].second));
            l += max(0, x[k].min() - S[j].second + b.u);
          }        
        }
        if (b.l < l && l <= b.u) {
          changed = true;
          b.l = l;
        }
        if (b.known())
          continue;
        // Refining upper bound of the cardinalities of y (and maybe its bases).
        if (C.contains(b.S)) {
          int u = 0;
          for (int j = 0; j < n; ++j) {
          int c = A[j].first, k = A[j].second;
            int m = x[k].max() - S[j].first;
            if (b.S.size() == 1)
              m += b.l;
            if (m == 0) {
              changed = true;
              Gecode::Set::SetDelta d;
              b.S.exclude(home, c, c, d);
            }
            u += m;
            if (u >= b.u)
              break;
          }
          if (u < b.u && b.l <= u) {
            changed = true;
            b.u = u;
          }
        }
      }
    }
    if (changed) {
      sl = su = 0;
      for (int i = 0; i < pd->length(); ++i) {
        const DSBlock& b = pd->at(i);
        sl += b.l;
        su += b.u;
        if (su > DashedString::_MAX_STR_LENGTH)
          su = DashedString::_MAX_STR_LENGTH;
      }
      pd->refine_card(home, sl, su);
      pd->normalize(home);
      StringDelta d(changed);
      GECODE_ME_CHECK(y.varimp()->notify(
        home, y.assigned() ? ME_STRING_VAL : ME_STRING_DOM, d)
      );
    }
    //std::cerr<<"\nGCC::propagated GCC("<<y<<", "<<x<<") -- chars = "<<C<<"\n";
    assert (pd->is_normalized());
    return y.assigned() ? propagate(home, m) : ES_FIX;
  }

}}
