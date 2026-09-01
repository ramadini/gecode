namespace Gecode { namespace String {

  forceinline ExecStatus
  Concat::post(Home home, StringView x0, StringView x1, StringView x2) {
    if (x0.same(x2))
      rel(home, x1, STRT_EQ, StringVar(home, ""));
    else if (x1.same(x2))
      rel(home, x0, STRT_EQ, StringVar(home, ""));
    else if (x0.same(x1)) {
      StringVar y(
        home, x1.may_chars(), x1.min_length(), x1.max_length()
      );
      (void) new (home) Concat(home, x0, y, x2);
      rel(home, x1, STRT_EQ, y);
    }
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
//    std::cerr<<"\nConcat::propagate: "<<x2<<" = "<<x0<<" ++ "<<x1<<std::endl;
    while (true) {
      assert (x0.domain().is_normalized() && x1.domain().is_normalized() &&
              x2.domain().is_normalized());
      GECODE_ME_CHECK(x2.concat(home, x0, x1));
      if(home.failed())
        return ES_FAILED;
      //std::cerr<<"After concat: "<<x2<<" = "<<x0<<" ++ "<<x1<<std::endl;
      assert (x0.domain().is_normalized() && x1.domain().is_normalized() &&
              x2.domain().is_normalized());
      int assigned = x0.assigned() + x1.assigned() + x2.assigned();
      if (assigned == 3) {
        if (x2.val() != x0.val() + x1.val())
          return ES_FAILED;
        return home.ES_SUBSUMED(*this);
      }
      if (assigned != 2)
        return ES_FIX;
    }
  }

}}
