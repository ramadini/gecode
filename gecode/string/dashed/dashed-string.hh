#ifndef __GECODE_STRING_DASHED_HH
#define __GECODE_STRING_DASHED_HH

#include <iostream>
#include <vector>
#include <tuple>
#include <list>
#include <map>
#include <set>
#include <math.h>
#include <assert.h>
#include <cmath>
#include <gecode/int.hh>
#include <gecode/set.hh>
#include <gecode/string/value.hpp>

/*
 * Configure linking
 *
 */
#if !defined(GECODE_STATIC_LIBS) && \
    (defined(__CYGWIN__) || defined(__MINGW32__) || defined(_MSC_VER))

#ifdef GECODE_BUILD_STRING
#define GECODE_STRING_EXPORT __declspec( dllexport )
#else
#define GECODE_STRING_EXPORT __declspec( dllimport )
#endif

#else

#ifdef GECODE_GCC_HAS_CLASS_VISIBILITY
#define GECODE_STRING_EXPORT __attribute__ ((visibility("default")))
#else
#define GECODE_STRING_EXPORT
#endif

#endif

// Configure auto-linking
#ifndef GECODE_BUILD_STRING
#define GECODE_LIBRARY_NAME "String"
#include <gecode/support/auto-link.hpp>
#endif

using std::string;

typedef std::pair<int, int> tpl2;
typedef std::tuple<int, int, int, int, int, int> tpl6;
typedef std::set<tpl6> set6;
typedef std::list<tpl6> lst6;
typedef std::vector<tpl2> vec2;

namespace Gecode { namespace String {

  class NSBlocks;
  /// Sequence of ordinary-lifetime dashed domains
  typedef std::vector<NSBlocks> vecm;

  /// Exception for symbols, lengths, or alphabets outside configured limits
  class GECODE_STRING_EXPORT OutOfLimitsDS : public Exception {
  public:
    /// Initialize with location \a l
    OutOfLimitsDS(const char* l);
  };

  /// Exception for an operation requiring a non-empty dashed domain
  class GECODE_STRING_EXPORT EmptyDS : public Exception {
  public:
    /// Initialize with location \a l
    EmptyDS(const char* l);
  };

  /// Exception for extracting a value from an unassigned dashed domain
  class GECODE_STRING_EXPORT UnknownValDS : public Exception {
  public:
    /// Initialize with location \a l
    UnknownValDS(const char* l);
  };

}}

namespace Gecode { namespace String {

  class NSIntSet;

  /**
   * \brief Space-owned finite set of string symbols
   *
   * The backing ranges are allocated by a Gecode Space. Copies therefore
   * require the destination Space and must not be retained outside its
   * lifetime. Use NSIntSet for ordinary-lifetime temporary sets.
   */
  class GECODE_STRING_EXPORT DSIntSet : public Gecode::Set::LUBndSet {

  private:

    DSIntSet& operator=(const DSIntSet& that);
    DSIntSet(const DSIntSet& that);

  public:

    DSIntSet();
    DSIntSet(Space& h, int a, int b);
    DSIntSet(Space& h, const NSIntSet& s);
    DSIntSet(Space& h, const DSIntSet& s);

    IntSet toIntSet() const;

    void init(Space& h, int c);
    void init(Space& h, int l, int u);
    void init(Space& h, const DSIntSet& s);
    void init(Space& h, const NSIntSet& s);

    void update(Space& h, const NSIntSet& s);
    void update(Space& h, const DSIntSet& s);
    void update(Space& h, char c);
    void update(Space& h, int c);

    bool contains(const DSIntSet& that) const;
    bool contains(const NSIntSet& that) const;

    bool disjoint(const DSIntSet& that) const;
    bool disjoint(const NSIntSet& that) const;

    bool operator==(const DSIntSet& that) const;
    bool operator==(const NSIntSet& that) const;

    bool operator!=(const DSIntSet& that) const;

    void intersect(Space& h, const NSIntSet& that);
    void intersect(Space& h, const DSIntSet& that);

    void remove(Space& h, int x);

    friend std::ostream& operator<<(std::ostream& os, const DSIntSet& that);

  };

}}


namespace Gecode { namespace String {

  class NSBlock;

  /**
   * \brief Space-owned dashed block
   *
   * A block represents between \a l and \a u consecutive symbols, each drawn
   * from \a S. Range storage in \a S belongs to the containing Space.
   */
  struct GECODE_STRING_EXPORT DSBlock {

    int l;
    int u;
    DSIntSet S;

    DSBlock();

    DSBlock(Space& h, const DSBlock& b);

    DSBlock(Space& h, const NSBlock& b);

    DSBlock(Space& h, const DSIntSet& s, int a, int b);

    DSBlock(Space& h, int min, int max, int a, int b);

    void init(Space& h, const DSBlock& d);

    void init(Space& h, const NSBlock& v);

    void set_null(Space& h);

    bool isTop() const;

    bool known() const;

    string val() const;

    static DSBlock top(Space& h);

    int min() const;

    int max() const;

    bool null() const;

    bool contains(const DSBlock& b) const;

    double logdim() const;

    void update(Space& h, const DSBlock& that);
    void update(Space& h, const NSBlock& that);

    template <typename Block> bool operator==(const Block& that) const;

    friend std::ostream& operator<<(std::ostream& os, const DSBlock& that);

  private:

    friend class Gecode::Space;
    friend class Gecode::Heap;

    DSBlock& operator=(const DSBlock& that);
    DSBlock(const DSBlock& that);

  };

  /// Read/write block-sequence interface used by generic sweep algorithms
  struct DashedView {
    virtual const DSBlock& at(int) const = 0;
    virtual DSBlock& at(int i) = 0;
    virtual int length() const = 0;
    int size() const { return length(); }
    const DSBlock& front() const { return at(0); }
    const DSBlock& back() const  { return at(length()-1); }
  };

}}


namespace Gecode { namespace String {

  /**
   * \brief Space-owned sequence of dashed blocks
   *
   * This container explicitly initializes and disposes each block's
   * space-allocated symbol ranges when resizing or replacing storage.
   * Ordinary temporary block sequences must use NSBlocks instead.
   */
  class GECODE_STRING_EXPORT DSBlocks :
  public Gecode::Support::DynamicArray<DSBlock, Space> {

  protected:

    int _size;

  private:

    DSBlocks(const DSBlocks& d);
    DSBlock& operator [](int i) const;
    DSBlocks& operator=(const DSBlocks& d);

    /// Dispose character sets in the half-open block range
    void dispose(Space& h, int begin, int end);
    /// Shrink initialized and allocated storage to \a size blocks
    void shrink(Space& h, int size);
    /// Dispose current storage and allocate \a size uninitialized blocks
    void reset(Space& h, int size);
    /// Replace current storage with one initialized null block
    void clear(Space& h);
    /// Dispose current storage and take ownership of \a blocks
    void replace(Space& h, DSBlock* blocks, int size);

  public:

    DSBlocks(Space& h);

    DSBlocks(Space& h, int k);

    DSBlocks(Space& h, int k, const DSBlock& b);

    DSBlocks(Space& h, const DSBlocks& d);

    DSBlocks(Space& h, const NSBlocks& v);

    int length() const;

    NSIntSet may_chars() const;

    void size(int i);

    double logdim() const;

    bool null() const;

    void set_null(Space& h);

    DSBlock& at(int i);
    const DSBlock& at(int i) const;

    DSBlock& front();
    const DSBlock& front() const;

    DSBlock& back();
    const DSBlock& back() const;

    void init(Space& h, const DSBlocks& d);
    void init(Space& h, const NSBlocks& v);

    void update(Space& h, const DSBlocks& d);
    /// Replace from ordinary storage and return aggregate length bounds
    void update(Space& h, const NSBlocks& that, int& l, int& u);
    void update(Space& h, const string& s);
    void update(Space& h, const StringVal& value);

    template <typename Block>
    void insert(Space& h, int k, const Block& d);

    /// Remove null blocks, merge equal neighbours, and return length bounds
    void normalize(Space& h, int&, int&);

    /// Replace current storage by taking ownership of space-allocated \a y
    void become(Space& h, DSBlock* y, int m);

    template <typename Blocks> bool operator==(const Blocks& that) const;

    friend std::ostream& operator<<(std::ostream& os, const DSBlocks& that);

  };

}}

namespace Gecode { namespace String { namespace Branch {

  /// Component of a dashed-domain branching decision
  enum Level {
    LENGTH, ///< Aggregate string length
    CARD,   ///< Repetition count of a selected block
    BASE    ///< Symbol set of a selected block
  };

  /// Split direction for a branching decision
  enum Value {
    MIN,     ///< Select the lower endpoint
    MAX,     ///< Select the upper endpoint
    MUSTMIN, ///< Prefer the smallest mandatory symbol
    MUSTMAX  ///< Prefer the largest mandatory symbol
  };

  /// Strategy for selecting a non-assigned block
  enum Block {
    FIRST,   ///< First non-assigned block
    SMALLEST ///< Non-assigned block with the smallest estimated domain
  };

}}}

namespace Gecode { namespace String {

  class Position;
  class DashedView;

  /**
   * \brief Space-owned dashed string domain
   *
   * A domain is an ordered sequence of DSBlock values plus aggregate minimum
   * and maximum lengths. Normalized domains have no null block (except the
   * unique block representing the empty string), no adjacent blocks with the
   * same symbol set, and length bounds consistent with their blocks.
   *
   * Mutation methods return \c false when refinement proves inconsistency.
   * Successful changes set an internal marker consumed by StringVarImp when
   * it emits the single aggregate modification event for a transaction.
   * DashedString itself never schedules propagators.
   */
  class GECODE_STRING_EXPORT DashedString {

  friend class StringVarImp;

  public:

  typedef std::map<tpl6, Gecode::String::NSBlock> map6;

  static bool _DECOMP_REGEX;
  static bool _REVERSE_REGEX;
  static bool _QUAD_SWEEP;
  static int  _MAX_STR_LENGTH;
  static int  _MAX_STR_ALPHA;

  /// Test whether two block sequences share at least one concrete string
  static bool deep_check_dp(
    const NSBlocks& x, int i, const NSBlock& x_i,
    const NSBlocks& y, int j, const NSBlock& y_j, set6 nogoods
  );
  template <typename Blocks1, typename Blocks2>
  static bool deep_check(const Blocks1& x, const Blocks2& y);

  friend std::ostream& operator<<(std::ostream& os, const DashedString& ds);

  friend DashedString operator+(
    const DashedString& ds1, const DashedString& ds2
  );

  /// Initialize the top domain in \a h
  DashedString(Space& h);

  /// Initialize from a legacy one-byte-per-symbol value
  DashedString(Space& h, const std::string& s);

  /// Initialize from an encoding-neutral scalar-value sequence
  DashedString(Space& h, const StringVal& value);

  /// Initialize from one space-owned block
  DashedString(Space& h, const DSBlock& b);

  /// Initialize from space-owned blocks
  DashedString(Space& h, const DSBlocks& bv);

  /// Initialize from temporary blocks and explicit aggregate length bounds
  DashedString(Space& h, NSBlocks& v, int mil, int mal);

  /// Clone \a d into destination Space \a h
  DashedString(Space& h, const DashedString& d);

  DSBlocks& blocks();
  const DSBlocks& blocks() const;

  DSBlock& at(int i);
  const DSBlock& at(int i) const;

  NSBlocks prefix(int idx, int off) const;
  NSBlocks suffix(int idx, int off) const;

  int min_length() const;
  void min_length(int i);

  int max_length() const;
  void max_length(int i);

  NSIntSet must_chars() const;

  NSIntSet may_chars() const;

  /// Return the symbols supported at zero-based \a position for fixed length
  NSIntSet chars_at(int position) const;

  int length() const;

  double logdim() const;

  bool known() const;

  /// Return the known prefix as legacy bytes, rejecting non-byte symbols.
  string known_pref() const;
  std::vector<StringSymbol> known_pref_symbols() const;

  /// Return the known suffix as legacy bytes, rejecting non-byte symbols.
  string known_suff() const;
  std::vector<StringSymbol> known_suff_symbols() const;

  bool null() const;

  bool isTop() const;

  bool is_normalized() const;

  /// Replace the domain with the singleton empty string
  void set_null(Space& h);

  /// Return assigned legacy bytes; throw if unassigned or not byte-valued
  string val() const;

  /// Extract the assigned value as bytes, returning false for symbols > 255.
  bool try_val_bytes(string& value) const;

  /// Return the assigned encoding-neutral scalar-value sequence
  StringVal val_symbols() const;

  /// Normalize \a v and replace this domain with the normalized result
  void norm_update(Space& h, NSBlocks& v);

  /// Reconcile aggregate length bounds with the current block bounds
  void refresh_cardinality(Space& h);

  /// Restore block and aggregate-length normalization invariants
  void normalize(Space& h);

  /// Test whether equality with \a that can have at least one solution
  bool check_equate(const DashedString& that) const;
  /// Test whether equality with \a that can have at least one solution
  bool check_equate(const NSBlocks& that) const;
  /// Test whether equality with \a that can have at least one solution
  bool check_equate(const StringVal& that) const;

  /**
   * \brief Refine both domains to enforce equality
   *
   * Return \c false on inconsistency. Each modified domain records its own
   * change marker; notification remains the caller's responsibility.
   */
  bool equate(Space& h, DashedString& that);
  /**
   * \brief Refine this domain to equality with temporary domain \a that
   *
   * Keep the intersected aggregate bounds when adopting the candidate's
   * blocks. For the byte-top domain, an alphabet-compatible candidate keeps
   * its structure even if its raw block maxima exceed the total length bound.
   */
  bool equate(Space& h, const NSBlocks& that);
  /// Refine this domain to assigned scalar sequence \a that
  bool equate(Space& h, const StringVal& that);

  /// Replace the domain with legacy byte value \a s
  void update(Space& h, string s);
  /// Replace the domain with encoding-neutral value \a value
  void update(Space& h, const StringVal& value);
  /// Replace the domain with a space-owned copy of \a that
  void update(Space& h, const DashedString& that);
  /// Replace the domain with ordinary-lifetime block sequence \a that
  void update(Space& h, const NSBlocks& that);

  /// Test exact representation equality with \a that
  bool equals(const DashedString& that) const;
  /// Test exact representation equality with \a that
  bool equals(const NSBlocks& that) const;

  /// Test whether every value represented by \a that is represented here
  bool contains(const NSBlocks&) const;
  /// Test whether every value represented by \a that is represented here
  bool contains(const DashedString&) const;
  /// Test whether this domain contains all concatenations of two domains
  bool contains(const DashedString&, const DashedString&) const;

  /**
   * \brief Refine a find relation against pattern domain \a x
   *
   * \a lb and \a ub are one-based candidate start bounds. If \a mod is true,
   * length and position bounds are refined in addition to feasibility.
   */
  bool find(Space&, DashedString&, int&, int&, bool);

  int first_na_block() const;

  int smallest_na_block() const;

  /// Test whether lexicographic relation with \a that is feasible
  bool check_lex(const DashedString& that, bool lt) const;

  NSBlocks lex_least(const std::vector<float>& succ) const;

  NSBlocks lex_greatest(const std::vector<float>& succ) const;

  /// Refine the domain to strictly/non-strictly increasing concrete strings
  bool increasing(Space&h, bool strict);

  /// Return a shortest concrete legacy byte string represented by this domain.
  string min_str() const;
  /// Return a shortest concrete encoding-neutral value represented by this domain.
  StringVal min_value() const;

  /**
   * \brief Apply branch alternative \a a at the requested domain level
   *
   * The operation selects a block using \a b, restores normalization, and
   * records a domain change. Alternative zero selects the split value;
   * alternative one excludes it or selects the complementary bound.
   */
  void commit(
    Space&, Branch::Level, Branch::Value, Branch::Block, int, unsigned);

  /// Raise the aggregate minimum length; return false on inconsistency
  bool refine_lb(int l);

  /// Lower the aggregate maximum length; return false on inconsistency
  bool refine_ub(Space&, int u);

  /// Intersect aggregate and per-block length bounds with [\a mil,\a mal]
  bool refine_card(Space&, int mil, int mal);

  /// Reconcile aggregate lengths for equality with \a that
  bool refine_card_eq(Space& h, DashedString& that);

  /// Reconcile aggregate lengths for this = \a x concatenated with \a y
  bool refine_card_cat(Space& h, DashedString& x, DashedString& y);

  /// Reconcile aggregate lengths for this = \a x repeated in [\a n1,\a n2]
  bool refine_card_pow(Space& h, DashedString& x, int n1, int n2);

  /// Refine all three domains for this = \a x concatenated with \a y
  bool concat(Space& h, DashedString& x, DashedString& y);

  /// Return the temporary domain for exactly \a n repetitions
  NSBlocks pow(int n) const;
  /// Refine this = \a x repeated between \a lx and \a ux times
  bool pow(Space& h, DashedString& x, int& lx, int& ux);

  /// Refine both domains for this = reverse(\a x)
  bool rev(Space& h, DashedString& x);

  /// Refine both domains for this lexicographically before \a y
  bool lex(Space& h, DashedString& y, bool lt);

  /// Test whether the current mutation transaction changed this domain
  bool changed() const;
  /// Set the mutation marker consumed by StringVarImp
  void changed(bool b);

  bool operator==(const DashedString& that) const;

  bool operator!=(const DashedString& that) const;

  private:

    /// Whether the current mutation transaction changed this domain
    bool _changed;
    /// Space-owned normalized block sequence
    DSBlocks _blocks;
    /// Length of the shortest represented concrete string
    int _min_length;
    /// Length of the longest represented concrete string
    int _max_length;

    void _commit0(Space&, Branch::Level, Branch::Value, int, int);

    void _commit1(Space&, Branch::Level, Branch::Value, int, int);

    DashedString(const DashedString& that);
    DashedString& operator=(const DashedString& that);

  };

}}

#include <gecode/string/dashed/dashed-string.hpp>

#endif

// STATISTICS: string-var
