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
    /// \name Constructors and initialization
    //@{
    /// Default constructor
    StringView(void);
    /// Initialize from set variable \a y
    StringView(const StringVar& y);
    /// Initialize from set variable implementation \a y
    StringView(StringVarImp* y);
    //@}

    //@}
    /// \name Value access
    //@{
    /// Return the minimum length for a string in the variable's domain
    int lenMin(void) const;
    /// Return the maximum length for a string in the variable's domain
    int lenMax(void) const;
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

}}
