namespace Gecode { namespace String { namespace Branch {

    LenMax_LLLL::LenMax_LLLL(Home home, ViewArray<String::StringView>& x0):
    StringBrancher(home, x0) {};

    LenMax_LLLL::LenMax_LLLL(Home home, LenMax_LLLL& b):
    StringBrancher(home, b) {}

    Actor*
    LenMax_LLLL::copy(Space& home) {
      return new (home) LenMax_LLLL(home, *this);
    }

    void
    LenMax_LLLL::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) LenMax_LLLL(home, x);
    }

    Choice*
    LenMax_LLLL::choice(Space&) {
      DashedString* p = x[start].pdomain();
      int l = p->max_length() - p->min_length();
      int pos = start;
      for (int i = start + 1; i < x.size(); ++i) {
        DashedString* q = x[i].pdomain();
        int m = q->max_length() - q->min_length();
        if (!x[i].assigned() && m > l) {
          p = q;
          l = m;
          pos = i;
        }
      }
      return val_llll(pos, p);
    }

    ExecStatus
    LenMax_LLLL::commit(Space& home, const Choice& c, unsigned a) {
      const PosLevVal& p = static_cast<const PosLevVal&>(c);
      // this->print(home, c, a, std::cerr); std::cerr << '\n';
      return commit_choice(home, p, Block::FIRST, a);
    }

}}}
