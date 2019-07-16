namespace Gecode { namespace String { namespace Branch {

    LenMax_LLLL::LenMax_LLLL(Home home, ViewArray<String::StringView>& x0):
    StringBrancher(home, x0) {};

    LenMax_LLLL::LenMax_LLLL(Home home, LenMax_LLLL& b):
    StringBrancher(home, b, b.start) {
      x.update(home, b.x);
    }

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
      unsigned j = 0, m = ext[0].size();
      for (unsigned i = 1; i < ext.size(); ++i) {
        if (ext[i].size() > m || (ext[i].size() == m && ext[i] > s)) {
          s = ext[i];
          m = ext[i].size();
          j = i;
        }
      }
      if (a == 0) {
        // std::cerr << "\"" << s << "\" chosen!\n";
        return me_failed(x[p.pos].eq(home, s)) ? ES_FAILED : ES_OK;
      }
      x[p.pos].ext_remove(j);
      if (ext.size() == 1) {
        // std::cerr << "\"" << ext[0] << "\" chosen!\n";
        return me_failed(x[p.pos].eq(home, ext[0])) ? ES_FAILED : ES_OK;
      }
      return ES_OK;
    }

}}}
