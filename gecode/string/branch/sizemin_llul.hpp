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
//        std::cerr << "Var. " << i << ": " << *(x[i].pdomain()) << " -- logdim: " << m << "\n";
        if (!x[i].assigned() && m < l) {
          p = x[i].pdomain();
          l = m;
          pos = i;
        }
      }
//      std::cerr << *p << " chosen for var. " << pos << '\n';
      return val_llul(pos, p);
    }

    ExecStatus
    SizeMin_LLUL::commit(Space& home, const Choice& c, unsigned a) {
        const PosLevVal& p = static_cast<const PosLevVal&>(c);
        const std::vector<string>& ext = x[p.pos].ext_list();
        if (ext.empty()) {
          x[p.pos].commit(home, p.lev, p.val, Block::FIRST, a);
          return ES_OK;
        }
  //      std::cerr<<"ext list = ["; for(auto x:ext) std::cerr<<"\""<<x<< "\", "; std::cerr<<"]\n";
        string s = ext[0];
        unsigned j = 0, m = ext[0].size();
        for (unsigned i = 1; i < ext.size(); ++i) {
          if (ext[i].size() < m || (ext[i].size() == m && ext[i] < s)) {
            s = ext[i];
            m = ext[i].size();
            j = i;
          }
        }
//        std::cerr << "\"" << s << "\" chosen!\n";
        if (a == 0)
          return me_failed(x[p.pos].eq(home, s)) ? ES_FAILED : ES_OK;
        if (ext.size() == 1)
          return ES_FAILED;
        x[p.pos].ext_remove(j);
        return ES_OK;
    }

}}}
