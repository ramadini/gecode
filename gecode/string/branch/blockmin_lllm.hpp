namespace Gecode { namespace String { namespace Branch {

    forceinline
    BlockMin_LLLM::BlockMin_LLLM(Home home, ViewArray<String::StringView>& x0)
    : StringBrancher(home, x0) {};

    forceinline 
    BlockMin_LLLM::BlockMin_LLLM(Home home, BlockMin_LLLM& b)
    : StringBrancher(home, b, b.start) {
      x.update(home, b.x);
    }

    forceinline Actor*
    BlockMin_LLLM::copy(Space& home) {
      return new (home) BlockMin_LLLM(home, *this);
    }

    forceinline void
    BlockMin_LLLM::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) BlockMin_LLLM(home, x);
    }

    forceinline Choice*
    BlockMin_LLLM::choice(Space& home) {
      // std::cerr << "\nVar. choice\n";
      DashedString* p = x[start].pdomain();
      const DSBlock& b = p->at(p->first_na_block());
      double s = b.logdim();
      int l = b.u - b.l;
      int m = p->min_length();
      int pos = start;      
      // std::cerr<<x[start]<<" (pos. "<<start<<", dim. "<<s<<")\n";
      if (_FIRST)
        DashedString::_MUST_CHARS.include(x[start].must_chars());
      for (int i = start + 1; i < x.size(); ++i) {
        if (_FIRST)
          DashedString::_MUST_CHARS.include(x[i].must_chars());
        if (!x[i].assigned()) {
          int di = x[i].degree();
          if (di == 0) {
            // std::cerr << "Warning: " << x[i] << " has degree 0!\n";
            x[i].pdomain()->update(home, "");
            continue;
          }
          p = x[i].pdomain();
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
      // if (_FIRST) std::cerr<<"Must chars: "<<DashedString::_MUST_CHARS<<"\n";      
      // std::cerr << "Chosen var. " << x[pos] << " (pos. " << pos << ")\n";
      // abort();
      _FIRST = false;
      return val_lllm(pos, x[pos].pdomain());
    }

    forceinline ExecStatus
    BlockMin_LLLM::commit(Space& home, const Choice& c, unsigned a) {
      const PosLevVal& p = static_cast<const PosLevVal&>(c);
      // this->print(home, c, a, std::cerr); std::cerr << '\n';
      x[p.pos].commit(home, p.lev, p.val, Block::FIRST, a);
      return ES_OK;
    }

}}}
