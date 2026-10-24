#ifndef __GECODE_STRING_BRANCH_HH__
#define __GECODE_STRING_BRANCH_HH__

#include <gecode/string.hh>

/**
 * \namespace Gecode::String::Branch
 * \brief String branchers
 */

namespace Gecode { namespace String { namespace Branch {

  /**
   * \brief Common brancher with archive-safe dashed-domain choices
   *
   * Length choices retain the selected endpoint. Symbol choices retain the
   * concrete scalar position, so replay is independent of the current dashed
   * layout and both alternatives remain exhaustive.
   * Choices archive scalar data only and never retain pointers into a Space.
   */
  class StringBrancher : public Brancher {

  protected:

    /// Views owned by this brancher and cloned with it
    ViewArray<StringView> x;
    /// First view that can still be unassigned
    mutable int start;
    /// Rotating scan hint for mandatory-symbol selection
    mutable int preferred_start;

    /// Encoding used to reconstruct a choice from an Archive
    enum class ChoiceKind {
      LENGTH,   ///< Split the aggregate length at a saved endpoint
      POSITION, ///< Restrict one concrete scalar position
      FIXED     ///< Assign the only concrete scalar sequence
    };

    /// Common archived choice header
    class StringChoice : public Choice {
    public:

      ChoiceKind kind;
      unsigned pos;

      StringChoice(
        const StringBrancher& b, ChoiceKind k, unsigned p
      ):
        Choice(b, 2), kind(k), pos(p) {}

      void
      archive_header(Archive& e) const {
        Choice::archive(e);
        e << static_cast<int>(kind) << pos;
      }

    };

    /// Choice that replays a saved length endpoint
    class LengthVal : public StringChoice {
    public:

      Value val;
      int split;

      LengthVal(
        const StringBrancher& b, unsigned p, Value v, int s
      ):
        StringChoice(b, ChoiceKind::LENGTH, p), val(v), split(s) {}

      virtual size_t
      size(void) const {
        return sizeof(*this);
      }

      virtual void
      archive(Archive& e) const {
        archive_header(e);
        e << val << split;
      }

    };

    /// Choice that replays a fixed-length positional symbol split
    class PositionVal : public StringChoice {
    public:

      Value val;
      int split = 0;
      int index;
      int length;
      NSIntSet alphabet;
      NSIntSet chars;

      PositionVal(
        const StringBrancher& b, unsigned p, Value v, int s, int i, int n,
        NSIntSet&& a, NSIntSet&& c
      ):
        StringChoice(b, ChoiceKind::POSITION, p), val(v), split(s),
        index(i), length(n), alphabet(std::move(a)), chars(std::move(c)) {}

      virtual size_t
      size(void) const {
        return sizeof(*this) +
          (alphabet.length() + chars.length()) * sizeof(NSRange);
      }

      virtual void
      archive(Archive& e) const {
        archive_header(e);
        e << val << split << index << length;
        e << alphabet.length();
        for (NSRange* range = alphabet.first(); range; range = range->next)
          e << range->l << range->u;
        e << chars.length();
        for (NSRange* range = chars.first(); range; range = range->next)
          e << range->l << range->u;
      }

    };

    /// Choice that replays assignment to an encoding-neutral value
    class FixedVal : public StringChoice {
    public:

      StringVal value;

      FixedVal(
        const StringBrancher& b, unsigned p, StringVal&& v
      ):
        StringChoice(b, ChoiceKind::FIXED, p), value(std::move(v)) {}

      virtual size_t
      size(void) const {
        return sizeof(*this) + value.size() * sizeof(StringSymbol);
      }

      virtual void
      archive(Archive& e) const {
        archive_header(e);
        e << static_cast<int>(value.size());
        for (StringVal::size_type i = 0; i < value.size(); ++i)
          e << value[i];
      }

    };

    /**
     * Test whether each scalar position belongs to one fixed dashed block.
     *
     * Optional repetition in a multi-block domain can make one concrete
     * position originate in several blocks; that case requires positional
     * replay. A fixed-length single block also has fixed repetition.
     */
    bool
    has_fixed_block_positions(const DashedString* p) const {
      if (p->length() == 1)
        return true;
      for (int i = 0; i < p->length(); ++i)
        if (p->at(i).l != p->at(i).u)
          return false;
      return true;
    }

    /// Select an extreme candidate, preferring symbols mandatory elsewhere
    template <typename CharacterSet>
    int
    preferred_symbol(const CharacterSet& candidates, Value val) const {
      const bool select_min = val == Value::MUSTMIN;
      const int extreme = select_min ? candidates.min() : candidates.max();
      int selected = extreme;
      bool found = false;
      for (int offset = 0; offset < x.size(); ++offset) {
        int i = preferred_start + offset;
        if (i >= x.size())
          i -= x.size();
        const DashedString& domain = x[i].domain();
        for (int j = 0; j < domain.length(); ++j) {
          const DSBlock& block = domain.at(j);
          if (block.l == 0 || block.S.size() != 1)
            continue;
          const int symbol = block.S.min();
          if (!candidates.in(symbol))
            continue;
          if (symbol == extreme) {
            preferred_start = i;
            return symbol;
          }
          if (!found ||
              (select_min ? symbol < selected : symbol > selected)) {
            selected = symbol;
            found = true;
          }
        }
      }
      return selected;
    }

    /// Save the first unassigned block's selected symbol and scalar offset
    PositionVal*
    block_decision(int pos, const DashedString* p, Value val) {
      assert(p->min_length() == p->max_length());
      int index = p->first_na_block();
      const DSBlock& block = p->at(index);
      int offset = 0;
      for (int i = 0; i < index; ++i)
        offset += p->at(i).l;
      int split = 0;
      switch (val) {
        case Value::MIN:
          split = block.S.min();
          break;
        case Value::MAX:
          split = block.S.max();
          break;
        case Value::MUSTMIN:
        case Value::MUSTMAX:
          split = preferred_symbol(block.S, val);
          break;
        default:
          GECODE_NEVER;
      }
      return new PositionVal(
        *this, pos, val, split, offset, p->min_length(),
        p->may_chars(), NSIntSet(block.S));
    }

    /**
     * Build an exhaustive fixed-length positional decision.
     *
     * The archived active alphabet reconstructs unconstrained prefix and
     * suffix positions without depending on the current dashed layout.
     */
    StringChoice*
    position_decision(
      int pos, const DashedString* p, Value val
    ) {
      assert(p->min_length() == p->max_length());
      std::vector<StringSymbol> concrete;
      concrete.reserve(p->min_length());
      for (int i = 0; i < p->min_length(); ++i) {
        NSIntSet chars(p->chars_at(i));
        assert(!chars.empty());
        if (chars.size() == 1) {
          concrete.push_back(chars.min());
          continue;
        }
        int split = 0;
        switch (val) {
          case Value::MIN:
            split = chars.min();
            break;
          case Value::MAX:
            split = chars.max();
            break;
          case Value::MUSTMIN:
          case Value::MUSTMAX:
            split = preferred_symbol(chars, val);
            break;
          default:
            GECODE_NEVER;
        }
        return new PositionVal(
          *this, pos, val, split, i, p->min_length(),
          p->may_chars(), std::move(chars));
      }
      return new FixedVal(
        *this, pos, StringVal::from_symbols(std::move(concrete)));
    }

    /// Replay the saved length predicate, including on assigned clones
    ExecStatus
    commit_length(Space& home, const LengthVal& p, unsigned a) {
      ModEvent me;
      if (p.val == Value::MIN)
        me = a == 0 ? x[p.pos].ub(home, p.split) :
          x[p.pos].lb(home, p.split + 1);
      else
        me = a == 0 ? x[p.pos].lb(home, p.split) :
          x[p.pos].ub(home, p.split - 1);
      return me_failed(me) ? ES_FAILED : ES_OK;
    }

    /// Replay any supported choice representation
    ExecStatus
    commit_choice(Space& home, const Choice& c, unsigned a) {
      const StringChoice& base = static_cast<const StringChoice&>(c);
      switch (base.kind) {
        case ChoiceKind::LENGTH:
          return commit_length(
            home, static_cast<const LengthVal&>(base), a);
        case ChoiceKind::POSITION: {
          const PositionVal& p = static_cast<const PositionVal&>(base);
          NSIntSet selected(p.chars);
          if (a == 0)
            selected = NSIntSet(p.split);
          else
            selected.remove(p.split);
          if (selected.empty())
            return ES_FAILED;
          NSBlocks restriction;
          if (p.index > 0)
            restriction.emplace_back(p.alphabet, p.index, p.index);
          restriction.emplace_back(std::move(selected), 1, 1);
          if (p.index + 1 < p.length)
            restriction.emplace_back(
              p.alphabet, p.length - p.index - 1,
              p.length - p.index - 1);
          ModEvent me = x[p.pos].dom(home, restriction);
          return me_failed(me) ? ES_FAILED : ES_OK;
        }
        case ChoiceKind::FIXED: {
          if (a != 0)
            return ES_FAILED;
          const FixedVal& p = static_cast<const FixedVal&>(base);
          ModEvent me = x[p.pos].eq(home, p.value);
          return me_failed(me) ? ES_FAILED : ES_OK;
        }
        default:
          GECODE_NEVER;
      }
      return ES_FAILED;
    }

    Choice*
    val_llll(int pos, const DashedString* p) {
      if (p->min_length() < p->max_length())
        return new LengthVal(*this, pos, Value::MIN, p->min_length());
      if (!has_fixed_block_positions(p))
        return position_decision(pos, p, Value::MIN);
      return block_decision(pos, p, Value::MIN);
    }

    Choice*
    val_lllm(int pos, const DashedString* p) {
      if (p->min_length() < p->max_length())
        return new LengthVal(*this, pos, Value::MIN, p->min_length());
      if (!has_fixed_block_positions(p))
        return position_decision(pos, p, Value::MUSTMIN);
      return block_decision(pos, p, Value::MUSTMIN);
    }

  public:

    /// Initialize from view array \a x0
    StringBrancher(Home home, ViewArray<String::StringView>& x0):
      Brancher(home), x(x0), start(0), preferred_start(0) {};

    /// Clone \a b and its complete view array into \a home
    StringBrancher(Space& home, StringBrancher& b):
      Brancher(home, b), start(b.start), preferred_start(b.preferred_start) {
      x.update(home, b.x);
    };

    /// Test whether an unassigned view remains
    bool
    status(const Space&) const {
      for (int i = this-> start; i < x.size(); ++i)
        if (!x[i].assigned()) {
          start = i;
          return true;
        }
      return false;
    }

    /// Reconstruct a choice from scalar archive data
    Choice*
    choice(const Space&, Archive& e) {
      int kind, pos;
      e >> kind >> pos;
      switch (ChoiceKind(kind)) {
        case ChoiceKind::LENGTH: {
          int val, split;
          e >> val >> split;
          return new LengthVal(*this, pos, Value(val), split);
        }
        case ChoiceKind::POSITION: {
          int val, split, index, length, range_count;
          e >> val >> split >> index >> length >> range_count;
          NSIntSet alphabet;
          for (int i = 0; i < range_count; ++i) {
            int lower, upper;
            e >> lower >> upper;
            alphabet.include(NSIntSet(lower, upper));
          }
          e >> range_count;
          NSIntSet chars;
          for (int i = 0; i < range_count; ++i) {
            int lower, upper;
            e >> lower >> upper;
            chars.include(NSIntSet(lower, upper));
          }
          return new PositionVal(
            *this, pos, Value(val), split, index, length,
            std::move(alphabet), std::move(chars));
        }
        case ChoiceKind::FIXED: {
          int value_size;
          e >> value_size;
          std::vector<StringSymbol> symbols;
          symbols.reserve(value_size);
          for (int i = 0; i < value_size; ++i) {
            int symbol;
            e >> symbol;
            symbols.push_back(symbol);
          }
          return new FixedVal(
            *this, pos, StringVal::from_symbols(std::move(symbols)));
        }
        default:
          GECODE_NEVER;
      }
      return nullptr;
    }

    virtual void
    print(const Space&, const Choice& c, unsigned a, std::ostream& o) const {
      const StringChoice& base = static_cast<const StringChoice&>(c);
      o << "Alt. " << a << " -- Var. " << base.pos;
      switch (base.kind) {
        case ChoiceKind::LENGTH: {
          const LengthVal& p = static_cast<const LengthVal&>(base);
          o << " -- Length. " << p.split << " -- Val. " << p.val;
          break;
        }
        case ChoiceKind::POSITION: {
          const PositionVal& p = static_cast<const PositionVal&>(base);
          o << " -- Pos. " << p.index << " -- Symbol. " << p.split;
          break;
        }
        case ChoiceKind::FIXED:
          o << " -- Fixed value";
          break;
        default:
          GECODE_NEVER;
      }
      o << " of " << x[base.pos].domain();
    }

  };

}}}

namespace Gecode { namespace String { namespace Branch {

  /// First-unassigned variable selection with LLLL value selection
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

  /// Minimum-length variable selection with LLLL value selection
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

  /// Maximum-length variable selection with LLLL value selection
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

  /// Minimum estimated-domain variable selection with LLUL value selection
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

  /// Minimum-block-count variable selection with LLLL value selection
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

  /// Minimum-block-count variable selection with LLLM value selection
  struct BlockMin_LLLM: public StringBrancher {

    BlockMin_LLLM(Home home, ViewArray<String::StringView>& x0);

    BlockMin_LLLM(Home home, BlockMin_LLLM& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);

  };

}}}
#include <gecode/string/branch/blockmin_lllm.hpp>

namespace Gecode { namespace String { namespace Branch {

  /// Minimum-length/block-count selection with LLLM value selection
  struct LenBlockMin_LLLM: public StringBrancher {

    LenBlockMin_LLLM(Home home, ViewArray<String::StringView>& x0);

    LenBlockMin_LLLM(Home home, LenBlockMin_LLLM& b);

    Actor* copy(Space& home);

    static void post(Home home, ViewArray<String::StringView>& x);

    Choice* choice(Space&);

    ExecStatus commit(Space& home, const Choice& c, unsigned a);

  };

}}}
#include <gecode/string/branch/lenblockmin_lllm.hpp>

#endif

// STATISTICS: string-branch
