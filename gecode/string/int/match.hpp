namespace Gecode { namespace String {

  forceinline
  Match::Match(Home home, StringView x, String::RegEx* R, Gecode::Int::IntView i)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>
    (home, x, i) { /*TODO: regex R */ }

  forceinline ExecStatus
  Match::post(Home home, StringView x, string r, Gecode::Int::IntView i) {    
    String::RegEx* regex = String::RegExParser(r).parse();
    // TODO
    (void) new (home) Match(home, x, regex, i);
    return ES_OK;
  }

  forceinline
  Match::Match(Space& home, Match& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> 
    (home, p) { /*TODO*/ }

  forceinline Actor*
  Match::copy(Space& home) {
    return new (home) Match(home, *this);
  }

  forceinline ExecStatus
  Match::propagate(Space& home, const ModEventDelta&) {
    // TODO
    assert (x0.pdomain()->is_normalized());
    return ES_FIX;
  }
  
  forceinline
  Match::~Match() {
    // TODO delete dfa;
  }

}}
