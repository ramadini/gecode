namespace Gecode { namespace String { namespace Branch {

    forceinline
    None_LLLL::None_LLLL(Home home, ViewArray<String::StringView>& x0):
    StringBrancher(home, x0) {};

    forceinline
    None_LLLL::None_LLLL(Home home, None_LLLL& b):
    StringBrancher(home, b) {}

    forceinline Actor*
    None_LLLL::copy(Space& home) {
      return new (home) None_LLLL(home, *this);
    }

    forceinline void
    None_LLLL::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) None_LLLL(home, x);
    }

    forceinline Choice*
    None_LLLL::choice(Space&) {
      const DashedString* p = &x[start].domain();
      return val_llll(start, p);
    }

    forceinline ExecStatus
    None_LLLL::commit(Space& home, const Choice& c, unsigned a) {
      return commit_choice(home, c, a);
    }

}}}

// STATISTICS: string-branch
