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
    <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> {
  private:
    stringDFA* s_R_s;
    stringDFA* R_s;
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
