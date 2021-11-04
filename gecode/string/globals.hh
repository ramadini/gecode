#ifndef __GECODE_STRING_GLOBALS_HH__
#define __GECODE_STRING_GLOBALS_HH__

#include <gecode/string.hh>

namespace Gecode { namespace String { namespace Globals {

  /**
   * \namespace Gecode::String::Globals
   * \brief Propagators for global constraints involving strings
   */

  /**
   * \brief %Propagator for element
   *
   * Requires \code #include <gecode/string/string.hh> \endcode
   * \ingroup FuncStringProp
   */
  template<class View>
  class Element : public MixNaryOnePropagator
  <View, PC_STRING_CARD, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> {
  protected:
    // x[0] = [x[1], ..., x[n]][y].
    using MixNaryOnePropagator
      <View, PC_STRING_CARD, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>::x;
    using MixNaryOnePropagator
      <View, PC_STRING_CARD, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>::y;
    /// Constructor for cloning \a p
    Element(Space& home, Element& p);
    /// Constructor for posting
    Element(Home home, ViewArray<View> v, Gecode::Int::IntView i);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator
    static  ExecStatus post(Home, ViewArray<View>, Gecode::Int::IntView, View);
  };

}}}

#include <gecode/string/globals/element.hpp>
#endif

// STATISTICS: string-prop
