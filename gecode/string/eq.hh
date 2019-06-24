#ifndef __GECODE_STRING_EQ_HH__
#define __GECODE_STRING_EQ_HH__

namespace Gecode { namespace String {

  /**
   * \brief %Propagator for string equality
   *
   */
  class Eq :
  public MixBinaryPropagator
  <StringView, PC_STRING_DOM, StringView, PC_STRING_DOM> {
  protected:
    using
    MixBinaryPropagator<StringView,PC_STRING_DOM,StringView,PC_STRING_DOM>:: x0;
    using 
    MixBinaryPropagator<StringView,PC_STRING_DOM,StringView,PC_STRING_DOM>:: x1;
    /// Constructor for cloning \a p
    Eq(Space& home, bool share, Eq& p);
    /// Constructor for posting
    Eq(Home home, StringView x0, StringView x1);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home, bool share);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ x=y \f$
    static  ExecStatus post(Home home, StringView x, StringView y);
  };

}}

#include <gecode/string/rel/eq.hpp>

#endif
