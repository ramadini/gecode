namespace Gecode { namespace String {

  forceinline
  Match::Match(Home home, StringView x, String::RegEx* R, Gecode::Int::IntView i) // Not RegEx* but stringDFA* s_R_s, stringDFA* R_s, minlen...
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>
    (home, x, i) { /*TODO*/ }

  forceinline ExecStatus
  Match::post(Home home, StringView x, string r, Gecode::Int::IntView i) {    
    String::RegEx* regex = String::RegExParser(r).parse();
    if (regex->has_empty()) {
      rel(home, i, IRT_EQ, 1);
      return ES_OK;
    }
//    TODO
    (void) new (home) Match(home, x, regex, i); // Not regex but stringDFA* s_R_s, stringDFA* R_s, minlen...
    return ES_OK;
  }

  forceinline
  Match::Match(Space& home, Match& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> 
    (home, p), s_R_s(p.s_R_s), R_s(p.R_s) {}

  forceinline Actor*
  Match::copy(Space& home) {
    return new (home) Match(home, *this);
  }

  forceinline ExecStatus
  Match::propagate(Space& home, const ModEventDelta& med) {
    // TODO
    assert (x0.pdomain()->is_normalized());
    return ES_FIX;
  }
  
  forceinline
  Match::~Match() {
    delete s_R_s;
    delete R_s;
  }

}}
