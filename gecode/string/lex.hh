#ifndef __GECODE_STRING_LEX_HH__
#define __GECODE_STRING_LEX_HH__

namespace Gecode { namespace String {

  class Lex :
  public MixBinaryPropagator
  <StringView, PC_STRING_DOM, StringView, PC_STRING_DOM> {
  private:
    bool lt;
  protected:
    using MixBinaryPropagator
      <StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>::x0;
    using MixBinaryPropagator
      <StringView, PC_STRING_DOM, StringView, PC_STRING_DOM>::x1;
    /// Constructor for cloning \a p
    Lex(Space& home, bool share, Lex& p);
    /// Constructor for posting
    Lex(Home home, StringView x0, StringView x1, bool eq);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home, bool share);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ x \neq y \f$
    static  ExecStatus post(Home home, StringView x, StringView y, bool eq);
  };

}}

#include <gecode/string/rel/lex.hpp>

#endif
