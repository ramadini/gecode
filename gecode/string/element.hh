#ifndef __GECODE_STRING_ELEMENT_HH__
#define __GECODE_STRING_ELEMENT_HH__

namespace Gecode { namespace String {

  using Gecode::Int::IntView;

  /**
   * \brief %Propagator for element.
   *
   */
  class Element : public MixNaryOnePropagator
  <StringView, PC_STRING_DOM, IntView, Gecode::Int::PC_INT_BND> {
  protected:
    using MixNaryOnePropagator
      <StringView, PC_STRING_DOM, IntView, Gecode::Int::PC_INT_BND>::x;
    using MixNaryOnePropagator
      <StringView, PC_STRING_DOM, IntView, Gecode::Int::PC_INT_BND>::y;
    /// Constructor for cloning \a p
    Element(Space& home, bool share, Element& p);
    /// Constructor for posting
    Element(Home home, ViewArray<StringView> v, IntView i);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home, bool share);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator
    static  ExecStatus post(Home, ViewArray<StringView>, IntView, StringView);
  };

}}

#include <gecode/string/global/element.hpp>

#endif
