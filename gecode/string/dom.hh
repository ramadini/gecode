#ifndef __GECODE_STRING_DOM_HH__
#define __GECODE_STRING_DOM_HH__

namespace Gecode { namespace String {

  /// Shared immutable domain for a domain propagator
  class DomDomain : public SharedHandle::Object {
  public:
    NSBlocks blocks;
    DomDomain(const NSBlocks& blocks0) : blocks(blocks0) {}
  };

  /// Handle for a shared domain propagator domain
  class DomDomainHandle : public SharedHandle {
  public:
    DomDomainHandle(DomDomain* domain) : SharedHandle(domain) {}
    DomDomainHandle(const DomDomainHandle& domain) : SharedHandle(domain) {}
    const NSBlocks& operator *(void) const {
      return static_cast<DomDomain*>(object())->blocks;
    }
  };

  /**
   * \brief %Propagator for string domain
   *
   */
  class Dom : public UnaryPropagator<StringView, PC_STRING_DOM> {
  private:
    DomDomainHandle dom;
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
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Post propagator \f$ x=y \f$
    static  ExecStatus post(Home home, StringView x, NSBlocks& d);
  };

}}

#include <gecode/string/rel/dom.hpp>

#endif
