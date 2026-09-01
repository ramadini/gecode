#ifndef __GECODE_STRING_UCASE_HH__
#define __GECODE_STRING_UCASE_HH__

namespace Gecode { namespace String {

  /**
  * \brief %Propagator for string reverse
  *
  */
  class UpperCase :
  public MixBinaryPropagator
    <StringView, PC_STRING_DOM, StringView, PC_STRING_DOM> {
  protected:
    using MixBinaryPropagator
    <StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>::x0;
    using MixBinaryPropagator
    <StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>::x1;
    /// Constructor for cloning \a p
    UpperCase(Space& home, UpperCase& p);
    /// Constructor for posting
    UpperCase(Home home, StringView x0, StringView x1);
  public:
    static NSIntSet _LCASE_SET;
    static NSIntSet _UCASE_SET;
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ z=x \cdot y \f$
    static  ExecStatus post(Home home, StringView x, StringView y);
  };

  NSIntSet UpperCase::_LCASE_SET = NSIntSet(char2int('a'), char2int('z'));
  NSIntSet UpperCase::_UCASE_SET = NSIntSet(char2int('A'), char2int('Z'));
}}

#include <gecode/string/rel-op/ucase.hpp>

#endif
