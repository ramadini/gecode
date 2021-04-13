#include <memory>

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
    /// \name Constructors and initialization
    //@{
    /// Creates the CharSet [0, String::Limits::MAX_ALPHABET_SIZE)
    CharSet(Space& home);
    /// Creates the CharSet {a}. Throws OutOfLimits if \f$ a < 0 \vee a \geq MAX\_ALPHABET\_SIZE \f$
    CharSet(Space& home, int a);
    /// Creates the CharSet [a, b]. The following exceptions might be thrown:
    /// - VariableEmptyDomain, if a > b.
    /// - OutOfLimits, if \f$ a < 0 \vee b \geq MAX\_ALPHABET\_SIZE \f$
    CharSet(Space& home, int a, int b);
    /// Creates the CharSet corresponding to set S. The following exceptions might be thrown:
    /// - VariableEmptyDomain, if S is empty.
    /// - OutOfLimits, if \f$ S \not\subseteq [0, String::Limits::MAX_ALPHABET_SIZE) \f$
    CharSet(Space& home, const IntSet& S);
  
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
    std::unique_ptr<CharSet> S;
    // FIXME: To save space, fixed blocks of the form {a}^(n,n) are encoded with
    // S = NULL; l = a ; u = n.
    
  public:
    /// \name Constructors and initialization
    //@{
    /// Creates the null block \f$ \{\}^(0,0) \$
    Block(void);
    /// Creates block \f$ {0, \dots, {MAX\_ALPHABET\_SIZE}-1}^{(0,{MAX\_STRING\_LENGTH})} \$
    Block(Space& home);
    /// Creates block \f$ S^{(0,{MAX\_STRING\_LENGTH})} \$
    Block(Space& home, const CharSet& S);
    /// Creates fixed block \f$ {a}^{1,1} \$
    /// Throws OutOfLimits exception if \f$a < 0 \vee a \geq MAX\_ALPHABET\_LENGTH\f$
    Block(Space& home, int a);
    /// Creates fixed block \f$ {a}^{n,n} \$
    /// Throws OutOfLimits exception if 
    /// \f$a < 0 \vee a \geq MAX\_ALPHABET\_LENGTH \vee n < 0 \vee n > MAX\_STRING\_LENGTH \f$
    Block(Space& home, int a, int n);    
    /// Creates block \f$ S^{l,u} \$
    /// The following exceptions might be thrown:
    /// - VariableEmptyDomain, if l > u.
    /// - OutOfLimits, if \f$ S \not\subseteq [0, MAX\_ALPHABET\_SIZE) \vee  l < 0 \vee u > MAX\_STRING\_LENGTH\f$
    Block(Space& home, const CharSet& S, int l, int u);
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
    /// Test whether this block contains block \a b, i.e. base() contains b.base() 
    /// and lb() is less or equal than b.lb() and ub() is greater or equal than b.ub()
    bool contains(const Block& b) const;
    //@}
    
    /// \name Update operations
    //@{
    /// Updates the current lower bound with l. If lb (resp. ub) is the current 
    /// lower (resp. upper) bound, the following exceptions might be thrown:
    /// - IllegalOperation, if l < lb
    /// - VariableEmptyDomain, if l > ub.
    void lb(int l);
    /// Updates the current upper bound with u. If lb (resp. ub) is the current 
    /// lower (resp. upper) bound, the following exceptions might be thrown:
    /// - IllegalOperation, if u > ub
    /// - VariableEmptyDomain, if u < lb.
    void ub(int u);
    /// Exclude all elements not in the set represented by \a i from the base
    /// A VariableEmptyDomain exception is raised if the base becomes empty but 
    /// the lower bound is greater than zero.
    template<class I> bool inters(Space& home, I& i);
    /// Exclude all elements in the set represented by \a i from the base
    /// A VariableEmptyDomain exception is raised if the base becomes empty but 
    /// the lower bound is greater than zero.
    template<class I> bool exclude(Space& home, I& i);
    //@}
    
    /// \name Cloning
    //@{
    /// Update this block to be the null block
    void nullify();
    /// Update this block to be a clone of \a b
    void update(Space& home, const Block& b);
    //@}
    
    /// Check whether the block is normalized and internal invariants hold
    bool isOK(void) const;
    /// Prints the block \a b
    friend std::ostream& operator<<(std::ostream& os, const Block& b);
    
  private:
    Block(const Block&);
    const Block& operator=(const Block&);
    
  };

}}

namespace Gecode { namespace String {
  
  /**
   * \brief Dashed string modelling the domain of a string variable.
   *
   * \ingroup TaskModelStringBranch
   */  
  class GECODE_STRING_EXPORT DashedString : 
  public Gecode::Support::DynamicArray<Block, Space> {
  
  private:
    // Sum of the lower bounds of each block of the dashed string
    int lb;
    // Sum of the upper bounds of each block of the dashed string
    int ub;
  
  public:
    /// \name Constructors and initialization
    //@{
    /// Creates a dashed string consisting of one block \f$ {0, \dots, {MAX\_ALPHABET\_SIZE}-1}^{(0,{MAX\_STRING\_LENGTH})} \$
    DashedString(Space& home);
    /// Creates a dashed string consisting of one \a block \$
    DashedString(Space& home, const Block& block);
    /// Creates a normalized dashed string from \a blocks
    DashedString(Space& home, const Block blocks[]);
    //FIXME: std::initializer_list?
    
    /// \name Dashed string access
    //@{
    /// Returns the minimum length for a concrete string denoted by the dashed string
    int min_length(void) const;
    /// Returns the maximum length for a concrete string denoted by the dashed string
    int max_length(void) const;
    /// Returns the number of blocks of the dashed string
    int size(void) const;
    /// Returns the natural logarithm of the dimension of this dashed string
    double logdim(void) const;
    //@}
  
    /// \name Dashed string tests
    //@{
    /// Test whether the dashed string is null
    bool isNull(void) const;
    /// Test whether the dashed string is fixed
    bool isFixed(void) const;
    /// Test whether the i-th block of this dashed string contains the i-th block of \d x 
    /// for \f$ i=1,\dots, size()\$ and the j-th block of this dashed string is null
    /// for \f$ j=b.size()+1,\dots, size()\$
    bool contains(void) const;
    //@}
    
    /// \name Cloning
    //@{
    /// Update this dashed string to be the null block.
    void nullify();
    /// Update this dashed string to be a clone of \a x
    void update(Space& home, const Block& x);
    //@}
    
    /// Normalize the dashed string
    bool normalize(void) const;
    /// Check whether the dashed string is normalized and internal invariants hold
    bool isOK(void) const;
    /// Prints the dashed string \a x
    friend std::ostream& operator<<(std::ostream& os, const DashedString& x);
    
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

}}


namespace Gecode { namespace String {

  using namespace Limits;

  forceinline
  CharSet::CharSet(Space& home) {
    Set::LUBndSet(home, 0, MAX_ALPHABET_SIZE-1);
  }

  forceinline
  CharSet::CharSet(Space& home, int a) {
    Limits::check_alphabet(a, "CharSet::CharSet");
    Set::LUBndSet(home, a, a);
  }

  forceinline
  CharSet::CharSet(Space& home, int a, int b) {
    Limits::check_alphabet(a, b, "CharSet::CharSet");
    Set::LUBndSet(home, a, b);
  }

  forceinline
  CharSet::CharSet(Space& home, const IntSet& S) {
    Limits::check_alphabet(S, "CharSet::CharSet");
    Set::LUBndSet(home, S);
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


namespace Gecode { namespace String {

  using namespace Limits;

  forceinline Block::Block() 
  : l(0), u(0), S() {};
  
  forceinline Block::Block(Space& home) 
  : l(0), u(MAX_STRING_LENGTH), S(new CharSet(home, 0, MAX_ALPHABET_SIZE-1)) {};
  
  forceinline Block::Block(Space& home, const CharSet& s)
  : l(0), u(MAX_STRING_LENGTH), S() {
    S->update(home, s);
  };
  
  forceinline Block::Block(Space& home, int a)
  : l(1), u(1), S(new CharSet(home, a)) {}
  
  forceinline Block::Block(Space& home, int a, int n)
  : l(1), u(1), S(new CharSet(home, a)) {
    Limits::check_length(n, "Block::Block");
  }
  
  forceinline Block::Block(Space& home, const CharSet& s, int a, int b)
  : l(a), u(b), S() {
    check_length(a, b, "Block::Block");
    S->update(home, s);
  }

//    
//    /// \name Block access
//    //@{
//    /// Returns the lower bound of the block
//    int lb(void) const;
//    /// Returns the upper bound of the block
//    int ub(void) const;
//    /// Returns the base of the block
//    const CharSet& base(void) const;
//    /// Returns the natural logarithm of the dimension of the block.
//    double logdim(void) const;
//    //@}
//    
//    /// \name Block tests
//    //@{
//    /// Test whether the block is null
//    bool isNull(void) const;
//    /// Test whether the block is fixed
//    bool isFixed(void) const;
//    /// Test whether the base of this block is disjoint with the base of \a b
//    bool disj(const Block& b) const;
//    /// Test whether the base of this block is disjoint with \a s
//    bool disj(const CharSet& s) const;
//    /// Test whether this block contains block \a b, i.e. base() contains b.base() 
//    /// and lb() is less or equal than b.lb() and ub() is greater or equal than b.ub()
//    bool contains(const Block& b) const;
//    //@}
//    
//    /// \name Update operations
//    //@{
//    /// Updates the current lower bound with l. If lb (resp. ub) is the current 
//    /// lower (resp. upper) bound, the following exceptions might be thrown:
//    /// - IllegalOperation, if l < lb
//    /// - VariableEmptyDomain, if l > ub.
//    void lb(int l);
//    /// Updates the current upper bound with u. If lb (resp. ub) is the current 
//    /// lower (resp. upper) bound, the following exceptions might be thrown:
//    /// - IllegalOperation, if u > ub
//    /// - VariableEmptyDomain, if u < lb.
//    void ub(int u);
//    /// Exclude all elements not in the set represented by \a i from the base
//    /// A VariableEmptyDomain exception is raised if the base becomes empty but 
//    /// the lower bound is greater than zero.
//    template<class I> bool inters(Space& home, I& i);
//    /// Exclude all elements in the set represented by \a i from the base
//    /// A VariableEmptyDomain exception is raised if the base becomes empty but 
//    /// the lower bound is greater than zero.
//    template<class I> bool exclude(Space& home, I& i);
//    //@}
//    
//    /// \name Cloning
//    //@{
//    /// Update this block to be the null block
//    void nullify();
//    /// Update this block to be a clone of \a b
//    void update(Space& home, const Block& b);
//    //@}
//    
//    /// Check whether the block is normalized and internal invariants hold
//    bool isOK(void) const;
//    /// Prints the block \a b
//    friend std::ostream& operator<<(std::ostream& os, const Block& b);

}}


namespace Gecode { namespace String {

}}





