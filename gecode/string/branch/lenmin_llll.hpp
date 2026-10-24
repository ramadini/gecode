namespace Gecode { namespace String { namespace Branch {

    forceinline
    LenMin_LLLL::LenMin_LLLL(Home home, ViewArray<String::StringView>& x0):
    StringBrancher(home, x0) {};

    forceinline
    LenMin_LLLL::LenMin_LLLL(Home home, LenMin_LLLL& b):
    StringBrancher(home, b) {}

    forceinline Actor*
    LenMin_LLLL::copy(Space& home) {
      return new (home) LenMin_LLLL(home, *this);
    }

    forceinline void
    LenMin_LLLL::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) LenMin_LLLL(home, x);
    }

    forceinline Choice*
    LenMin_LLLL::choice(Space&) {
      const DashedString* p = &x[start].domain();
      int l = p->max_length() - p->min_length();
      int pos = start;
      for (int i = start + 1; i < x.size(); ++i) {
        const DashedString* q = &x[i].domain();
        int m = q->max_length() - q->min_length();
        if (!x[i].assigned() && m < l) {
          p = q;
          l = m;
          pos = i;
        }
      }
      return val_llll(pos, p);
    }

    forceinline ExecStatus
    LenMin_LLLL::commit(Space& home, const Choice& c, unsigned a) {
      return commit_choice(home, c, a);
    }
}}}

// STATISTICS: string-branch
