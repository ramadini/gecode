#ifndef __GECODE_STRING_MATCH_HH__
#define __GECODE_STRING_MATCH_HH__

#include <gecode/string/extensional.hh>
#include <gecode/string/extensional/parse-reg.hpp>

namespace Gecode { namespace String {

  /// Internal implementation choice used by tests and benchmark programs.
  enum MatchImplementation {
    MATCH_AUTO,
    MATCH_DFA,
    MATCH_NFA
  };

  /// Test whether the language of a DFA contains exactly one word.
  forceinline bool match_language_is_singleton(const DFA& dfa);

  /// Return the propagator selected by the public match posting function.
  forceinline MatchImplementation match_default_implementation(const DFA& dfa);

  /// Post match with automatic or explicitly selected internal propagation.
  forceinline ExecStatus post_match(Home home, StringView x, const string& re,
                                    Gecode::Int::IntView i,
                                    MatchImplementation implementation);

  /**
   * \brief DFA-based propagator for match.
   *
   */
  class MatchDFA : public MixBinaryPropagator
    <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM> {
  private:
    TrimDFAHandle Rs;
    TrimDFAHandle sRs;
    CompDFAHandle sRsC;
    int minR;
    int must_idx(void) const;
    NSIntSet checkBlock(const DSBlock&, const NSIntSet&) const;
    template <typename DFA_t> int checkReg(Space& home, const NSBlocks& x, DFA_t* d) const;
  protected:
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView,
      Gecode::Int::PC_INT_DOM>::x0;
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView,
      Gecode::Int::PC_INT_DOM>::x1;
    /// Constructor for cloning \a p
    MatchDFA(Space& home, MatchDFA& p);
    /// Constructor for posting
    MatchDFA(Home, StringView, Gecode::Int::IntView,
             trimDFA*, trimDFA*, int);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator
    static ExecStatus post(Home home, StringView x, Gecode::Int::IntView i,
                           trimDFA* full, trimDFA* pref, int min_length);
  };

  /**
   * \brief NFA-based propagator for match.
   *
   */
  class MatchNFA : public MixBinaryPropagator
    <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_DOM> {
  private:
    TrimDFAHandle Rpref;
    TrimDFAHandle Rfull;
    CompDFAHandle Rcomp;
    MatchNFAHandle Rnfa;
    int minR;
    compDFA* full_complement(void);
    bool must_match(void) const;
    ExecStatus refine_idx(Space& home, int&, int&);
    std::vector<NSIntSet> reachFwd(const DSBlock&, const NSIntSet&) const;
    void reachBwd(int, NSIntSet&, const std::vector<NSIntSet>&, int&, int&) const;
  protected:
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView,
      Gecode::Int::PC_INT_DOM>::x0;
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView,
      Gecode::Int::PC_INT_DOM>::x1;
    /// Constructor for cloning \a p
    MatchNFA(Space& home, MatchNFA& p);
    /// Constructor for posting
    MatchNFA(Home, StringView, Gecode::Int::IntView, int,
             trimDFA*, trimDFA*, matchNFA*);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator
    static ExecStatus post(Home home, StringView x, Gecode::Int::IntView i,
                           trimDFA* pref, trimDFA* full, int min_length);
  };

}}

#include <gecode/string/match/match-dfa.hpp>
#include <gecode/string/match/match-nfa.hpp>
#include <gecode/string/match/post.hpp>
#endif
