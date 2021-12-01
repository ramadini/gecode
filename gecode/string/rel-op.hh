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
  private:
    ExecStatus refine_card(Space& home);
  };

}}}
#include <gecode/string/rel-op/concat.hpp>

namespace Gecode { namespace String { namespace RelOp {

  /**
   * \namespace Gecode::String::RelOp
   * \brief Standard set operation propagators
   */

  /**
   * \brief %Propagator for string reversal
   *
   * Requires \code #include <gecode/string/rel-op.hh> \endcode
   * \ingroup FuncStringProp
   */
  template<class View0, class View1>
  class Reverse:
    public MixBinaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY> {
  protected:
    using MixBinaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY>::x0;
    using MixBinaryPropagator<View0,PC_STRING_ANY,View1,PC_STRING_ANY>::x1;
    /// Constructor for cloning \a p
    Reverse(Space& home, Reverse& p);
    /// Constructor for posting
    Reverse(Home home,View0,View1);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus  propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ z=x \cdot y\f$
    static  ExecStatus  post(Home home,View0 x,View1 y);
  private:
    ExecStatus refine_card(Space& home);
  };

}}}
#include <gecode/string/rel-op/reverse.hpp>

namespace Gecode { namespace String { namespace RelOp {

  /**
   * \namespace Gecode::String::RelOp
   * \brief Standard set operation propagators
   */

  template <class View>
  class Replace : public NaryPropagator<View,PC_STRING_ANY> {
    void prefix(Space&, const View&, const Position&, Block*) const;
    void suffix(Space&, const View&, const Position&, Block*) const;
    ExecStatus decomp_all(Space&);
    ExecStatus refine_ori(Space&, int);
    ExecStatus refine_out(Space&, int);
    int occur(Space&) const;
    bool check_card() const;
    bool all;
    bool last;
  protected:
    using NaryPropagator<View, PC_STRING_ANY>::x;
    /// Constructor for cloning \a p
    Replace(Space& home, Replace& p);
    /// Constructor for posting
    Replace(
      Home home, ViewArray<View>& x, bool all = false, bool last = false
    );
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator string replace
    static  ExecStatus post(Home, ViewArray<View>&, bool all  = false, 
                                                    bool last = false);
  };

}}}
#include <gecode/string/rel-op/replace.hpp>


#endif

// STATISTICS: string-prop
