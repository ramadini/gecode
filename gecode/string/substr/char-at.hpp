namespace Gecode { namespace String {

  forceinline int
  CharAt::assigned_char_at(const DashedString& source, int index) {
    assert(source.known() && index >= 1 && index <= source.max_length());
    int end = 0;
    for (int block = 0; block < source.length(); ++block) {
      const DSBlock& current = source.at(block);
      end += current.l;
      if (index <= end) {
        assert(current.S.size() == 1);
        return current.S.min();
      }
    }
    GECODE_NEVER;
    return 0;
  }

  forceinline bool
  CharAt::may_contain(const DashedString& string, int character) {
    for (int block = 0; block < string.length(); ++block) {
      const DSBlock& current = string.at(block);
      if (current.u > 0 && current.S.in(character))
        return true;
    }
    return false;
  }

  forceinline bool
  CharAt::AllowedIndices::allowed(int index) const {
    bool in_bounds = index >= 1 && index <= source.max_length();
    return in_bounds
      ? allow_char && CharAt::may_contain
        (result, CharAt::assigned_char_at(source, index))
      : allow_empty;
  }

  forceinline void
  CharAt::AllowedIndices::next(void) {
    while (values() && !allowed(values.val()))
      ++values;
  }

  forceinline
  CharAt::AllowedIndices::AllowedIndices
  (Gecode::Int::IntView index, const DashedString& source0,
   const DashedString& result0, bool allow_empty0, bool allow_char0)
    : values(index), source(source0), result(result0),
      allow_empty(allow_empty0), allow_char(allow_char0) {
    next();
  }

  forceinline bool
  CharAt::AllowedIndices::operator ()(void) const {
    return values();
  }

  forceinline void
  CharAt::AllowedIndices::operator ++(void) {
    ++values;
    next();
  }

  forceinline int
  CharAt::AllowedIndices::val(void) const {
    return values.val();
  }

  forceinline
  CharAt::CharAt(Home home, StringView source, Gecode::Int::IntView index,
                 StringView result)
    : MixTernaryPropagator
      <StringView, PC_STRING_DOM,
       Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
       StringView, PC_STRING_DOM>(home, source, index, result) {}

  forceinline
  CharAt::CharAt(Space& home, CharAt& p)
    : MixTernaryPropagator
      <StringView, PC_STRING_DOM,
       Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
       StringView, PC_STRING_DOM>(home, p) {}

  forceinline ExecStatus
  CharAt::post(Home home, StringView source, Gecode::Int::IntView index,
               StringView result) {
    GECODE_ME_CHECK(result.ub(home, 1));
    (void) new (home) CharAt(home, source, index, result);
    return ES_OK;
  }

  forceinline Actor*
  CharAt::copy(Space& home) {
    return new (home) CharAt(home, *this);
  }

  forceinline bool
  CharAt::index_intersects(int lower, int upper) const {
    if (lower > upper)
      return false;
    for (Gecode::Int::ViewRanges<Gecode::Int::IntView> ranges(x1);
         ranges(); ++ranges) {
      if (ranges.max() < lower)
        continue;
      if (ranges.min() > upper)
        return false;
      return true;
    }
    return false;
  }

  forceinline NSIntSet
  CharAt::possible_chars(void) const {
    const DashedString& source = *x0.pdomain();
    NSIntSet chars;
    int min_prefix = 0;
    int max_prefix = 0;
    for (int block = 0; block < source.length(); ++block) {
      const DSBlock& current = source.at(block);
      int lower = min_prefix + 1;
      int upper = std::min(source.max_length(), max_prefix + current.u);
      if (current.u > 0 && index_intersects(lower, upper))
        chars.include(current.S);
      min_prefix += current.l;
      max_prefix = std::min(DashedString::_MAX_STR_LENGTH,
                            max_prefix + current.u);
    }
    return chars;
  }

  forceinline ExecStatus
  CharAt::propagate(Space& home, const ModEventDelta&) {
    GECODE_ME_CHECK(x2.ub(home, 1));

    if (x1.max() <= 0) {
      GECODE_ME_CHECK(x2.eq(home, ""));
      return home.ES_SUBSUMED(*this);
    }
    if (x1.min() > x0.max_length()) {
      GECODE_ME_CHECK(x2.eq(home, ""));
      return home.ES_SUBSUMED(*this);
    }

    bool valid = index_intersects(1, x0.max_length());
    bool invalid = x1.min() <= 0 || x1.max() > x0.min_length();
    if (!valid) {
      GECODE_ME_CHECK(x2.eq(home, ""));
      return home.ES_SUBSUMED(*this);
    }

    NSIntSet chars = possible_chars();
    if (chars.empty())
      return ES_FAILED;
    NSBlocks result(1, NSBlock(chars, invalid ? 0 : 1, 1));
    GECODE_ME_CHECK(x2.dom(home, result));

    if (x2.min_length() == 1) {
      GECODE_ME_CHECK(x1.gq(home, 1));
      GECODE_ME_CHECK(x1.lq(home, x0.max_length()));
      GECODE_ME_CHECK(x0.lb(home, x1.min()));
    } else if (x2.max_length() == 0 && x1.min() > 0) {
      GECODE_ME_CHECK(x1.gq(home, x0.min_length() + 1));
      GECODE_ME_CHECK(x0.ub(home, x1.max() - 1));
    } else if (x1.min() >= 1 && x1.max() <= x0.min_length()) {
      GECODE_ME_CHECK(x2.lb(home, 1));
    }

    if (x1.assigned()) {
      int index = x1.val();
      if (index <= 0 || index > x0.max_length()) {
        GECODE_ME_CHECK(x2.eq(home, ""));
        return home.ES_SUBSUMED(*this);
      }
      if (x2.max_length() == 0) {
        GECODE_ME_CHECK(x0.ub(home, index - 1));
        return home.ES_SUBSUMED(*this);
      }
      if (x2.min_length() == 1)
        GECODE_ME_CHECK(x0.lb(home, index));
    }

    if (x0.assigned()) {
      const DashedString& source = *x0.pdomain();
      bool allow_empty = x2.min_length() == 0;
      bool allow_char = x2.max_length() == 1;
      AllowedIndices allowed(x1, source, *x2.pdomain(),
                             allow_empty, allow_char);
      if (!allowed())
        return ES_FAILED;
      GECODE_ME_CHECK(x1.inter_v(home, allowed));

      NSIntSet exact_chars;
      bool exact_empty = false;
      for (Gecode::Int::ViewValues<Gecode::Int::IntView> values(x1);
           values(); ++values) {
        int index = values.val();
        if (index < 1 || index > source.max_length())
          exact_empty = true;
        else
          exact_chars.add(assigned_char_at(source, index));
      }
      if (exact_chars.empty()) {
        GECODE_ME_CHECK(x2.eq(home, ""));
      } else {
        NSBlocks exact(1, NSBlock(exact_chars, exact_empty ? 0 : 1, 1));
        GECODE_ME_CHECK(x2.dom(home, exact));
      }

      if (x1.assigned()) {
        int index = x1.val();
        string value = index < 1 || index > source.max_length()
          ? "" : string(1, int2char(assigned_char_at(source, index)));
        GECODE_ME_CHECK(x2.eq(home, value));
        return home.ES_SUBSUMED(*this);
      }
    }

    assert(x0.pdomain()->is_normalized() && x2.pdomain()->is_normalized());
    return ES_FIX;
  }

}}