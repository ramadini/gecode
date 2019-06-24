#ifndef __GECODE_STRING_NQ_HH__
#define __GECODE_STRING_NQ_HH__

namespace Gecode { namespace String {

  /**
   * \brief %Propagator for string inequality
   *
   */
  class Nq : public MixBinaryPropagator
  <StringView, PC_STRING_VAL, StringView, PC_STRING_VAL> {
  protected:
    using MixBinaryPropagator
    <StringView, PC_STRING_VAL, StringView, PC_STRING_VAL>:: x0;
    using MixBinaryPropagator
    <StringView, PC_STRING_VAL, StringView, PC_STRING_VAL>:: x1;
    /// Constructor for cloning \a p
    Nq(Space& home, bool share, Nq& p);
    /// Constructor for posting
    Nq(Home home, StringView x0, StringView x1);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home, bool share);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ x \neq y \f$
    static  ExecStatus post(Home home, StringView x, StringView y);
  };

}}

#include <gecode/string/rel/nq.hpp>

#endif
