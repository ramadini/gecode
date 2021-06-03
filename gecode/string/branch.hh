#ifndef __GECODE_STRING_BRANCH_HH__
#define __GECODE_STRING_BRANCH_HH__

#include <gecode/string.hh>

/**
 * \namespace Gecode::String::Branch
 * \brief String branchers
 */

namespace Gecode { namespace String { namespace Branch {

  enum Level {
    LENGTH,
    CARD,
    BASE
  };

  enum Block {
    LEFTMOST,
    SMALLEST,
    RIGHTMOST
  };

  enum Value {
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
      Level lev;
      Value val;

      PosLevVal(const StringBrancher& b, int p, Level l, Value v)
      :  Choice(b,2), pos(p), lev(l), val(v) {}

      virtual size_t size(void) const { return sizeof(*this); }

      virtual void
      archive(Archive& e) const {
        Choice::archive(e);
        e << pos << lev << val;
      }

    };

//    Choice*
//    val_llll(int pos, Gecode::String::DashedString* p) {
//      if (p->min_length() < p->max_length())
//        return new PosLevVal(*this, pos, Level::LENGTH, Value::MIN);
//      int i = p->leftmost_unfixed();
//      const DSBlock& b = p->at(i);
//      if (b.l < b.u)
//        return new PosLevVal(*this, pos, Level::CARD, Value::MIN);
//      else
//        return new PosLevVal(*this, pos, Level::BASE, Value::MIN);
//    }
    
//    Choice*
//    val_lllm(int pos, Gecode::String::DashedString* p) {
//      if (p->min_length() < p->max_length())
//        return new PosLevVal(*this, pos, Level::LENGTH, Value::MIN);
//      int i = p->leftmost_unfixed();
//      const DSBlock& b = p->at(i);
//      if (b.l < b.u)
//        return new PosLevVal(*this, pos, Level::CARD, Value::MIN);
//      else
//        return new PosLevVal(*this, pos, Level::BASE, Value::MUSTMIN);
//    }
    
//    Choice*
//    val_lslm(int pos, Gecode::String::DashedString* p) {
//      if (p->min_length() < p->max_length())
//        return new PosLevVal(*this, pos, Level::LENGTH, Value::MIN);
//      int i = p->smalles_unfixed();
//      const DSBlock& b = p->at(i);
//      if (b.l < b.u)
//        return new PosLevVal(*this, pos, Level::CARD, Value::MIN);
//      else
//        return new PosLevVal(*this, pos, Level::BASE, Value::MUSTMIN);
//    }
    

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
      return new PosLevVal(*this, pos, Level(lev), Value(val));
    }

    virtual void
    print(const Space&, const Choice& c, unsigned a, std::ostream& o) const {
      const PosLevVal& p = static_cast<const PosLevVal&>(c);
      o << "Alt. " << a << " -- Var. " << p.pos << " -- Lev. " << p.lev
        << " -- Val. " << p.val << " of " << x[p.pos];
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

  struct BDim_Min_LSLM: public StringBrancher {

    BDim_Min_LSLM(Home home, ViewArray<String::StringView>& x0);

    BDim_Min_LSLM(Home home, BDim_Min_LSLM& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);
    
    static bool _FIRST;

  };
  
  bool BDim_Min_LSLM::_FIRST = true;

}}}
#include <gecode/string/branch/bdim_min_lslm.hpp>

namespace Gecode { namespace String { namespace Branch {

  struct BDim_Min_LLLM: public StringBrancher {

    BDim_Min_LLLM(Home home, ViewArray<String::StringView>& x0);

    BDim_Min_LLLM(Home home, BDim_Min_LLLM& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);
    
    static bool _FIRST;

  };
  
  bool BDim_Min_LLLM::_FIRST = true;

}}}
#include <gecode/string/branch/bdim_min_lllm.hpp>

#endif
