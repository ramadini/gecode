#ifndef __GECODE_STRING_MATCH_HH__
#define __GECODE_STRING_MATCH_HH__

#include <gecode/string/extensional.hh>
#include <gecode/string/extensional/parse-reg.hpp>

namespace Gecode { namespace String {

  /**
   * \brief %Propagator for match.
   *
   */
  class Match : public MixBinaryPropagator
    <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM> {
  private:
    TrimDFAHandle Rs;
    TrimDFAHandle sRs;
    CompDFAHandle sRsC;
    int minR;
    int must_idx(void) const;
    NSIntSet reachMust(const DSBlock&, const NSIntSet&) const;
    NSIntSet checkBlock(const DSBlock&, const NSIntSet&) const;
    NSBlocks prefix(int,int) const;
    NSBlocks suffix(int,int) const;
    template <typename DFA_t> static ExecStatus propagateReg(Space&, NSBlocks&, DFA_t*);
    template <typename DFA_t> int checkReg(Space& home, const NSBlocks& x, DFA_t* d) const;
  protected:
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView, 
      Gecode::Int::PC_INT_DOM>::x0;
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView, 
      Gecode::Int::PC_INT_DOM>::x1;
    /// Constructor for cloning \a p
    Match(Space& home, Match& p);
    /// Constructor for posting
    Match(Home, StringView, Gecode::Int::IntView, trimDFA*, trimDFA*, int);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator
    static ExecStatus post(Home home, StringView x, string r,
      Gecode::Int::IntView i);
  };
  
  class MatchNew : public MixBinaryPropagator
    <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM> {
  private:
    TrimDFAHandle Rpref;
    TrimDFAHandle Rfull;
    CompDFAHandle Rcomp;
    MatchNFAHandle Rnfa;
    int minR;    
    bool must_match(void) const;
    ExecStatus refine_idx(Space& home, int&, int&);
    std::vector<NSIntSet> reachFwd(const DSBlock&, const NSIntSet&) const;
    void reachBwd(int, NSIntSet&, const std::vector<NSIntSet>&, int&, int&) const;
    NSIntSet reachMust(const DSBlock&, const NSIntSet&) const;
    NSBlocks prefix(int,int) const;
    NSBlocks suffix(int,int) const;
    template <typename DFA_t> static ExecStatus propagateReg(Space&, NSBlocks&, DFA_t*);
  protected:
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView, 
      Gecode::Int::PC_INT_DOM>::x0;
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView, 
      Gecode::Int::PC_INT_DOM>::x1;
    /// Constructor for cloning \a p
    MatchNew(Space& home, MatchNew& p);
    /// Constructor for posting
    MatchNew(Home, StringView, Gecode::Int::IntView, int, 
             trimDFA*, trimDFA*, matchNFA*);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator
    static ExecStatus post(Home home, StringView x, string r,
                           Gecode::Int::IntView i);
  };

}}

#include <gecode/string/match/match.hpp>
#include <gecode/string/match/match-new.hpp>
#endif
