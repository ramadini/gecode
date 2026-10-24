namespace Gecode { namespace String { namespace Branch {

    forceinline
    BlockMin_LLLL::BlockMin_LLLL(Home home, ViewArray<String::StringView>& x0)
    : StringBrancher(home, x0) {};

    forceinline
    BlockMin_LLLL::BlockMin_LLLL(Home home, BlockMin_LLLL& b)
    : StringBrancher(home, b) {}

    forceinline Actor*
    BlockMin_LLLL::copy(Space& home) {
      return new (home) BlockMin_LLLL(home, *this);
    }

    forceinline void
    BlockMin_LLLL::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) BlockMin_LLLL(home, x);
    }

    forceinline Choice*
    BlockMin_LLLL::choice(Space&) {
      const DashedString* p = &x[start].domain();
      const DSBlock& b = p->at(p->first_na_block());
      double s = b.logdim();
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
          if (si < s || (si == s && li < l) || (si == s && li == l && mi < m)) {
            s = si;
            l = li;
            m = mi;
            pos = i;
          }
        }
      }
      return val_llll(pos, &x[pos].domain());
    }

    forceinline ExecStatus
    BlockMin_LLLL::commit(Space& home, const Choice& c, unsigned a) {
      return commit_choice(home, c, a);
    }

}}}

// STATISTICS: string-branch
