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
     * \brief Initialize with block \f$ S^{(0, \lambda)} \f$ 
     * where \f$\lambda= MAX\_STRING\_LENGTH\f$
     *
     * The Gecode::String::OutOfLimits exception is thrown if S is not contained
     * in [0, MAX\_ALPHABET\_SIZE).
     */
    StringVarImp(Space& home, const IntSet& S);
    
    /**
     * \brief Initialize with block \f$ \Sigma^{(l,u)} \f$ 
     * where \f$\Sigma= [0, MAX\_ALPHABET\_SIZE)\f$
     *
     * The following exceptions might be thrown:
     *  - Gecode::String::VariableEmptyDomain, if l > u.
     *  - Gecode::String::OutOfLimits, if \f$l < 0 \vee u > MAX\_STRING\_LENGTH\f$
     */
    StringVarImp(Space& home, int l, int u);
    
    /**
     * \brief Initialize with block \f$ S^{(l,u)} \f$
     *
     * The following exceptions might be thrown:
     *  - Gecode::String::VariableEmptyDomain, if l > u.
     *  - Gecode::String::OutOfLimits, if \f$ S \not\subseteq [0, MAX\_ALPHABET\_SIZE) 
     *                                 \vee  l < 0 \vee u > MAX\_STRING\_LENGTH\f$
     */
    StringVarImp(Space& home, const IntSet& S, int l, int u);
    
    //@}
    /// \name Value access
    //@{
    /// Return the minimum length for a string in the variable's domain
    int lenMin(void) const;
    /// Return the maximum length for a string in the variable's domain
    int lenMax(void) const;
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
    /// 
    //@}
    
    /// \name Domain update by equation
    //@{
    /// Equates the domain with string \a w.
    ModEvent check_equate(Space& home, const std::vector<int>& w) const;
    /// Equates the domain with block \a b.
    ModEvent check_equate(Space& home, const Block& b) const;
    /// Equates with dashed string \a x.
    ModEvent check_equate(Space& home, const DashedString& x) const;
    //@}
    
    /// \name Domain update by cardinality refinement
    //@{
    /// Possibly update the lower bound of the blocks, knowing that the minimum 
    /// length for any string in the domain is \a l
    ModEvent lengthMin(Space& home, int l) const;
    /// Possibly update the upper bound of the blocks, knowing that the maximum 
    /// length for any string in the domain is \a u
    ModEvent lengthMax(Space& home, int u) const;
    /// Possibly update the cardinality of the blocks, knowing that the length 
    /// for any string in the domain is between \a l and \a u
    ModEvent lengthIn(Space& home, int l, int u) const;
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
  
  class StringView;
  
}}
#include <gecode/string/var-imp/string.hpp>

// STATISTICS: string-var

