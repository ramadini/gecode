#ifndef __GECODE_STRING_EXT_HH__
#define __GECODE_STRING_EXT_HH__

#include <gecode/string/extensional.hh>
#include <gecode/string/ext/parse-reg.hpp>

namespace Gecode { namespace String {

  // DFA data structure for regular matching.
//  struct stringMDFA {
//    typedef std::vector<std::vector<std::pair<int, int>>> delta_t;
//    int ua;
//    int ur;
//    int n_states;
//    int final_fst;
//    int final_lst;
//    delta_t delta;
//    stringDFA(const DFA&);
//    void negate(const NSIntSet&);
//    bool final(int) const;
//    int search(int, int) const;
//    bool accepted(string) const;
//    bool univ_accepted(const NSIntSet& Q) const;
//    bool univ_rejected(const NSIntSet& Q) const;
//    NSIntSet alphabet() const;
//    NSIntSet neighbours(int) const;
//    NSIntSet neighbours(int, const DSIntSet&) const;
//    void compute_univ(const NSIntSet& alphabet);
//    protected: int nstate(int) const;
//  };

  /**
   * \brief %Propagator for match.
   *
   */
  class Match : public MixBinaryPropagator
    <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> {
//  private:
//    stringMDFA* dfa;
  protected:
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView, 
      Gecode::Int::PC_INT_BND>::x0;
    using MixBinaryPropagator<StringView, PC_STRING_DOM, Gecode::Int::IntView, 
      Gecode::Int::PC_INT_BND>::x1;
    /// Constructor for cloning \a p
    Match(Space& home, Match& p);
    /// Constructor for posting
    Match(Home home, StringView, String::RegEx*, Gecode::Int::IntView);
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

}}

#include <gecode/string/int/match.hpp>
#endif
