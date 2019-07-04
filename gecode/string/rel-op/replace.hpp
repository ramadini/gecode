namespace Gecode { namespace String {

  // x[3] is the string resulting from replacing the first occurrence of x[0] 
  // with x[1] in the target string x[2].

  forceinline ExecStatus
  Replace::post(Home home, ViewArray<StringView>& x) {
    if (x[0].same(x[1])) {
      rel(home, x[2], STRT_EQ, x[3]);
      return ES_OK;
    }
    else if (x[0].same(x[2])) {
      rel(home, x[1], STRT_EQ, x[3]);
      return ES_OK;
    }
    else if (x[0].same(x[3]))
      rel(home, x[0], STRT_SUB, x[1]);
    (void) new (home) Replace(home, x);
    return ES_OK;
  }

  forceinline Actor*
  Replace::copy(Space& home) {
    return new (home) Replace(home, *this);
  }

  forceinline
  Replace::Replace(Home home, ViewArray<StringView>& x)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, x) {}

  forceinline
  Replace::Replace(Space& home, Replace& p)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, p) {}

  forceinline ExecStatus
  Replace::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"\nReplace::propagate: "<<x[2]<<" = "<<x[0]<<" ++ "<<x[1]<<std::endl;
    
    //TODO
    
    // std::cerr<<"After concat: "<<x[2]<<" = "<<x[0]<<" ++ "<<x[1]<<std::endl;
//    assert (x[0].pdomain()->is_normalized() && x[1].pdomain()->is_normalized() &&
//            x[2].pdomain()->is_normalized());
//    switch (x[0].assigned() + x[1].assigned() + x[2].assigned()) {
//      case 3:
//        assert (x[2].val() == x[0].val() + x[1].val());
//        return home.ES_SUBSUMED(*this);
//      case 2:
//        return ES_NOFIX;
//      default:
//        return ES_FIX;
//    }
    return ES_FIX;
  }

}}
