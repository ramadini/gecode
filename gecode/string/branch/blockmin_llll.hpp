namespace Gecode { namespace String { namespace Branch {

    forceinline
    BlockMin_LLLL::BlockMin_LLLL(Home home, ViewArray<String::StringView>& x0)
    : StringBrancher(home, x0) {};

    forceinline 
    BlockMin_LLLL::BlockMin_LLLL(Home home, BlockMin_LLLL& b)
    : StringBrancher(home, b, b.start) {
      x.update(home, b.x);
    }

    forceinline Actor*
    BlockMin_LLLL::copy(Space& home) {
      return new (home) BlockMin_LLLL(home, *this);
    }

    forceinline void
    BlockMin_LLLL::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) BlockMin_LLLL(home, x);
    }

    forceinline Choice*
    BlockMin_LLLL::choice(Space& home) {
      // std::cerr << "\nVar. choice\n";
      DashedString* p = x[start].pdomain();
      const DSBlock& b = p->at(p->first_na_block());
      double s = b.logdim();
      int l = b.u - b.l;
      int m = p->min_length();
      int e = x[start].ext_list().size();
      int pos = start;      
      // std::cerr<<x[start]<<" (pos. "<<start<<", dim. "<<s<<")\n";
      // if (e) { std::cerr << " ext list = ["; for (auto e : x[start].ext_list())        
      //  std::cerr<<"\""<< e << "\", "; std::cerr << "]\n"; }
      for (int i = start + 1; i < x.size(); ++i) {
        if (!x[i].assigned()) {
          int di = x[i].degree();
          if (di == 0) {
            // std::cerr << "Warning: " << x[i] << " has degree 0!\n";
            x[i].pdomain()->update(home, "");
            continue;
          }
          p = x[i].pdomain();
          int ei = x[i].ext_list().size();                    
          if (ei > 0 && ei < p->logdim() && (e == 0 || ei < e)) {
            // std::cerr<<x[i]<<" (pos. "<<i<<", deg. "<<di<<") ";
            // std::cerr << " ext list = ["; for (auto ei : x[i].ext_list())
            // std::cerr<<"\""<< ei << "\", "; std::cerr << "]\n";
            e = ei;
            pos = i;
            continue;
          }
          const DSBlock& bi = p->at(p->first_na_block());
          double si = bi.logdim();
          int li = b.u - bi.l;
          int mi = p->min_length();
          // std::cerr<<x[i]<<" (pos. "<<i<<", deg. "<<di<<", dim. "<<si<<")\n";
          if (si < s || (si == s && li < l) || (si == s && li == l && mi < m)) {
            s = si;   
            l = li;
            m = mi;
            pos = i;
          }
        }
      }
      // std::cerr << "Chosen var. " << x[pos] << " (pos. " << pos << ")\n";
      // abort();
      return val_llll(pos, x[pos].pdomain());
    }

    forceinline ExecStatus
    BlockMin_LLLL::commit(Space& home, const Choice& c, unsigned a) {
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
        if (ext[i].size() < m || (ext[i].size() == m && ext[i] < s)) {
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
