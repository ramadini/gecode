namespace Gecode { namespace String {

  /**
   * \brief Propagator view of a StringVar
   *
   * Public mutation methods delegate to StringVarImp, which classifies and
   * schedules one aggregate event per domain transaction. Mutable dashed
   * storage is deliberately restricted to the audited refinement protocol
   * used by GCC, Find, and decomposed regular-star propagation.
   */
  class StringView : public VarImpView<StringVar> {

  protected:

    using VarImpView<StringVar>::x;

  private:

    /// Begin an audited in-place domain refinement
    StringVarImp::DomainState begin_refinement(void) const;

    /// Return mutable storage associated with refinement transaction \a state
    String::DashedString& mutable_domain(
      const StringVarImp::DomainState& state) const;

    /// Commit an audited in-place domain refinement
    ModEvent commit_refinement(
      Space& home, const StringVarImp::DomainState& state) const;

    friend class Find;
    friend class GCC;
    friend class StarEx;

  public:

    /// Default constructor
    StringView(void);

    /// Initialize from string variable \a y
    StringView(const StringVar& y);

    /// Initialize from string variable implementation \a y
    StringView(StringVarImp* y);

    /// Return assigned legacy bytes
    string val() const;

    /// Return assigned encoding-neutral scalar values
    StringVal val_symbols() const;

    /// Return the immutable dashed domain
    const String::DashedString& domain(void) const;

    /// Test whether both views refer to the same variable implementation
    bool same(const StringView& that) const;

    /// Test whether equality with \a that is feasible
    bool check_equate(const StringView& that) const;

    int min_length(void) const;

    int max_length(void) const;

    NSIntSet may_chars(void) const;

    NSIntSet must_chars(void) const;

    /// Test whether this domain contains every value represented by \a s
    bool contains(const StringView& s) const;

    template<class Char, class Traits>
    friend std::basic_ostream<Char,Traits>&
    operator <<(std::basic_ostream<Char,Traits>& os, const StringView& x);

    /// Intersect the domain with ordinary-lifetime dashed domain \a d
    ModEvent dom(Space& home, const NSBlocks& d);

    /// Enforce equality with legacy byte value \a s
    ModEvent eq(Space& home, string s);

    /// Enforce equality with encoding-neutral value \a value
    ModEvent eq(Space& home, const StringVal& value);

    /// Enforce equality with \a y
    ModEvent eq(Space& home, StringView& y);

    /// Enforce strict/non-strict lexicographic order before \a y
    ModEvent lex(Space& home, StringView& y, bool lt);

    /// Enforce strictly/non-strictly increasing symbols
    ModEvent inc(Space& home, bool lt);

    /// Apply one archived branch alternative
    ModEvent commit(
      Space&, Branch::Level, Branch::Value, Branch::Block, int, unsigned);

    /// Raise the aggregate minimum length to \a l
    ModEvent lb(Space& home, int l);

    /// Lower the aggregate maximum length to \a u
    ModEvent ub(Space& home, int u);

    /// Enforce this = \a x concatenated with \a y
    ModEvent concat(Space& home, StringView& x, StringView& y);

    /// Enforce this as the concatenation of \a x
    ModEvent gconcat(Space& home, ViewArray<StringView>& x);

    /// Enforce this = \a x repeated between \a l and \a u times
    ModEvent pow(Space& home, StringView& x, int& l, int& u);

    /// Refine a find relation with one-based position bounds [\a l,\a u]
    ModEvent find(Space& home, StringView& x, int& l, int& u, bool b);

    /// Enforce this = reverse(\a y)
    ModEvent rev(Space& home, StringView& y);

    /// Test whether delta \a d describes a domain change
    bool any(const Delta& d) const;

    /// Update this view while cloning \a y into \a home
    void update(Space& home, StringView& y);

  };

  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringView& x);

}}

#include <gecode/string/view/string.hpp>

// STATISTICS: string-var
