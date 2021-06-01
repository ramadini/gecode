namespace Gecode { namespace String {

  /// Abstract class for iterating views, used to push/stretch blocks in dashed 
  /// string equation.
  /// FIXME: The current position of the iterator must always be normalized 
  ///        w.r.t. its view.
  template <class View>
  class SweepIterator {
  protected:
    /// The view on which we iterate
    const View& sv;
    /// The current position on the view, always normalized w.r.t. sv
    Position pos;
    /// Check if the iterator position is in a consistent state;
    bool isOK(void) const;
  public:
    /// Constructor
    SweepIterator(const View& x, const Position& p);
    /// Move iterator to the beginning of the next block (if possible)
    virtual void nextBlock(void) = 0;
    /// Test whether iterator is still within the dashed string or done
    virtual bool hasNextBlock(void) const = 0;
    /// Min. no. of chars that must be consumed from current position within current block
    virtual int must_consume(void) const = 0;
    /// Max. no. of chars that may be consumed from current position within current block
    virtual int may_consume(void) const = 0;
    /// Consume \a k characters from current position within current block
    virtual void consume(int k) = 0;
    /// Consume \a k mandatory characters from current position within current block
    virtual void consumeMand(int k) = 0;
    /// Returns const reference to the current position
    const Position& operator *(void);
    /// Return the lower bound of the current block
    int lb(void) const;
    /// Return the upper bound of the current block
    int ub(void) const;
    /// Check if the base of the current block is disjoint with that of \a b
    bool disj(const Block& b) const;
    bool disj(int c) const;
  };
  
  /// Iterator for pushing/stretching forward.
  template <class View>
  struct SweepFwdIterator : public SweepIterator<View> {
    using SweepIterator<View>::sv;
    using SweepIterator<View>::pos;
    SweepFwdIterator(const View& x);
    SweepFwdIterator(const View& x, const Position& p);
    void nextBlock(void);
    bool hasNextBlock(void) const;
    void consume(int k);
    void consumeMand(int k);
    int must_consume(void) const;
    int may_consume(void) const;
    bool operator()(void) const;
  };
  
  /// Iterator for pushing/stretching backward.
  template <class View>
  struct SweepBwdIterator : public SweepIterator<View> {
    using SweepIterator<View>::sv;
    using SweepIterator<View>::pos;
    SweepBwdIterator(const View& x);
    SweepBwdIterator(const View& x, const Position& p);
    bool disj(const Block& b) const;
    bool disj(int c) const;
    void nextBlock(void);
    bool hasNextBlock(void) const;
    void consume(int k);
    void consumeMand(int k);
    int must_consume(void) const;
    int may_consume(void) const;
    bool operator()(void) const;
    int lb(void) const;
    int ub(void) const;
  };  
    
    
}}


namespace Gecode { namespace String {

  class ConstStringView;

  /**
   * \defgroup TaskActorStringView String views
   *
   * String propagators and branchings compute with string views.
   * String views provide views on string variable implementations,
   * string constants, and TODO: ....
   * \ingroup TaskActorString
   */
  class StringView : public VarImpView<StringVar> {
  protected:
    using VarImpView<StringVar>::x;
  public:
    /// \name Constructors and initialization
    //@{
    /// Default constructor
    StringView(void);
    /// Initialize from string variable \a y
    StringView(const StringVar& y);
    /// Initialize from string variable implementation \a y
    StringView(StringVarImp* y);
    //@}
    /// \name Sweep iterators
    //@{
    SweepFwdIterator<StringView> fwd_iterator(void) const;
    SweepBwdIterator<StringView> bwd_iterator(void) const;
    //@}
    //@}
    /// \name Value access
    //@{
    /// Return the minimum length for a string in the variable's domain
    int min_length(void) const;
    /// Return the maximum length for a string in the variable's domain
    int max_length(void) const;
    /// Return the sum of the upper bounds
    long ubounds_sum(void) const;
    /// Returns the number of blocks of the domain
    int size(void) const;
    /// Returns the i-th block of the domain
    Block& operator[](int i);
    const Block& operator[](int i) const;
    /// Return the value of this string view, if assigned. Otherwise, an
    /// IllegalOperation exception is thrown.
    std::vector<int> val(void) const;
    //@}
    /// \name Domain tests
    //@{
    /// Test whether variable is assigned
    bool assigned(void) const;
    /// Test whether the view is null
    bool isNull(void) const;
    /// Consistency checks on the view
    bool isOK(void) const;
    /// If this view and y are the same
    bool same(const StringView& y) const;
    bool same(const ConstStringView& y) const;
    /// If this view contains y
    bool contains(const StringView& y) const;
    template <class T> bool contains(const T& y) const;
    /// If this view is equals to y
    bool equals(const StringView& y) const;
    template <class T> bool equals(const T& y) const;
    //@}
    /// \name Cloning
    //@{
    /// Update this view to be a clone of view \a y
    void gets(Space& home, const DashedString& d);
    void gets(Space& home, const StringView& y);
    void gets(Space& home, const ConstStringView& y);
    void gets(Space& home, const std::vector<int>& w);
    
    template <class IterY> Position push(int i, IterY& it) const;
    template <class IterY> void stretch(int i, IterY& it) const;
    template <class T> ModEvent equate(Space& home, const T& y);
    
    ModEvent nullify(Space& home);
    //@}
    /// \name Domain update by cardinality refinement
    //@{
    /// Possibly update the upper bound of the blocks, knowing that the minimum 
    /// length for any string in the domain is \a l and the maximum length for 
    /// any string in the domain is \a u
    ModEvent bnd_length(Space& home, int l, int u);
    //@}
    /// \name Methods for dashed string equation
    //@{
    /// Returns true if p and q are the same position in this view.
    bool equiv(const Position& p, const Position& q) const;
    /// Returns true if p precedes q according to this view.
    bool prec(const Position& p, const Position& q) const;
    ///TODO:
    int ub_new_blocks(const Matching& m) const;
    /// TODO:
    int min_len_mand(const Block& bx, const Position& lsp, 
                                      const Position& eep) const;
    /// TODO:
    int max_len_opt(const Block& bx, const Position& esp, 
                                     const Position& lep, int l1) const;
    /// TODO:                             
    void
    expandBlock(Space& home, const Block& bx, Block* y) const;
    /// TODO:
    void
    crushBase(Space& home, Block& bx, const Position& esp, 
                                      const Position& lep) const;
    /// TODO:                                   
    void
    opt_region(Space& home, const Block& bx, Block& bnew, 
                            const Position& p, const Position& q, int l1) const;                       
    
    /// TODO:                   
    void
    mand_region(Space& home, Block& bx, const Block& by,
                             const Position& p, const Position& q) const;
    /// TODO:                                                     
    void
    mand_region(Space& home, Block& bx, Block* bnew, int u,
                             const Position& p, const Position& q) const;
    /// TODO:
    void
    resize(Space& home, Block newBlocks[], int newSize, int U[], int uSize);
    /// Normalize this view
    void normalize(Space& home);
    //@}
  };
  /**
   * \brief Print string variable view
   * \relates Gecode::String::StringView
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringView& x);

}}

namespace Gecode { namespace String {  

  /**
   * \brief Constant string view
   *
   * A constant string view \f$x\f$ for a string \f$w\f$ provides operations such
   * that \f$x\f$ behaves like \f$w\f$.
   * \ingroup TaskActorStringView
   */
  class ConstStringView : public ConstView<StringView> {
  // FIXME: To save space, for a const view we do not require normalization 
  //        because we assume that almost all the blocks have cardinality 1.
  protected:
    /// The value
    int* _val;
    /// The size of the value
    int n;
  public:
    /// Construct with empty string
    ConstStringView(void);
    /// Construct with \a w as domain (n = |w|).
    ConstStringView(Space& home, int w[], int n);
    /// Return the length of the string (for compatibility with other views)
    int min_length(void) const;
    /// Return the length of the string (for compatibility with other views)
    int max_length(void) const;
    /// Return the sum of the upper bounds
    int ubounds_sum(void) const;
    /// Return the length of the string
    int size(void) const;
    /// Return true iff the view is on the empty string
    bool isNull(void) const;
    /// Returns the i-th character of the string
    int operator[](int i) const;
    /// Return the value of this view
    std::vector<int> val(void) const;
    /// \name Sweep iterators
    //@{
    SweepFwdIterator<ConstStringView> fwd_iterator(void) const;
    SweepBwdIterator<ConstStringView> bwd_iterator(void) const;
    template <class T> ModEvent gets(Space&, const T&) const;
    template <class T> ModEvent equate(Space& home, const T& y) const;
    template <class IterY> Position push(int i, IterY& it) const;
    template <class IterY> void stretch(int i, IterY& it) const;
    //@}
    /// Always returns true (for compatibility with other views)
    bool assigned(void) const;
    /// Checks length bounds (for compatibility with other views)
    ModEvent bnd_length(Space& home, int l, int u) const;
    /// If this view contains y
    template <class T> bool contains(const T& y) const;
    /// If this view is equals to y
    template <class T> bool equals(const T& y) const;
    /// Returns true if p and q are the same position in this view.
    bool equiv(const Position& p, const Position& q) const;
    /// Returns true if p precedes q according to this view.
    bool prec(const Position& p, const Position& q) const;
    /// \name Methods for dashed string equation
    //@{
    ///TODO:
    int ub_new_blocks(const Matching& m) const;
    
    /// TODO:
    int min_len_mand(const Block& bx, const Position& lsp, 
                                      const Position& eep) const;
    /// TODO:
    int max_len_opt(const Block& bx, const Position& esp, 
                                     const Position& lep, int l1) const;
    /// TODO:                             
    void
    expandBlock(Space& home, const Block& bx, Block* y) const;
    
    /// TODO:
    void
    crushBase(Space& home, Block& bx, const Position& esp, 
                                      const Position& lep) const;
    /// TODO:                                   
    void
    opt_region(Space& home, const Block& bx, Block& bnew, 
                            const Position& p, const Position& q, int l1) const;                       
    
    /// TODO:                   
    void
    mand_region(Space& home, Block& bx, const Block& by, const Position& p, 
                                                         const Position& q) const;
    /// TODO:                                                     
    void
    mand_region(Space& home, Block& bx, Block* bnew, int u,
                             const Position& p, const Position& q) const;
    void
    resize(Space& home, Block newBlocks[], int newSize, int U[], int uSize) const;
    /// Normalize this view
    void normalize(Space& home);
    ModEvent nullify(Space& home);                            
    //@}
    template<class Char, class Traits>
    friend std::basic_ostream<Char,Traits>&
    operator <<(std::basic_ostream<Char,Traits>& os, const ConstStringView& v);
  };
  /**
   * \brief Print string variable view
   * \relates Gecode::String::StringView
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConstStringView& x);
  
}}

namespace Gecode { namespace String {  
  
  /**
   * \brief Concat view
   */
  template <class View0, class View1>
  class ConcatView : public VarImpView<StringVar> {
  protected:
    View0& x0;
    View1& x1;
    int pivot;
    template <class T> ModEvent me(Space&, const T& xk, int lb, int ub) const;
  public:
    /// \name Constructors and initialization
    //@{
    /// Default constructor
    ConcatView(void);
    /// Initialize from string variables \a x and \a y
    ConcatView(View0& x, View1& y);
    //@}
    template <class T> ModEvent gets(Space& home, const T& d) const;
    template <class T> ModEvent equate(Space& home, const T& y);
    template <class IterY> Position push(int i, IterY& it) const;
    template <class IterY> void stretch(int i, IterY& it) const;
    
    ModEvent nullify(Space& home);
    
    /// \name Sweep iterators
    //@{
    SweepFwdIterator<ConcatView<View0,View1>> fwd_iterator(void) const;
    SweepBwdIterator<ConcatView<View0,View1>> bwd_iterator(void) const;
    //@}
    //@}
    //@}
    /// \name Value access
    //@{
    /// Return the minimum length for a string in the variable's domain
    int min_length(void) const;
    /// Return the maximum length for a string in the variable's domain
    int max_length(void) const;
    /// Returns the number of blocks of the domain
    int size(void) const;
    /// Returns the i-th block of the domain in the form of a GBlock.
    GBlock operator[](int i) const;
    /// Return the value of this string view, if assigned. Otherwise, an
    /// IllegalOperation exception is thrown.
    std::vector<int> val(void) const;
    View0 lhs(void) const;
    View1 rhs(void) const;
    //@}
    /// \name Domain tests
    //@{
    /// Test whether variable is assigned
    bool assigned(void) const;
    /// Consistency checks on the view
    bool isOK(void) const;
    bool isNull(void) const;

    /// If this view is equals to y
    template <class T> bool equals(const T& y) const;
    template <class T> bool contains(const T& y) const;
    //@}
    /// \name Domain update by cardinality refinement
    //@{
    /// Possibly update the upper bound of the blocks, knowing that the minimum 
    /// length for any string in the domain is \a l and the maximum length for 
    /// any string in the domain is \a u
    ModEvent bnd_length(Space& home, int l, int u) const;
    //@}
    /// \name Methods for dashed string equation
    //@{
    /// Returns true if p and q are the same position in this view.
    bool equiv(const Position& p, const Position& q) const;
    /// Returns true if p precedes q according to this view.
    bool prec(const Position& p, const Position& q) const;
    ///TODO:
    int ub_new_blocks(const Matching& m) const;
    /// TODO:
    int min_len_mand(const Block& bx, const Position& lsp, 
                                      const Position& eep) const;
    /// TODO:
    int max_len_opt(const Block& bx, const Position& esp, 
                                     const Position& lep, int l1) const;
    /// TODO:                             
    void
    expandBlock(Space& home, const Block& bx, Block* y) const;
    /// TODO:
    void
    crushBase(Space& home, Block& bx, const Position& esp, 
                                      const Position& lep) const;
    /// TODO:                                   
    void
    opt_region(Space& home, const Block& bx, Block& bnew, 
                            const Position& p, const Position& q, int l1) const;                       
    
    /// TODO:                   
    void
    mand_region(Space& home, Block& bx, const Block& by,
                             const Position& p, const Position& q) const;
    /// TODO:                                                     
    void
    mand_region(Space& home, Block& bx, Block* bnew, int u,
                             const Position& p, const Position& q) const;
    /// TODO:
    void
    resize(Space& home, Block newBlocks[], int newSize, int U[], int uSize);
    /// Normalize this view
    void normalize(Space& home);
    //@}                   
    template<class Char, class Traits, class X, class Y>
    friend std::basic_ostream<Char,Traits>&
    operator <<(std::basic_ostream<Char,Traits>& os, const ConcatView<X,Y>& z);
  };
  /**
   * \brief Print string variable view
   * \relates Gecode::String::StringView
   */
  template<class Char, class Traits, class X, class Y>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConcatView<X,Y>& z);
  
}}

#include <gecode/string/var/string.hpp>
#include <gecode/string/view/string.hpp>
#include <gecode/string/view/const.hpp>
#include <gecode/string/view/concat.hpp>
