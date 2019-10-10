namespace Gecode { namespace String { namespace Branch {

    LenMin_LLLL::LenMin_LLLL(Home home, ViewArray<String::StringView>& x0):
    StringBrancher(home, x0) {};

    LenMin_LLLL::LenMin_LLLL(Home home, LenMin_LLLL& b):
    StringBrancher(home, b, b.start) {
      x.update(home, b.x);
    }

    Actor*
    LenMin_LLLL::copy(Space& home) {
      return new (home) LenMin_LLLL(home, *this);
    }

    void
    LenMin_LLLL::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) LenMin_LLLL(home, x);
    }

    Choice*
    LenMin_LLLL::choice(Space&) {
      DashedString* p = x[start].pdomain();
      int l = p->max_length() - p->min_length();
      int pos = start;
      for (int i = start + 1; i < x.size(); ++i) {
        DashedString* q = x[i].pdomain();
        int m = q->max_length() - q->min_length();
        if (!x[i].assigned() && m < l) {
          p = q;
          l = m;
          pos = i;
        }
      }
      return val_llll(pos, p);
    }

    ExecStatus
    LenMin_LLLL::commit(Space& home, const Choice& c, unsigned a) {
      const PosLevVal& p = static_cast<const PosLevVal&>(c);
      // this->print(home, c, a, std::cerr); std::cerr << '\n';
      x[p.pos].commit(home, p.lev, p.val, Block::FIRST, a);
      return ES_OK;
    }
}}}
