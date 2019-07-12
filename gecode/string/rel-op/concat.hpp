namespace Gecode { namespace String {

  forceinline ExecStatus
  Concat::post(Home home, StringView x0, StringView x1, StringView x2) {
    if (x0.same(x2))
      rel(home, x1, STRT_EQ, StringVar(home, ""));
    else if (x1.same(x2))
      rel(home, x0, STRT_EQ, StringVar(home, ""));
    else
      (void) new (home) Concat(home, x0, x1, x2);
    return ES_OK;
  }

  forceinline Actor*
  Concat::copy(Space& home) {
    return new (home) Concat(home, *this);
  }

  forceinline
  Concat::Concat(Home home, StringView y0, StringView y1, StringView y2)
  : MixTernaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM, 
      StringView, PC_STRING_DOM>(home, y0, y1, y2) {}

  forceinline
  Concat::Concat(Space& home, Concat& p)
  : MixTernaryPropagator<StringView, PC_STRING_DOM,
         StringView, PC_STRING_DOM, StringView,PC_STRING_DOM>(home, p) {}

  forceinline ExecStatus
  Concat::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"\nConcat::propagate: "<<x2<<" = "<<x0<<" ++ "<<x1<<std::endl;
    assert (x0.pdomain()->is_normalized() && x1.pdomain()->is_normalized() &&
            x2.pdomain()->is_normalized());
    GECODE_ME_CHECK(x2.concat(home, x0, x1));
    // std::cerr<<"After concat: "<<x2<<" = "<<x0<<" ++ "<<x1<<std::endl;
    assert (x0.pdomain()->is_normalized() && x1.pdomain()->is_normalized() &&
            x2.pdomain()->is_normalized());
    switch (x0.assigned() + x1.assigned() + x2.assigned()) {
      case 3:
        assert (x2.val() == x0.val() + x1.val());
        return home.ES_SUBSUMED(*this);
      case 2:
        return ES_NOFIX;
      default:
        return ES_FIX;
    }
  }

}}
