/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Contributing authors:
 *     Gabriel Hjort Blindell <gabriel.hjort.blindell@gmail.com>
 *
 *  Copyright:
 *     Guido Tack, 2007-2012
 *     Gabriel Hjort Blindell, 2012
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __GECODE_FLATZINC_HH__
#define __GECODE_FLATZINC_HH__

#include <iostream>

#include <gecode/kernel.hh>
#include <gecode/int.hh>
#ifdef GECODE_HAS_SET_VARS
#include <gecode/set.hh>
#endif
#ifdef GECODE_HAS_FLOAT_VARS
#include <gecode/float.hh>
#endif
#ifdef GECODE_HAS_STRING_VARS
#include <gecode/string.hh>
#endif
#include <map>

/*
 * Support for DLLs under Windows
 *
 */

#if !defined(GECODE_STATIC_LIBS) && \
    (defined(__CYGWIN__) || defined(__MINGW32__) || defined(_MSC_VER))

#ifdef GECODE_BUILD_FLATZINC
#define GECODE_FLATZINC_EXPORT __declspec( dllexport )
#else
#define GECODE_FLATZINC_EXPORT __declspec( dllimport )
#endif

#else

#ifdef GECODE_GCC_HAS_CLASS_VISIBILITY

#define GECODE_FLATZINC_EXPORT __attribute__ ((visibility("default")))

#else

#define GECODE_FLATZINC_EXPORT

#endif
#endif

// Configure auto-linking
#ifndef GECODE_BUILD_FLATZINC
#define GECODE_LIBRARY_NAME "FlatZinc"
#include <gecode/support/auto-link.hpp>
#endif

#include <gecode/driver.hh>

#include <gecode/flatzinc/conexpr.hh>
#include <gecode/flatzinc/ast.hh>
#include <gecode/flatzinc/varspec.hh>

/**
 * \namespace Gecode::FlatZinc
 * \brief Interpreter for the %FlatZinc language
 *
 * The Gecode::FlatZinc namespace contains all functionality required
 * to parse and solve constraint models written in the %FlatZinc language.
 *
 */

namespace Gecode { namespace FlatZinc {

  /**
   * \brief Output support class for %FlatZinc interpreter
   *
   */
  class GECODE_FLATZINC_EXPORT Printer {
  private:
    /// Names of integer variables
    std::vector<std::string> iv_names;
    /// Names of Boolean variables
    std::vector<std::string> bv_names;
#ifdef GECODE_HAS_FLOAT_VARS
    /// Names of float variables
    std::vector<std::string> fv_names;
#endif
#ifdef GECODE_HAS_SET_VARS
    /// Names of set variables
    std::vector<std::string> sv_names;
#endif
#ifdef GECODE_HAS_STRING_VARS
    /// Names of string variables
    std::vector<std::string> tv_names;
#endif
    AST::Array* _output;
    void printElem(std::ostream& out,
                   AST::Node* ai,
                   const Gecode::IntVarArray& iv,
                   const Gecode::BoolVarArray& bv
#ifdef GECODE_HAS_SET_VARS
                   ,
                   const Gecode::SetVarArray& sv
#endif
#ifdef GECODE_HAS_FLOAT_VARS
                  ,
                  const Gecode::FloatVarArray& fv
#endif
#ifdef GECODE_HAS_STRING_VARS
                  ,
                  const Gecode::StringVarArray& tv
#endif
                   ) const;
    void printElemDiff(std::ostream& out,
                       AST::Node* ai,
                       const Gecode::IntVarArray& iv1,
                       const Gecode::IntVarArray& iv2,
                       const Gecode::BoolVarArray& bv1,
                       const Gecode::BoolVarArray& bv2
#ifdef GECODE_HAS_SET_VARS
                       ,
                       const Gecode::SetVarArray& sv1,
                       const Gecode::SetVarArray& sv2
#endif
#ifdef GECODE_HAS_FLOAT_VARS
                       ,
                       const Gecode::FloatVarArray& fv1,
                       const Gecode::FloatVarArray& fv2
#endif
#ifdef GECODE_HAS_STRING_VARS
                       ,
                       const Gecode::StringVarArray& tv1,
                       const Gecode::StringVarArray& tv2
#endif
                       ) const;
  public:
    Printer(void) : _output(NULL) {}
    void init(AST::Array* output);

    void print(std::ostream& out,
               const Gecode::IntVarArray& iv,
               const Gecode::BoolVarArray& bv
#ifdef GECODE_HAS_SET_VARS
               ,
               const Gecode::SetVarArray& sv
#endif
#ifdef GECODE_HAS_FLOAT_VARS
               ,
               const Gecode::FloatVarArray& fv
#endif
#ifdef GECODE_HAS_STRING_VARS
               ,
               const Gecode::StringVarArray& tv
#endif
               ) const;

    void printDiff(std::ostream& out,
               const Gecode::IntVarArray& iv1, const Gecode::IntVarArray& iv2,
               const Gecode::BoolVarArray& bv1, const Gecode::BoolVarArray& bv2
#ifdef GECODE_HAS_SET_VARS
               ,
               const Gecode::SetVarArray& sv1, const Gecode::SetVarArray& sv2
#endif
#ifdef GECODE_HAS_FLOAT_VARS
               ,
               const Gecode::FloatVarArray& fv1,
               const Gecode::FloatVarArray& fv2
#endif
#ifdef GECODE_HAS_STRING_VARS
               ,
               const Gecode::StringVarArray& tv1,
               const Gecode::StringVarArray& tv2
#endif
               ) const;


    ~Printer(void);

    void addIntVarName(const std::string& n);
    const std::string& intVarName(int i) const { return iv_names[i]; }
    void addBoolVarName(const std::string& n);
    const std::string& boolVarName(int i) const { return bv_names[i]; }
#ifdef GECODE_HAS_FLOAT_VARS
    void addFloatVarName(const std::string& n);
    const std::string& floatVarName(int i) const { return fv_names[i]; }
#endif
#ifdef GECODE_HAS_SET_VARS
    void addSetVarName(const std::string& n);
    const std::string& setVarName(int i) const { return sv_names[i]; }
#endif
#ifdef GECODE_HAS_STRING_VARS
    void addStringVarName(const std::string& n);
    const std::string& stringVarName(int i) const { return tv_names[i]; }
#endif

    void shrinkElement(AST::Node* node,
                       std::map<int,int>& iv, std::map<int,int>& bv,
                       std::map<int,int>& sv, std::map<int,int>& fv,
                       std::map<int,int>& tv);

    void shrinkArrays(Space& home,
                      int& optVar, bool optVarIsInt,
                      Gecode::IntVarArray& iv,
                      Gecode::BoolVarArray& bv
#ifdef GECODE_HAS_SET_VARS
                      ,
                      Gecode::SetVarArray& sv
#endif
#ifdef GECODE_HAS_FLOAT_VARS
                      ,
                      Gecode::FloatVarArray& fv
#endif
#ifdef GECODE_HAS_STRING_VARS
                      ,
                      Gecode::StringVarArray& tv
#endif
                     );

  private:
    Printer(const Printer&);
    Printer& operator=(const Printer&);
  };

  /**
   * \brief %Options for running %FlatZinc models
   *
   */
  class FlatZincOptions : public Gecode::BaseOptions {
  protected:
      /// \name Search options
      //@{
      Gecode::Driver::IntOption         _solutions; ///< How many solutions
      Gecode::Driver::BoolOption        _allSolutions; ///< Return all solutions
      Gecode::Driver::DoubleOption      _threads;   ///< How many threads to use
      Gecode::Driver::BoolOption        _free; ///< Use free search
      Gecode::Driver::DoubleOption      _decay;       ///< Decay option
      Gecode::Driver::UnsignedIntOption _c_d;       ///< Copy recomputation distance
      Gecode::Driver::UnsignedIntOption _a_d;       ///< Adaptive recomputation distance
      Gecode::Driver::UnsignedIntOption _node;      ///< Cutoff for number of nodes
      Gecode::Driver::UnsignedIntOption _fail;      ///< Cutoff for number of failures
      Gecode::Driver::UnsignedIntOption _time;      ///< Cutoff for time
      Gecode::Driver::UnsignedIntOption _time_limit;  ///< Cutoff for time (for compatibility with flatzinc command line)
      Gecode::Driver::IntOption         _seed;      ///< Random seed
      Gecode::Driver::StringOption      _restart;   ///< Restart method option
      Gecode::Driver::DoubleOption      _r_base;    ///< Restart base
      Gecode::Driver::UnsignedIntOption _r_scale;   ///< Restart scale factor
      Gecode::Driver::BoolOption        _nogoods;   ///< Whether to use no-goods
      Gecode::Driver::UnsignedIntOption _nogoods_limit; ///< Depth limit for extracting no-goods
      Gecode::Driver::BoolOption        _interrupt; ///< Whether to catch SIGINT
      Gecode::Driver::DoubleOption      _step;        ///< Step option
      //@}

      /// \name Execution options
      //@{
      Gecode::Driver::StringOption      _mode;       ///< Script mode to run
      Gecode::Driver::BoolOption        _stat;       ///< Emit statistics
      Gecode::Driver::StringValueOption _output;     ///< Output file
      
      /// \name String options
      //@{
      Gecode::Driver::BoolOption _reverse_regex; ///< Use reverse propagation for regular
      Gecode::Driver::BoolOption _decomp_regex; ///< Decompose flat regular expressions      
      Gecode::Driver::IntOption _max_length; ///< Maximum string length
      Gecode::Driver::IntOption _max_alpha; ///< Alphabet size (the identifier of each character is in [0, _max_alpha - 1]
      Gecode::Driver::BoolOption _quad_sweep; ///< Quadratic version of sweep algorithm      
      //@}

#ifdef GECODE_HAS_CPPROFILER

      Gecode::Driver::IntOption         _profiler_id; ///< Use this execution id for the CP-profiler
      Gecode::Driver::UnsignedIntOption _profiler_port; ///< Connect to this port
      Gecode::Driver::BoolOption        _profiler_info; ///< Whether solution information should be sent to the CP-profiler

#endif

      //@}
  public:
    /// Constructor
    FlatZincOptions(const char* s)
    : Gecode::BaseOptions(s),
      _solutions("n","number of solutions (0 = all, -1 = one/best)",-1),
      _allSolutions("a", "return all solutions (equal to -n 0)"),
      _threads("p","number of threads (0 = #processing units)",
               Gecode::Search::Config::threads),
      _free("f", "free search, no need to follow search-specification"),
      _decay("decay","decay factor",0.99),
      _c_d("c-d","recomputation commit distance",Gecode::Search::Config::c_d),
      _a_d("a-d","recomputation adaption distance",Gecode::Search::Config::a_d),
      _node("node","node cutoff (0 = none, solution mode)"),
      _fail("fail","failure cutoff (0 = none, solution mode)"),
      _time("time","time (in ms) cutoff (0 = none, solution mode)"),
      _time_limit("t","time (in ms) cutoff (0 = none, solution mode)"),
      _seed("r","random seed",0),
      _restart("restart","restart sequence type",RM_NONE),
      _r_base("restart-base","base for geometric restart sequence",1.5),
      _r_scale("restart-scale","scale factor for restart sequence",250),
      _nogoods("nogoods","whether to use no-goods from restarts",false),
      _nogoods_limit("nogoods-limit","depth limit for no-good extraction",
                     Search::Config::nogoods_limit),
      _interrupt("interrupt","whether to catch Ctrl-C (true) or not (false)",
                 true),
      _step("step","step distance for float optimization",0.0),
      _mode("mode","how to execute script",Gecode::SM_SOLUTION),
      _stat("s","emit statistics"),
      _output("o","file to send output to"),
      _reverse_regex("reverse-regex", "use reverse propagation for regular",
	      Gecode::String::DashedString::_DECOMP_REGEX),
	    _decomp_regex("decomp-regex", "decomp flat regular expression",
			  Gecode::String::DashedString::_DECOMP_REGEX),
	    _max_length("max-length", "maximum string length",
			  Gecode::String::DashedString::_MAX_STR_LENGTH),
	    _max_alpha("max-alpha", "alphabet size",
			  Gecode::String::DashedString::_MAX_STR_ALPHA),
			_quad_sweep("quad-sweep", "quadratic version of sweep algorithm",
			  Gecode::String::DashedString::_QUAD_SWEEP)

#ifdef GECODE_HAS_CPPROFILER
      ,
      _profiler_id("cpprofiler-id", "use this execution id with cpprofiler", 0),
      _profiler_port("cpprofiler-port", "connect to cpprofiler on this port", 6565),
      _profiler_info("cpprofiler-info", "send solution information to cpprofiler", false)

#endif
    {
      _mode.add(Gecode::SM_SOLUTION, "solution");
      _mode.add(Gecode::SM_STAT, "stat");
      _mode.add(Gecode::SM_GIST, "gist");
      _mode.add(Gecode::SM_CPPROFILER, "cpprofiler");
      _restart.add(RM_NONE,"none");
      _restart.add(RM_CONSTANT,"constant");
      _restart.add(RM_LINEAR,"linear");
      _restart.add(RM_LUBY,"luby");
      _restart.add(RM_GEOMETRIC,"geometric");

      add(_solutions); add(_threads); add(_c_d); add(_a_d);
      add(_allSolutions);
      add(_free);
      add(_decay);
      add(_node); add(_fail); add(_time); add(_time_limit); add(_interrupt);
      add(_seed);
      add(_step);
      add(_restart); add(_r_base); add(_r_scale);
      add(_nogoods); add(_nogoods_limit);
      add(_mode); add(_stat);
      add(_output);
      add(_reverse_regex); 
      add(_decomp_regex); 
      add(_max_length); 
      add(_max_alpha);
      add(_quad_sweep);
#ifdef GECODE_HAS_CPPROFILER
      add(_profiler_id);
      add(_profiler_port);
      add(_profiler_info);
#endif
    }

    void parse(int& argc, char* argv[]) {
      Gecode::BaseOptions::parse(argc,argv);
      if (_allSolutions.value() && _solutions.value()==-1) {
        _solutions.value(0);
      }
      if (_time_limit.value()) {
        _time.value(_time_limit.value());
      }
      if (_stat.value())
        _mode.value(Gecode::SM_STAT);
      Gecode::String::DashedString::_DECOMP_REGEX = _decomp_regex.value();
      Gecode::String::DashedString::_REVERSE_REGEX = _reverse_regex.value();
      Gecode::String::DashedString::_QUAD_SWEEP = _quad_sweep.value();
    }

    virtual void help(void) {
      std::cerr << "Gecode FlatZinc interpreter" << std::endl
                << " - Supported FlatZinc version: " << GECODE_FLATZINC_VERSION
                << std::endl << std::endl;
      Gecode::BaseOptions::help();
    }

    int solutions(void) const { return _solutions.value(); }
    bool allSolutions(void) const { return _allSolutions.value(); }
    double threads(void) const { return _threads.value(); }
    bool free(void) const { return _free.value(); }
    unsigned int c_d(void) const { return _c_d.value(); }
    unsigned int a_d(void) const { return _a_d.value(); }
    unsigned int node(void) const { return _node.value(); }
    unsigned int fail(void) const { return _fail.value(); }
    unsigned int time(void) const { return _time.value(); }
    int seed(void) const { return _seed.value(); }
    double step(void) const { return _step.value(); }
    const char* output(void) const { return _output.value(); }

    Gecode::ScriptMode mode(void) const {
      return static_cast<Gecode::ScriptMode>(_mode.value());
    }

    double decay(void) const { return _decay.value(); }
    RestartMode restart(void) const {
      return static_cast<RestartMode>(_restart.value());
    }
    void restart(RestartMode rm) {
      _restart.value(rm);
    }
    double restart_base(void) const { return _r_base.value(); }
    void restart_base(double d) { _r_base.value(d); }
    unsigned int restart_scale(void) const { return _r_scale.value(); }
    void restart_scale(int i) { _r_scale.value(i); }
    bool nogoods(void) const { return _nogoods.value(); }
    unsigned int nogoods_limit(void) const { return _nogoods_limit.value(); }
    bool interrupt(void) const { return _interrupt.value(); }

#ifdef GECODE_HAS_CPPROFILER

    int profiler_id(void) const { return _profiler_id.value(); }
    unsigned int profiler_port(void) const { return _profiler_port.value(); }
    bool profiler_info(void) const { return _profiler_info.value(); }

#endif

    void allSolutions(bool b) { _allSolutions.value(b); }
    
    bool decomp_regex() const { return _decomp_regex.value(); }
    void decomp_regex(bool b) { _decomp_regex.value(b); }
    bool reverse_regex() const { return _reverse_regex.value(); }
    void reverse_regex(bool b) { _reverse_regex.value(b); }
    int max_length() const { return _max_length.value(); }
    void max_length(int i) { _max_length.value(i); }
    int max_alpha() const { return _max_alpha.value(); }
    void max_alpha(int i) { _max_alpha.value(i); }
    bool quad_sweep() const { return _quad_sweep.value(); }
    void quad_sweep(bool b) { _quad_sweep.value(b); }
  };

  class BranchInformation : public SharedHandle {
  public:
    /// Constructor
    BranchInformation(void);
    /// Copy constructor
    BranchInformation(const BranchInformation& bi);
    /// Initialise for use
    void init(void);
    /// Add new brancher information
    void add(BrancherGroup bg,
             const std::string& rel0,
             const std::string& rel1,
             const std::vector<std::string>& n);
    /// Output branch information
    void print(const Brancher& b,
               unsigned int a, int i, int n, std::ostream& o) const;
#ifdef GECODE_HAS_FLOAT_VARS
    /// Output branch information
    void print(const Brancher& b,
               unsigned int a, int i, const FloatNumBranch& nl,
               std::ostream& o) const;
#endif
  };

  /// Uninitialized default random number generator
  GECODE_FLATZINC_EXPORT
  extern Rnd defrnd;

  class FlatZincSpaceInitData;

  /**
   * \brief A space that can be initialized with a %FlatZinc model
   *
   */
  class GECODE_FLATZINC_EXPORT FlatZincSpace : public Space {
  public:
    enum Meth {
      SAT, //< Solve as satisfaction problem
      MIN, //< Solve as minimization problem
      MAX  //< Solve as maximization problem
    };
  protected:
    /// Initialisation data (only used for posting constraints)
    FlatZincSpaceInitData* _initData;
    /// Number of integer variables
    int intVarCount;
    /// Number of Boolean variables
    int boolVarCount;
    /// Number of float variables
    int floatVarCount;
    /// Number of set variables
    int setVarCount;
    /// Number of string variables
    int stringVarCount;

    /// Index of the variable to optimize
    int _optVar;
    /// Whether variable to optimize is integer (or float)
    bool _optVarIsInt;

    /// Whether to solve as satisfaction or optimization problem
    Meth _method;

    /// Percentage of variables to keep in LNS (or 0 for no LNS)
    unsigned int _lns;

    /// Initial solution to start the LNS (or NULL for no LNS)
    IntSharedArray _lnsInitialSolution;

    /// Random number generator
    Rnd _random;

    /// Annotations on the solve item
    AST::Array* _solveAnnotations;

    /// Copy constructor
    FlatZincSpace(FlatZincSpace&);
  private:
    /// Run the search engine
    template<template<class> class Engine>
    void
    runEngine(std::ostream& out, const Printer& p,
              const FlatZincOptions& opt, Gecode::Support::Timer& t_total);
    /// Run the meta search engine
    template<template<class> class Engine,
             template<class, template<class> class> class Meta>
    void
    runMeta(std::ostream& out, const Printer& p,
            const FlatZincOptions& opt, Gecode::Support::Timer& t_total);
    void
    branchWithPlugin(AST::Node* ann);
  public:
    /// The integer variables
    Gecode::IntVarArray iv;
    /// The introduced integer variables
    Gecode::IntVarArray iv_aux;

    /// The integer variables used in LNS
    Gecode::IntVarArray iv_lns;

    /// Indicates whether an integer variable is introduced by mzn2fzn
    std::vector<bool> iv_introduced;
    /// Indicates whether an integer variable aliases a Boolean variable
    int* iv_boolalias;
    /// The Boolean variables
    Gecode::BoolVarArray bv;
    /// The introduced Boolean variables
    Gecode::BoolVarArray bv_aux;
    /// Indicates whether a Boolean variable is introduced by mzn2fzn
    std::vector<bool> bv_introduced;
#ifdef GECODE_HAS_SET_VARS
    /// The set variables
    Gecode::SetVarArray sv;
    /// The introduced set variables
    Gecode::SetVarArray sv_aux;
    /// Indicates whether a set variable is introduced by mzn2fzn
    std::vector<bool> sv_introduced;
#endif
#ifdef GECODE_HAS_FLOAT_VARS
    /// The float variables
    Gecode::FloatVarArray fv;
    /// The introduced float variables
    Gecode::FloatVarArray fv_aux;
    /// Indicates whether a float variable is introduced by mzn2fzn
    std::vector<bool> fv_introduced;
    /// Step by which a next solution has to have lower cost
    Gecode::FloatNum step;
#endif
#ifdef GECODE_HAS_STRING_VARS
    /// The string variables
    Gecode::StringVarArray tv;
    /// The introduced string variables
    Gecode::StringVarArray tv_aux;
    /// Indicates whether a string variable is introduced by mzn2fzn
    std::vector<bool> tv_introduced;
#endif
    /// Whether the introduced variables still need to be copied
    bool needAuxVars;
    /// Construct empty space
    FlatZincSpace(Rnd& random = defrnd);

    /// Destructor
    ~FlatZincSpace(void);

    /// Initialize space with given number of variables
    void init(int intVars, int boolVars, int setVars, int floatVars, int stringVars);

    /// Create new integer variable from specification
    void newIntVar(IntVarSpec* vs);
    /// Link integer variable \a iv to Boolean variable \a bv
    void aliasBool2Int(int iv, int bv);
    /// Return linked Boolean variable for integer variable \a iv
    int aliasBool2Int(int iv);
    /// Create new Boolean variable from specification
    void newBoolVar(BoolVarSpec* vs);
    /// Create new set variable from specification
    void newSetVar(SetVarSpec* vs);
    /// Create new float variable from specification
    void newFloatVar(FloatVarSpec* vs);
    /// Create new string variable from specification
    void newStringVar(StringVarSpec* vs);

    /// Post a constraint specified by \a ce
    void postConstraints(std::vector<ConExpr*>& ces);

    /// Post the solve item
    void solve(AST::Array* annotation);
    /// Post that integer variable \a var should be minimized
    void minimize(int var, bool isInt, AST::Array* annotation);
    /// Post that integer variable \a var should be maximized
    void maximize(int var, bool isInt, AST::Array* annotation);

    /// Run the search
    void run(std::ostream& out, const Printer& p,
             const FlatZincOptions& opt, Gecode::Support::Timer& t_total);

    /// Produce output on \a out using \a p
    void print(std::ostream& out, const Printer& p) const;
#ifdef GECODE_HAS_CPPROFILER
    /// Get string representing the domains of variables (for cpprofiler)
    std::string getDomains(const Printer& p) const;
#endif
    /// Compare this space with space \a s and print the differences on
    /// \a out
    void compare(const Space& s, std::ostream& out) const;
    /// Compare this space with space \a s and print the differences on
    /// \a out using \a p
    void compare(const FlatZincSpace& s, std::ostream& out,
                 const Printer& p) const;

    /**
     * \brief Remove all variables not needed for output
     *
     * After calling this function, no new constraints can be posted through
     * FlatZinc variable references, and the createBranchers method must
     * not be called again.
     *
     */
    void shrinkArrays(Printer& p);

    /// Return whether to solve a satisfaction or optimization problem
    Meth method(void) const;

    /// Return index of variable used for optimization
    int optVar(void) const;
    /// Return whether variable used for optimization is integer (or float)
    bool optVarIsInt(void) const;

    /**
     * \brief Create branchers corresponding to the solve item annotations
     *
     * If \a ignoreUnknown is true, unknown solve item annotations will be
     * ignored, otherwise a warning is written to \a err.
     *
     * The seed for random branchers is given by the \a seed parameter.
     *
     */
    void createBranchers(Printer& p, AST::Node* ann,
                         FlatZincOptions& opt, bool ignoreUnknown,
                         std::ostream& err = std::cerr);

    /// Return the solve item annotations
    AST::Array* solveAnnotations(void) const;

    /// Information for printing branches
    BranchInformation branchInfo;

    /// Implement optimization
    virtual void constrain(const Space& s);
    /// Copy function
    virtual Gecode::Space* copy(void);
    /// Slave function for restarts
    virtual bool slave(const MetaInfo& mi);

    /// \name AST to variable and value conversion
    //@{
    /// Convert \a arg (array of integers) to IntArgs
    IntArgs arg2intargs(AST::Node* arg, int offset = 0);
    /// Convert \a arg (array of integers) to IntSharedArray
    IntSharedArray arg2intsharedarray(AST::Node* arg, int offset = 0);
    /// Convert \a arg (array of Booleans) to IntArgs
    IntArgs arg2boolargs(AST::Node* arg, int offset = 0);
    /// Convert \a arg (array of integers) to IntSharedArray
    IntSharedArray arg2boolsharedarray(AST::Node* arg, int offset = 0);
    /// Convert \a n to IntSet
    IntSet arg2intset(AST::Node* n);
    /// Convert \a arg to IntSetArgs
    IntSetArgs arg2intsetargs(AST::Node* arg, int offset = 0);
    /// Convert \a arg to IntVarArgs
    IntVarArgs arg2intvarargs(AST::Node* arg, int offset = 0);
    /// Convert \a arg to BoolVarArgs
    BoolVarArgs arg2boolvarargs(AST::Node* arg, int offset = 0, int siv=-1);
    /// Convert \a n to BoolVar
    BoolVar arg2BoolVar(AST::Node* n);
    /// Convert \a n to IntVar
    IntVar arg2IntVar(AST::Node* n);
    /// Convert \a a to TupleSet
    TupleSet arg2tupleset(const IntArgs& a, int noOfVars);
    /// Check if \a b is array of Booleans (or has a single integer)
    bool isBoolArray(AST::Node* b, int& singleInt);
#ifdef GECODE_HAS_SET_VARS
    /// Convert \a n to SetVar
    SetVar arg2SetVar(AST::Node* n);
    /// Convert \a n to SetVarArgs
    SetVarArgs arg2setvarargs(AST::Node* arg, int offset = 0, int doffset = 0,
                              const IntSet& od=IntSet::empty);
#endif
#ifdef GECODE_HAS_FLOAT_VARS
    /// Convert \a n to FloatValArgs
    FloatValArgs arg2floatargs(AST::Node* arg, int offset = 0);
    /// Convert \a n to FloatVar
    FloatVar arg2FloatVar(AST::Node* n);
    /// Convert \a n to FloatVarArgs
    FloatVarArgs arg2floatvarargs(AST::Node* arg, int offset = 0);
#endif
#ifdef GECODE_HAS_STRING_VARS
    /// Convert \a n to a vector of strings.
    std::vector<string> arg2stringvec(AST::Node* arg);
    /// Convert \a n to SetVar
    StringVar arg2StringVar(AST::Node* n);
    /// Convert \a n to SetVarArgs
    StringVarArgs arg2stringvarargs(AST::Node* arg, int offset = 0);
#endif
    /// Convert \a ann to integer propagation level
    IntPropLevel ann2ipl(AST::Node* ann);
    /// Share DFA \a a if possible
    DFA getSharedDFA(DFA& a);
    //@}
  };

  /// %Exception class for %FlatZinc errors
  class GECODE_VTABLE_EXPORT Error {
  private:
    const std::string msg;
  public:
    Error(const std::string& where, const std::string& what)
    : msg(where+": "+what) {}
    const std::string& toString(void) const { return msg; }
  };

  /**
   * \brief Parse FlatZinc file \a fileName into \a fzs and return it.
   *
   * Creates a new empty FlatZincSpace if \a fzs is NULL.
   */
  GECODE_FLATZINC_EXPORT
  FlatZincSpace* parse(const std::string& fileName, Printer& p,
                       const FlatZincOptions&, std::ostream& err = std::cerr,
                       FlatZincSpace* fzs=NULL, Rnd& rnd=defrnd);

  /**
   * \brief Parse FlatZinc from \a is into \a fzs and return it.
   *
   * Creates a new empty FlatZincSpace if \a fzs is NULL.
   */
  GECODE_FLATZINC_EXPORT
  FlatZincSpace* parse(std::istream& is, Printer& p, 
                       const FlatZincOptions&,
                       std::ostream& err = std::cerr,
                       FlatZincSpace* fzs=NULL, Rnd& rnd=defrnd);

}}

#endif

// STATISTICS: flatzinc-any
