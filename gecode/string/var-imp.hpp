namespace Gecode { namespace String {

/**
   * \brief String delta information for advisors.
   */
  class StringDelta : public Delta {

    friend class StringVarImp;

  private:

    bool _changed; ///< Domain changed

  public:
    /// Create string delta as providing no information
    StringDelta(void);
    /// Create string delta with \a ch
    StringDelta(bool ch);
  private:
    /// Test whether any domain change has happened
    bool any(void) const;
  };

}}

#include <gecode/string/var-imp/delta.hpp>
#include <gecode/string/dashed/dashed-string.hh>

namespace Gecode { namespace String {

  /**
   * \brief String variable implementation
   *
   * Dashed-domain methods only mutate storage and set a change marker.
   * StringVarImp wraps each mutation in a DomainState and emits at most one
   * aggregate value, length, or domain event per affected variable.
   *
   * \ingroup Other
   */
  class StringVarImp : public StringVarImpBase {

  friend class StringView;

  public:
    /**
     * \brief Snapshot token for one domain-mutation transaction
     *
     * Construction records aggregate length bounds and assignment state, then
     * clears the dashed domain's change marker. Tokens are tied to exactly one
     * StringVarImp and are committed by that implementation.
     */
    class DomainState {
      friend class StringVarImp;
      friend class StringView;
    private:
      StringVarImp* x;
      int min_length;
      int max_length;
      bool unassigned;
      DomainState(StringVarImp& x);
      bool modified(void) const;
    };

  private:

    /// Classify and combine one transaction's aggregate modification event
    ModEvent notify(Space& home, ModEvent me, const DomainState& state);

    /// Notify after a mutation, classifying its aggregate length bounds
    ModEvent notify(Space& home, int old_min_length, int old_max_length,
                    Delta& d);

    /// Notify after a mutation with changed delta information
    ModEvent notify(Space& home, int old_min_length, int old_max_length);

  protected:

    DashedString ds;

    /// Constructor for cloning \a x
    GECODE_STRING_EXPORT StringVarImp(Space& home, StringVarImp& x);

  public:
    /// Initialize the top domain
    StringVarImp(Space& home);

    /// Initialize with aggregate length bounds [\a a,\a b]
    StringVarImp(Space& home, int a, int b);

    /// Initialize from a legacy one-byte-per-symbol value
    StringVarImp(Space& home, string s);

    /// Initialize from an encoding-neutral scalar-value sequence
    StringVarImp(Space& home, const StringVal& value);

    /// Initialize from temporary blocks and aggregate length bounds
    StringVarImp(Space& home, NSBlocks& v, int mil, int mal);

    /// Initialize from active alphabet \a s and length bounds [\a a,\a b]
    StringVarImp(Space& home, const NSIntSet& s, int a, int b);

    /// Return assigned value (only if assigned)
    string val(void) const;

    /// Return assigned encoding-neutral value (only if assigned)
    StringVal val_symbols(void) const;

    /// Return immutable dashed-domain storage
    const DashedString& domain(void) const;

    /// Return minimum represented string length
    int min_length(void) const;

    /// Return maximum represented string length
    int max_length(void) const;

    /// \name Domain tests
    //@{
    /// Test whether variable is assigned
    bool assigned(void) const;

    /// Apply one archived branch alternative and emit its aggregate event
    ModEvent commit(
      Space&, Branch::Level, Branch::Value, Branch::Block, int, unsigned);

    /// Intersect with ordinary-lifetime dashed domain \a d
    ModEvent dom(Space& home, const NSBlocks& d);

    /**
     * \brief Commit an already-analyzed normalized refinement \a d
     *
     * The caller must ensure that \a d is a subset of the current domain.
     * This method replaces storage and emits one aggregate event.
     */
    ModEvent refine(Space& home, const NSBlocks& d);

  private:

    /// Begin an in-place domain refinement
    DomainState begin_refinement(void);

    /// Commit an in-place domain refinement as one aggregate event
    ModEvent commit_refinement(Space& home, const DomainState& state);

  public:

    /// Enforce equality with variable implementation \a x
    ModEvent eq(Space& home, StringVarImp* x);

    /// Enforce equality with legacy byte value \a x
    ModEvent eq(Space& home, string x);

    /// Enforce equality with encoding-neutral value \a value
    ModEvent eq(Space& home, const StringVal& value);

    /// Enforce strict/non-strict lexicographic order before \a x
    ModEvent lex(Space& home, StringVarImp* x, bool lt);

    /// Enforce strictly/non-strictly increasing symbols
    ModEvent inc(Space& home, bool lt);

    /// Raise the aggregate minimum length to \a l
    ModEvent lb(Space& home, int l);

    /// Lower the aggregate maximum length to \a u
    ModEvent ub(Space& home, int u);

    /// Enforce this = \a x concatenated with \a y
    ModEvent concat(Space& home, StringVarImp* x, StringVarImp* y);

    /// Enforce this as the concatenation of \a x
    ModEvent gconcat(Space& home, vec<StringVarImp*> x);

    /// Enforce this = \a x repeated between \a l and \a u times
    ModEvent pow(Space& home, StringVarImp* x, int& l, int& u);

    /// Refine a find relation with one-based position bounds [\a l,\a u]
    ModEvent find(Space& home, StringVarImp* x, int& l, int& u, bool b);

    /// Enforce this = reverse(\a x)
    ModEvent rev(Space& home, StringVarImp* x);

    /// Test whether delta \a d describes a domain change
    static bool any(const Delta& d);

    /// \name Dependencies
    //@{
    /**
     * \brief Subscribe propagator \a p with propagation condition \a pc to
     * variable
     *
     * In case \a schedule is false, the propagator is just subscribed but not
     * scheduled for execution (this must be used when creating subscriptions
     * during propagation).
     *
     */
    void subscribe(Space& home, Propagator& p, PropCond pc, bool schedule);
    /// Cancel subscription of propagator \a p with propagation condition \a pc
    void cancel(Space& home, Propagator& p, PropCond pc);
    /// Re-schedule propagator \a p
    void reschedule(Space& home, Propagator& p, PropCond pc);
    /// Subscribe advisor \a a
    void subscribe(Space& home, Advisor& a, bool fail);
    /// Cancel subscription of advisor \a a
    void cancel(Space& home, Advisor& a, bool fail);
    //@}

    /// \name Cloning
    //@{
    /// Return copy of this variable
    StringVarImp* copy(Space& home);
    //@}

    };
  }}

#include <gecode/string/var-imp/string.hpp>

// STATISTICS: string-var
