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
      virtual bool hasNext(void) const = 0;
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
      bool hasNext(void) const;
      void consume(int k);
      void consumeMand(int k);
      int must_consume(void) const;
      int may_consume(void) const;
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
      bool hasNext(void) const;
      void consume(int k);
      void consumeMand(int k);
      int must_consume(void) const;
      int may_consume(void) const;
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
    SweepFwdIterator fwd_iterator(void);
    SweepBwdIterator bwd_iterator(void);
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
    ModEvent lengthMin(Space& home, int l);
    /// Possibly update the upper bound of the blocks, knowing that the maximum 
    /// length for any string in the domain is \a u
    ModEvent lengthMax(Space& home, int u);
    /// Possibly update the cardinality of the blocks, knowing that the length 
    /// for any string in the domain is between \a l and \a u
    ModEvent lengthIn(Space& home, int l, int u);
    //@}
    
    /// Returns the length of the longest common prefix for all the strings 
    /// denoted by this view
    int lcp_length(void) const;
    /// Returns the length of the longest common suffix for all the strings 
    /// denoted by this view
    int lcs_length(void) const;
    /// Returns true if p precedes q according to this view.
    bool prec(const Position& p, const Position& q) const;
    ///TODO:
    int ub_new_blocks(const Matching& m) const;
    
    /// TODO:
    int min_len_mand(const Block& bx, const Position& lsp, 
                                      const Position& eep) const;
    /// TODO:             
    int max_len_opt(const Block& bx, const Position& esp, 
                                     const Position& lep, int l) const;
    /// Normalize this view
    void normalize(Space& home);
    
    
    /// Possibly refines block \a bx according to matching region \a m, having
    /// at least \l characters, defined between \a le and \a ee in this view.
    /// If \a bx must be unfolded in k > 1 blocks b_1 b_2 ... b_k, then bx is 
    /// updated with b_1, \a n will be set to k-1 and \a r will store the 
    /// dashed string norm(b_2, ..., b_k)
    BlockEvent
    refine_mand(Space& home, Block& bx, const Matching& m, int l, int& n, 
                Region r);
    
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
    // TODO:
  };
  
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
