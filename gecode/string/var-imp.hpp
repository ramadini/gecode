namespace Gecode { namespace String {

  /**
   * \brief String delta information for advisors.
   */
  class StringDelta : public Delta {
    
    friend class StringVarImp;

    // FIXME: At the moment, a StringDelta is not really needed.

  public:
    /// Create string delta as providing no information
    StringDelta(void);
  };

}}

#include <gecode/string/var-imp/delta.hpp>
#include <gecode/string/var-imp/dashed-string.hpp>

namespace Gecode { namespace String {

  /**
   * \brief String variable implementation with dashed strings.
   *
   * \ingroup Other
   */
  class StringVarImp : public StringVarImpBase {
  
  private:
    /// The dashed string denoting the domain
    DashedString ds;
    
  protected:
    /// Constructor for cloning \a x
    StringVarImp(Space& home, StringVarImp& x);
  
  public:
    /// \name Constructors and initialization
    //@{
    /// Initialize with block \f${0, 1, \dots, \alpha-1}^{(0, \lambda)}\f$ 
    /// where \f$\alpha = MAX\_ALPHABET\_SIZE, \lambda= MAX\_STRING\_LENGTH\f$
    StringVarImp(Space& home);
    /**
     * \brief Initialize with block b
     */
    StringVarImp(Space& home, const Block& b);
    /**
     * \brief Initialize with dashed string d
     */
    StringVarImp(Space& home, const DashedString& d);
    
    //@}
    /// \name Value access
    //@{
    /// Return the minimum length for a string in the variable's domain
    int min_length(void) const;
    /// Return the maximum length for a string in the variable's domain
    int max_length(void) const;
    /// Returns the number of blocks of the domain
    int size(void) const;
    /// Returns a const reference to the domain
    const DashedString& dom(void) const;
    /// Return the value of this string variable, if assigned. Otherwise, an
    /// IllegalOperation exception is thrown.
    std::vector<int> val(void) const;
    //@}

    /// \name Domain tests
    //@{
    /// Test whether variable is assigned
    bool assigned(void) const;
    /// Test whether the domain is equatable with string \a w.
    bool check_equate(const std::vector<int>& w) const;
    /// Test whether the domain is equatable with block \a b.
    bool check_equate(const Block& b) const;
    /// Test whether the domain is equatable with dashed string \a x.
    bool check_equate(const DashedString& x) const;
    /// Consistency checks on the domain
    bool isOK(void) const;
    /// Returns the i-th block of the domain
    Block& operator[](int i);
    const Block& operator[](int i) const;
    //@}
    
    /// Normalize the domain
    void normalize(Space& home);
  
    void update(Space& home, const DashedString& d);
    
    /// \name Domain update by equation
    //@{
    /// Equates the domain with string \a w.
    ModEvent equate(Space& home, const std::vector<int>& w);
    /// Equates the domain with block \a b.
    ModEvent equate(Space& home, const Block& b);
    /// Equates with dashed string \a x.
    ModEvent equate(Space& home, const DashedString& x);
    //@}
    
    /// \name Domain update by cardinality refinement
    //@{
    /// Updates the minimum length of each string of the domain
    ModEvent min_length(Space& home, int l);
    /// Updates the maximum length of each string of the domain
    ModEvent max_length(Space& home, int u);
    //@}
    
    /// \name Dependencies
    //@{
    /**
     * \brief Subscribe propagator \a p with propagation condition \a pc to variable
     *
     * In case \a schedule is false, the propagator is just subscribed but
     * not scheduled for execution (this must be used when creating
     * subscriptions during propagation).
     */
    GECODE_STRING_EXPORT void subscribe(Space& home, Propagator& p, PropCond pc, bool schedule=true);
    /// Re-schedule propagator \a p with propagation condition \a pc
    GECODE_STRING_EXPORT void reschedule(Space& home, Propagator& p, PropCond pc);
    /** \brief Subscribe advisor \a a to variable
     *
     * The advisor \a a is only subscribed if \a assigned is false.
     *
     * If \a fail is true, the advisor \a a is also run when a variable
     * operation triggers failure. This feature is undocumented.
     *
     */
    GECODE_STRING_EXPORT void subscribe(Space& home, Advisor& a, bool fail);
    //@}
    
    /// \name Cloning
    //@{
    /// Return copy of this variable
    StringVarImp* copy(Space& home);
    //@}
  };
  
  /**
   * \brief Print string var. imp. \a x
   * \relates Gecode::StringVar
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringVarImp& x);
  
}}
#include <gecode/string/var-imp/string.hpp>

// STATISTICS: string-var

