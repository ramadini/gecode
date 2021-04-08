#ifndef __GECODE_STRING_GCONCAT_HH__
#define __GECODE_STRING_GCONCAT_HH__

namespace Gecode { namespace String {

  class GConcat : public NaryOnePropagator<StringView, PC_STRING_DOM> {
  private:
    bool refine_card(Space&);
  protected:
    using NaryOnePropagator<StringView, PC_STRING_DOM>::x;
    using NaryOnePropagator<StringView, PC_STRING_DOM>::y;
    /// Constructor for cloning \a p
    GConcat(Space& home, GConcat& p);
    /// Constructor for posting
    GConcat(Home home, ViewArray<StringView>& x, StringView y);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ y = x_1 + \dots + x_n \f$
    static  ExecStatus post(Home home, ViewArray<StringView>& x, StringView y);
  };

}}

#include <gecode/string/global/gconcat.hpp>

#endif
