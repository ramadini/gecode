#ifndef __GECODE_STRING_INC_HH__
#define __GECODE_STRING_INC_HH__

namespace Gecode { namespace String {
  
  class Inc : public UnaryPropagator <StringView, PC_STRING_DOM> {
  private:
    bool strict;
  protected:
    using UnaryPropagator<StringView, PC_STRING_DOM>::x0;
    /// Constructor for cloning \a p
    Inc(Space& home, Inc& p);
    /// Constructor for posting
    Inc(Home home, StringView x, bool lt);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator
    static  ExecStatus post(Home home, StringView x, bool lt);
  };

}}

#include <gecode/string/rel/inc.hpp>

#endif
