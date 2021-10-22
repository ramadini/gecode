#include <memory>

/*** Declarations ***/

namespace Gecode { namespace String {

  /**
   * \brief Set of integers contained in [0, String::Limits::MAX_ALPHABET_SIZE) 
   * abstracting a set of characters, used as base of dashed strings' blocks.
   *
   * \ingroup TaskModelStringBranch
   */
  class GECODE_STRING_EXPORT CharSet : public Set::LUBndSet {
  
  // FIXME: One may define subclasses of CharSet for fixed-size charsets 
  // relying on bit-vectors representation.
  
  public:
    /// \name Constructors and initialization
    //@{
    /// Creates the empty CharSet
    CharSet(void);
    /// Creates the CharSet [0, String::Limits::MAX_ALPHABET_SIZE)
    CharSet(Space& home);
    CharSet(Space& home, const BndSet& S);
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
    //@}
    
    /// \name CharSet tests
    //@{
    /// Test whether the set is [0, MAX_ALPHABET_SIZE)
    bool isUniverse(void) const;
    /// Test whether the set is disjoint with \a S
    bool disjoint(const CharSet& S) const;
    /// Test whether the set contains \a S
    bool contains(const CharSet& S) const;
    /// Test whether the set is equal to \a S
    template <class T> bool equals(const T& S) const;
    /// Intersect this set with \a S
    template <class T> void intersect(Space& home, const T& S);
    void intersect(Space& home, int i, int j);
    //@}
    
    /// Prints \a set according to Limits::CHAR_ENCODING
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
    // Base, if the block is unknown (otherwise S = {}).
    CharSet S;
   
    // FIXME: To save space, each fixed blocks {a}^(n,n) must be encoded with
    // (S={},l=a,u=n). The null block is (S={},l=0,u=0). For each block b
    // the invariant b.isFixed() <=> b.S == {} must hold.
    // Each non-const operation on a block must leave it in a consistent and
    // normalized state.
    // Once constructed, a Block can only shrink unless updated with cloning 
    // methods nullify, update or updateCard.
    
  public:
    /// \name Constructors and initialization
    //@{
    /// Creates the null block \f$ \{\}^(0,0) \$
    Block(void);
    /// Creates fixed block \f$ {a}^{1,1} \$
    /// Throws OutOfLimits exception if \f$a < 0 \vee a \geq MAX\_ALPHABET\_LENGTH\f$
    Block(int a);
    /// Creates fixed block \f$ {a}^{n,n} \$
    /// Throws OutOfLimits exception if 
    /// \f$a < 0 \vee a \geq MAX\_ALPHABET\_LENGTH \vee n < 0 \vee n > MAX\_STRING\_LENGTH \f$
    Block(int a, int n);
    /// Creates block \f$ {0, \dots, {MAX\_ALPHABET\_SIZE}-1}^{(0,{MAX\_STRING\_LENGTH})} \$
    Block(Space& home);
    /// Creates block \f$ S^{(0,{MAX\_STRING\_LENGTH})} \$
    Block(Space& home, const CharSet& S);    
    /// Creates block \f$ S^{l,u} \$
    /// The following exceptions might be thrown:
    /// - VariableEmptyDomain, if l > u or S = {} and l > 0.
    /// - OutOfLimits, if \f$ S \not\subseteq [0, MAX\_ALPHABET\_SIZE) \vee  l < 0 \vee u > MAX\_STRING\_LENGTH\f$
    Block(Space& home, const CharSet& S, int l, int u);
    //@}
    
    /// \name Block access
    //@{
    /// Returns the lower bound of the block
    int lb(void) const;
    /// Returns the upper bound of the block
    int ub(void) const;
    /// Returns the size of the base of this block
    int baseSize(void) const;
    /// Returns the least element of the base of this block. 
    /// Throws IllegalOperation if the block is null
    int baseMin(void) const;
    /// Returns the greatest element of the base of this block.
    /// Throws IllegalOperation if the block is null
    int baseMax(void) const;
    /// Returns a range iterator to the base. Throws IllegalOperation if the block is fixed
    Set::BndSetRanges ranges(void) const;
    /// Returns the natural logarithm of the dimension of the block.    
    double logdim(void) const;
    /// Returns the concrete string denoted by the block, if the block is fixed.
    /// Otherwise, an IllegalOperation exception is thrown.
    std::vector<int> val(void) const;
    //@}
    
    /// \name Block tests
    //@{
    /// Test whether the block is null
    bool isNull(void) const;
    /// Test whether the block is fixed (i.e., it denotes a single string)
    bool isFixed(void) const;
    /// Test whether the block is {0, \dots, {MAX\_ALPHABET\_SIZE}-1}^{(0,{MAX\_STRING\_LENGTH})
    bool isUniverse(void) const;    
    /// Test whether the base of this block is disjoint with the base of \a b
    bool baseDisjoint(const Block& b) const;
    /// Test whether the base of this block is equal to the base of \a b
    bool baseEquals(const Block& b) const;
    bool baseEquals(const Set::BndSet& s) const;
    /// Test whether the base of this block contains c
    bool baseContains(int c) const;
    /// Test whether this block contains block \a b, i.e. base() contains b.base() 
    /// and lb() <= b.lb() and ub() >= b.ub()    
    bool contains(const Block& b) const;
    /// Test whether this block is equal to b
    bool equals(const Block& b) const;
    //@}
    
    /// \name Update operations
    //@{
    /// Updates the current lower bound with l. If lb (resp. ub) is the current 
    /// lower (resp. upper) bound, the following exceptions might be thrown:
    /// - IllegalOperation, if l < lb
    /// - VariableEmptyDomain, if l > ub.
    void lb(Space& home, int l);
    /// Updates the current upper bound with u. If lb (resp. ub) is the current 
    /// lower (resp. upper) bound, the following exceptions might be thrown:
    /// - IllegalOperation, if u > ub
    /// - VariableEmptyDomain, if u < lb.
    void ub(Space& home, int u);
    /// Exclude all elements not in \a S from the base
    /// A VariableEmptyDomain exception is raised if the base becomes empty but 
    /// the lower bound is greater than zero.
    void baseIntersect(Space& home, const Set::BndSet& S);
    /// Exclude all elements not in \a the base of b from this base
    /// A VariableEmptyDomain exception is raised if the base becomes empty but 
    /// the lower bound is greater than zero.
    void baseIntersect(Space& home, const Block& b);
    /// Exclude all elements of \a S from the base
    /// A VariableEmptyDomain exception is raised if the base becomes empty but 
    /// the lower bound is greater than zero.
    void baseExclude(Space& home, const Set::BndSet& S);
    void baseRemove(Space& home, int c);
    /// Include the elements of this base in S.
    void includeBaseIn(Space& home, Set::GLBndSet& S) const;
    //@}
    
    /// \name Cloning
    //@{
    /// Update this block to be the null block
    void nullify(Space& home);
    /// Update this block to be a clone of \a b
    void update(Space& home, const Block& b);
    /// Update the lower/upper bounds of the block. It throws:
    /// - OutOfLimits, if lb < 0 or ub > MAX_STRING_LENGTH
    /// - VariableEmptyDomain, if lb > ub or lb > 0 and the base is empty
    void updateCard(Space& home, int lb, int ub);
    //@}
    
    /// Check whether the block is normalized and internal invariants hold
    bool isOK(void) const;
    /// Prints the block \a b
    friend std::ostream& operator<<(std::ostream& os, const Block& b);
    
  private:
    Block(const Block&);
    const Block& operator=(const Block&);
    /// From fixed block S={a},l=u to consistent encoding with S=NULL and l=a
    void fix(Space& home);
    /// Dispose S and set it to NULL
    void nullifyBase(Space& home);
    
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
  // FIXME: Each block of the dashed string is always consistent and normalized,
  // but this does not imply that the whole dashed string is always normalized. 
  // This is to avoid too many resizing due to eager normalization.
  private:
    int lbs;
    long ubs;
  public:
    /// \name Constructors and initialization
    //@{
    /// Creates a dashed string consisting of one block 
    /// \f$ {0, \dots, {MAX\_ALPHABET\_SIZE}-1}^{(0,{MAX\_STRING\_LENGTH})} \$
    DashedString(Space& home);
    /// Creates a dashed string consisting of one \a block \$
    DashedString(Space& home, const Block& block);
    /// Creates a dashed string consisting of a copy of \a x \$
    /// Note: if \a x is not normalized, then this won't be normalized.
    DashedString(Space& home, const DashedString& x);\
    /// Creates a dashed string consisting of \a n > 0 null blocks
    /// NOTE: If n > 1, the dashed will be _not_ normalized
    DashedString(Space& home, int n);
    /// Creates a dashed string consisting of \a n > 0 copies of b
    /// NOTE: If n > 1, the dashed will be _not_ normalized
    DashedString(Space& home, int n, const Block& b);    
    /// Creates a normalized dashed string from array \a b of \n blocks
    /// The following exceptions might be thrown:
    /// - IllegalOperation, if \a blocks is empty
    /// - OutOfLimits, if the sum of lower bounds of blocks is bigger than
    ///                MAX_STRING_LENGTH
    DashedString(Space& home, const Block b[], int n);
    /// TODO
    DashedString(Space& home, const std::vector<int>& v);
    
    /// \name Dashed string access
    //@{
    /// Returns the sum of the lower bounds
    int lb_sum(void) const;
    /// Returns the sum of the upper bounds
    long ub_sum(void) const;    
    /// Returns the number of blocks of the dashed string
    int size(void) const;
    /// Returns the natural logarithm of the dimension of this dashed string
    double logdim(void) const;
    /// Returns the i-th block of the dashed string. Throws OutOfLimits if \a i < 0 or 
    /// \a i >= n, where n is the dashed string size
    const Block& operator[](int i) const;
    /// Returns the concrete string denoted by the dashed string, if it is fixed.
    /// Otherwise, an IllegalOperation exception is thrown.
    std::vector<int> val(void) const;
    //@}
  
    /// \name Dashed string tests
    //@{
    /// Test whether the dashed string is a null block
    bool isNull(void) const;
    /// Test whether the dashed string is fixed
    bool isFixed(void) const;
    /// Test whether the dashed string is a single block {0, \dots, {MAX\_ALPHABET\_SIZE}-1}^{(0,{MAX\_STRING\_LENGTH})
    bool isUniverse(void) const;
    /// Test whether the i-th block of this dashed string contains the i-th block of \d x 
    /// for \f$ i=1,\dots, size()\$ and the j-th block of this dashed string is nullable
    /// for \f$ j=b.size()+1,\dots, size()\$
    bool contains(const DashedString& x) const;
    bool contains_rev(const DashedString& x) const;    
    /// Test whether this dashed string is equal to x
    bool equals(const DashedString& x) const;
    /// Test whether this dashed string is equal to the reverse of x
    bool equals_rev(const DashedString& x) const;
    //@}
    
    /// \name Cloning and updating
    //@{
    /// Update this dashed string to be the null block.
    void nullify(Space& home);
    /// Update this dashed string to be a clone of \a x
    void update(Space& home, const DashedString& x);
    void update_rev(Space& home, const DashedString& x);
    /// Update this dashed string to be assigned to \a w
    void update(Space& home, const std::vector<int>& w);
    
    void update(Space& home, const DashedString& x, const DashedString& y);
    /// Possibly refine this dashed string given minimum length l.
    /// Pre-condition: lb_sum() <= l <= ub_sum()
    void min_length(Space& home, int l);
    /// Possibly refine this dashed string given maximum length u.
    /// Pre-condition: lb_sum() <= l <= ub_sum()
    void max_length(Space& home, int u);
    //@}
    
    void lbAt(Space& home, int idx, int l);
    void ubAt(Space& home, int idx, int u);
    void baseIntersectAt(Space& home, int idx, const Set::BndSet& S);
    void baseIntersectAt(Space& home, int idx, const Block& b);
    void baseExcludeAt(Space& home, int idx, const Set::BndSet& S);
    void baseRemoveAt(Space& home, int idx, int c);
    void nullifyAt(Space& home, int idx);
    void updateAt(Space& home, int idx, const Block& b);
    void updateCardAt(Space& home, int idx, int lb, int ub);
    
    /// Check whether the dashed string is normalized
    bool isNorm(void) const;
    /// Normalize the dashed string, assuming each block already consistent
    void normalize(Space& home);
    
    /// Splits x[i]=S^{k,k} into: {c}^{1,1} S^{k-1,k-1}, if a = 0;
    /// (S-{c})^{1,1} S^{k-1,k-1} otherwise, and possibly normalizes.
    void splitBlock(Space& home, int idx, int c, unsigned a);
    
    /// Prints the dashed string \a x
    friend std::ostream& operator<<(std::ostream& os, const DashedString& x);
    
    /// Check whether each block is consistent and lb_sum/ub_sum correspond to
    /// the sum of each lb/ub
    bool isOK(void) const;
  };

}}



/*** Definitions ***/


namespace Gecode { namespace String {

  /// Char encoding. FIXME: This better be an option. 
  const CharEncoding CHAR_ENCODING = UNSPEC;

  /** 
   * Convert \a x to its string representation according to the character
   * encoding. By default, CHAR_ENCODING=UNSPEC, i.e.:  //FIXME: Change this?
   * int2str(x) = char(x), if \f$32 \leq x \leq 126\f$ (non-whitespace, printable characters);
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
        return int2str_unspec(x);
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

  forceinline std::string 
  vec2str(const std::vector<int>& v) {
    std::string s = "";
    for (auto i : v)
      s += int2str(i);
    return s;
  }
  
  forceinline int
  str2int_unspec(char c) {
    return (int) c;
  }
  
  forceinline int
  str2int(const std::string& s) {
    switch (CHAR_ENCODING) {
      case UNSPEC:
        return str2int_unspec(s[0]);
      // TODO:
      case ASCII:
      case EASCII:
      case UTF_8:
      case UTF_16:
      case UTF_32:
      default:
        return str2int_unspec(s[0]);
    }
    GECODE_NEVER;
  }
  
  forceinline std::vector<int>
  str2vec(const std::string& s) {
    //FIXME: This assumes that each character of s maps to an integer. If an 
    // integer maps to more than one character we need to tokenize s.
    std::vector<int> v;
    for (auto c : s)
      v.push_back(str2int(std::string(1,c)));
    return v;
  }

}}

/*** CharSet ***/

namespace Gecode { namespace String {

  using namespace Limits;

  forceinline
  CharSet::CharSet() {
    Set::LUBndSet();
  }
  
  forceinline
  CharSet::CharSet(Space& home, const BndSet& S) : Set::LUBndSet() {
    update(home, S);
  }

  forceinline
  CharSet::CharSet(Space& home) : Set::LUBndSet(home, 0, MAX_ALPHABET_SIZE-1) {}

  forceinline
  CharSet::CharSet(Space& home, int a) : Set::LUBndSet(home, a, a) {
    Limits::check_alphabet(a, "CharSet::CharSet");
  }

  forceinline
  CharSet::CharSet(Space& home, int a, int b) : Set::LUBndSet(home, a, b) {
    Limits::check_alphabet(a, b, "CharSet::CharSet");    
  }

  forceinline
  CharSet::CharSet(Space& home, const IntSet& S) : Set::LUBndSet(home, S) {
    Limits::check_alphabet(S, "CharSet::CharSet");
  }
  
  forceinline bool
  CharSet::disjoint(const CharSet& x) const {
    if (empty() || x.empty() ||  max() < x.min() || min() > x.max())
      return true;
    if (min() == x.min() || max() == x.max())
      return false;
    Set::BndSetRanges i1(*this), i2(x);
    while (i1() && i2()) {
      if ((i1.min() <= i2.min() && i2.min() <= i1.max())
      ||  (i1.min() <= i2.max() && i2.max() <= i1.max())
      ||  (i2.min() <= i1.min() && i1.min() <= i2.max())
      ||  (i2.min() <= i1.max() && i1.max() <= i2.max()))
        return false;
      if (i1.max() > i2.max())
        ++i2;
      else
        ++i1;
    }
    return true;
  }
  
  forceinline bool
  CharSet::isUniverse() const {
    return fst() == lst() && min() == 0 && max() == MAX_ALPHABET_SIZE-1;
  }
  
  forceinline bool
  CharSet::contains(const CharSet& x) const {
    if (x.empty() || fst() == x.fst())
      return true;
    if (_size < x._size)
      return false;
    Set::BndSetRanges i1(*this), i2(x);
    while (i1() && i2())
      if (i1.min() > i2.min() || i1.max() < i2.max())
        ++i1;
      else
        ++i2;
    return !i2();
  }
  
  template <class T>
  forceinline bool
  CharSet::equals(const T& s) const {
    if (_size != s.size())
      return false;
    if (min() != s.min() || max() != s.max())
      return false;
    Set::BndSetRanges i1(*this);
    Set::BndSetRanges i2(s);    
    while (i1() && i2()) {
      if (i1.min() != i2.min() || i1.max() != i2.max())
        return false;
      ++i1;
      ++i2;
    }
    return !i1() && !i2();
  }

  template <class T>
  forceinline void
  CharSet::intersect(Space& home, const T& s) {
    Set::BndSetRanges i(s);
    intersectI(home, i);
  }
  forceinline void
  CharSet::intersect(Space& home, int i, int j) {
    Set::LUBndSet::intersect(home, i, j);
  }
  
  forceinline std::ostream&
  operator<<(std::ostream& os, const CharSet& set) {
    Set::BndSetRanges i(set);
    os << "{";
    while (i()) {
      os << int2str(i.min());
      if (i.min() < i.max())
        os << ".." << int2str(i.max());
      ++i;
      if (i())
        os << ",";
    }
    return os << "}";
  }

}}


/*** Block ***/

namespace Gecode { namespace String {

  using namespace Limits;

  forceinline Block::Block() 
  : l(0), u(0), S() {};
  
  forceinline Block::Block(int a)
  : l(a), u(1), S() { Limits::check_alphabet(a, "Block::Block"); };
  
  forceinline Block::Block(int a, int n)
  : l(a), u(n), S() { Limits::check_length(n, "Block::Block"); };
  
  forceinline Block::Block(Space& home) 
  : l(0), u(MAX_STRING_LENGTH), S(home) {};
  
  forceinline Block::Block(Space& home, const CharSet& s) 
  : l(0), u(MAX_STRING_LENGTH), S() {
    S.update(home, s); 
  };
  
  forceinline Block::Block(Space& home, const CharSet& s, int lb, int ub)
  : l(lb), u(ub), S() {
    check_length(lb, ub, "Block::Block");
    switch (s.size()) {
      case 0:
        if (lb > 0)
          throw VariableEmptyDomain("Block::Block");
        // Null block.
        l = u = 0;
        return;
      case 1:
        if (lb == ub)
          // Fixed block s^n with n=lb=ub.
          l = s.min();
        else
          S.update(home, s);
        return;
      default:
        // General case.        
        if (ub > 0)
          S.update(home, s);
    }
  }

  forceinline int Block::lb() const { return isFixed() ? u : l; }
  forceinline int Block::ub() const { return u; }
  
  forceinline bool Block::isNull()  const { return u == 0; }
  forceinline bool Block::isFixed() const { return S.empty(); }
  
  forceinline bool Block::isUniverse() const {
    return l == 0 && u == MAX_STRING_LENGTH && S.isUniverse();
  }
  
  forceinline double
  Block::logdim() const {
    if (isFixed())
      return 0;
    if (S.size() == 1)
      return log(u - l + 1);
    double s = S.size();
    return u * log(s) + log(1.0 - std::pow(s, l-u-1)) - log(1 - 1/s);
  }
  
  forceinline int
  Block::baseSize(void) const {
    return isFixed() ? u > 0 : S.size();
  }
  
  forceinline int
  Block::baseMin(void) const {
    if (isNull())
      throw IllegalOperation("Block::baseMin");
    return isFixed() ? l: S.min();
  }
  
  forceinline int
  Block::baseMax(void) const {
    if (isNull())
      throw IllegalOperation("Block::baseMax");
    return isFixed() ? l: S.max();
  }
  
  forceinline Set::BndSetRanges
  Block::ranges() const {
    if (isFixed())
      throw IllegalOperation("Block::ranges");
    return Set::BndSetRanges(S);
  }
  
  forceinline bool
  Block::baseDisjoint(const Block& b) const {
    if (isFixed())
      return b.isFixed() ? l != b.l : !b.S.in(l);
    if (b.isFixed())
      return !S.in(b.l);
    return S.disjoint(b.S);
  }
  
  forceinline bool
  Block::baseEquals(const Block& b) const {
    if (isFixed())
      return b.isFixed() ? l == b.l : b.S.size() == 1 && b.S.min() == l;
    if (b.isFixed())
      return S.size() == 1 && S.min() == b.l;
    return S.equals(b.S);
  }
  forceinline bool
  Block::baseEquals(const Set::BndSet& s) const {
    if (isFixed())
      return s.size() == 1 && s.min() == l;
    return S.equals(s);
  }
  
  forceinline bool
  Block::baseContains(int c) const {
    return isFixed() ? u > 0 && l == c : S.in(c);
  }
  
  forceinline bool
  Block::contains(const Block& b) const {
    if (l > b.l || u < b.u)
      return false;
    if (isFixed())
      return b.isFixed() && l == b.l && u == b.u;
    return b.isFixed() ? S.in(b.l) : S.contains(b.S);
  }
  
  forceinline bool
  Block::equals(const Block& b) const {
    if (this == &b)
      return true;
    if (l != b.l || u != b.u)
      return false;
    return baseEquals(b);
  }
  
  forceinline std::vector<int>
  Block::val() const {
    if (!isFixed())
      throw IllegalOperation("Block::val");
    return std::vector<int>(u, l);
  }
  
  forceinline void
  Block::fix(Space& home) {
    assert(l == u && S.size() <= 1);
    l = S.empty() ? 0 : S.min();
    nullifyBase(home);
  }
  
  forceinline void
  Block::lb(Space& home, int x) {
    if (isFixed()) {
      if (x != u)
        throw VariableEmptyDomain("Block::ub");
      return;
    }
    if (x > u)
      throw VariableEmptyDomain("Block::lb");
    if (x < l)
      throw IllegalOperation("Block::lb");
    l = x;
    if (l == u && S.size() == 1)
      fix(home);
    assert(isOK());
  }
  
  forceinline void
  Block::ub(Space& home, int x) {
    if (isFixed()) {
      if (x != u)
        throw VariableEmptyDomain("Block::ub");
      return;
    }
    if (x > u)
      throw IllegalOperation("Block::ub");
    if (x < l)
      throw VariableEmptyDomain("Block::ub");
    u = x;
    if (x == l && S.size() <= 1)
      fix(home);
    assert(isOK());
  }
  
  forceinline void
  Block::baseIntersect(Space& home, const Set::BndSet& s) {
    if (isNull())
      return;
    if (isFixed()) {
      if (!s.in(l))
        throw VariableEmptyDomain("Block::baseIntersect"); 
      return;
    }
    Set::BndSetRanges i(s);
    S.intersectI(home, i);
    if (S.empty()) {
      if (l > 0)
        throw VariableEmptyDomain("Block::baseIntersect");
      nullify(home);
    }
    else if (l == u && S.size() == 1)
      fix(home);
    assert(isOK());
  }
  
  forceinline void
  Block::baseIntersect(Space& home, const Block& b) {
    if (isNull())
      return;
    if (isFixed()) {
      if ((b.isFixed() && l != b.l) || (!isFixed() && !b.S.in(l)))
        throw VariableEmptyDomain("Block::baseIntersect");
      return;
    }
    if (b.isFixed()) {
      if (S.in(b.l)) {
        if (l == u) {
          nullifyBase(home);
          l = b.l;
        }
        else
          S.intersect(home, b.l, b.l);
      }
      else {
        if (l > 0)
          throw VariableEmptyDomain("Block::baseIntersect");
        nullify(home);
      }
      assert(isOK());
      return;
    }
    Set::BndSetRanges i(b.S);    
    S.intersectI(home, i);
    if (S.empty()) {
      if (l > 0)
        throw VariableEmptyDomain("Block::baseIntersect");
      nullify(home);
    }
    else if (l == u && S.size() == 1)
      fix(home);
    assert(isOK());
  }
  
  forceinline void
  Block::baseExclude(Space& home, const Set::BndSet& s) {
    if (isNull())
      return;
    if (isFixed()) {
      if (s.in(l))
        throw VariableEmptyDomain("Block::baseExclude"); 
      return;
    }
    Set::BndSetRanges i(s);
    S.excludeI(home, i);
    if (S.empty() && l > 0)
      throw VariableEmptyDomain("Block::baseExclude");
    if (l == u && S.size() == 1)
      fix(home);
    assert(isOK());
  }
  
  forceinline void
  Block::baseRemove(Space& home, int c) {
    if (isFixed()|| (l > 0 && S.size() == 1))
      throw VariableEmptyDomain("Block::baseRemove"); 
    Set::SetDelta d;
    S.exclude(home, c, c, d);
    if (l == u && S.size() == 1)
      fix(home);
    assert(isOK());
  }
  
  forceinline void
  Block::includeBaseIn(Space& home, Set::GLBndSet& s) const {
    switch (baseSize()) {
    case 0:
      return;
    case 1: {
      int m = baseMin();
      Set::SetDelta d;
      s.include(home, m, m, d);
      return;
    }
    default:
      Set::BndSetRanges r(ranges());
      s.includeI(home, r);
    }
  }
  
  forceinline void
  Block::nullifyBase(Space& home) {
    if (!S.empty())
      S.excludeAll(home);
    assert(isOK());
  }
  
  forceinline void
  Block::nullify(Space& home) {
    l = u = 0; 
    nullifyBase(home);
  }
  
  forceinline void
  Block::update(Space& home, const Block& b) {
    l = b.l;
    u = b.u;
    if (b.isFixed())
      nullifyBase(home);
    else
      S.update(home, b.S);
    assert(isOK());
  }
  
  forceinline void
  Block::updateCard(Space& home, int lb, int ub) {
    assert (lb >= 0 && ub >= 0);
    check_length(lb, ub, "Block::updateCard");
    if (isFixed()) {
      if (lb < ub) {
        S.update(home, CharSet(home,l));
        l = lb;
      }
      u = ub;
      return;
    }
    if (l == lb && u == ub)
      return;   
    if (lb > 0 && S.empty())
      throw VariableEmptyDomain("Block::updateCard");
    if (ub == 0 || (lb == ub && l < u && S.size() <= 1)) {
      // Block become fixed.
      l = ub > 0 && !S.empty() ? S.min() : 0;
      nullifyBase(home);
    }
    else
      l = lb;
    u = ub;
    assert(isOK());
  }
  
  forceinline bool
  Block::isOK() const {
    if (isFixed()) {
      check_alphabet(l, "Block::isOK");
      check_length(u, "Block::isOK");
      return true;
    }
    check_length(l, u, "Block::isOK");
    check_alphabet(S.min(), S.max(), "Block::isOK");
    return u > 0 && (S.size() > 1 || (S.size() == 1 && l < u));
  }

  forceinline std::ostream&
  operator<<(std::ostream& os, const Block& b) {
    if (b.isFixed())
      os << "{" + (b.u > 0 ? int2str(b.l) : "") + "}" << "^(" << b.u << ",";
    else
      os << b.S << "^(" << b.l << ",";
    os << b.u << ")";
    return os;
  }

}}

/*** DashedString ***/

namespace Gecode { namespace String {

  using namespace Limits;

  forceinline
  DashedString::DashedString(Space& home) 
  : DynamicArray(home, 1), lbs(0), ubs(MAX_STRING_LENGTH) {
    x->update(home, Block(home));
    assert (isOK() && isNorm());
  }
  
  forceinline
  DashedString::DashedString(Space& home, const Block& block) 
  : DynamicArray(home, 1), lbs(block.lb()), ubs(block.ub()) {
    x->update(home, block);
    assert (isOK() && isNorm());
  }
  
  forceinline
  DashedString::DashedString(Space& home, const DashedString& d) 
  : DynamicArray(home, d.size()), lbs(d.lbs), ubs(d.ubs) {
    update(home, d);
    assert (isOK() && (isNorm() || !d.isNorm()));
  }
  
  forceinline
  DashedString::DashedString(Space& home, int n) 
  : DynamicArray(home, n), lbs(0), ubs(0) {
    assert (isOK() && (n == 1 || !isNorm()));
  }
  
  forceinline
  DashedString::DashedString(Space& home, int n, const Block& b) 
  : DynamicArray(home, n), lbs(n*b.lb()), ubs(n*b.ub()) {
    for (int i = 0; i < n; ++i)
      (x+i)->update(home, b);
    assert (isOK() && (n == 1 || !isNorm()));
  }
  
  forceinline
  DashedString::DashedString(Space& home, const Block blocks[], int n)
  : DynamicArray(home, n) {
    bool norm = blocks[0].isNull();
    x->update(home, blocks[0]);
    lbs = blocks[0].lb(), ubs = blocks[0].ub();
    for (int i = 1; i < n; ++i) {
      (x+i)->update(home, blocks[i]);
      lbs += x[i].lb();
      ubs += x[i].ub();
      // NOTE: The sum of the blocks' bounds might overflow.
      if (lbs > MAX_STRING_LENGTH || lbs + x[i].ub() < lbs) {
        for (int j = 0; j <= i; ++j)
          x[j].nullify(home);
        a.free(x, n);
        throw OutOfLimits("DashedString::DashedString");
      }      
      norm |= x[i].isNull() || x[i].baseEquals(x[i-1]);
    }
    if (ubs > Limits::MAX_STRING_LENGTH) {
      // Possibly refining upper bounds.
      for (int i = 0; i < n; ++i) {
        Block& bi = x[i];
        int ll = lbs - bi.lb(), uu = ubounded_sum(bi.ub(), ll);
        if (uu == Limits::MAX_STRING_LENGTH) {
          long d = bi.ub() + ll - Limits::MAX_STRING_LENGTH;
          assert (d <= MAX_STRING_LENGTH);
          bi.ub(home, bi.ub() - d);
          ubs -= d;
          if (bi.isNull())
            norm = true;
        }
      }
    }
    if (norm)
      normalize(home);
//    std::cerr << *this << '\n';
    assert(isNorm());
    assert(isOK());
  }
  
  forceinline
  DashedString::DashedString(Space& home, const std::vector<int>& v)
  : DynamicArray(home) {
    assert (n > 0);
    update(home, v);
  }

  forceinline int DashedString::lb_sum() const {
    return lbs;  
  }
  forceinline long DashedString::ub_sum() const {
    return ubs;  
  }
  forceinline int DashedString::size() const { return n; }
 
  forceinline const Block&
  DashedString::operator[](int i) const {
    assert (i >= 0 && i < n);
    return *(x + i);
  }
  
  forceinline double
  DashedString::logdim() const {
    double d = 0.0;
    for (int i = 0; i < n; ++i) {
      d += x[i].logdim();
      if (std::isinf(d))
        return d;
    }
    return d;
  }
  
  forceinline bool
  DashedString::isNull() const {
    return ubs == 0;
  }
  
  forceinline bool
  DashedString::isUniverse() const {
    return n == 1 && x[0].isUniverse();
  }
  
  forceinline bool
  DashedString::isFixed() const {
    if (lbs != ubs)
      return false;
    for (int i = 0; i < n; ++i)
      if (!x[i].isFixed())
        return false;
    return true;
  }
  
  forceinline bool
  DashedString::contains(const DashedString& d) const {
    if (lbs > d.lbs || ubs < d.ubs)
      return false;
    if (n == 1) {      
      for (int i = 0; i < d.n; ++i)
        if (!x[0].contains(d[i]))
          return false;
      return true;
    }
    if (n < d.n)
      return false;
    for (int i = 0; i < d.n; ++i)
      if (!x[i].contains(d.x[i]))
        return false;
    for (int i = d.n; i < n; ++i)
      if (x[i].lb() > 0)
        return false;
    return true;
  }
  
  forceinline bool
  DashedString::contains_rev(const DashedString& d) const {
    if (lbs > d.lbs || ubs < d.ubs)
      return false;
    if (n == 1) { 
      for (int i = 0; i < d.n; ++i)
        if (!x[0].contains(d[i]))
          return false;
      return true;
    }
    if (n < d.n)
      return false;
    for (int i = 0; i < d.n; ++i) {
      if (!x[i].contains(d.x[d.n-i-1]))
        return false;
    }
    for (int i = n-d.n-1; i >= 0; --i)
      if (d.x[i].lb() > 0)
        return false;
    return true;
  }
  
  forceinline bool
  DashedString::equals(const DashedString& d) const {
    if (this == &d)
      return true;
    if (n != d.n || lbs != d.lbs || ubs != d.ubs)
      return false;
    for (int i = 0; i < n; ++i)
      if (!x[i].equals(d[i]))
        return false;
    return true;
  }
  
  forceinline bool
  DashedString::equals_rev(const DashedString& d) const {
    if (this == &d)
      return true;
    if (n != d.n || lbs != d.lbs || ubs != d.ubs)
      return false;
    for (int i = 0; i < n; ++i)
      if (!x[i].equals(d[n-i-1]))
        return false;
    return true;
  }
  
  forceinline std::vector<int>
  DashedString::val() const {
    if (!isFixed())
      throw IllegalOperation("DashedString::val");
    std::vector<int> v;
    for (int i = 0; i < n; ++i) {
      std::vector<int> val_i = x[i].val();
      v.insert(v.end(), val_i.begin(), val_i.end());
    }
    return v;
  }
  
  forceinline void
  DashedString::nullify(Space& home) {
    if (n > 1) {
      a.free(x+1, n-1);
      n = 1;
    }
    x[0].nullify(home);
    lbs = ubs = 0;
    assert (isOK());
    assert (isNorm());
  }
  
  forceinline void 
  DashedString::lbAt(Space& home, int idx, int l) {
    lbs += l - x[idx].lb();
    (x + idx)->lb(home, l);
  }
  forceinline void
  DashedString::ubAt(Space& home, int idx, int u) {
    ubs += u - x[idx].ub();
    (x + idx)->ub(home, u);
  }
  forceinline void 
  DashedString::baseIntersectAt(Space& home, int idx, const Set::BndSet& S) {
    int u = x[idx].ub();
    (x + idx)->baseIntersect(home, S);
    if (x[idx].isNull())
      ubs -= u;
  }
  forceinline void 
  DashedString::baseIntersectAt(Space& home, int idx, const Block& b) {
    int u = x[idx].ub();
    (x + idx)->baseIntersect(home, b);
    if (x[idx].isNull())
      ubs -= u;
  }
  forceinline void 
  DashedString::baseExcludeAt(Space& home, int idx, const Set::BndSet& S) {
    int u = x[idx].ub();
    (x + idx)->baseExclude(home, S);
    if (x[idx].isNull())
      ubs -= u;
  }
  forceinline void 
  DashedString::baseRemoveAt(Space& home, int idx, int c) {
    int u = x[idx].ub();
    (x + idx)->baseRemove(home, c);
    if (x[idx].isNull())
      ubs -= u;
  }
  forceinline void 
  DashedString::nullifyAt(Space& home, int idx) {
    lbs -= x[idx].lb();
    ubs -= x[idx].ub();
    (x + idx)->nullify(home);
  }
  forceinline void 
  DashedString::updateAt(Space& home, int idx, const Block& b) {
    lbs += b.lb() - x[idx].lb();
    ubs += b.ub() - x[idx].ub();
    (x + idx)->update(home, b);
  }
  forceinline void 
  DashedString::updateCardAt(Space& home, int idx, int lb, int ub) {    
    lbs += lb - x[idx].lb();
    ubs += ub - x[idx].ub();
    (x + idx)->updateCard(home, lb, ub);
  }
  
  forceinline void 
  DashedString::min_length(Space& home, int l) {
    assert (lbs <= l && l <= ubs);
    for (int i = 0; i < n; ++i) {
      Block& bi = x[i];
      int li = bi.lb();
      long ui = bi.ub(), min_i = std::min(ui, l - ubs + ui);
      if (li < min_i) {
        bi.lb(home, min_i);
        lbs += min_i - li;  
      }
    }
    assert (isOK());
  }
  
  forceinline void 
  DashedString::max_length(Space& home, int u) {
    assert (lbs <= u && u <= ubs);
    bool norm = false;
    for (int i = 0; i < n; ++i) {
      Block& bi = x[i];
      int li = bi.lb(), ui = bi.ub(), max_i = std::max(li, u - lbs + li);
      if (ui > max_i) {
        if (max_i == 0) {
          norm = true;
          bi.nullify(home);
        }
        else
          bi.ub(home, max_i);
        ubs += max_i - ui;
      }
    }
    if (norm)
      normalize(home);
    assert (isOK());
  }
  
  forceinline void 
  DashedString::update(Space& home, const DashedString& d) {
    lbs = d.lbs;
    ubs = d.ubs;
    if (d.n <= n) {
      for (int i = 0; i < d.n; ++i)
        (x+i)->update(home, d[i]);
      if (d.n < n) {
        for (int i = d.n; i < n; ++i)
          (x+i)->nullify(home);
        a.free(x + d.n, n - d.n);
      }
      n = d.n;      
      return;
    }
    for (int i = 0; i < n; ++i)
      (x+i)->nullify(home);
    a.free(x, n);
    x = a.template alloc<Block>(d.n);
    for (int i = 0; i < d.n; ++i)
      (x+i)->update(home, d[i]);
    n = d.n;
    assert (isOK());
    assert (isNorm());
  }
  
  forceinline void 
  DashedString::update_rev(Space& home, const DashedString& d) {
    lbs = d.lbs;
    ubs = d.ubs;
    if (d.n <= n) {
      for (int i = 0; i < d.n; ++i)
        (x+i)->update(home, d[d.n-i-1]);
      if (d.n < n) {
        for (int i = d.n; i < n; ++i)
          (x+i)->nullify(home);
        a.free(x + d.n, n - d.n);
      }
      n = d.n;
      return;
    }
    for (int i = 0; i < n; ++i)
      (x+i)->nullify(home);
    a.free(x, n);
    x = a.template alloc<Block>(d.n);
    for (int i = 0; i < d.n; ++i)
      (x+i)->update(home, d[d.n-i-1]);
    n = d.n;
    assert (isOK());
    assert (isNorm());
  }
  
  forceinline void 
  DashedString::update(Space& home, const std::vector<int>& w) {
    int m = w.size();
    lbs = ubs = m;
    if (m == 0) {
      nullify(home);
      return;
    } 
    for (int i = 1; i < (int) w.size(); ++i)
      if (w[i] == w[i-1])
        m--;
    if (m <= n) {
      x->update(home, Block(w[0]));
      for (int i = 1, j = 0; i < (int) w.size(); ++i)
        if (w[i] == w[i-1])
          (x+j)->updateCard(home, x[j].lb() + 1, x[j].ub() + 1);
        else
          (x+(++j))->update(home, Block(w[i]));
      if (m < n) {
        for (int i = m; i < n; ++i)
          (x+i)->nullify(home);
        a.free(x + m, n - m);
      }
      n = m;
      return;
    }
    for (int i = 0; i < n; ++i)
      (x+i)->nullify(home);
    a.free(x, n);
    x = a.template alloc<Block>(m);
    x->update(home, Block(w[0]));
    for (int i = 1, j = 0; i < (int) w.size(); ++i)
      if (w[i] == w[i-1])
        (x+j)->updateCard(home, x[j].lb() + 1, x[j].ub() + 1);
      else
        (x+(++j))->update(home, Block(w[i]));
    n = m;
//    std::cerr << *this << '\n';
    assert (isOK());
    assert (isNorm());
  }
  
  forceinline void
  DashedString::update(Space& home, const DashedString& x0, 
                                    const DashedString& x1) {
    lbs = x0.lbs + x1.lbs;
    ubs = x0.ubs + x1.ubs;
    int n0 = x0.size(), n1 = x1.size();
    bool norm = x0[n0-1].equals(x1[0]);
    int m = n0 + n1 - norm;
    if (n < m) {
      a.free(x, n);
      x = a.template alloc<Block>(m);
    }
    else
      a.free(x+m,n-m);
    n = m;
    for (int i = 0; i < n0; ++i)
      (x+i)->update(home,x0[i]);
    if (norm)
      (x+n0-1)->updateCard(home, x0[n0-1].lb() + x1[0].lb(), 
                                 x0[n0-1].lb() + x1[0].ub());
    else
      (x+n0)->update(home, x1[0]);
    for (int i = n0+1-norm, j = 1; j < n1; ++i, ++j)
      (x+i)->update(home, x1[j]);
  }
 
  forceinline void
  DashedString::splitBlock(Space& home, int idx, int c, unsigned alt) {
//    std::cerr <<"split"<<alt<<"["<<idx<<"]: "<<*this<<" <- "<<int2str(c)<<"\n"; 
    Block& x_i = x[idx];
    int k = x_i.lb();
    assert (!x_i.isFixed() && x_i.ub() == k);
    bool lnorm = false;
    if (idx > 0) {
      Block& x_prev = x[idx-1];
      if (alt == 0)
        lnorm = x_prev.isFixed() && x_prev.baseMin() == c;
      else if (x_prev.baseSize() == x_i.baseSize()-1 
           && !x_prev.baseContains(c)) {
        Set::GLBndSet s;
        x_prev.includeBaseIn(home, s);
        Set::SetDelta d;
        s.include(home, c, c, d);
        lnorm = x_i.baseEquals(s);
      }
    }
    bool rnorm = false;
    if (k == 1 && idx < n-1) {
      Block& x_next = x[idx+1];
      if (alt == 0)
        rnorm = x_next.isFixed() && x_next.baseMin() == c;
      else if (x_next.baseSize() == x_i.baseSize()-1 
           && !x_next.baseContains(c)) {
        Set::GLBndSet s;
        x_next.includeBaseIn(home, s);
        Set::SetDelta d;
        s.include(home, c, c, d);
        rnorm = x_i.baseEquals(s);
      }
    }
//    std::cerr << (*this)[idx] << ' ' << lnorm << ' ' << rnorm << '\n';
    if (lnorm) {
      Block& x_prev = x[idx-1];
      if (rnorm) {
        Block& x_next = x[idx+1];
        (x+idx-1)->updateCard(home, x_prev.lb() + 1 + x_next.lb(), 
                                    x_prev.ub() + 1 + x_next.ub());
        for (int j = idx; j < n-2; ++j)
          (x+j)->update(home, x[j+2]);
        a.free(x+n-2, 2);
        n -= 2;
      }
      else {
        (x+idx-1)->updateCard(home, x_prev.lb() + 1, x_prev.ub() + 1);
        if (k == 1) {
          for (int j = idx; j < n-1; ++j)
            (x+j)->update(home, x[j+1]);
          a.free(x+n-1, 1);
          --n;
        }
        else {
          --k;
          (x+idx)->updateCard(home, k, k);
        }
      }
    }
    else if (rnorm) {
      Block& x_next = x[idx+1];
      if (alt == 1)
        (x+idx)->baseRemove(home, c);
      else if (x_i.baseSize() > 1)
        (x+idx)->update(home, c);
      (x+idx)->updateCard(home, x_next.lb() + 1, x_next.ub() + 1);
      for (int j = idx+1; j < n-1; ++j)
        (x+j)->update(home, x[j+1]);
      a.free(x+n-1, 1);
      --n;
    }
    else {
      if (k > 1) {
        Block* y = a.template alloc<Block>(++n);
        for (int j = 0; j < idx; ++j)
          (y+j)->update(home, x[j]);
        if (alt == 0)
          (y+idx)->update(home, Block(c));
        else {
          (y+idx)->update(home, x_i);
          (y+idx)->baseRemove(home, c);
          (y+idx)->updateCard(home, 1, 1);
        }
        for (int j = idx+1; j < n; ++j)
          (y+j)->update(home, x[j-1]);
        --k;
        (y+idx+1)->updateCard(home, k, k);
        a.free(x, n-1);        
        x = y;
      }
      else
        alt == 0 ? (x+idx)->update(home,Block(c)) : (x+idx)->baseRemove(home,c);
    }
//    std::cerr << "After splitBlock: " << *this << "\n";
    assert (isOK());
    assert (isNorm());
  }
  
  forceinline void
  DashedString::normalize(Space& home) {
//    std::cerr << "Normalize: " << *this << '\n';
    int newSize = n;
    int j = -1;
    // 1st pass: determine new size, settle adjacent blocks with same base
    for (int i = 0; i < n; ++i) {
      if (x[i].isNull()) {
        --newSize;
        continue;
      }
      // j is the index of the last encountered non-null block.
      if (j != -1 && x[i].baseEquals(x[j])) {
        int u = x[i].ub() + x[j].ub();
        if (u > MAX_STRING_LENGTH || u < x[i].ub())
          u = MAX_STRING_LENGTH;
        (x+j)->updateCard(home, x[i].lb() + x[j].lb(), u);
        (x+i)->nullify(home);
        --newSize;
      }
      else if (j < i-1) {
        // x[i] and x[j] have different base, and there is at least a null block
        // x[k] with j < k < i:
        (x+(++j))->update(home, x[i]);
        (x+i)->nullify(home);
      }
      else
        j = i;
    }
//    std::cerr << "After 1st pass: " << *this << '\n';
    // 2nd pass: possibly downsize the dynamic array due to nullification.    
    if (newSize < n) {
      if (newSize == 0) {
        // All the blocks in x are null.
        if (n > 1) {
          a.free(x+1, n-1);
          n = 1;
        }
      }
      else {
        // Shrinking the array.
        a.free(x + newSize, n - newSize);
        n = newSize;
      }
    }
    assert(isNorm());
//    std::cerr << *this << '\n';
    assert(isOK());
  }
  
  forceinline bool
  DashedString::isNorm() const {
    if (x[0].isNull())
      return n == 1;
    for (int i = 1; i < n; i++)
      if (x[i].isNull() || x[i-1].baseEquals(x[i]))
        return false;
    return true;
  }
  
  forceinline bool
  DashedString::isOK() const {
    if (n == 0)
      return false;
    int l = 0; long u = 0;
    for (int i = 0; i < n; i++) {
      if (!x[i].isOK())
        return false;
      l += x[i].lb();
      u += x[i].ub();
    }
    return l == lbs && u == ubs;
  }
  
  forceinline std::ostream&
  operator<<(std::ostream& os, const DashedString& d) {
    if (d.isFixed()) {
      os << "\"";
      for (int x : d.val())
        os << int2str(x);
      os << "\"";
    }
    else {
      int n = d.size();
      for (int i = 0; i < n - 1; ++i)
        os << d[i] << " + ";
      os << d[n-1];
    }
    return os << " <" << d.lbs << ".." << d.ubs << ">";
  }

}}

