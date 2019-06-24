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

      PosLevVal(const StringBrancher& b, unsigned p, Level l, Value v):
        Choice(b, 2), pos(p), lev(l), val(v) {}

      virtual size_t
      size(void) const {
        return sizeof(*this);
      }

      virtual void
      archive(Archive& e) const {
        Choice::archive(e);
        e << pos << lev << val;
      }

    };

    Choice*
    val_llll(int pos, Gecode::String::DashedString* p) {
      if (p->min_length() < p->max_length())
        return new PosLevVal(*this, pos, Level::LENGTH, Value::MIN);
      int i = p->first_na_block();
      const DSBlock& b = p->at(i);
      if (b.l < b.u)
        return new PosLevVal(*this, pos, Level::CARD, Value::MIN);
      else
        return new PosLevVal(*this, pos, Level::BASE, Value::MIN);
    }

    Choice*
    val_llul(int pos, const Gecode::String::DashedString* p) {
      if (p->min_length() < p->max_length())
        return new PosLevVal(*this, pos, Level::LENGTH, Value::MIN);
      int i = p->first_na_block();
      const DSBlock& b = p->at(i);
      if (b.l < b.u)
        return new PosLevVal(*this, pos, Level::CARD, Value::MAX);
      else
        return new PosLevVal(*this, pos, Level::BASE, Value::MIN);
    }

    Choice*
    val_ulul(int pos, Gecode::String::DashedString* p) {
      if (p->min_length() < p->max_length())
        return new PosLevVal(*this, pos, Level::LENGTH, Value::MAX);
      int i = p->first_na_block();
      const DSBlock& b = p->at(i);
      if (b.l < b.u)
        return new PosLevVal(*this, pos, Level::CARD, Value::MAX);
      else
        return new PosLevVal(*this, pos, Level::BASE, Value::MIN);
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
      int pos, lev, val;
      e >> pos >> lev >> val;
      return new PosLevVal(*this, pos, Level(lev), Value(val));
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

  private:

    double dim(int s, int l) const;

  };

}}}
#include <gecode/string/branch/blockmin_llll.hpp>

#endif
