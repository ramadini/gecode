#ifndef __GECODE_STRING_INT_HH__
#define __GECODE_STRING_INT_HH__

#include <gecode/string.hh>

namespace Gecode { namespace String { namespace Int {

  /**
   * \namespace Gecode::String::Int
   * \brief Propagators connecting string and int variables
   */

  /**
   * \brief %Propagator for length
   *
   * Requires \code #include <gecode/string/int.hh> \endcode
   * \ingroup FuncStringProp
   */
  template<class View>
  class Length :
    public MixBinaryPropagator<View,PC_STRING_CARD,
      Gecode::Int::IntView,Gecode::Int::PC_INT_BND> {
  protected:
    using MixBinaryPropagator<View,PC_STRING_CARD,
      Gecode::Int::IntView,Gecode::Int::PC_INT_BND>::x0;
    using MixBinaryPropagator<View,PC_STRING_CARD,
      Gecode::Int::IntView,Gecode::Int::PC_INT_BND>::x1;
    /// Constructor for cloning \a p
    Length(Space& home, Length& p);
    /// Constructor for posting
    Length(Home home, View, Gecode::Int::IntView);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator for \f$ |s|=x \f$
    static ExecStatus post(Home home, View s, Gecode::Int::IntView x);
  };

  template<class View0,class View1>
  class Pow :
    public MixTernaryPropagator<View0,PC_STRING_CARD,
      Gecode::Int::IntView,Gecode::Int::PC_INT_BND,View1,PC_STRING_CARD> {
  protected:
    using MixTernaryPropagator<View0,PC_STRING_CARD,
      Gecode::Int::IntView,Gecode::Int::PC_INT_BND,View1,PC_STRING_CARD>::x0;
    using MixTernaryPropagator<View0,PC_STRING_CARD,
      Gecode::Int::IntView,Gecode::Int::PC_INT_BND,View1,PC_STRING_CARD>::x1;
    using MixTernaryPropagator<View0,PC_STRING_CARD,
      Gecode::Int::IntView,Gecode::Int::PC_INT_BND,View1,PC_STRING_CARD>::x2;
    /// Constructor for cloning \a p
    Pow(Space& home, Pow& p);
    /// Constructor for posting
    Pow(Home, View0, Gecode::Int::IntView, View1);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator for \f$ y = x^n \f$
    static ExecStatus post(Home home, View0 s, Gecode::Int::IntView n, View1 y);
  };

}}}

#include <gecode/string/int/length.hpp>
#include <gecode/string/int/pow.hpp>
#endif

// STATISTICS: string-prop
