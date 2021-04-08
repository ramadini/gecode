#ifndef __GECODE_STRING_CONCAT_HH__
#define __GECODE_STRING_CONCAT_HH__

namespace Gecode { namespace String {

  /**
   * \brief %Propagator for string concatenation
   *
   */
  class Concat :
  public MixTernaryPropagator
    <StringView, PC_STRING_DOM, StringView, PC_STRING_DOM, StringView, PC_STRING_DOM> {
  protected:
    using MixTernaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM,
                               StringView, PC_STRING_DOM>::x0;
    using MixTernaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM,
                               StringView, PC_STRING_DOM>::x1;
    using MixTernaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM,
                               StringView, PC_STRING_DOM>::x2;
    /// Constructor for cloning \a p
    Concat(Space& home, Concat& p);
    /// Constructor for posting
    Concat(Home home, StringView x0, StringView x1, StringView x2);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ z=x \cdot y \f$
    static  ExecStatus post(Home home, StringView x, StringView y, StringView z);
  };

}}

#include <gecode/string/rel-op/concat.hpp>

#endif
