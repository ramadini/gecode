#ifndef __GECODE_STRING_HH__
#define __GECODE_STRING_HH__

#include <climits>
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
 * \brief Bounded length string.
 *
 * The Gecode::String namespace contains all functionality required
 * to program propagators and branchers for bounded length strings.
 * In addition, all propagators and branchers for bounded length strings
 * provided by %Gecode are contained as nested namespaces.
 *
 */

#include <gecode/string/exception.hpp>
#include <gecode/string/var-imp.hpp>

namespace Gecode {


  namespace String {
    class StringView;
  }

  /**
   * \brief String variables
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
    /// Initialize from string view \a y
    StringVar(const String::StringView& y);

    GECODE_STRING_EXPORT
    StringVar(Space& home);

    GECODE_STRING_EXPORT
    StringVar(Space& home, int a, int b);

    GECODE_STRING_EXPORT
    StringVar(Space& home, const string& s);

    GECODE_STRING_EXPORT
    StringVar(Space& home, String::NSBlocks& v, int mil, int mal);

    GECODE_STRING_EXPORT
    StringVar(Space& home, const String::NSIntSet& s, int a, int b);

    //@}


    /**
    * \brief Return assigned value
    *
    * Throws an exception of type String::ValOfUnassignedVar if variable
    * is not yet assigned.
    *
    */
    string val(void) const;

    bool assigned(void) const;

    /// Return size (dimension) of domain
    double size(void) const;

    const String::DashedString& domain(void) const;

    int min_length() const;

    int max_length() const;

    String::NSIntSet must_chars() const;

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
    STRT_LEXLQ, ///< Lex. less or equal than (\f$\prec\f$)
	  STRT_INCLT, ///<
	  STRT_INCLQ,
	  STRT_DECLT,
	  STRT_DECLQ,
  };

  enum StringOpType {
    STRT_CAT,    ///< Concatenation (\f$\cdot\f$)
    STRT_SUB,    ///< Sub-string
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
     * Each variable is initialized with domain \d
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
   * \ingroup TaskModelSet
   */

  /**
   * \brief %Set variable array
   * \ingroup TaskModelStringVarArrays
   */
  class StringVarArray : public VarArray<StringVar> {
  public:
    /// \name Creation and initialization
    //@{
    /// Default constructor (array of size 0)
    StringVarArray(void);
    /// Initialize from set variable array \a a (share elements)
    StringVarArray(const StringVarArray&);
    /// Initialize from set variable argument array \a a (copy elements)
    StringVarArray(Space& home, const StringVarArgs&);
    /// Allocate array for \a n set variables (variables are uninitialized)
    GECODE_STRING_EXPORT
    StringVarArray(Space& home, int n);
    /*
     * \brief Create an array of size \a n.
     *
     * Each variable is initialized with domain \d
     */
    GECODE_STRING_EXPORT
    StringVarArray(Space& home, int n, String::NSBlocks& v);
    //@}

  };

  struct VarArgs {
    IntVarArgs    iva;
    BoolVarArgs   bva;
    StringVarArgs sva;
  };
}
#include <gecode/string/array.hpp>

namespace Gecode {

  GECODE_STRING_EXPORT void
  nat2str(Home home, IntVar n, StringVar x);

  GECODE_STRING_EXPORT void
  str2nat(Home home, StringVar x, IntVar n);

  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringRelType r);

  /// Post propagator for \f$ x \sim_r y\f$
  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringRelType r, StringVar y);

  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringRelType r, String::NSBlocks& d, int, int);

  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringRelType rt, StringVar y, Reify r);

  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringOpType ot, StringVar y);

  GECODE_STRING_EXPORT void
  rel(Home home, StringVar x, StringVar y, StringOpType ot, StringVar z);

  GECODE_STRING_EXPORT void
  rel(Home home, StringVar, StringVar, StringVar, StringOpType, StringVar);

  /// Post propagator for \f$ |x| = l \f$
  GECODE_STRING_EXPORT void
  length(Home home, StringVar x, IntVar l);

  GECODE_STRING_EXPORT void
  char2code(Home home, StringVar x, IntVar n);

  /// Post propagator for \f$ y = x[i..j] \f$
  GECODE_STRING_EXPORT void
  substr(Home home, StringVar x, IntVar i, IntVar j, StringVar y);

  GECODE_STRING_EXPORT void
  startswith(Home home, StringVar x, StringVar y);
  GECODE_STRING_EXPORT void
  startswith(Home home, StringVar x, StringVar y, BoolVar b);

  GECODE_STRING_EXPORT void
  endswith(Home home, StringVar x, StringVar y);
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

  /// Post propagator for \f$ n = find(x, y) \f$
  GECODE_STRING_EXPORT void
  find(Home home, StringVar x, StringVar y, IntVar n);

  /// Post propagator for \f$ n = rfind(x, y) \f$
  GECODE_STRING_EXPORT void
  rfind(Home home, StringVar x, StringVar y, IntVar n);

  /// Post propagator for \f$ y = replace(x, q, q1) \f$
  GECODE_STRING_EXPORT void
  replace(Home home, StringVar x, StringVar q, StringVar q1, StringVar y);
  
  /// Post propagator for \f$ y1 = replaceAll(x, q, q1) \f$
  GECODE_STRING_EXPORT void
  replace_all(Home home, StringVar x, StringVar q, StringVar q1, StringVar y);
  
  /// Post propagator for \f$ y1 = replaceLast(x, q, q1) \f$
  GECODE_STRING_EXPORT void
  replace_last(Home home, StringVar x, StringVar q, StringVar q1, StringVar y);

  /// Post propagator for \f$ gcc(x, a, n) \f$
  GECODE_STRING_EXPORT void
  gcc(Home home, StringVar x, const IntArgs& a, IntVarArgs n);

  /// Post propagator for \f$ gcc(x, a, n) \f$
  GECODE_STRING_EXPORT void
  must_chars(Home home, StringVar x, const String::NSIntSet& s);

  /// Post propagator for \f$ y = x_1 + \dots + x_n \f$
  GECODE_STRING_EXPORT void
  gconcat(Home home, StringVarArgs x, StringVar y);

  /// Post propagator for \f$ x \in expr \f$
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, string expr);
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, string expr, BoolVar b, ReifyMode rm);
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, const DFA& d, BoolVar b, ReifyMode rm);
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, string expr, VarArgs& va);
  /// Post propagator for \f$ x \in L(M) \f$
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, DFA M);
  // Post propagator for \f$ x \in a \f$
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, StringVarArgs a);
  GECODE_STRING_EXPORT void
  extensional(Home home, StringVar x, const std::vector<std::string>& a);

  // Post propagator for \f$ a[i] = x \f$
  GECODE_STRING_EXPORT void
  element(Home home, StringVarArgs a, IntVar i, StringVar x);

  /**
   * \brief Print string variable \a x
   * \relates Gecode::StringVar
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringVar& x);
}

namespace Gecode {

  // FIXME: Stubs for compatibility with version 6.
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
  
  class StringVarBranch : public VarBranch<StringVar> {

  public:
    /// Which variable selection
    enum Select {
      STRING_VAR_NONE = 0,    ///< First unassigned
      STRING_VAR_LENBLOCKMIN,
	    STRING_VAR_BLOCKMIN,
      STRING_VAR_SIZEMIN,
      STRING_VAR_LENMIN,
      STRING_VAR_LENMAX
    };

  protected:
     /// Which variable to select
     Select s;

  public:
     /// Initialize with strategy STRING_VAR_NONE
     StringVarBranch(void);

     /// Initialize with selection strategy \a s
     StringVarBranch(Select s);

     /// Return selection strategy
     Select select(void) const;
  };

}
#include <gecode/string/branch/var.hpp>

namespace Gecode {

  class StringValBranch : public ValBranch<StringVar> {

  public:
    /// Which variable selection
    enum Select {
      STRING_VAL_LLLL,
      STRING_VAL_LLUL,
      STRING_VAL_LLLM
    };

  protected:
     /// Which variable to select
     Select s;

  public:

     StringValBranch(void);

     /// Initialize with selection strategy \a s
     StringValBranch(Select s);

     /// Return selection strategy
     Select select(void) const;
  };

}
#include <gecode/string/branch/val.hpp>

namespace Gecode {

  GECODE_STRING_EXPORT void
  none_llll(Home home, const StringVarArray& x);

  GECODE_STRING_EXPORT void
  none_llll(Home home, const StringVarArgs& x);

  GECODE_STRING_EXPORT void
  sizemin_llul(Home home, const StringVarArray& x);

  GECODE_STRING_EXPORT void
  sizemin_llul(Home home, const StringVarArgs& x);

  GECODE_STRING_EXPORT void
  lenmax_llll(Home home, const StringVarArray& x);

  GECODE_STRING_EXPORT void
  lenmax_llll(Home home, const StringVarArgs& x);

  GECODE_STRING_EXPORT void
  lenmin_llll(Home home, const StringVarArray& x);

  GECODE_STRING_EXPORT void
  lenmin_llll(Home home, const StringVarArgs& x);

  GECODE_STRING_EXPORT void
  blockmin_llll(Home home, const StringVarArray& x);

  GECODE_STRING_EXPORT void
  blockmin_llll(Home home, const StringVarArgs& x);
  
  GECODE_STRING_EXPORT void
  blockmin_lllm(Home home, const StringVarArray& x);

  GECODE_STRING_EXPORT void
  blockmin_lllm(Home home, const StringVarArgs& x);
  
  GECODE_STRING_EXPORT void
  lenblockmin_lllm(Home home, const StringVarArray& x);

  GECODE_STRING_EXPORT void
  lenblockmin_lllm(Home home, const StringVarArgs& x);

  StringVarBranch STRING_VAR_NONE(void);
  StringVarBranch STRING_VAR_SIZEMIN(void);
  StringVarBranch STRING_VAR_BLOCKMIN(void);
  StringVarBranch STRING_VAR_LENBLOCKMIN(void);
  StringVarBranch STRING_VAR_LENMIN(void);
  StringVarBranch STRING_VAR_LENMAX(void);

  StringValBranch STRING_VAL_LLLL(void);
  StringValBranch STRING_VAL_LLUL(void);
  StringValBranch STRING_VAL_LLLM(void);
  
  GECODE_STRING_EXPORT void
  branch(Home home, const StringVarArgs& x,
         StringVarBranch vars, StringValBranch vals);

}

#endif
