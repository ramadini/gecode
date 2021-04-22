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
    public:
      /// Constructor
      SweepIterator(const StringView& x, const Position& p);
      /// Move iterator to the beginning of the next block (if possible)
      virtual void next(void) = 0;
      /// Test whether iterator is still within the dashed string or done
      virtual bool operator ()(void) const = 0;
      /// Consume \a k characters from current position
      virtual void consume(int k) = 0;
      /// Returns const reference to the current position
      const Position& operator *(void);
      /// Return the lower bound of the current block
      int lb(void) const;
      /// Return the upper bound of the current block
      int ub(void) const;
      /// Check if the base of the current block is disjoint with that of \a b
      bool disj(const Block& b) const;
    };
    /// Iterator for pushing/stretching forwards
    struct SweepFwdIterator : public SweepIterator {
      SweepFwdIterator(const StringView& x);
      SweepFwdIterator(const StringView& x, const Position& p);
      void next(void);
      bool operator ()(void) const;
      void consume(int k);
      /// Min. no. of chars that must be consumed from current position within current block
      int must_consume(void) const;
      /// Max. no. of chars that may be consumed from current position within current block
      int may_consume(void) const;
    };
    /// Iterator for pushing backwards
    struct PushBwdIterator : public SweepIterator {
      PushBwdIterator(const StringView& x);
      PushBwdIterator(const StringView& x, const Position& p);
      void next(void);
      bool operator ()(void) const;
      void consume(int k);
      /// Max. consumable characters from current position within current block
      int may_consume(void) const;
    };
    /// Iterator for stretching backwards
    struct StretchBwdIterator : public SweepIterator {
      StretchBwdIterator(const StringView& x);
      StretchBwdIterator(const StringView& x, const Position& p);
      void next(void);
      bool operator ()(void) const;
      /// Min. no. of chars that must be consumed from current position within current block
      int must_consume(void) const;
      void consume(int k);
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
    SweepFwdIterator sweep_fwd_iterator(void);
    PushBwdIterator push_bwd_iterator(void);
    StretchBwdIterator stretch_bwd_iterator(void);
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
    /// Returns the i-th block of the domain
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
    
    /// \name Domain update by equation
    //@{
    /// Equates the domain with string \a w.
    ModEvent equate(Space& home, const std::vector<int>& w);
    /// Equates the domain with block \a b.
    ModEvent equate(Space& home, const Block& b);
    /// Equates with dashed string \a x.
    ModEvent equate(Space& home, const DashedString& x);
    //@}
    
    /// \name Domain update by cardinality refinement
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
