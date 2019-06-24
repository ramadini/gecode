namespace Gecode { namespace String { namespace Branch {

    None_LLLL::None_LLLL(Home home, ViewArray<String::StringView>& x0):
    StringBrancher(home, x0) {};

    None_LLLL::None_LLLL(Home home, None_LLLL& b):
    StringBrancher(home, b, b.start) {
      x.update(home, b.x);
    }

    Actor*
    None_LLLL::copy(Space& home) {
      return new (home) None_LLLL(home, *this);
    }

    void
    None_LLLL::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) None_LLLL(home, x);
    }

    Choice*
    None_LLLL::choice(Space&) {
      DashedString* p = x[start].pdomain();
//      std::cerr << "Var. " << start << " chosen: " << *p;
      return val_llll(start, p);
    }

    ExecStatus
    None_LLLL::commit(Space& home, const Choice& c, unsigned a) {
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
