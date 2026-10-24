namespace Gecode { namespace String { namespace Branch {

    forceinline
    SizeMin_LLUL::SizeMin_LLUL(Home home, ViewArray<String::StringView>& x0):
    StringBrancher(home, x0) {};

    forceinline
    SizeMin_LLUL::SizeMin_LLUL(Home home, SizeMin_LLUL& b):
    StringBrancher(home, b) {}

    forceinline Actor*
    SizeMin_LLUL::copy(Space& home) {
      return new (home) SizeMin_LLUL(home, *this);
    }

    forceinline void
    SizeMin_LLUL::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) SizeMin_LLUL(home, x);
    }

    forceinline Choice*
    SizeMin_LLUL::choice(Space&) {
      const DashedString* p = &x[start].domain();
      double l = p->logdim();
      int pos = start;
      for (int i = start + 1; i < x.size(); ++i) {
        double m = x[i].domain().logdim();
        if (!x[i].assigned() && m < l) {
          p = &x[i].domain();
          l = m;
          pos = i;
        }
      }
      return val_llll(pos, p);
    }

    forceinline ExecStatus
    SizeMin_LLUL::commit(Space& home, const Choice& c, unsigned a) {
      return commit_choice(home, c, a);
    }

}}}

// STATISTICS: string-branch
