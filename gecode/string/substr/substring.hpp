namespace Gecode { namespace String {

  forceinline
  Substring::ConcreteSlice::ConcreteSlice
  (const string& value0, int offset0, int count0)
    : value(value0), offset(offset0), count(count0) {
    assert(offset >= 0 && count > 0 &&
           offset + count <= static_cast<int>(value.size()));
  }

  forceinline int
  Substring::ConcreteSlice::length(void) const {
    return count;
  }

  forceinline char
  Substring::ConcreteSlice::at(int index) const {
    assert(index >= 0 && index < count);
    return value[offset + index];
  }

  forceinline char
  Substring::ConcreteSlice::front(void) const {
    return value[offset];
  }

  forceinline char
  Substring::ConcreteSlice::back(void) const {
    return value[offset + count - 1];
  }

  forceinline int
  Substring::slice_length(int source_length, int from, int to) {
    long long start = std::max(1LL, static_cast<long long>(from));
    long long end = std::min(static_cast<long long>(source_length),
                             static_cast<long long>(to));
    return end < start ? 0 : static_cast<int>(end - start + 1);
  }

  forceinline string
  Substring::slice(const string& source, int from, int to) {
    int length = slice_length(static_cast<int>(source.size()), from, to);
    if (length == 0)
      return "";
    int start = std::max(1, from);
    return source.substr(start - 1, length);
  }

  forceinline bool
  Substring::candidate_supported(const string& concrete_source,
                                 int from, int to,
                                 const string* expected) const {
    int length = slice_length
      (static_cast<int>(concrete_source.size()), from, to);
    if (length < x2.min_length() || length > x2.max_length())
      return false;
    int start = std::max(1, from);
    if (expected != nullptr)
      return length == static_cast<int>(expected->size()) &&
        (length == 0 || concrete_source.compare
          (start - 1, length, *expected) == 0);
    if (length == 0)
      return x2.min_length() == 0;
    ConcreteSlice candidate(concrete_source, start - 1, length);
    return check_sweep<DSBlock, DSBlocks, char, ConcreteSlice>
      (x2.pdomain()->blocks(), candidate);
  }

  forceinline bool
  Substring::from_supported(const string& concrete_source, int from,
                            const string* expected) const {
    for (Gecode::Int::ViewValues<Gecode::Int::IntView> values(x1);
         values(); ++values)
      if (candidate_supported
          (concrete_source, from, values.val(), expected))
        return true;
    return false;
  }

  forceinline bool
  Substring::to_supported(const string& concrete_source, int to,
                          const string* expected) const {
    for (Gecode::Int::ViewValues<Gecode::Int::IntView> values(x0);
         values(); ++values)
      if (candidate_supported
          (concrete_source, values.val(), to, expected))
        return true;
    return false;
  }

  forceinline void
  Substring::SupportedFrom::next(void) {
    while (values() &&
           !propagator.from_supported(source, values.val(), expected))
      ++values;
  }

  forceinline
  Substring::SupportedFrom::SupportedFrom
  (Gecode::Int::IntView from, const Substring& propagator0,
   const string& source0, const string* expected0)
    : values(from), propagator(propagator0), source(source0),
      expected(expected0) {
    next();
  }

  forceinline bool
  Substring::SupportedFrom::operator ()(void) const {
    return values();
  }

  forceinline void
  Substring::SupportedFrom::operator ++(void) {
    ++values;
    next();
  }

  forceinline int
  Substring::SupportedFrom::val(void) const {
    return values.val();
  }

  forceinline void
  Substring::SupportedTo::next(void) {
    while (values() &&
           !propagator.to_supported(source, values.val(), expected))
      ++values;
  }

  forceinline
  Substring::SupportedTo::SupportedTo
  (Gecode::Int::IntView to, const Substring& propagator0,
   const string& source0, const string* expected0)
    : values(to), propagator(propagator0), source(source0),
      expected(expected0) {
    next();
  }

  forceinline bool
  Substring::SupportedTo::operator ()(void) const {
    return values();
  }

  forceinline void
  Substring::SupportedTo::operator ++(void) {
    ++values;
    next();
  }

  forceinline int
  Substring::SupportedTo::val(void) const {
    return values.val();
  }

  forceinline bool
  Substring::result_is_envelope(const NSIntSet& chars,
                                int lower, int upper) const {
    const DashedString& result = *x2.pdomain();
    return result.length() == 1 && result.at(0).l == lower &&
      result.at(0).u == upper && result.at(0).S == chars;
  }

  forceinline
  Substring::Substring(Home home, StringView source0,
                       Gecode::Int::IntView from,
                       Gecode::Int::IntView to, StringView result)
    : MixTernaryPropagator
      <Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
       Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
       StringView, PC_STRING_DOM>(home, from, to, result), source(source0) {}

  forceinline
  Substring::Substring(Space& home, Substring& p)
    : MixTernaryPropagator
      <Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
       Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
       StringView, PC_STRING_DOM>(home, p) {
    source.update(home, p.source);
  }

  forceinline ExecStatus
  Substring::post(Home home, StringView source,
                  Gecode::Int::IntView from, Gecode::Int::IntView to,
                  StringView result) {
    assert(source.assigned());
    if (source.max_length() == 0) {
      GECODE_ME_CHECK(result.eq(home, ""));
      return ES_OK;
    }
    if (from.assigned() && to.assigned()) {
      GECODE_ME_CHECK(result.eq
        (home, slice(source.val(), from.val(), to.val())));
      return ES_OK;
    }
    GECODE_ME_CHECK(result.ub(home, source.max_length()));
    (void) new (home) Substring
      (home, source, from, to, result);
    return ES_OK;
  }

  forceinline bool
  Substring::supports_domains(Gecode::Int::IntView from,
                              Gecode::Int::IntView to) {
    const unsigned int support_limit = 256;
    unsigned int from_size = from.size();
    unsigned int to_size = to.size();
    return from_size != 0 && to_size != 0 &&
      from_size <= support_limit / to_size;
  }

  forceinline Actor*
  Substring::copy(Space& home) {
    return new (home) Substring(home, *this);
  }

  forceinline PropCost
  Substring::cost(const Space&, const ModEventDelta&) const {
    return PropCost::linear(PropCost::LO, source.max_length());
  }

  forceinline ExecStatus
  Substring::propagate(Space& home, const ModEventDelta&) {
    const int source_length = source.max_length();
    const NSIntSet source_chars = source.may_chars();
    bool repeat;
    do {
      repeat = false;
      int lower = slice_length(source_length, x0.max(), x1.min());
      int upper = slice_length(source_length, x0.min(), x1.max());

      ModEvent me = x2.lb(home, lower);
      GECODE_ME_CHECK(me);
      repeat |= me_modified(me);
      me = x2.ub(home, upper);
      GECODE_ME_CHECK(me);
      repeat |= me_modified(me);

      if (upper == 0) {
        GECODE_ME_CHECK(x2.eq(home, ""));
      } else {
        NSBlocks envelope(1, NSBlock(source_chars, lower, upper));
        me = x2.dom(home, envelope);
        GECODE_ME_CHECK(me);
        repeat |= me_modified(me);
      }

      int result_min = x2.min_length();
      int result_max = x2.max_length();
      if (result_min > 0) {
        int from_upper = std::min(source_length - result_min + 1,
                                  x1.max() - result_min + 1);
        me = x0.lq(home, from_upper);
        GECODE_ME_CHECK(me);
        repeat |= me_modified(me);
        int to_lower = std::max(result_min,
          std::max(1, x0.min()) + result_min - 1);
        me = x1.gq(home, to_lower);
        GECODE_ME_CHECK(me);
        repeat |= me_modified(me);
      }
      if (result_max == 0) {
        int earliest_end = std::min(source_length, x1.min());
        if (earliest_end >= 1) {
          me = x0.gq(home, earliest_end + 1);
          GECODE_ME_CHECK(me);
          repeat |= me_modified(me);
        }
        int latest_start = std::max(1, x0.max());
        if (latest_start <= source_length) {
          me = x1.lq(home, latest_start - 1);
          GECODE_ME_CHECK(me);
          repeat |= me_modified(me);
        }
      } else {
        int latest_start = std::max(1, x0.max());
        if (latest_start <= source_length &&
            result_max < source_length - latest_start + 1) {
          me = x1.lq(home, latest_start + result_max - 1);
          GECODE_ME_CHECK(me);
          repeat |= me_modified(me);
        }
        int earliest_end = std::min(source_length, x1.min());
        int from_lower = earliest_end - result_max + 1;
        if (from_lower > 1) {
          me = x0.gq(home, from_lower);
          GECODE_ME_CHECK(me);
          repeat |= me_modified(me);
        }
      }
    } while (repeat);

    if (x0.assigned() && x1.assigned()) {
      GECODE_ME_CHECK(x2.eq
        (home, slice(source.val(), x0.val(), x1.val())));
      return home.ES_SUBSUMED(*this);
    }

    int lower = slice_length(source_length, x0.max(), x1.min());
    int upper = slice_length(source_length, x0.min(), x1.max());
    bool constrained = x2.assigned() ||
      !result_is_envelope(source_chars, lower, upper);
    if (constrained && supports_domains(x0, x1)) {
      const string concrete_source = source.val();
      string expected;
      const string* expected_ptr = nullptr;
      if (x2.assigned()) {
        expected = x2.val();
        expected_ptr = &expected;
      }

      SupportedFrom supported_from
        (x0, *this, concrete_source, expected_ptr);
      if (!supported_from())
        return ES_FAILED;
      ModEvent from_me = x0.inter_v(home, supported_from);
      GECODE_ME_CHECK(from_me);

      SupportedTo supported_to(x1, *this, concrete_source, expected_ptr);
      if (!supported_to())
        return ES_FAILED;
      ModEvent to_me = x1.inter_v(home, supported_to);
      GECODE_ME_CHECK(to_me);

      if (x0.assigned() && x1.assigned()) {
        GECODE_ME_CHECK(x2.eq(home, slice(concrete_source,
                                          x0.val(), x1.val())));
        return home.ES_SUBSUMED(*this);
      }
      if (me_modified(from_me) || me_modified(to_me))
        return ES_NOFIX;
    }

    return ES_FIX;
  }

}}