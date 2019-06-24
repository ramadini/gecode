#ifndef __GECODE_STRING_RE_LEX_HH__
#define __GECODE_STRING_RE_LEX_HH__

namespace Gecode { namespace String {

  /**
   * \brief %Reified equality propagator
   *
   */
  template<class CtrlView, ReifyMode rm>
  class ReLex : public Propagator {
  private:
    bool lt;
  protected:
    StringView x0;
    StringView x1;
    CtrlView b;
    /// Constructor for cloning \a p
    ReLex(Space& home, ReLex&);
    /// Constructor for posting
    ReLex(Home home, StringView x0, StringView x1, CtrlView b, bool strict);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home,bool);
    /// Cost function (defined as PC_TERNARY_LO)
    virtual PropCost cost(const Space& home, const ModEventDelta& med) const;
    /// Schedule function
    virtual void reschedule(Space& home);
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator for \f$ (x=y) \Leftrightarrow b\f$
    static ExecStatus post(Home, StringView, StringView, CtrlView, bool);
  };

}}

#include <gecode/string/rel/re-lex.hpp>

#endif
