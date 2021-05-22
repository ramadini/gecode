#ifndef __GECODE_STRING_REL_HH__
#define __GECODE_STRING_REL_HH__

#include <gecode/string.hh>

namespace Gecode { namespace String { namespace Rel {

  /**
   * \brief %Propagator for string equality
   *
   * Requires \code #include <gecode/string/rel.hh> \endcode
   * \ingroup FuncStringProp
   */
  template<class View0, class View1>
  class Eq 
  : public MixBinaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY> {
  protected:
    using MixBinaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY>::x0;
    using MixBinaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY>::x1;
    /// Constructor for cloning \a p
    Eq(Space& home, Eq& p);
    /// Constructor for posting
    Eq(Home home, View0 x0, View1 x1);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ x=y \f$
    static ExecStatus post(Home home, View0 x, View1 y);
  private:
    ExecStatus refine_card(Space& home);
  };
  
   /**
   * \brief %Propagator for negated equality
   *
   * Requires \code #include <gecode/string/rel.hh> \endcode
   * \ingroup FuncStringProp
   */
  template<class View0, class View1>
  class Nq 
  : public MixBinaryPropagator<View0,PC_STRING_VAL,View1,PC_STRING_VAL> {
  protected:
    using MixBinaryPropagator<View0,PC_STRING_VAL,View1,PC_STRING_VAL>::x0;
    using MixBinaryPropagator<View0,PC_STRING_VAL,View1,PC_STRING_VAL>::x1;
    /// Constructor for cloning \a p
    Nq(Space& home, Nq& p);
    /// Constructor for posting
    Nq(Home home, View0 x0, View1 x1);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ x\neq y \f$
    static ExecStatus post(Home home, View0 x, View1 y);
  };
  
  /**
   * \brief %Reified string equality propagator
   *
   * Requires \code #include <gecode/string/rel.hh> \endcode
   * \ingroup FuncStringProp
   */
  template<class View0, class View1, class CtrlView, ReifyMode rm>
  class ReEq : public Propagator {
  protected:
    View0 x0;
    View1 x1;
    CtrlView b;
    /// Constructor for cloning \a p
    ReEq(Space& home, ReEq&);
    /// Constructor for posting
    ReEq(Home home, View0 x0, View1 x1, CtrlView b);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Cost function (defined as PC_TERNARY_LO)
    virtual PropCost cost(const Space& home, const ModEventDelta& med) const;
    /// Schedule function
    virtual void reschedule(Space& home);
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator for \f$ (x=y) \Leftrightarrow b\f$
    static ExecStatus post(Home home, View0 x, View1 y,
                           CtrlView b);
  };
  
  //TODO: Lex

}}}
#include <gecode/string/rel/eq.hpp>
#include <gecode/string/rel/nq.hpp>
#include <gecode/string/rel/re-eq.hpp>

namespace Gecode {
  //FIXME: Workaround, because of linking problems in rel.cpp.
 
  forceinline void
  eq(Home home, StringVar x, StringVar y) {
    using namespace String;
    GECODE_POST;
    GECODE_ES_FAIL((Rel::Eq<StringView,StringView>::post(home, x, y)));
  }
  forceinline void
  eq(Home home, StringVar x, std::vector<int> w) {
    using namespace String;
    StringView vx(x);
    ConstStringView vw(home, &w[0], w.size());
    if (check_equate_x(vx,vw) && check_equate_x(vw,vx))
      vx.update(home, w);
    else
      home.fail();
  }
  forceinline void
  eq(Home home, std::vector<int> w, StringVar x) {
    eq(home, x, w);
  }
  forceinline void
  eq(Home home, StringVar x, String::ConstStringView v) {
    using namespace String;
    StringView vx(x);
    if (check_equate_x(vx,v) && check_equate_x(v,vx))
      vx.update(home, v.val());
    else
      home.fail();
  }
  forceinline void
  eq(Home home, String::ConstStringView v, StringVar y) {
    eq(home, y, v);
  }
  
  forceinline void
  nq(Home home, StringVar x, StringVar y) {
    using namespace String;
    GECODE_POST;
    GECODE_ES_FAIL((Rel::Nq<StringView,StringView>::post(home, x, y)));
  }
  
  forceinline void
  nq(Home home, StringVar x, std::vector<int> w) {
    using namespace String;
    GECODE_POST;
    ConstStringView vw(home, &w[0], w.size());
    GECODE_ES_FAIL((Rel::Nq<StringView,ConstStringView>::post(home, x, vw)));
  }
  forceinline void
  nq(Home home, std::vector<int> w, StringVar x) {
    nq(home, x, w);
  }
}

#endif

// STATISTICS: string-prop
