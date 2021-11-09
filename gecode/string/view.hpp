namespace Gecode { namespace String {

  class ConcatView;
  class ConstStringView;
  template <class T> class SweepIterator;
  
  template <class T>
  class DashedViewBase {
  protected:
    const T* ptr;
  public:
    DashedViewBase(void);
    DashedViewBase(const T* x);
    int size(void) const;
    const Block& operator[](int i) const;
    bool equiv(const Position& p, const Position& q) const;
    bool prec(const Position& p, const Position& q) const;
    template <class IterY> Position push(int i, IterY& it) const;
    template <class IterY> void stretch(int i, IterY& it) const;
    int max_new_blocks(const Matching& m) const;
    int min_len_mand(const Block& bx, const Position& lsp, 
                                      const Position& eep) const;
    int max_len_opt(const Block& bx, const Position& esp, 
                                     const Position& lep, int l1) const;
    template <class ViewX> void
    crushBase(Space& home, ViewX& x, int idx, const Position& p,
                                              const Position& q) const;
    void
    opt_region(Space& home, const Block& bx, Block& bnew,
                            const Position& p, const Position& q, int l1) const;
    void
    mand_region(Space& home, const Block& bx, Block* bnew, int u, 
                             const Position& p, const Position& q) const;
    template <class ViewX> void
    mand_region(Space& home, ViewX& x, int idx, const Position& p,
                                                const Position& q) const;
    std::vector<int> fixed_pref(const Position& p, 
                                const Position& q, int & np) const;
    std::vector<int> fixed_suff(const Position& p, 
                                const Position& q, int & ns) const;
  };

  /**
   * \defgroup TaskActorStringView String views
   *
   * String propagators and branchings compute with string views.
   * String views provide views on string variable implementations,
   * string constants, and TODO: ....
   * \ingroup TaskActorString
   */
  class StringView : public VarImpView<StringVar>,
                     public DashedViewBase<DashedString> {
  public:
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
    
    SweepFwdIterator<StringView> fwd_iterator(void) const;
    SweepBwdIterator<StringView> bwd_iterator(void) const;
    
    int min_length(void) const;
    int max_length(void) const;
    int lb_sum(void) const;
    long ub_sum(void) const;
    double logdim(void) const;
    
    int leftmost_unfixed_idx(void) const;
    int rightmost_unfixed_idx(void) const;
    int smallest_unfixed_idx(void) const;
    
    bool isOK(void) const;
    bool isNull(void) const;
    bool assigned(void) const;
    std::vector<int> val(void) const;
    
    template <class T> bool same(const T& y) const;
    bool equals(const StringView& y) const;
    bool equals_rev(const StringView& y) const;
    template <class T> bool equals(const T& y) const;
    bool contains(const StringView& y) const;
    bool contains_rev(const StringView& y) const;
    template <class T> bool contains(const T& y) const;
    
    template <class T> ModEvent equate(Space& home, const T& y);
    template <class T> ModEvent find(Space& home, const T& y, int& ln, int& un,
                                                                      bool occ);
    ModEvent nullify(Space& home);
    ModEvent bnd_length(Space& home, int l, int u);
    ModEvent min_length(Space& home, int l);
    ModEvent max_length(Space& home, int u);
    ModEvent splitBlock(Space& home, int idx, int c, unsigned a);
    
    void update(Space& home, StringView& y);
    void gets(Space& home, const DashedString& d);    
    void gets(Space& home, const StringView& y);
    void gets(Space& home, const ConcatView& y);
    void gets(Space& home, const ConstStringView& y);
    void gets(Space& home, const std::vector<int>& w);
    void gets_rev(Space& home, const StringView& y);
    
    void normalize(Space& home);
    void nullifyAt(Space& home, int i);
    void lbAt(Space& home, int i, int l);
    void ubAt(Space& home, int i, int u);
    void baseRemoveAt(Space& home, int i, int c);
    void baseIntersectAt(Space& home, int idx, const Set::BndSet& S);
    void baseIntersectAt(Space& home, int idx, const Block& b);
    void updateCardAt(Space& home, int i, int l, int u);
    void updateAt(Space& home, int i, const Block& b);
    void resize(Space& home, Block newBlocks[], int newSize, int U[], int uSize);    
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
   * \brief ConstDashed view
   */
  class ConstDashedView : public DashedViewBase<ConstDashedView> {
  protected:
    /// We never refine the blocks
    const Block* b0;
    int n;
  public:
    ConstDashedView(void);
    ConstDashedView(const Block& b, int n);
    int size(void) const;
    const Block& operator[](int i) const;
    SweepFwdIterator<ConstDashedView> fwd_iterator(void) const;
    SweepBwdIterator<ConstDashedView> bwd_iterator(void) const;
    bool isOK(void) const;
  };
  /**
   * \brief Print string variable view
   * \relates Gecode::String::StringView
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConstDashedView& z);
  
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
    SweepFwdIterator<ConstStringView> fwd_iterator(void) const;
    SweepBwdIterator<ConstStringView> bwd_iterator(void) const;
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
    int max_new_blocks(const Matching& m) const;
    
    /// TODO:
    int min_len_mand(const Block& bx, const Position& lsp, 
                                      const Position& eep) const;
    /// TODO:
    int max_len_opt(const Block& bx, const Position& esp, 
                                     const Position& lep, int l1) const;
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
                                                
    std::vector<int> fixed_pref(const Position& p, 
                                const Position& q, int & np) const;
    std::vector<int> fixed_suff(const Position& p,
                                const Position& q, int & np) const;    
    /// Update this view to be a clone of view \a y
    void update(Space& home, ConstStringView& y);
    ModEvent nullify(Space& home);
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
  class ConcatView {
  protected:
    StringView& x0;
    StringView& x1;
    int pivot;
  public:
    ConcatView(StringView& x, StringView& y);
    template <class T> void gets(Space& home, const T& d) const;
    template <class T> ModEvent equate(Space& home, const T& y);
    template <class IterY> Position push(int i, IterY& it) const;
    template <class IterY> void stretch(int i, IterY& it) const;
    double logdim(void) const;
    ModEvent nullify(Space& home);
    /// \name Sweep iterators
    //@{
    SweepFwdIterator<ConcatView> fwd_iterator(void) const;
    SweepBwdIterator<ConcatView> bwd_iterator(void) const;
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
    bool assigned(void) const;
    /// Consistency checks on the view
    bool isOK(void) const;
    bool isNull(void) const;
    int lb_sum(void) const;
    long ub_sum(void) const;
    /// Returns true if p and q are the same position in this view.
    bool equiv(const Position& p, const Position& q) const;
    /// Returns true if p precedes q according to this view.
    bool prec(const Position& p, const Position& q) const;
    ///TODO:
    int max_new_blocks(const Matching& m) const;
    /// TODO:
    int min_len_mand(const Block& bx, const Position& lsp, 
                                      const Position& eep) const;
    /// TODO:
    long max_len_opt(const Block& bx, const Position& esp, 
                                      const Position& lep, int l1) const;
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
    
    std::vector<int> fixed_pref(const Position& p, 
                                const Position& q, int & np) const;
    std::vector<int> fixed_suff(const Position& p,
                                const Position& q, int & np) const;
    
    void nullifyAt(Space& home, int i);
    void lbAt(Space& home, int i, int l);
    void ubAt(Space& home, int i, int u);    
    void baseIntersectAt(Space& home, int idx, const Set::BndSet& S);
    void baseIntersectAt(Space& home, int idx, const Block& b);
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
  class ReverseView : public DashedViewBase<StringView> {
  public:
    ReverseView(const StringView& x);
    int size(void) const;
    const Block& operator[](int i) const;
    SweepFwdIterator<ReverseView> fwd_iterator(void) const;
    SweepBwdIterator<ReverseView> bwd_iterator(void) const;
    
    bool equiv(const Position& p, const Position& q) const;
    bool prec(const Position& p, const Position& q) const;
    int max_new_blocks(const Matching& m) const;
    int min_len_mand(const Block& bx, const Position& lsp, 
                                      const Position& eep) const;
    int max_len_opt(const Block& bx, const Position& esp, 
                                     const Position& lep, int l1) const;
    template <class ViewX> void
    crushBase(Space& home, ViewX& x, int idx, const Position& p,
                                              const Position& q) const;
    void
    opt_region(Space& home, const Block& bx, Block& bnew, 
                            const Position& p, const Position& q, int l1) const;
    void
    mand_region(Space& home, const Block& bx, Block* bnew, int u, 
                             const Position& p, const Position& q) const;
    template <class ViewX> void
    mand_region(Space& home, ViewX& x, int idx, const Position& p,
                                                const Position& q) const;
    std::vector<int> fixed_pref(const Position& p, 
                                const Position& q, int & np) const;
    std::vector<int> fixed_suff(const Position& p,
                                const Position& q, int & np) const;
    bool isOK(void) const;
    const StringView& baseView(void) const;
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
#include <gecode/string/view/ds-iterators.hpp>
#include <gecode/string/view/base.hpp>
#include <gecode/string/view/string.hpp>
#include <gecode/string/view/concat.hpp>
#include <gecode/string/view/reverse.hpp>
#include <gecode/string/view/const-string.hpp>
#include <gecode/string/view/const-dashed.hpp>
