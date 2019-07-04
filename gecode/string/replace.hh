#ifndef __GECODE_STRING_REPLACE_HH__
#define __GECODE_STRING_REPLACE_HH__

namespace Gecode { namespace String {

  class Replace : public NaryPropagator<StringView, PC_STRING_DOM> {
  protected:
    using NaryPropagator<StringView, PC_STRING_DOM>::x;
    /// Constructor for cloning \a p
    Replace(Space& home, Replace& p);
    /// Constructor for posting
    Replace(Home home, ViewArray<StringView>& x);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator string replace
    static  ExecStatus post(Home home, ViewArray<StringView>& x);
  };

}}

#include <gecode/string/rel-op/replace.hpp>

#endif
