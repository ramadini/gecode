namespace Gecode { namespace String { namespace Branch {

    forceinline
    LenMax_LLLL::LenMax_LLLL(Home home, ViewArray<String::StringView>& x0):
    StringBrancher(home, x0) {};

    forceinline
    LenMax_LLLL::LenMax_LLLL(Home home, LenMax_LLLL& b):
    StringBrancher(home, b) {}

    forceinline Actor*
    LenMax_LLLL::copy(Space& home) {
      return new (home) LenMax_LLLL(home, *this);
    }

    forceinline void
    LenMax_LLLL::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) LenMax_LLLL(home, x);
    }

    forceinline Choice*
    LenMax_LLLL::choice(Space&) {
      const DashedString* p = &x[start].domain();
      int l = p->max_length() - p->min_length();
      int pos = start;
      for (int i = start + 1; i < x.size(); ++i) {
        const DashedString* q = &x[i].domain();
        int m = q->max_length() - q->min_length();
        if (!x[i].assigned() && m > l) {
          p = q;
          l = m;
          pos = i;
        }
      }
      return val_llll(pos, p);
    }

    forceinline ExecStatus
    LenMax_LLLL::commit(Space& home, const Choice& c, unsigned a) {
      return commit_choice(home, c, a);
    }

}}}

// STATISTICS: string-branch
