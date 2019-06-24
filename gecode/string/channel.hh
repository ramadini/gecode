#ifndef __GECODE_STRING_CHANNEL_HH__
#define __GECODE_STRING_CHANNEL_HH__

namespace Gecode { namespace String {

  /**
   * \brief %Propagator for string channeling with natural numbers.
   *
   */
  class StrToNat :
  public MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> {
  private:
    NSBlocks strdom() const;
    ExecStatus refine_int(Space& home);
    string min_str() const;
    string max_str() const;
  protected:
    using MixBinaryPropagator
    <StringView,PC_STRING_DOM,Gecode::Int::IntView,Gecode::Int::PC_INT_BND>::x0;
    using MixBinaryPropagator
    <StringView,PC_STRING_DOM,Gecode::Int::IntView,Gecode::Int::PC_INT_BND>::x1;
    /// Constructor for cloning \a p
    StrToNat(Space& home, bool share, StrToNat& p);
    /// Constructor for posting
    StrToNat(Home home, StringView x0, Gecode::Int::IntView x1);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home, bool share);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ x=y \f$
    static  ExecStatus post(Home home, StringView x, Gecode::Int::IntView y);
  };

}}
#include <gecode/string/int/str2nat.hpp>

namespace Gecode { namespace String {

  /**
   * \brief %Propagator for string channeling with natural numbers.
   *
   */
  class NatToStr :
  public MixBinaryPropagator
  <Gecode::Int::IntView, Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> {
  private:
    NSBlocks strdom() const;
    string min_str() const;
    string max_str() const;
  protected:
    using MixBinaryPropagator
    <Gecode::Int::IntView,Gecode::Int::PC_INT_BND,StringView,PC_STRING_DOM>::x0;
    using MixBinaryPropagator
    <Gecode::Int::IntView,Gecode::Int::PC_INT_BND,StringView,PC_STRING_DOM>::x1;
    /// Constructor for cloning \a p
    NatToStr(Space& home, bool share, NatToStr& p);
    /// Constructor for posting
    NatToStr(Home home, Gecode::Int::IntView x0, StringView x1);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home, bool share);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ x=y \f$
    static  ExecStatus post(Home home, Gecode::Int::IntView x, StringView y);
  };

}}
#include <gecode/string/int/nat2str.hpp>

#endif
