#ifndef __GECODE_STRING_MATCH_HH__
#define __GECODE_STRING_MATCH_HH__

#include <gecode/string/extensional.hh>
#include <gecode/string/ext/parse-reg.hpp>

namespace Gecode { namespace String {

  /**
   * \brief %Propagator for match.
   *
   */
  class Match : public MixBinaryPropagator
    <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM> {
  private:
    stringDFA* Rs;
    stringDFA* sRs;    
    stringDFA* sRsC;
    int minR;
    std::pair<NSIntSet,int> checkBlock(const DSBlock&, const NSIntSet&) const;
    NSBlocks prefix(int,int) const;
    NSBlocks suffix(int,int) const;
    static ExecStatus propagateReg(Space&, NSBlocks&, stringDFA*);
  protected:
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView, 
      Gecode::Int::PC_INT_DOM>::x0;
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView, 
      Gecode::Int::PC_INT_DOM>::x1;
    /// Constructor for cloning \a p
    Match(Space& home, Match& p);
    /// Constructor for posting
    Match(Home, StringView, Gecode::Int::IntView, stringDFA*, stringDFA*, int);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator
    static ExecStatus post(Home home, StringView x, string r,
      Gecode::Int::IntView i);
    ~Match();
  };
  
  class MatchNew : public MixBinaryPropagator
    <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM> {
  private:
    stringDFA* Rs;
    stringDFA* sRs;    
    stringDFA* sRsC;
    int minR;
    NSBlocks prefix(int,int) const;
    NSBlocks suffix(int,int) const;
    static ExecStatus propagateReg(Space&, NSBlocks&, stringDFA*);
  protected:
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView, 
      Gecode::Int::PC_INT_DOM>::x0;
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView, 
      Gecode::Int::PC_INT_DOM>::x1;
    /// Constructor for cloning \a p
    MatchNew(Space& home, MatchNew& p);
    /// Constructor for posting
    MatchNew(Home, StringView, Gecode::Int::IntView, stringDFA*, stringDFA*, int);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator
    static ExecStatus post(Home home, StringView x, string r,
      Gecode::Int::IntView i);
    ~MatchNew();
  };

}}

#include <gecode/string/int/match.hpp>
#include <gecode/string/int/match-new.hpp>
#endif
