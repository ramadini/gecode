#ifndef __GECODE_STRING_HH__
#define __GECODE_STRING_HH__

#include <climits>
#include <cmath>
#include <gecode/kernel.hh>

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

/**
 * \namespace Gecode::String
 * \brief Bounded-length string support.
 *
 * The Gecode::String namespace contains all functionality required
 * to program propagators and branchers for bounded length strings.
 * In addition, all propagators and branchers for bounded length strings
 * provided by %Gecode are contained as nested namespaces.
 *
 * A solver symbol is a Unicode scalar value. String lengths and positions
 * count scalar values, independently of their UTF-8 or UTF-16 encoding.
 *
 */

#include <gecode/string/exception.hpp>
#include <gecode/string/value.hpp>
#include <gecode/string/var-imp.hpp>

namespace Gecode { namespace String {

  /**
   * \brief Finite alphabet for a string variable
   *
   * An alphabet contains Unicode scalar values.  In particular, surrogate
   * code points are rejected.  The predefined byte profile preserves the
   * legacy string-variable alphabet.
   *
   * \ingroup TaskModelStringVars
   */
  class GECODE_STRING_EXPORT StringAlphabet {
  public:
    /// Closed interval of Unicode scalar values
    struct Range {
      StringSymbol lower;
      StringSymbol upper;

      Range(StringSymbol l, StringSymbol u) : lower(l), upper(u) {}
    };

  private:
    NSIntSet _symbols;

    explicit StringAlphabet(const NSIntSet& symbols);

  public:
    /// Legacy byte alphabet U+0000..U+00FF
    static StringAlphabet bytes(void);
    /// ASCII alphabet U+0000..U+007F
    static StringAlphabet ascii(void);
    /// ISO-8859-1/Latin-1 code-point alphabet U+0000..U+00FF
    static StringAlphabet latin1(void);
    /// All Unicode scalar values, excluding UTF-16 surrogates
    static StringAlphabet unicode_scalars(void);

    /// Construct an alphabet from closed ranges
    static StringAlphabet from_ranges(const std::vector<Range>& ranges);
    /// Construct an alphabet from closed ranges
    static StringAlphabet from_ranges(std::initializer_list<Range> ranges);
    /// Construct an alphabet from individual code points
    static StringAlphabet from_codepoints(
      const std::vector<StringSymbol>& codepoints);
    /// Construct an alphabet from individual code points
    static StringAlphabet from_codepoints(
      std::initializer_list<StringSymbol> codepoints);

    /// Test whether the alphabet contains \a symbol
    bool contains(StringSymbol symbol) const;
    /// Return the number of scalar values in the alphabet
    int size(void) const;
    /// Return the number of ranges used by the compact representation
    int ranges(void) const;
    /// Return the compact symbol set
    const NSIntSet& symbols(void) const;
  };

}}

namespace Gecode {


  namespace String {
    class StringView;
  }

  /**
   * \brief Bounded-length string variable
   *
   * A string variable ranges over finite sequences of Unicode scalar values.
   * Lengths and positions count scalar values, not encoded bytes or grapheme
   * clusters. No normalization or locale-sensitive text processing is
   * performed.
   *
   * Construction with negative or individually oversized lengths throws
   * String::OutOfLimitsDS. Reversed bounds, an empty block container, and
   * bounds or mandatory alphabets admitting no string throw String::EmptyDS.
   * Zero-length bounds and optional empty alphabets represent the singleton
   * empty string. Submitted block bounds are checked before normalization.
   *
   * \ingroup TaskModelStringVars
   */
  class StringVar : public VarImpVar<String::StringVarImp> {

  private:

    friend class StringVarArray;
    friend class StringVarArgs;

    using VarImpVar<String::StringVarImp>::x;

  public:
    /// \name Constructors and initialization
    //@{
    /// Default constructor
    StringVar(void);

    /// Initialize from string variable \a y
    StringVar(const StringVar& y);
    /// Assign from string variable \a y
    StringVar& operator =(const StringVar& y) = default;
    /// Initialize from string view \a y
    StringVar(const String::StringView& y);

    /// Initialize with the legacy byte alphabet and default length bounds
    GECODE_STRING_EXPORT
    StringVar(Space& home);

    /// Initialize with length bounds and the legacy byte alphabet
    GECODE_STRING_EXPORT
    StringVar(Space& home, int a, int b);

    /// Initialize with length bounds and an explicit active alphabet
    GECODE_STRING_EXPORT
    StringVar(Space& home, int a, int b,
              const String::StringAlphabet& alphabet);

    /// Initialize using the legacy one-byte-per-symbol interpretation
    GECODE_STRING_EXPORT
    StringVar(Space& home, const string& s);

    /// Initialize to the encoding-neutral value \a value
    GECODE_STRING_EXPORT
    StringVar(Space& home, const String::StringVal& value);

    /// Initialize from a scalar-value domain and explicit length bounds
    GECODE_STRING_EXPORT
    StringVar(Space& home, String::NSBlocks& v, int mil, int mal);

    /// Initialize from an active scalar-value set and length bounds
    GECODE_STRING_EXPORT
    StringVar(Space& home, const String::NSIntSet& s, int a, int b);

    //@}


    /**
    * \brief Return assigned legacy byte value
    *
    * Throws an exception of type String::ValOfUnassignedVar if variable
    * is not yet assigned, and String::OutOfLimitsDS if a symbol cannot be
    * represented as one byte. Use val_symbols() for encoding-neutral values.
    *
    */
    string val(void) const;

    /// Return the assigned encoding-neutral scalar-value sequence
    String::StringVal val_symbols(void) const;

    /// Test whether the domain contains exactly one concrete string
    bool assigned(void) const;

    /**
     * \brief Return the block-product estimate of the number of strings
     *
     * Assigned values, including the empty string, have size one. Coupled
     * aggregate bounds and ambiguous block decompositions can overestimate
     * the number of strings. Large estimates can overflow to infinity.
     */
    double size(void) const;

    /// Return the dashed representation of the domain
    const String::DashedString& domain(void) const;

    /// Return the minimum length of a string in the domain
    int min_length() const;
    /// Return the maximum length of a string in the domain
    int max_length() const;

    /// Return symbols from non-optional singleton blocks of the dashed domain
    String::NSIntSet must_chars() const;
    /// Return scalar values occurring in at least one string in the domain
    String::NSIntSet may_chars() const;

    template<class Char, class Traits>
    friend std::basic_ostream<Char,Traits>&
    operator <<(std::basic_ostream<Char,Traits>& os, const StringVar& x);

  };

  enum StringRelType {
    STRT_DOM,   ///< Domain (\f$::\f$)
    STRT_EQ,    ///< Equality (\f$=\f$)
    STRT_NQ,    ///< Disequality (\f$\neq\f$)
    STRT_LEXLT, ///< Lex. less than (\f$\prec\f$)
    STRT_LEXLQ, ///< Lexicographically less than or equal (\f$\preceq\f$)
    STRT_INCLT, ///< Strictly increasing symbols (unary rel)
    STRT_INCLQ, ///< Non-decreasing symbols (unary rel)
    STRT_DECLT, ///< Strictly decreasing symbols (unary rel)
    STRT_DECLQ, ///< Non-increasing symbols (unary rel)
  };

  enum StringOpType {
    STRT_CAT,    ///< Concatenation (\f$\cdot\f$)
    STRT_SUB,    ///< Reserved substring operation (not supported by rel)
    STRT_REV,    ///< Reverse
    STRT_REP,    ///< Replace
    STRT_REPALL, ///< Replace all
    STRT_REPLST, ///< Replace last
    STRT_LCASE,  ///< Lower case
    STRT_UCASE,  ///< Upper case
  };

}

#include <gecode/string/view.hpp>
#include <gecode/string/var/string.hpp>
#include <gecode/string/array-traits.hpp>

namespace Gecode {

  /** \brief Passing string variables
   *
   * We could have used a simple typedef instead, but doxygen cannot
   * resolve some overloading then, leading to unusable documentation for
   * important parts of the library. As long as there is no fix for this,
   * we will keep this workaround.
   *
   */
  class StringVarArgs : public VarArgArray<StringVar> {
  public:
    /// \name Constructors and initialization
    //@{
    /// Allocate empty array
    StringVarArgs(void);
    /// Allocate array with \a n elements
    explicit StringVarArgs(int n);
    /// Initialize from variable argument array \a a (copy elements)
    StringVarArgs(const StringVarArgs& a);
    /// Initialize from variable array \a a (copy elements)
    StringVarArgs(const VarArray<StringVar>& a);
    /// Initialize from vector \a a
    StringVarArgs(const std::vector<StringVar>& a);
    /// Initialize from InputIterator \a first and \a last
    template<class InputIterator>
    StringVarArgs(InputIterator first, InputIterator last);
    /**
     * \brief Create an array of size \a n.
     *
     * Each variable is initialized with domain \a v
     */
    GECODE_STRING_EXPORT
    StringVarArgs(Space& home, int n, const String::NSBlocks& v);
    //@}
  };
  //@}

  /**
   * \defgroup TaskModelStringVarArrays Variable arrays
   *
   * Variable arrays can store variables. They are typically used
   * for storing the variables being part of a solution. However,
   * they can also be used for temporary purposes (even though
   * memory is not reclaimed until the space it is created for
   * is deleted).
   * \ingroup TaskModelString
   */

  /**
   * \brief String variable array
   * \ingroup TaskModelStringVarArrays
   */
  class StringVarArray : public VarArray<StringVar> {
  public:
    /// \name Creation and initialization
    //@{
    /// Default constructor (array of size 0)
    StringVarArray(void);
    /// Initialize from string variable array \a a (share elements)
    StringVarArray(const StringVarArray&);
    /// Initialize from string variable argument array \a a (copy elements)
    StringVarArray(Space& home, const StringVarArgs&);
    /// Allocate \a n string variables with default length bounds and alphabet
    GECODE_STRING_EXPORT
    StringVarArray(Space& home, int n);
    /**
     * \brief Create an array of size \a n.
     *
     * Each variable is initialized with domain \a v.
     */
    GECODE_STRING_EXPORT
    StringVarArray(Space& home, int n, String::NSBlocks& v);
    /// Assignment operator
    StringVarArray& operator =(const StringVarArray&) = default;
    //@}

  };

  /// Variables introduced by regular-expression decomposition
  struct VarArgs {
    /// Introduced integer variables
    IntVarArgs    iva;
    /// Introduced Boolean variables
    BoolVarArgs   bva;
    /// Introduced string variables
    StringVarArgs sva;
  };
}
#include <gecode/string/array.hpp>

namespace Gecode {

  /**
   * \defgroup TaskModelStringChannel String channel constraints
   * \ingroup TaskModelString
   */
  //@{
  /**
   * \brief Channel \a n to its ASCII decimal representation \a x
   *
   * Negative values of \a n map to the empty string. Nonnegative values map
   * to their decimal representation without leading zeros, with zero as "0".
   */
  GECODE_STRING_EXPORT void
  nat2str(Home home, IntVar n, StringVar x);

  /**
   * \brief Channel an ASCII decimal representation \a x to \a n
   *
   * Leading zeros are accepted. Empty strings, strings containing non-digits,
   * and values exceeding native int map to -1. A parsed value must also fit
   * the IntVar range; native INT_MAX has no support and does not map to -1.
   */
  GECODE_STRING_EXPORT void
  str2nat(Home home, StringVar x, IntVar n);

  /**
   * \brief Channel an empty or single-scalar string \a x to \a n
   *
   * The empty string maps to -1. A single Unicode scalar value maps to its
   * numeric value. Strings containing more than one scalar value have no support.
   */
  GECODE_STRING_EXPORT void
  char2code(Home home, StringVar x, IntVar n);
  //@}

  /**
   * \defgroup TaskModelStringRel String relation constraints
   * \ingroup TaskModelString
   */
  //@{
  /// Post unary increasing or decreasing symbol-order relation \a r on \a x
  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringRelType r);

  /// Post binary relation \f$ x \sim_r y\f$
  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringRelType r, StringVar y);

  /**
   * \brief Restrict \a x by ordinary-lifetime dashed domain \a d
   *
   * For \c STRT_DOM, \a a and \a b are ignored. Other relations compare
   * \a x with a temporary string variable initialized from \a d using
   * length bounds \a a and \a b.
   */
  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringRelType r, String::NSBlocks& d,
      int a, int b);

  /// Post reified relation \f$ x \sim_{rt} y\f$
  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringRelType rt, StringVar y, Reify r);

  /// Post unary operation \f$ y = ot(x)\f$
  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringOpType ot, StringVar y);

  /// Post binary operation \f$ z = ot(x,y)\f$
  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringVar y, StringOpType ot, StringVar z);

  /// Post ternary operation \f$ t = ot(x,y,z)\f$
  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringVar y, StringVar z,
      StringOpType ot, StringVar t);
  //@}

  /**
   * \defgroup TaskModelStringLength String length constraints
   * \ingroup TaskModelString
   */
  //@{
  /// Post propagator for \f$ |x| = l \f$
  GECODE_STRING_EXPORT void
  length(Home home, StringVar x, IntVar l);
  //@}

  /**
   * \defgroup TaskModelStringSequence String sequence constraints
   * \ingroup TaskModelString
   */
  //@{
  /**
   * \brief Post \f$ y = x[i..j] \f$ with one-based inclusive scalar positions
   *
   * Select positions from max(1,i) through min(|x|,j). If the selected
   * interval is empty, \a y is the empty string.
   */
  GECODE_STRING_EXPORT void
  substr(Home home, StringVar x, IntVar i, IntVar j, StringVar y);

  /// Post propagator for \f$ x\f$ starting with \f$ y\f$
  GECODE_STRING_EXPORT void
  startswith(Home home, StringVar x, StringVar y);
  /// Post propagator for \f$ b \iff x\f$ starts with \f$ y\f$
  GECODE_STRING_EXPORT void
  startswith(Home home, StringVar x, StringVar y, BoolVar b);

  /// Post propagator for \f$ x\f$ ending with \f$ y\f$
  GECODE_STRING_EXPORT void
  endswith(Home home, StringVar x, StringVar y);
  /// Post propagator for \f$ b \iff x\f$ ends with \f$ y\f$
  GECODE_STRING_EXPORT void
  endswith(Home home, StringVar x, StringVar y, BoolVar b);

  /// Post propagator for \f$ x contains y \f$
  GECODE_STRING_EXPORT void
  contains(Home home, StringVar x, StringVar y);

  /// Post propagator for \f$ b \iff x contains y \f$
  GECODE_STRING_EXPORT void
  contains(Home home, StringVar x, StringVar y, BoolVar b);

  /// Post propagator for \f$ y = x^n \f$
  GECODE_STRING_EXPORT void
  pow(Home home, StringVar x, IntVar n, StringVar y);

  /**
   * \brief Set \a n to the one-based position of \a x in \a y
   *
   * Here \a x is the pattern and \a y is the haystack. The value zero means
   * that \a x does not occur in \a y. Positions count Unicode scalar values.
   */
  GECODE_STRING_EXPORT void
  find(Home home, StringVar x, StringVar y, IntVar n);

  /**
   * \brief Set \a n to the one-based position of the last \a x in \a y
   *
   * Here \a x is the pattern and \a y is the haystack. The value zero means
   * that \a x does not occur in \a y. Positions count Unicode scalar values.
   */
  GECODE_STRING_EXPORT void
  rfind(Home home, StringVar x, StringVar y, IntVar n);

  /// Post propagator for \f$ y = replace(x, q, q1) \f$
  GECODE_STRING_EXPORT void
  replace(Home home, StringVar x, StringVar q, StringVar q1, StringVar y);

  /// Post propagator for \f$ y1 = replaceAll(x,  q, q1) \f$
  GECODE_STRING_EXPORT void
  replace_all(Home home, StringVar x, StringVar q, StringVar q1, StringVar y);

  /// Post propagator for \f$ y1 = replaceLast(x, q, q1) \f$
  GECODE_STRING_EXPORT void
  replace_last(Home home, StringVar x, StringVar q, StringVar q1, StringVar y);
  //@}

  /**
   * \defgroup TaskModelStringGCC String global-cardinality constraints
   * \ingroup TaskModelString
   */
  //@{
  /** \brief Post propagator for \f$ gcc(x, a, n) \f$
   *
   * Throws Int::ArgumentSizeMismatch if \a a and \a n have different sizes.
   */
  GECODE_STRING_EXPORT void
  gcc(Home home, StringVar x, const IntArgs& a, IntVarArgs n);

  /// Post propagator requiring every scalar value in \a s to occur in \a x
  GECODE_STRING_EXPORT void
  must_chars(Home home, StringVar x, const String::NSIntSet& s);
  //@}

  /**
   * \defgroup TaskModelStringConcat String concatenation constraints
   * \ingroup TaskModelString
   */
  //@{
  /// Post propagator for \f$ y = x_1 + \dots + x_n \f$
  GECODE_STRING_EXPORT void
  gconcat(Home home, StringVarArgs x, StringVar y);
  //@}

  /**
   * \defgroup TaskModelStringExtensional String extensional constraints
   * \ingroup TaskModelString
   */
  //@{
  /**
   * \brief Post \f$ x \in expr \f$ from strict UTF-8 regular-expression text
   *
   * Regular-expression atoms and character classes denote Unicode scalar
   * values. The wildcard and negated classes range over the active alphabet
   * of \a x.
   */
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, string expr);
  /// Reified variant of extensional(Home,StringVar,string)
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, string expr, BoolVar b, ReifyMode rm);
  /// Post reified membership of \a x in DFA \a d
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, const DFA& d, BoolVar b, ReifyMode rm);
  /**
   * \brief Post regex membership with variables introduced by decomposition
   *
   * \a va receives the integer, Boolean, and string variables introduced by
   * the selected regular-expression decomposition.
   */
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, string expr, VarArgs& va);
  /// Post propagator for \f$ x \in L(M) \f$
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, DFA M);
  /// Post propagator for \f$ x \in a \f$
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, StringVarArgs a);
  /// Post using encoding-neutral concrete scalar-value sequences
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x,
              const std::vector<String::StringVal>& a);
  /// Post using legacy one-byte-per-symbol concrete values
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, const std::vector<std::string>& a);

  /**
   * \brief Set \a i to the earliest match position of strict UTF-8 regex \a expr
   *
   * The position is the one-based starting position of the earliest
   * non-empty substring of \a x matched by \a expr, measured in Unicode scalar
   * values. The value zero means that no substring matches. Patterns accepting
   * the empty string violate the posting precondition.
   */
  GECODE_STRING_EXPORT void
  match(Home home, StringVar x, string expr, IntVar i);
  //@}

  /**
  * \defgroup TaskModelStringElement String element constraints
  * \ingroup TaskModelString
  */
  //@{
  /**
   * \brief Post element selection with a one-based index \a i
   *
   * The valid index interval is 1 through a.size(), inclusive. The selected
   * entry in the zero-based C++ argument array satisfies \f$ a[i-1] = x \f$.
   */
  GECODE_STRING_EXPORT void
  element(Home home, StringVarArgs a, IntVar i, StringVar x);
  //@}

  /**
   * \brief Print string variable \a x
   * \relates Gecode::StringVar
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringVar& x);
}

namespace Gecode {

  // Compatibility aliases required by the generic Gecode branch interface.
  typedef std::function<double(const Space& home, StringVar x, int i)>
    StringBranchMerit;

  typedef std::function<int(const Space& home, StringVar x, int i)>
    StringBranchVal;

  typedef std::function<void(Space& home, unsigned int a,
                             StringVar x, int i, int n)> StringBranchCommit;

  template<>
  class BranchTraits<StringVar> {
  public:
   typedef StringBranchMerit Merit;
   typedef StringBranchVal Val;
   typedef StringBranchCommit Commit;
  };

  /**
  * \defgroup TaskModelStringBranch String branching
  * \ingroup TaskModelString
  */
  //@{

  /**
  * \brief String variable selection
  * \ingroup TaskModelStringBranch
  */
  class StringVarBranch : public VarBranch<StringVar> {

  public:
    /// Which variable selection
    enum Select {
      STRING_VAR_NONE = 0,    ///< First unassigned variable
      /**
       * \brief Scan by length span and first-unassigned-block estimates
       *
       * Scan unassigned variables in array order, initially selecting the
       * first. The running length-span threshold is its maximum minus minimum
       * length, or the maximum supported string length if its length is fixed.
       * A later variable with positive length span replaces the selection only
       * if its span is smaller than the threshold, and updates the threshold.
       * Equal positive spans do not use block estimates to break ties.
       *
       * A fixed-length candidate instead compares its first unassigned block's
       * log-cardinality estimate, then that block's repetition span, then its
       * minimum string length against the current selection. A smaller value
       * at the first differing comparison replaces the selection; exact ties
       * retain it. This replacement leaves the length-span threshold unchanged.
       * Mixed fixed/variable-length scans therefore depend on array order,
       * rather than a single ordering key. For two variables with lengths
       * 1..5 and 4..5, the latter is selected.
       */
      STRING_VAR_LENBLOCKMIN,
      /**
       * \brief Smallest estimate for the first unassigned block
       *
       * Compare the estimated cardinality of each variable's first unassigned
       * block, then its repetition span, then the variable's minimum length.
       */
      STRING_VAR_BLOCKMIN,
      STRING_VAR_SIZEMIN, ///< Smallest estimated domain
      STRING_VAR_LENMIN,  ///< Smallest minimum length
      STRING_VAR_LENMAX   ///< Largest maximum length
    };

  protected:
     /// Which variable to select
     Select s;

  public:
     /// Initialize with strategy STRING_VAR_BLOCKMIN
     StringVarBranch(void);

     /// Initialize with selection strategy \a s
     StringVarBranch(Select s);

     /// Return selection strategy
     Select select(void) const;
  };

}
#include <gecode/string/branch/var.hpp>

namespace Gecode {

  /**
   * \brief String value selection
   * \ingroup TaskModelStringBranch
   *
   * When length varies, first try the minimum length, then larger lengths.
   * At fixed length, branch on a symbol at the first unresolved scalar
   * position. LLLL first tries the smallest available symbol. LLLM prefers
   * the smallest available symbol that occurs in a mandatory singleton block
   * of any variable in the brancher's array, falling back to the smallest
   * available symbol. There is no separate repetition decision.
   */
  class StringValBranch : public ValBranch<StringVar> {

  public:
    /// Which value selection
    enum Select {
      STRING_VAL_LLLL, ///< Minimum length, then smallest positional symbol
      STRING_VAL_LLUL, ///< Legacy spelling for LLLL decisions; SIZEMIN only
      STRING_VAL_LLLM  ///< Minimum length, then preferred positional symbol
    };

  protected:
     /// Which value strategy to use
     Select s;

  public:

     /// Initialize with strategy STRING_VAL_LLLM
     StringValBranch(void);

     /// Initialize with selection strategy \a s
     StringValBranch(Select s);

     /// Return selection strategy
     Select select(void) const;
  };

}
#include <gecode/string/branch/val.hpp>

namespace Gecode {

  /// Branch on the first unassigned variable using STRING_VAL_LLLL
  GECODE_STRING_EXPORT void
  none_llll(Home home, const StringVarArray& x);

  /// Branch on the first unassigned variable using STRING_VAL_LLLL
  GECODE_STRING_EXPORT void
  none_llll(Home home, const StringVarArgs& x);

  /// Branch on the smallest estimated domain using legacy LLUL (LLLL decisions)
  GECODE_STRING_EXPORT void
  sizemin_llul(Home home, const StringVarArray& x);

  /// Branch on the smallest estimated domain using legacy LLUL (LLLL decisions)
  GECODE_STRING_EXPORT void
  sizemin_llul(Home home, const StringVarArgs& x);

  /// Branch on the largest maximum length using STRING_VAL_LLLL
  GECODE_STRING_EXPORT void
  lenmax_llll(Home home, const StringVarArray& x);

  /// Branch on the largest maximum length using STRING_VAL_LLLL
  GECODE_STRING_EXPORT void
  lenmax_llll(Home home, const StringVarArgs& x);

  /// Branch on the smallest minimum length using STRING_VAL_LLLL
  GECODE_STRING_EXPORT void
  lenmin_llll(Home home, const StringVarArray& x);

  /// Branch on the smallest minimum length using STRING_VAL_LLLL
  GECODE_STRING_EXPORT void
  lenmin_llll(Home home, const StringVarArgs& x);

  /// Branch by the first unassigned block's estimate using STRING_VAL_LLLL
  GECODE_STRING_EXPORT void
  blockmin_llll(Home home, const StringVarArray& x);

  /// Branch by the first unassigned block's estimate using STRING_VAL_LLLL
  GECODE_STRING_EXPORT void
  blockmin_llll(Home home, const StringVarArgs& x);

  /// Branch by the first unassigned block's estimate using STRING_VAL_LLLM
  GECODE_STRING_EXPORT void
  blockmin_lllm(Home home, const StringVarArray& x);

  /// Branch by the first unassigned block's estimate using STRING_VAL_LLLM
  GECODE_STRING_EXPORT void
  blockmin_lllm(Home home, const StringVarArgs& x);

  /// Branch by the length-span/block scan using STRING_VAL_LLLM
  /// \sa StringVarBranch::STRING_VAR_LENBLOCKMIN
  GECODE_STRING_EXPORT void
  lenblockmin_lllm(Home home, const StringVarArray& x);

  /// Branch by the length-span/block scan using STRING_VAL_LLLM
  /// \sa StringVarBranch::STRING_VAR_LENBLOCKMIN
  GECODE_STRING_EXPORT void
  lenblockmin_lllm(Home home, const StringVarArgs& x);

  /// Select the first unassigned variable
  StringVarBranch STRING_VAR_NONE(void);
  /// Select the variable with the smallest estimated domain
  StringVarBranch STRING_VAR_SIZEMIN(void);
  /// Select by the first unassigned block's estimate and its tie-breaks
  /// \sa StringVarBranch::STRING_VAR_BLOCKMIN
  StringVarBranch STRING_VAR_BLOCKMIN(void);
  /// Select by the length-span/block scan
  /// \sa StringVarBranch::STRING_VAR_LENBLOCKMIN
  StringVarBranch STRING_VAR_LENBLOCKMIN(void);
  /// Select the variable with the smallest minimum length
  StringVarBranch STRING_VAR_LENMIN(void);
  /// Select the variable with the largest maximum length
  StringVarBranch STRING_VAR_LENMAX(void);

  /// Select minimum length, then the smallest positional symbol
  StringValBranch STRING_VAL_LLLL(void);
  /// Select legacy LLUL spelling for LLLL decisions (SIZEMIN only)
  StringValBranch STRING_VAL_LLUL(void);
  /// Select minimum length, then prefer mandatory positional symbols
  StringValBranch STRING_VAL_LLLM(void);

  /**
   * \brief Post string branching with compatible selectors
   *
   * Supported combinations are:
   * - STRING_VAR_NONE(), STRING_VAR_LENMIN(), or STRING_VAR_LENMAX() with
   *   STRING_VAL_LLLL();
   * - STRING_VAR_SIZEMIN() with STRING_VAL_LLUL(), whose decisions are LLLL;
   * - STRING_VAR_BLOCKMIN() with STRING_VAL_LLLL() or STRING_VAL_LLLM();
   * - STRING_VAR_LENBLOCKMIN() with STRING_VAL_LLLM().
   *
   * Unsupported variable/value selector combinations raise
   * String::UnknownBranching.
   */
  GECODE_STRING_EXPORT void
  branch(Home home, const StringVarArgs& x,
         StringVarBranch vars, StringValBranch vals);
  //@}

}

#endif

// STATISTICS: string-post
