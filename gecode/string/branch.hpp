namespace Gecode { namespace String { namespace Branch {

  forceinline Choice*
  StringBrancher::val_llll(int pos, Gecode::String::StringView& x) const {
    if (x.min_length() < x.max_length())
      return new PosLevVal(*this, pos, Lev::LENGTH, Val::MIN);
    Block& b = x[x.leftmost_unfixed_idx()];
    if (b.lb() < b.ub())
      return new PosLevVal(*this, pos, Lev::CARD, Val::MIN);
    else
      return new PosLevVal(*this, pos, Lev::BASE, Val::MIN);
  }
  
  forceinline Choice*
  StringBrancher::val_lllm(int pos, Gecode::String::StringView& x) const {
    if (x.min_length() < x.max_length())
      return new PosLevVal(*this, pos, Lev::LENGTH, Val::MIN);
    Block& b = x[x.leftmost_unfixed_idx()];
    if (b.lb() < b.ub())
      return new PosLevVal(*this, pos, Lev::CARD, Val::MIN);
    else
      return new PosLevVal(*this, pos, Lev::BASE, Val::MUST_MIN);
  }
  
  forceinline Choice*
  StringBrancher::val_lslm(int pos, Gecode::String::StringView& x) const {
    if (x.min_length() < x.max_length())
      return new PosLevVal(*this, pos, Lev::LENGTH, Val::MIN);
    Block& b = x[x.smallest_unfixed_idx()];
    if (b.lb() < b.ub())
      return new PosLevVal(*this, pos, Lev::CARD, Val::MIN);
    else
      return new PosLevVal(*this, pos, Lev::BASE, Val::MUST_MIN);
  }
  
  forceinline void
  StringBrancher::commit1(Space& home, StringView& x, Lev lev, Val val, int i) {
    Block& block = x[i];
    switch (lev) {
      case LENGTH: {
        if (x.min_length() == x.max_length()) {
          home.fail();
          return;
        }
        switch (val) {
          case MIN:
            x.min_length(home, x.min_length() + 1);
            return;
          case MAX:
            x.max_length(home, x.max_length() - 1);
            return;
          default:
            GECODE_NEVER;
        }
      }
      case CARD: {
        if (block.lb() == block.ub()) {
          home.fail();
          return;
        }
        switch (val) {
          case MIN:
             block.lb(home, block.lb()+1);
             x.bnd_length(home, x.min_length()+1, x.max_length());
             return;
          case MAX:
             block.ub(home, block.ub()-1);
             x.bnd_length(home, x.min_length(), x.max_length()-1);
             return;
          default:
            GECODE_NEVER;
        }
      }
      case BASE: {
        if (block.baseSize() == 1) {
          home.fail();
          return;
        }
        Gecode::Set::LUBndSet S1;
//        block.includeBaseIn(home,S1);
        int l = block.lb();
        Gecode::Set::SetDelta d;
        switch (val) {
          case MIN: {
            int m = S1.min();
            S1.exclude(home, m, m, d);
            break;
          }
          case MAX: {
            int m = S1.max();
            S1.exclude(home, m, m, d);
            break;
          }
          case MUST_MIN: {
//            int m;
//            if (_MUST_CHARS.disjoint(S1))
//              m = S1.min();
//            else {
//              NSIntSet t(_MUST_CHARS);
//              t.intersect(S1);
//              m = t.min();
//            }
//            S1.exclude(home, m, m, d);
            break;
          }
          default:
            GECODE_NEVER;
        }
        bool norm = (i > 0 /* && x[i-1].baseEquals(S1)*/) ||
          (l == 1 && i < x.size() - 1 /*&& x[i+1].baseEquals(S1)*/);
        if (l == 1 && !norm) {
//          x[i].baseUpdate(home,S1);
          return;
        }
//        _blocks.insert(h, i, DSBlock(h, S1, 1, 1));
//        _blocks.at(i + 1).l--;
//        _blocks.at(i + 1).u--;
        if (norm)
          x.normalize(home);
        return;
      }
      default:
        GECODE_NEVER;
    }
  }
  
  forceinline void
  StringBrancher::commit0(Space& home, StringView& x, Lev lev, Val val, int i) {
    Block& block = x[i];
    switch (lev) {
      case LENGTH: {
        assert (x.min_length() < x.max_length());
        switch (val) {
          case MIN:
            x.max_length(home, x.min_length());
            return;
          case MAX:
            x.min_length(home, x.max_length());
            return;
          default:
            GECODE_NEVER;
        }
      }
      case CARD: {
        int k = block.ub() - block.lb();
        assert (k > 0);
        switch (val) {
          case MIN:
            block.ub(home, block.lb());
            x.max_length(home, x.max_length()-k);
            return;
          case MAX:
            block.lb(home, block.ub());
            x.min_length(home, x.min_length()+k);
            return;
          default:
            GECODE_NEVER;
        }
      }
      case BASE: {
        int u = block.ub();
        assert (u > 0 && block.lb() == u && block.baseSize() > 1);
        int c;
        switch (val) {
          case MIN:
            c = block.baseMin();
            break;
          case MAX:
            c = block.baseMax();
            break;
          case MUST_MIN:
            c = block.baseMin();
            if (!_MUST_CHARS.in(c)) { 
              Gecode::Set::BndSetRanges i(_MUST_CHARS);
              Gecode::Set::BndSetRanges j = block.ranges();
              while (i() && j()) {
                int li = i.min(), lj = j.min(), ui = i.max(), uj = j.max();
                if (li > uj)
                  ++j;
                else if (lj > ui)
                  ++i;
                else
                  c = li > lj ? li : lj;
              }
            }
            break;
          default:
            GECODE_NEVER;
        }
//        x.splitBlock(home, block, c);
        return;
      }
      default:
        GECODE_NEVER;
    }
  }
  
  forceinline void
  StringBrancher::commit(Space& home, StringView& x, Lev l, Val v, Blc b, unsigned a) {
    int i;
    switch (b) {
    case LEFTMOST:
      i = x.leftmost_unfixed_idx();
      break;
    case RIGHTMOST:
      i = x.rightmost_unfixed_idx();
      break;
    case SMALLEST:
      i = x.smallest_unfixed_idx();
      break;
    default:
      GECODE_NEVER;
    }
    a == 0 ? commit0(home, x, l, v, i) : commit1(home, x, l, v, i);  
    assert (x.isOK());
  }

  
}}}
