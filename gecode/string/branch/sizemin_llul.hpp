namespace Gecode { namespace String { namespace Branch {

    SizeMin_LLUL::SizeMin_LLUL(Home home, ViewArray<String::StringView>& x0):
    StringBrancher(home, x0) {};

    SizeMin_LLUL::SizeMin_LLUL(Home home, SizeMin_LLUL& b):
    StringBrancher(home, b, b.start) {
      x.update(home, b.x);
    }

    Actor*
    SizeMin_LLUL::copy(Space& home) {
      return new (home) SizeMin_LLUL(home, *this);
    }

    void
    SizeMin_LLUL::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) SizeMin_LLUL(home, x);
    }

    Choice*
    SizeMin_LLUL::choice(Space&) {
      DashedString* p = x[start].pdomain();
      double l = p->logdim();
      int pos = start;
      for (int i = start + 1; i < x.size(); ++i) {
        double m = x[i].pdomain()->logdim();
        // std::cerr << "Var. " << i << ": " << *(x[i].pdomain()) << " -- logdim: " << m << "\n";
        if (!x[i].assigned() && m < l) {
          p = x[i].pdomain();
          l = m;
          pos = i;
        }
      }
      // std::cerr << *p << " chosen for var. " << pos << '\n';
      return val_llul(pos, p);
    }

    ExecStatus
    SizeMin_LLUL::commit(Space& home, const Choice& c, unsigned a) {
      const PosLevVal& p = static_cast<const PosLevVal&>(c);
      // this->print(home, c, a, std::cerr); std::cerr << '\n';
      x[p.pos].commit(home, p.lev, p.val, Block::FIRST, a);
      return ES_OK;
    }

}}}
