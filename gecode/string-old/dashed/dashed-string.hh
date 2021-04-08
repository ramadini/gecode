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

#define USET Gecode::Set::LUBndSet

namespace Gecode { namespace String {

  class NSBlocks;
  typedef std::vector<NSBlocks> vecm;

  class GECODE_STRING_EXPORT OutOfLimitsDS : public Exception {
  public:
    /// Initialize with location \a l
    OutOfLimitsDS(const char* l);
  };

  class GECODE_STRING_EXPORT EmptyDS : public Exception {
  public:
    /// Initialize with location \a l
    EmptyDS(const char* l);
  };

  class GECODE_STRING_EXPORT UnknownValDS : public Exception {
  public:
    /// Initialize with location \a l
    UnknownValDS(const char* l);
  };

}}

namespace Gecode { namespace String {

  class NSIntSet;

  class GECODE_STRING_EXPORT DSIntSet : public USET {

  private:

    DSIntSet& operator=(const DSIntSet& that);
    DSIntSet(const DSIntSet& that);

  public:

    DSIntSet();
    DSIntSet(Space& h, int a, int b);
    DSIntSet(Space& h, const NSIntSet& s);
    DSIntSet(Space& h, const DSIntSet& s);

    IntSet toIntSet() const;

    void init(Space& h, const DSIntSet& s);
    void init(Space& h, const NSIntSet& s);

    void update(Space& h, const NSIntSet& s);
    void update(Space& h, const DSIntSet& s);
    void update(Space& h, char c);

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

  struct GECODE_STRING_EXPORT DSBlock {

    int l;
    int u;
    DSIntSet S;

    DSBlock();

    DSBlock(Space& h, const DSBlock& b);

    DSBlock(Space& h, const NSBlock& b);

    DSBlock(Space& h, const DSIntSet& s, int a, int b);

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

    DSBlock& operator=(const DSBlock& that);
    DSBlock(const DSBlock& that);

  };
  
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

  class GECODE_STRING_EXPORT DSBlocks :
  public Gecode::Support::DynamicArray<DSBlock, Space> {

  protected:

    int _size;

  private:

    DSBlocks(const DSBlocks& d);
    DSBlock& operator [](int i) const;
    DSBlocks& operator=(const DSBlocks& d);

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
    void update(Space& h, const NSBlocks& that, int& l, int& u);
    void update(Space& h, const string& s);

    void insert(Space& h, int k, const DSBlock& d);

    //void normalize(Space& h);
    void normalize(Space& h, int&, int&);
    
    void become(Space& h, DSBlock* y, int m);

    template <typename Blocks> bool operator==(const Blocks& that) const;

    friend std::ostream& operator<<(std::ostream& os, const DSBlocks& that);

  };

}}

namespace Gecode { namespace String { namespace Branch {

  enum Level {
    LENGTH,
    CARD,
    BASE
  };

  enum Value {
    MIN,
    MAX,
    MUSTMIN,
    MUSTMAX
  };

  enum Block {
    FIRST,
    SMALLEST
  };

}}}

namespace Gecode { namespace String {

  class Position;
  class DashedView;

  /**
   * \brief Class implementing the dashed string representation.
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
  static NSIntSet _MUST_CHARS;
  
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

  DashedString(Space& h);

  DashedString(Space& h, const std::string& s);

  DashedString(Space& h, const DSBlock& b);

  DashedString(Space& h, const DSBlocks& bv);

  DashedString(Space& h, NSBlocks& v, int mil, int mal);

  DashedString(Space& h, const DashedString& d);

  DSBlocks& blocks();
  const DSBlocks& blocks() const;

  DSBlock& at(int i);
  const DSBlock& at(int i) const;

  int min_length() const;
  void min_length(int i);

  int max_length() const;
  void max_length(int i);

  NSIntSet must_chars() const;

  NSIntSet may_chars() const;

  int length() const;

  double logdim() const;

  bool known() const;

  string known_pref() const;

  string known_suff() const;

  bool null() const;
  
  bool isTop() const;
  
  bool is_normalized() const; 

  void set_null(Space& h);

  string val() const;

  void norm_update(Space& h, NSBlocks& v);

  void normalize(Space& h);

  bool check_equate(const DashedString& that) const;
  bool check_equate(const NSBlocks& that) const;

  bool equate(Space& h, DashedString& that);
  bool equate(Space& h, const NSBlocks& that);

  void update(Space& h, string s);
  void update(Space& h, const DashedString& that);
  void update(Space& h, const NSBlocks& that);

  bool equals(const DashedString& that) const;
  bool equals(const NSBlocks& that) const;
  
  bool contains(const NSBlocks&) const;
  bool contains(const DashedString&) const;
  bool contains(const DashedString&, const DashedString&) const;

  bool find(Space&, DashedString&, int&, int&, bool);

  int first_na_block() const;

  int smallest_na_block() const;

  bool check_lex(const DashedString& that, bool lt) const;

  NSBlocks lex_least(const std::vector<float>& succ) const;

  NSBlocks lex_greatest(const std::vector<float>& succ) const;

  bool increasing(Space&h, bool strict);

  string min_str() const;

  void commit(Space&, Branch::Level, Branch::Value, Branch::Block, unsigned);

  bool refine_lb(int l);

  bool refine_ub(Space&, int u);
  
  bool refine_card(Space&, int mil, int mal);
  
  bool refine_card_eq(Space& h, DashedString& that);
  
  bool refine_card_cat(Space& h, DashedString& x, DashedString& y);
  
  bool refine_card_pow(Space& h, DashedString& x, int n1, int n2);

  bool concat(Space& h, DashedString& x, DashedString& y);

  NSBlocks pow(int n) const;
  bool pow(Space& h, DashedString& x, int& lx, int& ux);

  bool rev(Space& h, DashedString& x);

  bool lex(Space& h, DashedString& y, bool neq);
  
  bool changed() const;
  void changed(bool b);

  bool operator==(const DashedString& that) const;

  bool operator!=(const DashedString& that) const;

  private:

    // True iff the dashed string has changed after a propagation method.
    bool _changed;
    // The blocks of the dashed string.
    DSBlocks _blocks;
    // Length of the shortest concrete string this dashed dtring can represent.
    int _min_length;
    // Length of the longest concrete string this dashed dtring can represent.
    int _max_length;

    void _commit0(Space&, Branch::Level, Branch::Value, int);

    void _commit1(Space&, Branch::Level, Branch::Value, int);

    DashedString(const DashedString& that);
    DashedString& operator=(const DashedString& that);

  };

}}

#include <gecode/string/dashed/dashed-string.hpp>

#endif
