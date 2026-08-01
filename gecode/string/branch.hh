#ifndef __GECODE_STRING_BRANCH_HH__
#define __GECODE_STRING_BRANCH_HH__

#include <gecode/string.hh>

/**
 * \namespace Gecode::String::Branch
 * \brief String branchers
 */

namespace Gecode { namespace String { namespace Branch {

  class StringBrancher : public Brancher {

  protected:

    ViewArray<StringView> x;
    mutable int start;

    class PosLevVal : public Choice {
    public:

      unsigned pos;
      Level lev;
      Value val;
      int split;
      int offset;

      PosLevVal(
        const StringBrancher& b, unsigned p, Level l, Value v, int s, int o
      ):
        Choice(b, 2), pos(p), lev(l), val(v), split(s), offset(o) {}

      virtual size_t
      size(void) const {
        return sizeof(*this);
      }

      virtual void
      archive(Archive& e) const {
        Choice::archive(e);
        e << pos << lev << val << split << offset;
      }

    };

    PosLevVal*
    decision(
      int pos, const Gecode::String::DashedString* p, Level lev, Value val
    ) {
      if (lev == Level::LENGTH) {
        int split = val == Value::MIN ? p->min_length() : p->max_length();
        return new PosLevVal(*this, pos, lev, val, split, -1);
      }
      int index = p->first_na_block();
      const DSBlock& block = p->at(index);
      if (lev == Level::CARD) {
        int split = val == Value::MIN ? block.l : block.u;
        return new PosLevVal(*this, pos, lev, val, split, -1);
      }
      int offset = 0;
      for (int i = 0; i < index; ++i)
        offset += p->at(i).l;
      int split;
      switch (val) {
        case Value::MIN:
          split = block.S.min();
          break;
        case Value::MAX:
          split = block.S.max();
          break;
        case Value::MUSTMIN:
        case Value::MUSTMAX: {
          NSIntSet must(DashedString::_MUST_CHARS);
          must.intersect(block.S);
          if (must.empty())
            split = val == Value::MUSTMIN ? block.S.min() : block.S.max();
          else
            split = val == Value::MUSTMIN ? must.min() : must.max();
          break;
        }
        default:
          GECODE_NEVER;
      }
      return new PosLevVal(*this, pos, lev, val, split, offset);
    }

    ExecStatus
    commit_choice(Space& home, const PosLevVal& p, Block block, unsigned a) {
      if (!x[p.pos].assigned()) {
        x[p.pos].commit(home, p.lev, p.val, block, a);
        return ES_OK;
      }
      if (p.lev == Level::CARD)
        return ES_OK;
      const string value = x[p.pos].val();
      bool selected;
      if (p.lev == Level::LENGTH)
        selected = static_cast<int>(value.size()) == p.split;
      else
        selected = p.offset < static_cast<int>(value.size()) &&
          static_cast<int>(char2int(value[p.offset])) == p.split;
      return selected == (a == 0) ? ES_OK : ES_FAILED;
    }

    Choice*
    val_llll(int pos, Gecode::String::DashedString* p) {
      if (p->min_length() < p->max_length())
        return decision(pos, p, Level::LENGTH, Value::MIN);
      int i = p->first_na_block();
      const DSBlock& b = p->at(i);
      if (b.l < b.u)
        return decision(pos, p, Level::CARD, Value::MIN);
      else
        return decision(pos, p, Level::BASE, Value::MIN);
    }
        
    Choice*
    val_llul(int pos, const Gecode::String::DashedString* p) {
      if (p->min_length() < p->max_length())
        return decision(pos, p, Level::LENGTH, Value::MIN);
      int i = p->first_na_block();
      const DSBlock& b = p->at(i);
      if (b.l < b.u)
        return decision(pos, p, Level::CARD, Value::MAX);
      else
        return decision(pos, p, Level::BASE, Value::MIN);
    }

    Choice*
    val_ulul(int pos, Gecode::String::DashedString* p) {
      if (p->min_length() < p->max_length())
        return decision(pos, p, Level::LENGTH, Value::MAX);
      int i = p->first_na_block();
      const DSBlock& b = p->at(i);
      if (b.l < b.u)
        return decision(pos, p, Level::CARD, Value::MAX);
      else
        return decision(pos, p, Level::BASE, Value::MIN);
    }
        
    Choice*
    val_lllm(int pos, Gecode::String::DashedString* p) {
      if (p->min_length() < p->max_length())
        return decision(pos, p, Level::LENGTH, Value::MIN);
      int i = p->first_na_block();
      const DSBlock& b = p->at(i);
      if (b.l < b.u)
        return decision(pos, p, Level::CARD, Value::MIN);
      else
        return decision(pos, p, Level::BASE, Value::MUSTMIN);
    }

    forceinline void
    complete(Space& home, int pos) {
      ModEvent me = x[pos].eq(home, x[pos].pdomain()->min_str());
      assert(!me_failed(me));
      (void) me;
    }

  public:

    StringBrancher(Home home, ViewArray<String::StringView>& x0):
      Brancher(home), x(x0), start(0) {};

    StringBrancher(Space& home, Brancher& b, int s):
      Brancher(home, b), start(s) {};

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
      int pos, lev, val, split, offset;
      e >> pos >> lev >> val >> split >> offset;
      return new PosLevVal(
        *this, pos, Level(lev), Value(val), split, offset);
    }

    virtual void
    print(const Space&, const Choice& c, unsigned a, std::ostream& o) const {
      const PosLevVal& p = static_cast<const PosLevVal&>(c);
      o << "Alt. " << a << " -- Var. " << p.pos << " -- Lev. " << p.lev
        << " -- Val. " << p.val << " of " << *x[p.pos].pdomain();
    }

  };

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

  struct LenMin_LLLL: public StringBrancher {

    LenMin_LLLL(Home home, ViewArray<String::StringView>& x0);

    LenMin_LLLL(Home home, LenMin_LLLL& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);

  };

}}}
#include <gecode/string/branch/lenmin_llll.hpp>

namespace Gecode { namespace String { namespace Branch {

  struct LenMax_LLLL: public StringBrancher {

    LenMax_LLLL(Home home, ViewArray<String::StringView>& x0);

    LenMax_LLLL(Home home, LenMax_LLLL& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);

  };

}}}
#include <gecode/string/branch/lenmax_llll.hpp>

namespace Gecode { namespace String { namespace Branch {

  struct SizeMin_LLUL: public StringBrancher {

    SizeMin_LLUL(Home home, ViewArray<String::StringView>& x0);

    SizeMin_LLUL(Home home, SizeMin_LLUL& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);

  };

}}}
#include <gecode/string/branch/sizemin_llul.hpp>

namespace Gecode { namespace String { namespace Branch {

  struct BlockMin_LLLL: public StringBrancher {

    BlockMin_LLLL(Home home, ViewArray<String::StringView>& x0);

    BlockMin_LLLL(Home home, BlockMin_LLLL& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);

  };

}}}
#include <gecode/string/branch/blockmin_llll.hpp>

namespace Gecode { namespace String { namespace Branch {

  struct BlockMin_LLLM: public StringBrancher {

    BlockMin_LLLM(Home home, ViewArray<String::StringView>& x0);

    BlockMin_LLLM(Home home, BlockMin_LLLM& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);
    
    static bool _FIRST;

  };
  
  bool BlockMin_LLLM::_FIRST = true;

}}}
#include <gecode/string/branch/blockmin_lllm.hpp>

namespace Gecode { namespace String { namespace Branch {

  struct LenBlockMin_LLLM: public StringBrancher {

    LenBlockMin_LLLM(Home home, ViewArray<String::StringView>& x0);

    LenBlockMin_LLLM(Home home, LenBlockMin_LLLM& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);
    
    static bool _FIRST;

  };
  
  bool LenBlockMin_LLLM::_FIRST = true;

}}}
#include <gecode/string/branch/lenblockmin_lllm.hpp>

#endif
