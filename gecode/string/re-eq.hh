#ifndef __GECODE_STRING_RE_EQ_HH__
#define __GECODE_STRING_RE_EQ_HH__

namespace Gecode { namespace String {

  /**
   * \brief %Reified equality propagator
   *
   */
  template<class CtrlView, ReifyMode rm>
  class ReEq : public Propagator {
  protected:
    StringView x0;
    StringView x1;
    CtrlView b;
    /// Constructor for cloning \a p
    ReEq(Space& home, bool share, ReEq&);
    /// Constructor for posting
    ReEq(Home home, StringView x0, StringView x1, CtrlView b);
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
    static ExecStatus post(Home home, StringView x, StringView y, CtrlView b);
  };

}}

#include <gecode/string/rel/re-eq.hpp>

#endif
