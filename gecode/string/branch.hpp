namespace Gecode { namespace String { namespace Branch {

  forceinline Choice*
  StringBrancher::val_llll(int pos, Gecode::String::StringView& x) const {
    if (x.min_length() < x.max_length())
      return new PosLevVal(*this, pos, Lev::LENGTH, Val::MIN);
    const Block& b = x[x.leftmost_unfixed_idx()];
    if (b.lb() < b.ub())
      return new PosLevVal(*this, pos, Lev::CARD, Val::MIN);
    else
      return new PosLevVal(*this, pos, Lev::BASE, Val::MIN);
  }
  
  forceinline Choice*
  StringBrancher::val_lllm(int pos, Gecode::String::StringView& x) const {
    if (x.min_length() < x.max_length())
      return new PosLevVal(*this, pos, Lev::LENGTH, Val::MIN);
    const Block& b = x[x.leftmost_unfixed_idx()];
    if (b.lb() < b.ub())
      return new PosLevVal(*this, pos, Lev::CARD, Val::MIN);
    else
      return new PosLevVal(*this, pos, Lev::BASE, Val::MUST_MIN);
  }
  
  forceinline Choice*
  StringBrancher::val_lslm(int pos, Gecode::String::StringView& x) const {
    if (x.min_length() < x.max_length())
      return new PosLevVal(*this, pos, Lev::LENGTH, Val::MIN);
    const Block& b = x[x.smallest_unfixed_idx()];
    if (b.lb() < b.ub())
      return new PosLevVal(*this, pos, Lev::CARD, Val::MIN);
    else
      return new PosLevVal(*this, pos, Lev::BASE, Val::MUST_MIN);
  }
  
  forceinline ModEvent
  StringBrancher::commit0(Space& home, StringView& x, Lev lev, Val val, int i) {
    const Block& x_i = x[i];
    switch (lev) {
      case LENGTH: {
        assert (x.min_length() < x.max_length());
        switch (val) {
          case MIN:
            x.max_length(home, x.min_length());
            return x.assigned() ? ME_STRING_VAL : ME_STRING_CARD;
          case MAX:
            x.min_length(home, x.max_length());
            return x.assigned() ? ME_STRING_VAL : ME_STRING_CARD;
          default:
            GECODE_NEVER;
        }
      }
      case CARD: {
        assert (x_i.ub() - x_i.lb() > 0 && x.min_length() == x.max_length());
        switch (val) {
          case MIN:
            x.ubAt(home, i, x_i.lb());
            return x.assigned() ? ME_STRING_VAL : ME_STRING_CARD;
          case MAX:
            x.lbAt(home, i, x_i.ub());
            return x.assigned() ? ME_STRING_VAL : ME_STRING_CARD;
          default:
            GECODE_NEVER;
        }
      }
      case BASE:
        return splitBlock(home, x, i, val, 0);
      default:
        GECODE_NEVER;
    }
  }
  
  forceinline ModEvent
  StringBrancher::commit1(Space& home, StringView& x, Lev lev, Val val, int i) {
    const Block& x_i = x[i];
    switch (lev) {
      case LENGTH: {
        if (x.min_length() == x.max_length())
          return ME_STRING_FAILED;
        switch (val) {
          case MIN:
            x.min_length(home, x.min_length() + 1);
            return x.assigned() ? ME_STRING_VAL : ME_STRING_CARD;
          case MAX:
            x.max_length(home, x.max_length() - 1);
            return x.assigned() ? ME_STRING_VAL : ME_STRING_CARD;
          default:
            GECODE_NEVER;
        }
      }
      case CARD: {
        if (x_i.lb() == x_i.ub())
          return ME_STRING_FAILED;
        switch (val) {
          case MIN:
             x.lbAt(home, i, x_i.lb() + 1);
             return x.assigned() ? ME_STRING_VAL : ME_STRING_CARD;
          case MAX:
             x.ubAt(home, i, x_i.ub() - 1);
             return x.assigned() ? ME_STRING_VAL : ME_STRING_CARD;
          default:
            GECODE_NEVER;
        }
      }
      case BASE:
        return x_i.baseSize() == 1 ? ME_STRING_FAILED
                                   : splitBlock(home, x, i, val, 1);
      default:
        GECODE_NEVER;
    }
  }
  
  forceinline ModEvent
  StringBrancher::splitBlock(Space& home, StringView& x, int i, Val val, 
                                                                unsigned a) {
    const Block& x_i = x[i];
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
        if (_MUST_CHARS.size() > 0) {
          bool in = false;
          int n = _MUST_CHARS.size();
          for (int i = 0; i < n;  i += 2)
            if (_MUST_CHARS[i] > c)
              break;
            else if (c <= _MUST_CHARS[i+1]) {
              in = true;
              break;
            }
          if (in)
            break;
          int i = 0;
          Gecode::Set::BndSetRanges j = x_i.ranges();
          while (i < n && j()) {
            int li = _MUST_CHARS[i], ui = _MUST_CHARS[i+1];
            int lj = j.min(), uj = j.max();
            if (li > uj)
              ++j;
            else if (lj > ui)
              i += 2;
            else {
              c = li > lj ? li : lj;
              break;
            }
          }
        }
        break;
      default:
        GECODE_NEVER;
    }
//    std::cerr << int2str(c) << " chosen\n";
    return x.splitBlock(home, i, c, a);
  }
  
  forceinline ModEvent
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
    return a == 0 ? commit0(home, x, l, v, i) : commit1(home, x, l, v, i);
  }

  
}}}
