namespace Gecode { namespace String {

  forceinline
  Char2Code::Char2Code(Home home, StringView y0, Gecode::Int::IntView y1)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView,Gecode::Int::PC_INT_DOM>
    (home, y0, y1) {}

  forceinline ExecStatus
  Char2Code::post(Home home, StringView x0, Gecode::Int::IntView x1) {
    (void) new (home) Char2Code(home, x0, x1);
    return ES_OK;
  }

  forceinline
  Char2Code::Char2Code(Space& home, Char2Code& p)
  : MixBinaryPropagator
  <StringView, PC_STRING_DOM, Gecode::Int::IntView,Gecode::Int::PC_INT_DOM>
    (home, p) {}

  forceinline Actor*
  Char2Code::copy(Space& home) {
    return new (home) Char2Code(home, *this);
  }

  forceinline ExecStatus
  Char2Code::propagate(Space& home, const ModEventDelta&) {
    if (x0.max_length() == 0) {
      GECODE_ME_CHECK(x1.eq(home, -1));
      return home.ES_SUBSUMED(*this);
    }
    if (x1.max() == -1) {
      GECODE_ME_CHECK(x0.ub(home, 0));
      return home.ES_SUBSUMED(*this);
    }

    NSIntSet possible_chars = x0.may_chars();
    IntArgs values;
    if (x0.min_length() == 0)
      values << -1;
    for (NSIntSet::iterator value(possible_chars); value(); ++value)
      values << *value;
    IntSet allowed(values);
    IntSetRanges allowed_ranges(allowed);
    GECODE_ME_CHECK(x1.inter_r(home, allowed_ranges));

    if (x1.max() == -1) {
      GECODE_ME_CHECK(x0.ub(home, 0));
      return home.ES_SUBSUMED(*this);
    }
    if (x1.min() > -1)
      GECODE_ME_CHECK(x0.lb(home, 1));

    NSIntSet allowed_chars;
    for (Gecode::Int::ViewValues<Gecode::Int::IntView> value(x1);
         value(); ++value)
      if (value.val() >= 0)
        allowed_chars.add(value.val());
    NSBlocks refined(x0.domain().blocks());
    for (NSBlock& block : refined) {
      block.S.intersect(allowed_chars);
      if (block.l > 0 && block.S.empty())
        return ES_FAILED;
    }
    refined.normalize();
    GECODE_ME_CHECK(x0.dom(home, refined));
    assert (x0.domain().is_normalized());
    return x0.assigned() && x1.assigned() ? home.ES_SUBSUMED(*this) : ES_FIX;
  }

}}
