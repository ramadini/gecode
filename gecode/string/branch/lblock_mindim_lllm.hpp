namespace Gecode { namespace String { namespace Branch {

    forceinline
    LBlock_MinDim_LLLM::LBlock_MinDim_LLLM(Home home, 
                                           ViewArray<String::StringView>& x0)
    : StringBrancher(home, x0) {};

    forceinline 
    LBlock_MinDim_LLLM::LBlock_MinDim_LLLM(Home home, LBlock_MinDim_LLLM& b)
    : StringBrancher(home, b, b.start) {
      x.update(home, b.x);
    }

    forceinline Actor*
    LBlock_MinDim_LLLM::copy(Space& home) {
      return new (home) LBlock_MinDim_LLLM(home, *this);
    }

    forceinline void
    LBlock_MinDim_LLLM::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) LBlock_MinDim_LLLM(home, x);
    }

    forceinline Choice*
    LBlock_MinDim_LLLM::choice(Space& home) {
//      std::cerr << "\nVar. choice\n";
      StringView& vx = x[start];
      Block& b = vx[vx.smallest_unfixed_idx()];
      double d = b.logdim();
      int l = b.ub() - b.lb();
      int m = vx.min_length();
      int pos = start;      
//      std::cerr<<x[start]<<" (pos. "<<start<<", dim. "<<d<<")\n";
      if (_FIRST) {        
        assert (StringBrancher::_MUST_CHARS.empty());
        Gecode::Set::GLBndSet s;
        for (int i = 0; i < start; ++i) {
          const StringView& vi = x[i];
          for (int j = 0; j < vi.size(); ++j) {
            Gecode::Set::SetDelta d;
            int m = vi[j].baseMin();
            s.include(home, m, m, d);
          }
        }
        for (int i = start; i < x.size(); ++i) {
          const StringView& vi = x[i];
          for (int j = 0; j < vi.size(); ++j)
            if (vi[j].baseSize() == 1) {
              Gecode::Set::SetDelta d;
              int m = vi[j].baseMin();
              s.include(home, m, m, d);
            }
        }
//        std::cerr<<"Must chars: "<<CharSet(home,s)<<"\n";        
        _MUST_CHARS.resize(s.size()*2);
        Gecode::Set::BndSetRanges is(s);
        for (int i = 0; is(); ++is, i += 2) {
          _MUST_CHARS[i]   = is.min();
          _MUST_CHARS[i+1] = is.max();
        }
        _FIRST = false;
      }
      for (int i = start + 1; i < x.size(); ++i) {
        StringView& xi = x[i];
        if (!xi.assigned()) {
          if (xi.degree() == 0) {
            std::cerr << "Warning: " << xi << " has degree 0!\n";
            xi.varimp()->gets(home, std::vector<int>());
            continue;
          }
          Block& bi = xi[xi.leftmost_unfixed_idx()];
          double di = bi.logdim();
          int li = bi.ub() - bi.lb();
          int mi = xi.min_length();
//          std::cerr<<x[i]<<" (pos. "<<i<<", dim. "<<di<<")\n";
          if (di < d || (di == d && li < l) || (di == d && li == l && mi < m)) {
            d = di;   
            l = li;
            m = mi;
            pos = i;
          }
        }
      }
//       std::cerr << "Chosen var. " << x[pos] << " (pos. " << pos << ")\n";
//       abort();
      return val_lllm(pos, x[pos]);
    }

    forceinline ExecStatus
    LBlock_MinDim_LLLM::commit(Space& home, const Choice& c, unsigned a) {
      const PosLevVal& p = static_cast<const PosLevVal&>(c);
//       std::cerr << '\n'; this->print(home, c, a, std::cerr); std::cerr << '\n';
      StringBrancher::commit(home, x[p.pos], p.lev, p.val, Blc::LEFTMOST, a);
      return ES_OK;
    }

}}}
