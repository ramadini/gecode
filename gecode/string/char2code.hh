#ifndef __GECODE_STRING_CHAR2CODE_HH__
#define __GECODE_STRING_CHAR2CODE_HH__

namespace Gecode { namespace String {

  /**
    * \brief %Propagator linking an optional character to its integer code.
   *
   */
  class Char2Code :
    public MixBinaryPropagator
    <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM> {
  protected:
    using MixBinaryPropagator
    <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM>
      ::x0;
    using MixBinaryPropagator
    <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM>
      ::x1;
    /// Constructor for cloning \a p
    Char2Code(Space& home, Char2Code& p);
    /// Constructor for posting
    Char2Code(Home home, StringView, Gecode::Int::IntView);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator linking an optional character to its integer code
    static ExecStatus post(Home home, StringView s, Gecode::Int::IntView x);
  };

}}

#include <gecode/string/char2code/char2code.hpp>

#endif
