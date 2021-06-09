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
    Block& x_i = x[i];
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
        if (x_i.lb() == x_i.ub()) {
          home.fail();
          return;
        }
        switch (val) {
          case MIN:
             x_i.lb(home, x_i.lb()+1);
             x.min_length(home, x.min_length()+1);
             return;
          case MAX:
             x_i.ub(home, x_i.ub()-1);
             x.max_length(home, x.max_length()+1);
             return;
          default:
            GECODE_NEVER;
        }
      }
      case BASE: {
        if (x_i.baseSize() == 1) {
          home.fail();
          return;
        }
        Gecode::Set::LUBndSet S1;
//        x_i.includeBaseIn(home,S1);
        int l = x_i.lb();
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
//        _x_is.insert(h, i, DSBlock(h, S1, 1, 1));
//        _x_is.at(i + 1).l--;
//        _x_is.at(i + 1).u--;
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
    Block& x_i = x[i];
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
        int k = x_i.ub() - x_i.lb();
        assert (k > 0);
        switch (val) {
          case MIN:
            x_i.ub(home, x_i.lb());
            x.max_length(home, x.max_length()-k);
            return;
          case MAX:
            x_i.lb(home, x_i.ub());
            x.min_length(home, x.min_length()+k);
            return;
          default:
            GECODE_NEVER;
        }
      }
      case BASE:
        splitBlock(home, x, i, val, 0);
        return;
      default:
        GECODE_NEVER;
    }
  }
  
  forceinline void
  StringBrancher::splitBlock(Space& home, StringView& x, int i, Val val, 
                                                                unsigned a) {
    Block& x_i = x[i];
    int u = x_i.ub();
    assert (u > 0 && x_i.lb() == u && x_i.baseSize() > 1);
    int c;
    switch (val) {
      case MIN:
        c = x_i.baseMin();
        break;
      case MAX:
        c = x_i.baseMax();
        break;
      case MUST_MIN:
        c = x_i.baseMin();
        if (!_MUST_CHARS.in(c)) { 
          Gecode::Set::BndSetRanges i(_MUST_CHARS);
          Gecode::Set::BndSetRanges j = x_i.ranges();
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
    x.splitBlock(home, i, c); //FIXME: a.
  }
  
  forceinline void
  StringBrancher::commit(Space& home, StringView& x, Lev l, Val v, Blc b, 
                                                                   unsigned a) {
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
