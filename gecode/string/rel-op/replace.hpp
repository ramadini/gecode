namespace Gecode { namespace String {

  forceinline ExecStatus
  Replace::post(Home home, ViewArray<StringView>& x) {
    //TODO
//    if (x0.same(x1))
//      rel(home, x2, STRT_EQ, StringVar(home, ""));
//    else if (x0.same(x2))
//      rel(home, x1, STRT_EQ, StringVar(home, ""));
//    else if (x1.same(x2))
//      rel(home, x0, STRT_EQ, StringVar(home, ""));
//    else
//      (void) new (home) Replace(home, x0, x1, x2);
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
    // std::cerr<<"\nReplace::propagate: "<<x2<<" = "<<x0<<" ++ "<<x1<<std::endl;
    
    //TODO
    
    // std::cerr<<"After concat: "<<x2<<" = "<<x0<<" ++ "<<x1<<std::endl;
//    assert (x0.pdomain()->is_normalized() && x1.pdomain()->is_normalized() &&
//            x2.pdomain()->is_normalized());
//    switch (x0.assigned() + x1.assigned() + x2.assigned()) {
//      case 3:
//        assert (x2.val() == x0.val() + x1.val());
//        return home.ES_SUBSUMED(*this);
//      case 2:
//        return ES_NOFIX;
//      default:
//        return ES_FIX;
//    }
    return ES_FIX;
  }

}}
