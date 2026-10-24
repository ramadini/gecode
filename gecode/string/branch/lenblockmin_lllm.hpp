namespace Gecode { namespace String { namespace Branch {

    forceinline
    LenBlockMin_LLLM::LenBlockMin_LLLM(Home home, ViewArray<String::StringView>& x0)
    : StringBrancher(home, x0) {};

    forceinline
    LenBlockMin_LLLM::LenBlockMin_LLLM(Home home, LenBlockMin_LLLM& b)
    : StringBrancher(home, b) {}

    forceinline Actor*
    LenBlockMin_LLLM::copy(Space& home) {
      return new (home) LenBlockMin_LLLM(home, *this);
    }

    forceinline void
    LenBlockMin_LLLM::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) LenBlockMin_LLLM(home, x);
    }

    forceinline Choice*
    LenBlockMin_LLLM::choice(Space&) {
      const DashedString* p = &x[start].domain();
      const DSBlock& b = p->at(p->first_na_block());
      double s = b.logdim();
      int d = p->max_length() - p->min_length();
      if (d == 0)
        d = DashedString::_MAX_STR_LENGTH;
      int l = b.u - b.l;
      int m = p->min_length();
      int pos = start;
      for (int i = start + 1; i < x.size(); ++i) {
        if (!x[i].assigned()) {
          p = &x[i].domain();
          const DSBlock& bi = p->at(p->first_na_block());
          double si = bi.logdim();
          int li = bi.u - bi.l;
          int mi = p->min_length();
          int di = p->max_length() - p->min_length();
          if (di > 0) {
            if (di < d) {
              d = di;
              s = si;
              l = li;
              m = mi;
              pos = i;
            }
            continue;
          }
          if (si < s || (si == s && li < l) || (si == s && li == l && mi < m)) {
            s = si;
            l = li;
            m = mi;
            pos = i;
          }
        }
      }
      return val_lllm(pos, &x[pos].domain());
    }

    forceinline ExecStatus
    LenBlockMin_LLLM::commit(Space& home, const Choice& c, unsigned a) {
      return commit_choice(home, c, a);
    }

}}}

// STATISTICS: string-branch
