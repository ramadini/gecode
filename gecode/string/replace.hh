#ifndef __GECODE_STRING_REPLACE_HH__
#define __GECODE_STRING_REPLACE_HH__

namespace Gecode { namespace String {

  class Replace : public NaryPropagator<StringView, PC_STRING_DOM> {
    int lb_card(int, const Position&, const Position&) const;
    int ub_card(int, const Position&, const Position&) const;
    NSBlocks prefix(int, const Position&) const;
    NSBlocks suffix(int, const Position&) const;
    NSBlocks pref(int k, const Position& p) const;
    NSBlocks suff(int k, const Position& p) const;
    ExecStatus decomp_all(Space&);
    ExecStatus replace_q_x(Space&, int);
    ExecStatus replace_q1_y(Space&, int);
    ModEvent refine_card(Space&);
    int occur(const string& q) const;
    bool check_card() const;
    bool all;
    bool last;
  protected:
    using NaryPropagator<StringView, PC_STRING_DOM>::x;
    /// Constructor for cloning \a p
    Replace(Space& home, Replace& p);
    /// Constructor for posting
    Replace(
      Home home, ViewArray<StringView>& x, bool all = false, bool last = false
    );
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator string replace
    static  ExecStatus post(
      Home, ViewArray<StringView>&, bool all = false, bool last = false
    );
  };

}}

#include <gecode/string/rel-op/replace.hpp>

#endif
