#ifndef __GECODE_STRING_BRANCH_HH__
#define __GECODE_STRING_BRANCH_HH__

#include <gecode/string.hh>

/**
 * \namespace Gecode::String::Branch
 * \brief String branchers
 */

namespace Gecode { namespace String { namespace Branch {

  enum Lev {
    LENGTH,
    CARD,
    BASE
  };

  enum Blc {
    LEFTMOST,
    SMALLEST,
    RIGHTMOST
  };

  enum Val {
    MIN,
    MAX,
    MUST_MIN,
  };

}}}

namespace Gecode { namespace String { namespace Branch {

  class StringBrancher : public Brancher {

  protected:

    ViewArray<StringView> x;
    mutable int start;

    class PosLevVal : public Choice {
    public:
      int pos;
      Lev lev;
      Val val;

      PosLevVal(const StringBrancher& b, int p, Lev l, Val v)
      :  Choice(b,2), pos(p), lev(l), val(v) {}

      virtual size_t size(void) const { return sizeof(*this); }

      virtual void
      archive(Archive& e) const {
        Choice::archive(e);
        e << pos << lev << val;
      }

    };
    
  forceinline Choice*
  val_llll(int pos, Gecode::String::StringView& x) {
    if (x.min_length() < x.max_length())
      return new PosLevVal(*this, pos, Lev::LENGTH, Val::MIN);
    Block& b = x[x.leftmost_unfixed_idx()];
    if (b.lb() < b.ub())
      return new PosLevVal(*this, pos, Lev::CARD, Val::MIN);
    else
      return new PosLevVal(*this, pos, Lev::BASE, Val::MIN);
  }
  
  forceinline Choice*
  val_lllm(int pos, Gecode::String::StringView& x) {
    if (x.min_length() < x.max_length())
      return new PosLevVal(*this, pos, Lev::LENGTH, Val::MIN);
    Block& b = x[x.leftmost_unfixed_idx()];
    if (b.lb() < b.ub())
      return new PosLevVal(*this, pos, Lev::CARD, Val::MIN);
    else
      return new PosLevVal(*this, pos, Lev::BASE, Val::MUST_MIN);
  }
  
  forceinline Choice*
  val_lslm(int pos, Gecode::String::StringView& x) {
    if (x.min_length() < x.max_length())
      return new PosLevVal(*this, pos, Lev::LENGTH, Val::MIN);
    Block& b = x[x.smallest_unfixed_idx()];
    if (b.lb() < b.ub())
      return new PosLevVal(*this, pos, Lev::CARD, Val::MIN);
    else
      return new PosLevVal(*this, pos, Lev::BASE, Val::MUST_MIN);
  }
  
  forceinline static void
  commit0(Space& home, StringView& x, Lev lev, Val val, int idx) {
   // FIXME: Implement me.
    Block& block = x[idx];
    switch (lev) {
      case LENGTH: {
        if (x.min_length() == x.max_length())
          home.fail();
        switch (val) {
          case MIN:
//            x.min_length(home, x.min_length() + 1);
            return;
          case MAX:
//            x.max_length(home, x.max_length() - 1);
            return;
          default:
            GECODE_NEVER;
        }
      }
      case Branch::CARD: {
        if (block.lb() == block.ub())
          home.fail();
        switch (val) {
          case Branch::MIN:
             block.lb(home, block.lb()+1);
             x.bnd_length(home, x.min_length()+1, x.max_length());
             return;
          case Branch::MAX:
             block.ub(home, block.ub()-1);
             x.bnd_length(home, x.min_length(), x.max_length()-1);
             return;
          default:
            GECODE_NEVER;
        }
      }
      case Branch::BASE: {
        Gecode::Set::LUBndSet S1;
//        block.includeBaseIn(home,S1);
        if (S1.size() == 1)
          home.fail();
        int l = block.lb();
        Gecode::Set::SetDelta d;
        switch (val) {
          case Branch::MIN: {
            int m = S1.min();
            S1.exclude(home, m, m, d);
            break;
          }
          case Branch::MAX: {
            int m = S1.max();
            S1.exclude(home, m, m, d);
            break;
          }
          case Branch::MUST_MIN: {
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
        bool norm = (idx > 0 /* && x[idx-1].baseEquals(S1)*/) ||
          (l == 1 && idx < x.size() - 1 /*&& x[idx+1].baseEquals(S1)*/);
        if (l == 1 && !norm) {
//          x[idx].baseUpdate(home,S1);
          return;
        }
//        _blocks.insert(h, idx, DSBlock(h, S1, 1, 1));
//        _blocks.at(idx + 1).l--;
//        _blocks.at(idx + 1).u--;
        if (norm)
          x.normalize(home);
        return;
      }
      default:
        GECODE_NEVER;
    }
  }
  
  forceinline static void
  commit1(Space& home, StringView& x, Lev l, Val v, int i) {
  
  }

  forceinline static void
  commit(Space& home, StringView& x, Lev l, Val v, Blc b, unsigned a) {
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
  
  static Gecode::Set::GLBndSet _MUST_CHARS;
  
  public:    

    StringBrancher(Home home, ViewArray<String::StringView>& x0)
    : Brancher(home), x(x0), start(0) {};

    StringBrancher(Space& home, Brancher& b, int s)
    : Brancher(home, b), start(s) {};

    bool
    status(const Space&) const {
      for (int i = this-> start; i < x.size(); ++i)
        if (!x[i].assigned()) {
          start = i;
          return true;
        }
      return false;
    }

    Choice*
    choice(const Space&, Archive& e) {
      int pos, lev, val;
      e >> pos >> lev >> val;
      return new PosLevVal(*this, pos, Lev(lev), Val(val));
    }

    virtual void
    print(const Space&, const Choice& c, unsigned a, std::ostream& o) const {
      const PosLevVal& p = static_cast<const PosLevVal&>(c);
      o << "Alt. " << a << " -- Var. " << p.pos << " -- Lev. " << p.lev
        << " -- Val. " << p.val << " of " << x[p.pos];
    }

  };
  Gecode::Set::GLBndSet StringBrancher::_MUST_CHARS;  

}}}

namespace Gecode { namespace String { namespace Branch {

  struct None_LLLL: public StringBrancher {

    None_LLLL(Home home, ViewArray<String::StringView>& x0);

    None_LLLL(Home home, None_LLLL& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);

  };

}}}
#include <gecode/string/branch/none_llll.hpp>


namespace Gecode { namespace String { namespace Branch {

  struct Block_MinDim_LSLM: public StringBrancher {

    Block_MinDim_LSLM(Home home, ViewArray<String::StringView>& x0);

    Block_MinDim_LSLM(Home home, Block_MinDim_LSLM& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);
    
    static bool _FIRST;

  };
  
  bool Block_MinDim_LSLM::_FIRST = true;

}}}
#include <gecode/string/branch/block_mindim_lslm.hpp>

namespace Gecode { namespace String { namespace Branch {

  struct LBlock_MinDim_LLLM: public StringBrancher {

    LBlock_MinDim_LLLM(Home home, ViewArray<String::StringView>& x0);

    LBlock_MinDim_LLLM(Home home, LBlock_MinDim_LLLM& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);
    
    static bool _FIRST;

  };
  
  bool LBlock_MinDim_LLLM::_FIRST = true;

}}}
#include <gecode/string/branch/lblock_mindim_lllm.hpp>

#endif
