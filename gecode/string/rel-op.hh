#ifndef __GECODE_STRING_RELOP_HH__
#define __GECODE_STRING_RELOP_HH__

#include <gecode/string.hh>

namespace Gecode { namespace String { namespace RelOp {

  /**
   * \namespace Gecode::String::RelOp
   * \brief Standard set operation propagators
   */

  /**
   * \brief %Propagator for ternary string concatenation
   *
   * Requires \code #include <gecode/string/rel-op.hh> \endcode
   * \ingroup FuncStringProp
   */
  template<class View0, class View1, class View2>
  class Concat:
    public MixTernaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY,
                                View2,PC_STRING_ANY> {
  protected:
    using MixTernaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY,
                               View2,PC_STRING_ANY>::x0;
    using MixTernaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY,
                               View2,PC_STRING_ANY>::x1;
    using MixTernaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY,
                               View2,PC_STRING_ANY>::x2;
    /// Constructor for cloning \a p
    Concat(Space& home, Concat& p);
    /// Constructor for posting
    Concat(Home home,View0,View1,View2);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus  propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ z=x \cdot y\f$
    static  ExecStatus  post(Home home,View0 x,View1 y,View2 z);
  };


}}}
#include <gecode/string/rel-op/concat.hpp>

namespace Gecode {
  //FIXME: Workaround, because of linking problems in rel.cpp.
 
  forceinline void
  concat(Home home, StringVar x, StringVar y, StringVar z) {
    using namespace String;
    GECODE_POST;
    GECODE_ES_FAIL((RelOp::Concat<StringView,StringView,StringView>
      ::post(home,x,y,z)));
  }
  
}

#endif

// STATISTICS: string-prop
