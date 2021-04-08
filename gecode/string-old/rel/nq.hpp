namespace Gecode { namespace String {

  forceinline
  Nq::Nq(Home home, StringView x, StringView y)
  : MixBinaryPropagator<StringView, PC_STRING_VAL, StringView, PC_STRING_VAL>
    (home, x, y) {}

  forceinline
  Nq::Nq(Space& home, Nq& p)
  : MixBinaryPropagator<StringView, PC_STRING_VAL, StringView, PC_STRING_VAL>
    (home, p) {}

  forceinline ExecStatus
  Nq::post(Home home, StringView x, StringView y) {
    if (x.same(y))
      return ES_FAILED;
    (void) new (home) Nq(home, x, y);
    return ES_OK;
  }

  forceinline Actor*
  Nq::copy(Space& home) {
    return new (home) Nq(home, *this);
  }

  forceinline ExecStatus
  Nq::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"Nq::propagate "<<x0<<" != "<<x1<<std::endl;
    if (x1.assigned()) {
      if (x0.assigned())
        return x1.val() == x0.val() ? ES_FAILED : home.ES_SUBSUMED(*this);
      if (x1.val() == "") {
        IntVar n(home, 1, x0.max_length());
        length(home, x0, n);
        return home.ES_SUBSUMED(*this);
      }
    }
    if (x0.assigned() && x0.val() == "") {
      IntVar n(home, 1, x1.max_length());
      length(home, x1, n);
      return home.ES_SUBSUMED(*this);
    }
    if (!x0.check_equate(x1))
      return home.ES_SUBSUMED(*this);
    assert (x0.pdomain()->is_normalized() && x1.pdomain()->is_normalized());
    // std::cerr << "Nq::propagated.\n";
    return ES_FIX;
  }

}}
