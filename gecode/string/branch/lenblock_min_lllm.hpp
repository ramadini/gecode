namespace Gecode { namespace String { namespace Branch {

    forceinline
    LenBlock_Min_LLLM::LenBlock_Min_LLLM(Home home, 
                                           ViewArray<String::StringView>& x0)
    : StringBrancher(home, x0) {};

    forceinline 
    LenBlock_Min_LLLM::LenBlock_Min_LLLM(Home home, LenBlock_Min_LLLM& b)
    : StringBrancher(home, b, b.start) {
      x.update(home, b.x);
    }

    forceinline Actor*
    LenBlock_Min_LLLM::copy(Space& home) {
      return new (home) LenBlock_Min_LLLM(home, *this);
    }

    forceinline void
    LenBlock_Min_LLLM::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) LenBlock_Min_LLLM(home, x);
    }

    forceinline Choice*
    LenBlock_Min_LLLM::choice(Space& home) {
//      std::cerr << "\nVar. choice\n";      
      if (_FIRST) {
        assert (StringBrancher::_MUST_CHARS.empty());
        Gecode::Set::GLBndSet s;
        for (int i = 0; i < start; ++i) {
          const StringView& vi = x[i];
          for (int j = 0; j < vi.size(); ++j) 
            if (vi[j].baseSize() == 1) {
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
      StringView& vx = x[start];
      const Block& b = vx[vx.smallest_unfixed_idx()];
      double d = b.logdim();
      int l = b.ub() - b.lb();
      int m = vx.min_length();
      int f = Limits::MAX_STRING_LENGTH;
      int pos = start;
//      std::cerr<<x[start]<<" (pos. "<<start<<", dim. "<<d<<")\n";
      for (int i = start + 1; i < x.size(); ++i) {
        StringView& xi = x[i];
        if (!xi.assigned()) {
          if (xi.degree() == 0) {
//            std::cerr << "Warning: " << xi << " has degree 0!\n";
            xi.varimp()->update(home, std::vector<int>());
            continue;
          }
          const Block& bi = xi[xi.leftmost_unfixed_idx()];
          double di = bi.logdim();
          int li = bi.ub() - bi.lb();
          int mi = xi.min_length();
          int fi = xi.max_length() - mi;
//          std::cerr<<x[i]<<" (pos. "<<i<<", dim. "<<di<<")\n";
          if (fi > 0) {
            if (fi < f) {
              f = fi;
              d = di;
              l = li;
              m = mi;
              pos = i;   
            }
            continue;
          }
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
    LenBlock_Min_LLLM::commit(Space& home, const Choice& c, unsigned a) {
      const PosLevVal& p = static_cast<const PosLevVal&>(c);
//      std::cerr << '\n'; this->print(home, c, a, std::cerr); std::cerr << '\n';
      StringBrancher::commit(home, x[p.pos], p.lev, p.val, Blc::LEFTMOST, a);
      return ES_OK;
    }

}}}
