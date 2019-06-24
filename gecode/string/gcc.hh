#ifndef __GECODE_STRING_GCC_HH__
#define __GECODE_STRING_GCC_HH__

namespace Gecode { namespace String {

  class GCC :
  public MixNaryOnePropagator
  <Gecode::Int::IntView, Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> {
  private:
    const vec2 A;
    NSIntSet C;
  protected:
    using MixNaryOnePropagator
      <Gecode::Int::IntView, Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> 
        ::x;
    using MixNaryOnePropagator
      <Gecode::Int::IntView, Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> 
        ::y;
    /// Constructor for cloning \a p
    GCC(Space& home, GCC& p);
    /// Constructor for posting
    GCC(Home home, StringView, const vec2&, ViewArray<Gecode::Int::IntView>&);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ gcc(x, a, n) \f$
    static  ExecStatus post(
      Home, StringView, const vec2&, ViewArray<Gecode::Int::IntView>
    );
  };

}}

#include <gecode/string/global/gcc.hpp>

#endif
