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
      // this->print(home, c, a, std::cerr); std::cerr << '\n';
      const std::vector<string>& ext = x[p.pos].ext_list();
      // std::cerr << "ext_list.size() = " << ext.size() << "\n";
      if (ext.empty()) {
        x[p.pos].commit(home, p.lev, p.val, Block::FIRST, a);
        // std::cerr << a << ") Set: " << x[p.pos] << "\n";
        return ES_OK;
      }
      // std::cerr<< a << ") ext list = ["; for(auto x:ext) std::cerr<<"\""<<x<< "\", "; std::cerr<<"]\n";
      if (a == 1 && ext.size() == 1)
        return ES_FAILED;
      string s = ext[0];
      if (a == 0) {
        // std::cerr << "\"" << s << "\" chosen!\n";
        return me_failed(x[p.pos].eq(home, s)) ? ES_FAILED : ES_OK;
      }
      x[p.pos].ext_remove(0);
      if (ext.size() == 1) {
        // std::cerr << "\"" << ext[0] << "\" chosen!\n";
        return me_failed(x[p.pos].eq(home, ext[0])) ? ES_FAILED : ES_OK;
      }
      return ES_OK;
    }

}}}
