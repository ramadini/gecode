#ifndef __GECODE_STRING_DOM_HH__
#define __GECODE_STRING_DOM_HH__

namespace Gecode { namespace String {

  /**
   * \brief %Propagator for string domain
   *
   */
  class Dom : public UnaryPropagator<StringView, PC_STRING_DOM> {
  private:
    const NSBlocks* dom;
  protected:
    using UnaryPropagator<StringView, PC_STRING_DOM>::x0;
    /// Constructor for cloning \a p
    Dom(Space& home, Dom& p);
    /// Constructor for posting
    Dom(Home home, StringView, NSBlocks& p);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ x=y \f$
    static  ExecStatus post(Home home, StringView x, NSBlocks& d);
    ~Dom();
  };

}}

#include <gecode/string/rel/dom.hpp>

#endif
