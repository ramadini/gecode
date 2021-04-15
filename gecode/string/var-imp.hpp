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
    //@}

    class StringView;
  
  };
  
}}
#include <gecode/string/var-imp/string.hpp>
