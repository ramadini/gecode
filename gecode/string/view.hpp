namespace Gecode { namespace String {

  /**
   * \defgroup TaskActorStringView String views
   *
   * String propagators and branchings compute with string views.
   * String views provide views on string variable implementations,
   * string constants, and TODO: ....
   * \ingroup TaskActorString
   */

  /**
   * \brief %String view for string variables
   * \ingroup TaskActorStringView
   */

  class StringView : public VarImpView<StringVar> {
  
  protected:
    using VarImpView<StringVar>::x;
    
  public:
  
    /// Abstract class for iterating string views, used to push/stretch blocks 
    /// in dashed string equation.
    /// FIXME: The current position of the iterator must always be normalized 
    ///        w.r.t. its view.
    class SweepIterator {
    protected:
      /// The views on which we iterate
      const StringView& sv;
      /// The current position on the view, always normalized w.r.t. sv
      Position pos;
      /// Check if the iterator position is in a consistent state;
      bool isOK(void) const;
    public:
      /// Constructor
      SweepIterator(const StringView& x, const Position& p);
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
    };
    /// Iterator for pushing/stretching forwards. Positions range from (0,0) 
    /// included to (|x|,0) excluded
    struct SweepFwdIterator : public SweepIterator {
      SweepFwdIterator(const StringView& x);
      SweepFwdIterator(const StringView& x, const Position& p);
      void nextBlock(void);
      bool hasNextBlock(void) const;
      void consume(int k);
      void consumeMand(int k);
      int must_consume(void) const;
      int may_consume(void) const;
      bool operator()(void) const;
    };
    /// Iterator for pushing/stretching backwards. Positions range from (|x|,0)
    /// included to (0,0) excluded
    struct SweepBwdIterator : public SweepIterator {
      SweepBwdIterator(const StringView& x);
      SweepBwdIterator(const StringView& x, const Position& p);
      int lb(void) const;
      int ub(void) const;
      bool disj(const Block& b) const;
      void nextBlock(void);
      bool hasNextBlock(void) const;
      void consume(int k);
      void consumeMand(int k);
      int must_consume(void) const;
      int may_consume(void) const;
      bool operator()(void) const;
    };
    
  public:
  
    /// \name Constructors and initialization
    //@{
    /// Default constructor
    StringView(void);
    /// Initialize from set variable \a y
    StringView(const StringVar& y);
    /// Initialize from set variable implementation \a y
    StringView(StringVarImp* y);
    //@}


    /// \name Sweep iterators
    //@{
    SweepFwdIterator fwd_iterator(void) const;
    SweepBwdIterator bwd_iterator(void) const;
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
    /// Test whether the domain is equatable with string \a w.
    bool check_equate(const std::vector<int>& w) const;
    /// Test whether the domain is equatable with block \a b.
    bool check_equate(const Block& b) const;
    /// Test whether the domain is equatable with dashed string \a x.
    bool check_equate(const DashedString& x) const;
    /// Consistency checks on the view
    bool isOK(void) const;
    /// If this view and y are the same
    bool same(const StringView& y) const;
    /// If this view contains y
    bool contains(const StringView& y) const;
    /// If this view is equals to y
    bool equals(const StringView& y) const;
    //@}
    
    /// \name Cloning
    //@{
    /// Update this view to be a clone of view \a y
    void update(Space& home, const DashedString& d);
    void update(Space& home, const StringView& y);
    //@}
    
    /// \name Domain update by equation: FIXME: Check if needed
    //@{
    /// Equates the domain with string \a w.
    ModEvent equate(Space& home, const std::vector<int>& w);
    /// Equates the domain with block \a b.
    ModEvent equate(Space& home, const Block& b);
    /// Equates with dashed string \a x.
    ModEvent equate(Space& home, const DashedString& x);
    //@}
    
    /// \name Domain update by cardinality refinement: FIXME: Check if needed
    //@{
    /// Possibly update the lower bound of the blocks, knowing that the minimum 
    /// length for any string in the domain is \a l
    ModEvent min_length(Space& home, int l);
    /// Possibly update the upper bound of the blocks, knowing that the maximum 
    /// length for any string in the domain is \a u
    ModEvent max_length(Space& home, int u);
    /// Possibly update the upper bound of the blocks, knowing that the minimum 
    /// length for any string in the domain is \a l and the maximum length for 
    /// any string in the domain is \a u
    ModEvent bnd_length(Space& home, int l, int u);
    //@}
    
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
        
  };
  /**
   * \brief Print string variable view
   * \relates Gecode::String::StringView
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringView& x);



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
    /// Construct with \a w as domain (n = |w|).
    ConstStringView(Space& home, int w[], int n);
    /// Return the length of the string (for compatibility with other views)
    int min_length(void) const;
    /// Return the length of the string (for compatibility with other views)
    int max_length(void) const;
    /// Return the length of the string
    int size(void) const;
    /// Returns the i-th character of the string
    int operator[](int i) const;
    /// Return the value of this view
    std::vector<int> val(void) const;

    /// Always returns true (for compatibility with other views)
    bool assigned(void) const;
    /// If this view contains y
    bool contains(const StringView& y) const;
    /// If this view is equals to y
    bool equals(const StringView& y) const;
    
    
  };
  /**
   * \brief Print string variable view
   * \relates Gecode::String::StringView
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConstStringView& x);
  
  class ReverseView : public StringView {
    
//    class SweepFwdIterator   : public StringView::SweepFwdIterator {};
//    class PushBwdIterator    : public StringView::PushBwdIterator {};
//    class StretchBwdIterator : public StringView::StretchBwdIterator {};
    
//    SweepFwdIterator sweep_fwd_iterator(void);
//    PushBwdIterator push_bwd_iterator(void);
//    StretchBwdIterator stretch_bwd_iterator(void);
    
  };

}}

#include <gecode/string/var/string.hpp>
#include <gecode/string/view/string.hpp>
#include <gecode/string/view/const.hpp>
