#ifndef __GECODE_STRING_HH__
#define __GECODE_STRING_HH__

#include <gecode/kernel.hh>
#include <gecode/set.hh>
//FIXME: Do we need this import?
#include <functional>

/*
 * Configure linking
 *
 */
#if !defined(GECODE_STATIC_LIBS) && \
    (defined(__CYGWIN__) || defined(__MINGW32__) || defined(_MSC_VER))

#ifdef GECODE_BUILD_String
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
#ifndef GECODE_BUILD_String
#define GECODE_LIBRARY_NAME "String"
#include <gecode/support/auto-link.hpp>
#endif

/**
 * \namespace Gecode::String
 * \brief Bounded-length strings
 *
 * The Gecode::String namespace contains all functionality required
 * to program propagators and branchers for bounded-length strings.
 * All propagators and branchers are based on the dashed string abstraction.
 * TODO: Note that characters are actually sequences of integers...
 * TODO: Say something more about dashed strings, char encoding, and bla bla.
 *
 */

namespace Gecode { namespace String {
  
  /// Character encodings for string variables 
  enum CharEncoding {
    UNSPEC,
    ASCII,
    EASCII,
    UTF_8,
    UTF_16,
    UTF_32
  };
  
  class CharSet;

  /// Numerical limits for string variables
  namespace Limits {
    /// Upper bound on the string length. It has to be in \f$[1, 2^31)\f$, otherwise the behaviour is undefined.
    const int MAX_STRING_LENGTH = 10000;
    /// Upper bound on the alphabet size. It has to be in \f$[1, 2^31]\f$, otherwise the behaviour is undefined.
    /// The alphabet of each string variable is \f$\{0, 1, \dots, MAX\_ALPHABET\_SIZE\}\f$
    const int MAX_ALPHABET_SIZE = 65536;
    /// Check whether \f$ n \in [0, MAX\_STRING\_LENGTH]\f$, otherwise throw overflow exception with location \a l
    void check_length(int n, const char* l);
    /// Check whether \f$ [n,m] \subseteq [0, MAX\_STRING\_LENGTH]\f$. The following exceptions might be thrown:
    /// - VariableEmptyDomain, if n > m.
    /// - OutOfLimits, if \f$ [n,m] \not\subseteq [0, MAX\_STRING\_LENGTH] \f$
    void check_length(int n, int m, const char* l);
    /// Check whether \f$ a \in [0, MAX\_ALPHABET\_SIZE)\f$, otherwise throw overflow exception with location \a l
    void check_alphabet(int a, const char* l);
    /// Check whether \f$ [a,b] \subseteq \in [0, MAX\_ALPHABET\_SIZE)\f$. The following exceptions might be thrown:
    /// - VariableEmptyDomain, if a > b.
    /// - OutOfLimits, if \f$ [a,b] \not\subseteq [0, MAX\_ALPHABET\_SIZE)\f$
    void check_alphabet(int a, int b, const char* l);
    /// Check whether \f$ S \subseteq [0, MAX\_ALPHABET\_SIZE)\f$, otherwise throw overflow exception with location \a l
    void check_alphabet(const IntSet& S, const char* l);
  }

}}

#include <gecode/string/exception.hpp>
#include <gecode/string/limits.hpp>
#include <gecode/string/var-imp.hpp>

namespace Gecode {

  namespace String {
    class StringView;
  }

  /**
   * \brief %String variables
   *
   * \ingroup TaskModelStringVars
   */
  class StringVar : public VarImpVar<Gecode::String::StringVarImp> {
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
    /// Initialize from string view \a y
    StringVar(const String::StringView& y);

    /// Initialize variable's domain with block \f${0, 1, \dots, \alpha-1}^{(0, \lambda)}\f$ 
    /// where \f$\alpha = MAX\_ALPHABET\_SIZE, \lambda= MAX\_STRING\_LENGTH\f$
    GECODE_STRING_EXPORT StringVar(Space& home);
    /**
     * \brief Initialize variable's domain with block \f$ S^{(0, \lambda)} \f$ 
     * where \f$\lambda= MAX\_STRING\_LENGTH\f$
     *
     * The Gecode::String::OutOfLimits exception is thrown if S is not contained
     * in [0, MAX\_ALPHABET\_SIZE).
     */
    GECODE_STRING_EXPORT StringVar(Space& home, const IntSet& S);
    
    /**
     * \brief Initialize variable's domain with block \f$ \Sigma^{(l,u)} \f$ 
     * where \f$\Sigma= [0, MAX\_ALPHABET\_SIZE)\f$
     *
     * The following exceptions might be thrown:
     *  - Gecode::String::VariableEmptyDomain, if l > u.
     *  - Gecode::String::OutOfLimits, if \f$l < 0 \vee u > MAX\_STRING\_LENGTH\f$
     */
    GECODE_STRING_EXPORT StringVar(Space& home, int l, int u);
    
    /**
     * \brief Initialize variable's domain with block \f$ S^{(l,u)} \f$
     *
     * The following exceptions might be thrown:
     *  - Gecode::String::VariableEmptyDomain, if l > u.
     *  - Gecode::String::OutOfLimits, if \f$ S \not\subseteq [0, MAX\_ALPHABET\_SIZE) \vee  l < 0 \vee u > MAX\_STRING\_LENGTH\f$
     */
    GECODE_STRING_EXPORT StringVar(Space& home, const IntSet& S, int l, int u);

    //@}
    /// \name Value access
    //@{
    /// Return the minimum length for a string in the variable's domain
    int lenMin(void) const;
    /// Return the maximum length for a string in the variable's domain
    int lenMax(void) const;
    // TODO: Possible additions: mayChars, mustChars, logDim, ...if needed!
    //@}

    /// \name Domain tests
    // FIXME: Do we need these functions?
    //@{
    /// Test whether the string must contain character \a c
    bool contains(int c) const;
    /// Test whether the string cannot contain character \a c
    bool notContains(int c) const;
    //@}
  };

//  /**
//   * \defgroup TaskModelStringIter Block iterators for string variables
//   * \ingroup TaskModelString
//   */
//  //@{
//  
// FIXME: Do we need block iterators here?
//  /// Iterator for the blocks of a string variable
//  class StringVarBlocks {
//  private:
//    String::Blocks<String::StringVarImp*> iter;
//  public:
//    /// \name Constructors and initialization
//    //@{
//    /// Default constructor
//    StringVarBlocks(void);
//    /// Initialize to iterate blocks of variable \a x
//    StringVarBlocks(const StringVar& x);
//    //@}

//    /// \name Iteration control
//    //@{
//    /// Test whether iterator is still at a block or done
//    bool operator ()(void) const;
//    /// Move iterator to next block (if possible)
//    void operator ++(void);
//    //@}

//    /// \name Block access
//    //@{
//    /// Return the lower bound of block
//    int lb(void) const;
//    /// Return the upper bound of block
//    int ub(void) const;
//    //@}
//  };
//  //@}

  /**
   * \brief Print string variable \a x
   * \relates Gecode::StringVar
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringVar& x);

}

#include <gecode/string/view.hpp>

namespace Gecode {
  /**
   * \defgroup TaskModelStringArgs Argument arrays
   *
   * Argument arrays are just good enough for passing arguments
   * with automatic memory management.
   * \ingroup TaskModelString
   */

  //@{

}
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
    /// Initialize from list \a a
    StringVarArgs(std::initializer_list<StringVar> a);
    /// Initialize from InputIterator \a first and \a last
    template<class InputIterator>
    StringVarArgs(InputIterator first, InputIterator last);
////    // TODO: Copy-paste from StringVar constructors here.
//////    /**
//////     * \brief Create an array of size \a n.
//////     *
//////     * Each variable is initialized with the bounds and cardinality as
//////     * given by the arguments.
//////     */
//    GECODE_STRING_EXPORT
//    StringVarArgs(Space& home,int n,int glbMin,int glbMax,
//               int lubMin,int lubMax,
//               unsigned int minCard = 0,
//               unsigned int maxCard = String::Limits::card);
//    //@}
  };
}


namespace Gecode {
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
   * \brief %String variable array
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
    /// Allocate array for \a n string variables (variables are uninitialized)
    GECODE_STRING_EXPORT StringVarArray(Space& home, int n);
    // TODO: Same as above.
//////    /**
//////     * \brief Create an array of size \a n.
//////     *
//////     * Each variable is initialized with the bounds and cardinality as
//////     * given by the arguments.
//////     */
//////    GECODE_STRING_EXPORT
//////    StringVarArray(Space& home,int n,int glbMin,int glbMax,int lubMin,int lubMax,
//////                unsigned int minCard = 0,
//////                unsigned int maxCard = String::Limits::card);
    //@}s
  };

}
#include <gecode/string/array.hpp>

namespace Gecode {

  /**
   * \brief Common relation types for strings
   *
   *
   * \ingroup TaskModelString
   */
  enum stringRelType {
    STRT_EQ,    ///< Equality
    STRT_NQ,    ///< Disequality
    STRT_LEXLE, ///< Lexicographic less
    STRT_LEXLQ, ///< Lexicographic less or equal
    STRT_LEXGE, ///< Lexicographic greater
    STRT_LEXGQ, ///< Lexicographic greater or equal
  };

  /**
   * \defgroup TaskModelStringRel Relation constraints
   * \ingroup TaskModelString
   *
   */
  //@{
  /// Post propagator for \f$ x \sim_r y\f$
  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, stringRelType r, StringVar y);
  /// Post propagator for \f$ (x \sim_{rt} y) \equiv r\f$
  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, stringRelType rt, StringVar y, Reify r);
  //@}

}

namespace Gecode {

// TODO: Add string constraints here:

  /**
   * \defgroup TaskModelStringElement
   * \ingroup TaskModelString
   *
   * An element constraint selects zero, one or more elements out of a
   * sequence. We write \f$ \langle x_0,\dots, x_{n-1} \rangle \f$ for the
   * sequence, and \f$ [y] \f$ for the index variable.
   *
   * string element constraints are closely related to the ::element constraint
   * on integer variables.
   */
  //@{
  /**
   * \brief Post propagator for \f$ z=\diamond_{\mathit{op}}\langle x_0,\dots,x_{n-1}\rangle[y] \f$
   *
   * If \a y is the empty string, the usual conventions for string operations apply:
   * an empty union is empty, while an empty intersection is the universe,
   * which can be given as the optional parameter \a u.
   *
   * The indices for \a y start at 0.
   */
  
  //@}


// FIXME: Do we need wait/when functions?
  /**
   * \defgroup TaskModelStringExec Synchronized execution
   * \ingroup TaskModelString
   *
   * Synchronized execution executes a function or a static member function
   * when a certain event happends.
   *
   * \ingroup TaskModelString
   */
  //@{
  // Execute \a c when \a x becomes assigned
  GECODE_STRING_EXPORT void
  wait(Home home, StringVar x, std::function<void(Space& home)> c);
  // Execute \a c when all variables in \a x become assigned
  GECODE_STRING_EXPORT void
  wait(Home home, const StringVarArgs& x, std::function<void(Space& home)> c);
  //@}
}


namespace Gecode {

  /**
   * \defgroup TaskModelStringBranch Branching
   * \ingroup TaskModelString
   */
// FIXME: Do we need these functions?
  /**
   * \brief Branch filter function type for string variables
   *
   * The variable \a x is considered for selection and \a i refers to the
   * variable's position in the original array passed to the brancher.
   *
   * \ingroup TaskModelStringBranch
   */
  typedef std::function<bool(const Space& home, StringVar x, int i)>
    StringBranchFilter;
  /**
   * \brief Branch merit function type for string variables
   *
   * The function must return a merit value for the variable
   * \a x.
   * The value \a i refers to the variable's position in the original array
   * passed to the brancher.
   *
   * \ingroup TaskModelStringBranch
   */
  typedef std::function<double(const Space& home, StringVar x, int i)>
    StringBranchMerit;

  /**
   * \brief Branch value function type for string variables
   *
   * Returns a value for the variable \a x that is to be used in the
   * corresponding branch commit function. The integer \a i refers
   * to the variable's position in the original array passed to the
   * brancher.
   *
   * \ingroup TaskModelStringBranch
   */
  typedef std::function<int(const Space& home, StringVar x, int i)>
    StringBranchVal;

  /**
   * \brief Branch commit function type for string variables
   *
   * The function must post a constraint on the variable \a x which
   * corresponds to the alternative \a a. The integer \a i refers
   * to the variable's position in the original array passed to the
   * brancher. The value \a n is the value
   * computed by the corresponding branch value function.
   *
   * \ingroup TaskModelStringBranch
   */
  typedef std::function<void(Space& home, unsigned int a,
                             StringVar x, int i, int n)>
    StringBranchCommit;

}
#include <gecode/string/branch/traits.hpp>

////namespace Gecode {

//// FIXME: AFC not a priority
////  /**
////   * \brief Recording AFC information for string variables
////   *
////   * \ingroup TaskModelStringBranch
////   */
////  class StringAFC : public AFC {
////}
////#include <gecode/string/branch/afc.hpp>

////namespace Gecode {
//// FIXME: Do we need a StringAction?
////  /**
////   * \brief Recording actions for string variables
////   *
////   * \ingroup TaskModelStringBranch
////   */
////  class StringAction : public Action {
////  public:
////    /**
////     * \brief Construct as not yet initialized
////     *
////     * The only member functions that can be used on a constructed but not
////     * yet initialized action storage is init or the assignment operator.
////     *
////     */
////}
////#include <gecode/string/branch/action.hpp>

//// FIXME: CHB not a priority.
////namespace Gecode {
////  /**
////   * \brief Recording CHB for string variables
////   *
////   * \ingroup TaskModelStringBranch
////   */
////  class StringCHB : public CHB {
////  public:
////}
////#include <gecode/string/branch/chb.hpp>

namespace Gecode {

  /// Function type for printing branching alternatives for string variables
  typedef std::function<void(const Space &home, const Brancher& b,
                             unsigned int a,
                             StringVar x, int i, const int& n,
                             std::ostream& o)>
    StringVarValPrint;

}

namespace Gecode {

  /**
   * \brief Which variable to select for branching
   *
   * \ingroup TaskModelStringBranch
   */
  class StringVarBranch : public VarBranch<StringVar> {
  public:
    /// Which variable selection
    enum Select {
      STR_NONE = 0,        ///< First unassigned
      STR_RND,             ///< Random (uniform, for tie breaking)
      // TODO: Add search heuristics
    };
//  protected:
//    /// Which variable to select
//    Select s;
//  public:
//    /// Initialize with strategy SEL_NONE
//    StringVarBranch(void);
//    /// Initialize with random number generator \a r
//    StringVarBranch(Rnd r);
//    /// Initialize with selection strategy \a s
//    StringVarBranch(Select s);
//    /// Return selection strategy
//    Select select(void) const;
//  };

//  /**
//   * \defgroup TaskModelStringBranchVar Selecting string variables
//   * \ingroup TaskModelStringBranch
//   */
//  //@{
//  /// Select first unassigned variable
//  StringVarBranch STRING_VAR_NONE(void);
//// TODO: Add search heuristics
////  /// Select random variable (uniform distribution, for tie breaking)
////  StringVarBranch string_VAR_RND(Rnd r);
//  //@}
  };
}
#include <gecode/string/branch/var.hpp>

namespace Gecode {

  /**
   * \brief Which values to select for ...?
   *
   * \ingroup TaskModelStringBranch
   */
  class StringValBranch : public ValBranch<StringVar> {
  public:
    /// Which value selection
    enum Select {
      STRING_VAL_LLLL,
      STRING_VAL_LLUL,
      STRING_VAL_LLLM
      // TODO: Add heuristics
    };
  protected:
    /// Which value to select
    Select s;
  public:
    StringValBranch(void);
    /// Initialize with selection strategy \a s
    StringValBranch(Select s);
    /// Return selection strategy
    Select select(void) const;
  };

  /**
   * \defgroup TaskModelStringBranchVal Value selection for string variables
   * \ingroup TaskModelStringBranch
   */
  //@{
  // TODO: Add functions.

  //@}

}
#include <gecode/string/branch/val.hpp>

// FIXME: A class for each type of selection? which var, which len, which block, which card, which char?
//namespace Gecode {
//  /**
//   * \brief Which value to select for assignment
//   *
//   * \ingroup TaskModelStringBranch
//   */
//  class StringAssign : public ValBranch<StringVar> {
//  public:
//    /// Which value selection
//    enum Select {
//      SEL_MIN_INC,   ///< Include smallest element
//      SEL_MIN_EXC,   ///< Exclude smallest element
//      SEL_MED_INC,   ///< Include median element (rounding downwards)
//    };
//  }
//};
//#include <gecode/string/branch/assign.hpp>

// TODO: Add branch function(s?)
//namespace Gecode {
//  /**
//   * \brief Branch over \a x with variable selection \a vars and value selection \a vals
//   *
//   * \ingroup TaskModelStringBranch
//   */
//  GECODE_STRING_EXPORT void
//  branch(Home home, const StringVarArgs& x,
//         StringVarBranch vars, stringValBranch vals,
//         StringBranchFilter bf=nullptr,
//         StringVarValPrint vvp=nullptr);
//  /**
//   * \brief Branch over \a x with tie-breaking variable selection \a vars and value selection \a vals
//   *
//   * \ingroup TaskModelStringBranch
//   */
//  GECODE_STRING_EXPORT void
//  branch(Home home, const StringVarArgs& x,
//         TieBreak<StringVarBranch> vars, stringValBranch vals,
//         StringBranchFilter bf=nullptr,
//         StringVarValPrint vvp=nullptr);
//}
//namespace Gecode {
//  /**
//   * \brief Branch over \a x with value selection \a vals
//   *
//   * \ingroup TaskModelStringBranch
//   */
//  void
//  branch(Home home, const StringVarArgs& x,
//         stringValBranch vals,
//         StringBranchFilter bf=nullptr,
//         StringVarValPrint vvp=nullptr);
//  /**
//   * \brief Assign all \a x with value selection \a vals
//   *
//   * \ingroup TaskModelStringBranch
//   */
//  void
//  assign(Home home, const StringVarArgs& x,
//         stringAssign vals,
//         StringBranchFilter bf=nullptr,
//         StringVarValPrint vvp=nullptr);
//}
//#include <gecode/string/branch.hpp>

// TODO: Add support for Dynamic Symmetry Breaking?

// FIXME: Delta information hardly traceable: dashed strings do not form a lattice...

#endif
// IFDEF: GECODE_HAS_STRING_VARS
// STATISTICS: string-post
