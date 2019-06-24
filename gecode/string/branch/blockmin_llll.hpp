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
      DashedString* p = x[start].pdomain();
      int k = p->first_na_block();
      double c = p->at(k).logdim();
      int l = p->min_length();
      int pos = start;
      int d = 0;
      // std::cerr << x[start] << " (pos. " << start << ")\n";
      for (int i = start + 1; i < x.size(); ++i) {
        if (!x[i].assigned()) {
          int di = x[i].degree();
          if (di == 0) {
            // std::cerr << "Warning: " << x[i] << " has degree 0!\n";
            x[i].pdomain()->update(home, "");
            continue;
          }
          //std::cerr << x[i] << " ext list = ["; for (auto e : x[i].ext_list())
          //std::cerr<<"\""<< e << "\", "; std::cerr<< "] (pos. " << i << ")\n";
          const DSBlock& b = p->at(k);
          DashedString* q = x[i].pdomain();
          double ki = q->first_na_block();
          int li = q->min_length();
          const DSBlock& bi = q->at(ki);
          int s = x[i].ext_list().size();
          double bil = bi.logdim();
          double ci = s > 0 ? min((double) s, bil) : bil;
          if ((ci < c)
          || (ci == c && bi.l > b.l)
        	|| (ci == c && bi.l == b.l && li < l)
        	|| (ci == c && bi.l == b.l && li < l && di > d)) {
              l = li;
              c = ci;
              k = ki;
              d = di;
              pos = i;
              p = q;
          }
        }
      }
      //std::cerr << "Chosen var. " << x[pos] << " (pos. " << pos << ")\n";
      // abort();
      return val_llll(pos, p);
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
