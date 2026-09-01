#ifndef __GECODE_STRING_REV_HH__
#define __GECODE_STRING_REV_HH__

namespace Gecode { namespace String {

  /**
  * \brief %Propagator for string reverse
  *
  */
  class Rev : public MixBinaryPropagator
    <StringView, PC_STRING_DOM, StringView, PC_STRING_DOM> {
  protected:
    using MixBinaryPropagator
    <StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>::x0;
    using MixBinaryPropagator
    <StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>::x1;

    /// Constructor for cloning \a p
    Rev(Space& home, Rev& p);
    /// Constructor for posting
    Rev(Home home, StringView x0, StringView x1);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ z=x \cdot y \f$
    static  ExecStatus post(Home home, StringView x, StringView y);
  };


}}

#include <gecode/string/rel-op/rev.hpp>

#endif
