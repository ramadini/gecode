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
    
    Choice* val_llll(int pos, Gecode::String::StringView& x) const;
    Choice* val_lllm(int pos, Gecode::String::StringView& x) const;
    Choice* val_lslm(int pos, Gecode::String::StringView& x) const;
    
    static void commit0(Space&, StringView&, Lev, Val, int);
    static void commit1(Space&, StringView&, Lev, Val, int);
    static void commit (Space&, StringView&, Lev, Val, Blc, unsigned);
    static void splitBlock(Space&, StringView&, int, Val, unsigned);
    
    static std::vector<int> _MUST_CHARS;
  
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
  std::vector<int> StringBrancher::_MUST_CHARS;

}}}
#include <gecode/string/branch.hpp>

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
