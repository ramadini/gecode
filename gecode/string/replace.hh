#ifndef __GECODE_STRING_REPLACE_HH__
#define __GECODE_STRING_REPLACE_HH__

namespace Gecode { namespace String {

  class Replace : public NaryPropagator<StringView, PC_STRING_DOM> {
    NSBlock crush(int, const Position&, const Position&) const;
    NSBlocks prefix(int, const Position&) const;
    NSBlocks suffix(int, const Position&) const;
    ExecStatus replace_all(Space&);
    ModEvent refine_card(Space&);
    bool all;
  protected:
    using NaryPropagator<StringView, PC_STRING_DOM>::x;
    /// Constructor for cloning \a p
    Replace(Space& home, Replace& p);
    /// Constructor for posting
    Replace(Home home, ViewArray<StringView>& x, bool all = false);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator string replace
    static  ExecStatus post(Home, ViewArray<StringView>&, bool all = false);
  };

}}

#include <gecode/string/rel-op/replace.hpp>

#endif
