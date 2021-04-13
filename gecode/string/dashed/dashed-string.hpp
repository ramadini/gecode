#include <gecode/set.hh>

/*** Declarations ***/

namespace Gecode { namespace String {

  /**
   * \brief Set of integers contained in [0, String::Limits::MAX_ALPHABET_SIZE) 
   * abstracting a set of characters, used as base of dashed strings' blocks.
   *
   * \ingroup TaskModelStringBranch
   */
  class GECODE_STRING_EXPORT CharSet : public Gecode::Set::LUBndSet {
  public:    
    /// Prints set according to Limits::CHAR_ENCODING
    friend std::ostream& operator<<(std::ostream& os, const CharSet& set);
    
  };

}}

namespace Gecode { namespace String {
  
  /**
   * \brief Block of a dashed string modelling the domain of a string variable.
   *
   * \ingroup TaskModelStringBranch
   */
  class GECODE_STRING_EXPORT Block {
  
  private:
    // Cardinality bounds.
    int l, u;
    // Pointer to the base.
    CharSet* S;
    // FIXME: To save space, fixed blocks of the form {a}^(n,n) are encoded with
    // S = NULL; l = S = {} ? 0 : a ; u = n.
    
  public:
    /// \name Constructors and initialization
    //@{
    /// Creates the null block \f$ \{\}^(0,0) \$
    Block(void);
    /// Creates block \f$ {0, \dots, {MAX\_ALPHABET\_SIZE}-1}^{(0,{MAX\_STRING\_LENGTH})} \$
    Block(Space& home);
    /// Creates block \f$ S^{(0,{MAX\_STRING\_LENGTH})} \$
    Block(Space& home, CharSet S);
    /// Creates fixed block \f$ {a}^{1,1} \$
    /// Throws OutOfLimits exception if \f$a < 0 \vee a \geq MAX\_ALPHABET\_LENGTH\f$
    Block(Space& home, int a);
    /// Creates fixed block \f$ {a}^{n,n} \$
    /// Throws OutOfLimits exception if 
    /// \f$a < 0 \vee a \geq MAX\_ALPHABET\_LENGTH \vee n < 0 \vee n > MAX\_STRING\_LENGTH \f$
    Block(Space& home, int a, int n);    
    /// Creates block \f$ S^{l,u} \$
    /// The following exceptions might be thrown:
    /// - Gecode::String::VariableEmptyDomain, if l > u.
    /// - Gecode::String::OutOfLimits, if \f$ S \not\subseteq [0, MAX\_ALPHABET\_SIZE) \vee  l < 0 \vee u > MAX\_STRING\_LENGTH\f$
    Block(Space& home, CharSet S, int l, int u);
    //@}
    
    /// \name Block access
    //@{
    /// Returns the lower bound of the block
    int lb(void) const;
    /// Returns the upper bound of the block
    int ub(void) const;
    /// Returns the base of the block
    const CharSet& base(void) const;
    /// Returns the natural logarithm of the dimension of the block.
    double logdim(void) const;
    //@}
    
    /// \name Block tests
    //@{
    /// Test whether the block is null
    bool isNull(void) const;
    /// Test whether the block is fixed
    bool isFixed(void) const;
    /// Test whether the base of this block is disjoint with the base of \a b
    bool disj(const Block& b) const;
    /// Test whether the base of this block is disjoint with \a s
    bool disj(const CharSet& s) const;
    /// Test whther 
    //@}
    
    /// \name Update operations
    //@{
//    lb(l)
//    ub(u)
//    exclude
//    intersect
    
    //@}
    
    /// \name Cloning
    //@{
    /// Update this block to be the null block. 
    /// NOTE: no consistency checks are performed on this block.
    void nullify();
    /// Update this block to be a clone of \a b
    /// NOTE: no consistency checks are performed on this block.
    void update(Space& home, const Block& b);
    //@}
    
    /// Check whether the block is normalized and internal invariants hold
    bool isOK(void) const;
    /// Prints the block
    friend std::ostream& operator<<(std::ostream& os, const CharSet& set);
    
  private:
    Block(const Block&);
    const Block& operator=(const Block&);
    
  };

}}

namespace Gecode { namespace String {
  
  class GECODE_STRING_EXPORT DashedString : 
  public Gecode::Support::DynamicArray<Block, Space> {
  
  };

}}

/*** Definitions ***/

namespace Gecode { namespace String {

  /// Char encoding. FIXME: This better be an option. 
  const CharEncoding CHAR_ENCODING = UNSPEC;

  /** 
   * Convert \a x to its string representation according to the character
   * encoding. By default, CHAR_ENCODING=UNSPEC, i.e.:  //FIXME: Change this?
   * int2str(x) = char(x), if \f$32 \leq x \leq 126\f$ (printable character);
   * otherwise, int2str(x) = "<" + std::to_string(x) + ">" 
   */
  std::string int2str(int x);

  forceinline std::string 
  int2str_unspec(int x) {
    return x > 31 && x < 127 ? std::string(1, char(x)) 
                             : "<" + std::to_string(x) + ">";
  }

  forceinline std::string 
  int2str(int x) {
    switch (CHAR_ENCODING) {
      case UNSPEC:
      // TODO:
      case ASCII:
      case EASCII:
      case UTF_8:
      case UTF_16:
      case UTF_32:
      default:
        return int2str_unspec(x);
    }
    GECODE_NEVER;
  }

  forceinline std::ostream&
  operator<<(std::ostream& os, const CharSet& set) {
    Gecode::Set::BndSetRanges i(set);
    os << "{";
    while (i()) {
      std::string lb = int2str(i.min()), ub = int2str(i.max());
      os << lb;
      if (lb < ub)
        os << ".." << ub;
      ++i;
      if (i())
        os << ",";
    }
    return os << "}";
  }


}}
