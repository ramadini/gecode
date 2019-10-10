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
   * \ingroup Other
   */
  class StringVarImp : public StringVarImpBase {

  friend class StringView;

  protected:

    DashedString ds;

    /// Constructor for cloning \a x
    GECODE_STRING_EXPORT StringVarImp(Space& home, StringVarImp& x);

  public:
    /// Initialize with domain
    StringVarImp(Space& home);

    StringVarImp(Space& home, int a, int b);

    StringVarImp(Space& home, string s);

    StringVarImp(Space& home, NSBlocks& v, int mil, int mal);

    StringVarImp(Space& home, const NSIntSet& s, int a, int b);

    /// Return assigned value (only if assigned)
    string val(void) const;

    const DashedString& domain(void) const;

    int min_length(void) const;

    int max_length(void) const;

    int must_chars(void) const;

    int may_chars(void) const;

    /// \name Domain tests
    //@{
    /// Test whether variable is assigned
    bool assigned(void) const;

    ModEvent commit(Space&, Branch::Level, Branch::Value, Branch::Block, unsigned);

    ModEvent dom(Space& home, const NSBlocks& d);

    ModEvent eq(Space& home, StringVarImp* x);

    ModEvent eq(Space& home, string x);

    ModEvent lex(Space& home, StringVarImp* x, bool lt);

    ModEvent inc(Space& home, bool lt);

    ModEvent lb(Space& home, int l);

    ModEvent ub(Space& home, int u);

    ModEvent concat(Space& home, StringVarImp* x, StringVarImp* y);

    ModEvent gconcat(Space& home, vec<StringVarImp*> x);

    ModEvent pow(Space& home, StringVarImp* x, int& l, int& u);

    ModEvent find(Space& home, StringVarImp* x, int& l, int& u, bool b);

    ModEvent rev(Space& home, StringVarImp* x);

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
    /// Cancel subscription of advisor \a a
    void cancel(Space& home, Advisor& a);
    //@}

    /// \name Cloning
    //@{
    /// Return copy of this variable
    StringVarImp* copy(Space& home);
    //@}

    };
  }}

#include <gecode/string/var-imp/string.hpp>
