namespace Gecode { namespace String {
  
  //FIXME: Implement this:
  
  struct SweepFwd {};
  struct SweepBwd {};
  
  /// Iterator used by sweep-based equation algorithms for pushing/stretching 
  /// a block against the specified View in the specified direction Dir.
  /// FIXME: The current position of the iterator _must always_ be normalized 
  ///        w.r.t. the view regardless of the direction.
  template <class Dir, class View>
  class SweepIterator {
  protected:
    /// The view on which we iterate
    const View& sv;
    /// The current position on the view, always normalized w.r.t. sv
    Position pos;
    /// Check if the iterator position is in a consistent state;
    bool isOK(void) const;
  public:
    /// Constructors
    SweepIterator(const View& x);
    SweepIterator(const View& x, const Position& p);
    /// Move iterator to the beginning of the next block (if possible)
    void nextBlock(void);
    /// Test whether iterator is still within the dashed string or done
    bool hasNextBlock(void) const;
    /// Min. no. of chars that must be consumed from current position within current block
    int must_consume(void) const;
    /// Max. no. of chars that may be consumed from current position within current block
    int may_consume(void) const;
    /// Consume \a k characters from current position within current block
    void consume(int k);
    /// Consume \a k mandatory characters from current position within current block
    void consumeMand(int k);
    /// Returns const reference to the current position
    const Position& operator *(void) const;
    /// Returns true iff we are not done in the iteration.
    bool operator()(void) const;
    /// Returns the lower bound of the current block
    int lb(void) const;
    /// Returns the upper bound of the current block
    int ub(void) const;
    /// Check if the base of the current block is disjoint with that of \a b
    bool disj(const Block& b) const;
    /// Check if the base of the current block is disjoint with singleton {c}
    bool disj(int c) const;
  };
  
    
}}


namespace Gecode { namespace String {

  class ConcatView;
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
    SweepIterator<SweepFwd,StringView> fwd_iterator(void) const;
    SweepIterator<SweepBwd,StringView> bwd_iterator(void) const;
    //@}
    //@}
    
    int leftmost_unfixed_idx(void) const;
    int rightmost_unfixed_idx(void) const;
    int smallest_unfixed_idx(void) const;
    
    /// \name Value access
    //@{
    /// Return the minimum length for a string in the variable's domain
    int min_length(void) const;
    /// Return the maximum length for a string in the variable's domain
    int max_length(void) const;
    int lb_sum(void) const;
    long ub_sum(void) const;
    /// Returns the number of blocks of the domain
    int size(void) const;
    /// Returns the i-th block of the domain
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
    bool contains_rev(const StringView& y) const;
    template <class T> bool contains(const T& y) const;
    /// If this view is equals to y
    bool equals(const StringView& y) const;
    bool equals_rev(const StringView& y) const;
    template <class T> bool equals(const T& y) const;
    double logdim(void) const;
    //@}
    /// \name Cloning
    //@{
    /// Update this view to be a clone of view \a y
    void gets(Space& home, const DashedString& d);    
    void gets(Space& home, const StringView& y);
    void gets(Space& home, const ConcatView& y);
    void gets(Space& home, const ConstStringView& y);
    void gets(Space& home, const std::vector<int>& w);
    void gets_rev(Space& home, const StringView& y);
    
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
    ModEvent min_length(Space& home, int l);
    ModEvent max_length(Space& home, int u);
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
    template <class T> void 
    expandBlock(Space& home, const Block& bx, T& x) const;
    /// TODO:
    template <class ViewX> void
    crushBase(Space& home, ViewX& x, int idx, const Position& p,
                                              const Position& q) const;
    /// TODO:                                   
    void
    opt_region(Space& home, const Block& bx, Block& bnew, 
                            const Position& p, const Position& q, int l1) const;                       
    
    /// TODO:                   
    void
    mand_region(Space& home, const Block& bx, Block* bnew, int u, 
                             const Position& p, const Position& q) const;
    /// TODO:                                                     
    template <class ViewX> void
    mand_region(Space& home, ViewX& x, int idx, const Position& p,
                                                const Position& q) const;
    /// TODO:
    void
    resize(Space& home, Block newBlocks[], int newSize, int U[], int uSize);
    
    int fixed_chars_suff(const Position& p, const Position& q) const;
    int fixed_chars_pref(const Position& p, const Position& q) const;    
    forceinline std::vector<int> fixed_pref(const Position& p, 
                                            const Position& q) const;
    forceinline std::vector<int> fixed_suff(const Position& p,
                                            const Position& q) const;
    
    /// Normalize this view
    void normalize(Space& home);
    ModEvent splitBlock(Space& home, int idx, int c, unsigned a);
    
    ModEvent nullifyAt(Space& home, int i);
    ModEvent lbAt(Space& home, int i, int l);
    ModEvent ubAt(Space& home, int i, int u);
    ModEvent baseIntersectAt(Space& home, int idx, const Set::BndSet& S);
    ModEvent baseIntersectAt(Space& home, int idx, const Block& b);
    void updateCardAt(Space& home, int i, int l, int u);
    void updateAt(Space& home, int i, const Block& b);
    
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
    SweepIterator<SweepFwd,ConstStringView> fwd_iterator(void) const;
    SweepIterator<SweepBwd,ConstStringView> bwd_iterator(void) const;
    template <class T> void gets(Space&, const T&) const;
    template <class T> ModEvent equate(Space& home, const T& y) const;
    template <class IterY> Position push(int i, IterY& it) const;
    template <class IterY> void stretch(int i, IterY& it) const;
    double logdim(void) const;
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
    template <class T> void
    expandBlock(Space& home, const Block& bx, T& x) const;
    
    /// TODO:
    template <class ViewX> void
    crushBase(Space& home, ViewX& x, int idx, const Position& p,
                                              const Position& q) const;
    /// TODO:                                   
    void
    opt_region(Space& home, const Block& bx, Block& bnew, 
                            const Position& p, const Position& q, int l1) const;                       
    
    /// TODO:                   
    void
    mand_region(Space& home, const Block& bx, Block* bnew, int u, 
                             const Position& p, const Position& q) const;
    /// TODO:                                                     
    template <class ViewX> void
    mand_region(Space& home, ViewX& x, int idx, const Position& p,
                                                const Position& q) const;
    void
    resize(Space& home, Block newBlocks[], int newSize, int U[], int uSize) const;
    
    int
    fixed_chars_suff(const Position& p, const Position& q) const;
    int
    fixed_chars_pref(const Position& p, const Position& q) const;
    std::vector<int> fixed_pref(const Position& p, 
                                            const Position& q) const;
    std::vector<int> fixed_suff(const Position& p,
                                            const Position& q) const;
    
    /// Normalize this view
    void normalize(Space& home);
    ModEvent nullify(Space& home);
    
    /// Update this view to be a clone of view \a y
    void update(Space& home, ConstStringView& y);
    
    ModEvent min_length(Space& home, int l);
    ModEvent max_length(Space& home, int u);
                                
    //@}
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
  class ConcatView : public VarImpView<StringVar> {
  protected:
    StringView& x0;
    StringView& x1;
    int pivot;
  public:
    /// \name Constructors and initialization
    //@{
    /// Default constructor
    ConcatView(void);
    /// Initialize from string variables \a x and \a y
    ConcatView(StringView& x, StringView& y);
    //@}
    template <class T> void gets(Space& home, const T& d) const;
    template <class T> ModEvent equate(Space& home, const T& y);
    template <class IterY> Position push(int i, IterY& it) const;
    template <class IterY> void stretch(int i, IterY& it) const;
    double logdim(void) const;
    
    ModEvent nullify(Space& home);
    
    /// \name Sweep iterators
    //@{
    SweepIterator<SweepFwd,ConcatView> fwd_iterator(void) const;
    SweepIterator<SweepBwd,ConcatView> bwd_iterator(void) const;
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
    /// Returns the i-th block of the domain
    const Block& operator[](int i) const;
    /// Return the value of this string view, if assigned. Otherwise, an
    /// IllegalOperation exception is thrown.
    std::vector<int> val(void) const;
    StringView lhs(void) const;
    StringView rhs(void) const;
    //@}
    /// \name Domain tests
    //@{
    /// Test whether variable is assigned
    bool assigned(void) const;
    /// Consistency checks on the view
    bool isOK(void) const;
    bool isNull(void) const;
    
    int lb_sum(void) const;
    long ub_sum(void) const;

    /// If this view is equals to y
    template <class T> bool equals(const T& y) const;
    template <class T> bool contains(const T& y) const;
    ModEvent max_length(Space& home, int u) const;
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
    template <class T> void
    expandBlock(Space& home, const Block& bx, T& x) const;
    /// TODO:
    template <class ViewX> void
    crushBase(Space& home, ViewX& x, int idx, const Position& p,
                                              const Position& q) const;
    /// TODO:                                   
    void
    opt_region(Space& home, const Block& bx, Block& bnew, 
                            const Position& p, const Position& q, int l1) const;                       
    
    /// TODO:                   
    void
    mand_region(Space& home, const Block& bx, Block* bnew, int u, 
                             const Position& p, const Position& q) const;
    /// TODO:                                                     
    template <class ViewX> void
    mand_region(Space& home, ViewX& x, int idx, const Position& p,
                                                const Position& q) const;
    /// TODO:
    void
    resize(Space& home, Block newBlocks[], int newSize, int U[], int uSize);
    /// Normalize this view
    void normalize(Space& home);
    
    int
    fixed_chars_suff(const Position& p, const Position& q) const;
    int
    fixed_chars_pref(const Position& p, const Position& q) const;
    std::vector<int> fixed_pref(const Position& p, 
                                            const Position& q) const;
    std::vector<int> fixed_suff(const Position& p,
                                            const Position& q) const;
    
    ModEvent nullifyAt(Space& home, int i);
    ModEvent lbAt(Space& home, int i, int l);
    ModEvent ubAt(Space& home, int i, int u);    
    ModEvent baseIntersectAt(Space& home, int idx, const Set::BndSet& S);
    ModEvent baseIntersectAt(Space& home, int idx, const Block& b);
    void updateCardAt(Space& home, int i, int l, int u);
    void updateAt(Space& home, int i, const Block& b);    
    //@}
  };
  /**
   * \brief Print string variable view
   * \relates Gecode::String::StringView
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConcatView& z);
  
}}

namespace Gecode { namespace String {  
  
  /**
   * \brief Reverse view
   */
  class ReverseView : public VarImpView<StringVar> {
  protected:
    /// We never refine the blocks of sv.
    const StringView& sv;
  public:
    /// Construct with empty string
    ReverseView(void);
    /// Construct
    ReverseView(const StringView& x);
    /// 
    int min_length(void) const;
    /// 
    int max_length(void) const;
    /// 
    int size(void) const;
    /// 
    bool isNull(void) const;
    /// 
    const Block& operator[](int i) const;
    /// Return the value of this view
    std::vector<int> val(void) const;
    /// \name Sweep iterators
    //@{
    SweepIterator<SweepFwd,ReverseView> fwd_iterator(void) const;
    SweepIterator<SweepBwd,ReverseView> bwd_iterator(void) const;
    template <class IterY> Position push(int i, IterY& it) const;
    template <class IterY> void stretch(int i, IterY& it) const;
    double logdim(void) const;
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
    template <class T> void
    expandBlock(Space& home, const Block& bx, T& x) const;
    
    /// TODO:
    template <class ViewX> void
    crushBase(Space& home, ViewX& x, int idx, const Position& p,
                                              const Position& q) const;
    /// TODO:                                   
    void
    opt_region(Space& home, const Block& bx, Block& bnew, 
                            const Position& p, const Position& q, int l1) const;                       
    
    /// TODO:                   
    void
    mand_region(Space& home, const Block& bx, Block* bnew, int u, 
                             const Position& p, const Position& q) const;
    /// TODO:                                                     
    template <class ViewX> void
    mand_region(Space& home, ViewX& x, int idx, const Position& p,
                                                const Position& q) const;
    void
    resize(Space& home, Block newBlocks[], int newSize, int U[], int uSize) const;
    
    int
    fixed_chars_suff(const Position& p, const Position& q) const;
    int
    fixed_chars_pref(const Position& p, const Position& q) const;
    std::vector<int> fixed_pref(const Position& p, 
                                            const Position& q) const;
    std::vector<int> fixed_suff(const Position& p,
                                            const Position& q) const;
                                            
    bool isOK(void) const;
    StringView x(void) const;
  };
  /**
   * \brief Print string variable view
   * \relates Gecode::String::StringView
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ReverseView& z);
  
}}

#include <gecode/string/var/string.hpp>
#include <gecode/string/view/string.hpp>
#include <gecode/string/view/const.hpp>
#include <gecode/string/view/concat.hpp>
#include <gecode/string/view/reverse.hpp>
