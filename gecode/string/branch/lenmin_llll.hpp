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
//      std::cerr << "\"" << s << "\" chosen!\n";
      if (a == 0)
        return me_failed(x[p.pos].eq(home, s)) ? ES_FAILED : ES_OK;
      if (ext.size() == 1)
        return ES_FAILED;
      x[p.pos].ext_remove(j);
      return ES_OK;
    }

}}}
