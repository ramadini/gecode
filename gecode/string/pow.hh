#ifndef __GECODE_STRING_POW_HH__
#define __GECODE_STRING_POW_HH__

namespace Gecode { namespace String {

  /**
   * \brief %Propagator for iterated concatenation.
   *
   */
  class Pow : public MixTernaryPropagator<StringView, PC_STRING_DOM,
    Gecode::Int::IntView, Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> {
  protected:
    using MixTernaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView,
      Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM>::x0;
    using MixTernaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView,
      Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM>::x1;
    using MixTernaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView,
      Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM>::x2;
    /// Constructor for cloning \a p
    Pow(Space& home, Pow& p);
    /// Constructor for posting
    Pow(Home home, StringView, Gecode::Int::IntView, StringView);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator for \f$ |s|=x \f$
    static ExecStatus post(Home, StringView, Gecode::Int::IntView, StringView);
  };

}}

#include <gecode/string/int/pow.hpp>

#endif
