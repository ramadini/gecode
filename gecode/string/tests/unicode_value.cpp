#include <gecode/string.hh>

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

using Gecode::String::StringSymbol;
using Gecode::String::StringVal;
using Gecode::String::is_unicode_scalar_value;


class FixedStringValSpace : public Gecode::Space {
public:
  Gecode::StringVar value;

  explicit FixedStringValSpace(const StringVal& v)
    : value(*this, v) {}

  FixedStringValSpace(FixedStringValSpace& other)
    : Gecode::Space(other) {
    value.update(*this, other.value);
  }

  virtual Gecode::Space* copy(void) {
    return new FixedStringValSpace(*this);
  }
};

namespace {

  void expect_bad_utf8(const std::string& input) {
    bool rejected = false;
    try {
      (void) StringVal::decode_utf8(input);
    } catch (const std::invalid_argument&) {
      rejected = true;
    }
    assert(rejected);
  }

  void expect_bad_symbol(StringSymbol symbol) {
    bool rejected = false;
    try {
      (void) StringVal::from_symbols({symbol});
    } catch (const std::out_of_range&) {
      rejected = true;
    }
    assert(rejected);
  }

  Gecode::String::NSIntSet two_symbols(StringSymbol a, StringSymbol b) {
    Gecode::String::NSIntSet symbols(a);
    symbols.include(b);
    return symbols;
  }

  class UnicodeEqSpace : public Gecode::Space {
  public:
    Gecode::StringVar candidate;
    Gecode::StringVar fixed;

    UnicodeEqSpace(void)
      : candidate(*this, two_symbols(0x65E5, 0x1F600), 1, 1),
        fixed(*this, StringVal::from_symbols({0x65E5})) {
      Gecode::rel(*this, candidate, Gecode::STRT_EQ, fixed);
    }

    UnicodeEqSpace(UnicodeEqSpace& other)
      : Gecode::Space(other) {
      candidate.update(*this, other.candidate);
      fixed.update(*this, other.fixed);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeEqSpace(*this);
    }
  };

  Gecode::String::NSIntSet symbol_set(
    std::initializer_list<StringSymbol> values
  ) {
    assert(values.size() > 0);
    std::initializer_list<StringSymbol>::const_iterator i = values.begin();
    Gecode::String::NSIntSet symbols(*i);
    for (++i; i != values.end(); ++i)
      symbols.include(*i);
    return symbols;
  }

  class UnicodeNqSpace : public Gecode::Space {
  public:
    Gecode::StringVar left;
    Gecode::StringVar right;

    explicit UnicodeNqSpace(bool equal)
      : left(*this, StringVal::from_symbols({0x65E5, 0x1F600})),
        right(*this, equal
          ? StringVal::from_symbols({0x65E5, 0x1F600})
          : StringVal::from_symbols({0x65E5, 0x672C})) {
      Gecode::rel(*this, left, Gecode::STRT_NQ, right);
    }

    UnicodeNqSpace(UnicodeNqSpace& other)
      : Gecode::Space(other) {
      left.update(*this, other.left);
      right.update(*this, other.right);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeNqSpace(*this);
    }
  };

  class UnicodeIncreasingSpace : public Gecode::Space {
  public:
    Gecode::StringVar value;

    UnicodeIncreasingSpace(const StringVal& concrete, bool strict)
      : value(*this, concrete) {
      Gecode::rel(*this, value,
        strict ? Gecode::STRT_INCLT : Gecode::STRT_INCLQ);
    }

    UnicodeIncreasingSpace(UnicodeIncreasingSpace& other)
      : Gecode::Space(other) {
      value.update(*this, other.value);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeIncreasingSpace(*this);
    }
  };

  class UnicodeRevSpace : public Gecode::Space {
  public:
    Gecode::StringVar source;
    Gecode::StringVar result;

    explicit UnicodeRevSpace(bool fixed_mismatch)
      : source(*this, StringVal::from_symbols({'A', 0x65E5, 0x1F600})),
        result() {
      if (fixed_mismatch) {
        result = Gecode::StringVar(
          *this, StringVal::from_symbols({0x1F600, 0x672C, 'A'}));
      } else {
        result = Gecode::StringVar(
          *this, symbol_set({'A', 0x65E5, 0x1F600}), 3, 3);
      }
      Gecode::rel(*this, source, Gecode::STRT_REV, result);
    }

    UnicodeRevSpace(UnicodeRevSpace& other)
      : Gecode::Space(other) {
      source.update(*this, other.source);
      result.update(*this, other.result);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeRevSpace(*this);
    }
  };

  class UnicodePowSpace : public Gecode::Space {
  public:
    Gecode::StringVar base;
    Gecode::IntVar exponent;
    Gecode::StringVar result;

    explicit UnicodePowSpace(bool fixed_mismatch)
      : base(*this, StringVal::from_symbols({0x65E5, 0x1F600})),
        exponent(*this, 2, 2), result() {
      if (fixed_mismatch) {
        result = Gecode::StringVar(*this, StringVal::from_symbols(
          {0x65E5, 0x1F600, 0x65E5, 0x672C}));
      } else {
        result = Gecode::StringVar(
          *this, symbol_set({0x65E5, 0x1F600}), 4, 4);
      }
      Gecode::pow(*this, base, exponent, result);
    }

    UnicodePowSpace(UnicodePowSpace& other)
      : Gecode::Space(other) {
      base.update(*this, other.base);
      exponent.update(*this, other.exponent);
      result.update(*this, other.result);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodePowSpace(*this);
    }
  };

  class UnicodeConcatSpace : public Gecode::Space {
  public:
    Gecode::StringVar left;
    Gecode::StringVar right;
    Gecode::StringVar result;

    enum Mode { InferRight, InferLeft, InferBoth, InferResult, InvalidFixed };

    explicit UnicodeConcatSpace(Mode mode)
      : left(), right(), result() {
      if (mode == InferRight) {
        left = Gecode::StringVar(*this, StringVal::from_symbols({0x65E5}));
        right = Gecode::StringVar(
          *this, two_symbols(0x672C, 0x1F600), 1, 1);
        result = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5, 0x672C}));
      } else if (mode == InferLeft) {
        left = Gecode::StringVar(
          *this, two_symbols(0x65E5, 0x1F600), 1, 1);
        right = Gecode::StringVar(*this, StringVal::from_symbols({0x672C}));
        result = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5, 0x672C}));
      } else if (mode == InferBoth) {
        left = Gecode::StringVar(
          *this, two_symbols(0x65E5, 0x672C), 1, 1);
        right = Gecode::StringVar(
          *this, two_symbols(0x672C, 0x1F600), 1, 1);
        result = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5, 0x1F600}));
      } else if (mode == InferResult) {
        left = Gecode::StringVar(*this, StringVal::from_symbols({0x65E5}));
        right = Gecode::StringVar(*this, StringVal::from_symbols({0x1F600}));
        result = Gecode::StringVar(
          *this, two_symbols(0x65E5, 0x1F600), 2, 2);
      } else {
        left = Gecode::StringVar(*this, StringVal::from_symbols({0x65E5}));
        right = Gecode::StringVar(*this, StringVal::from_symbols({0x1F600}));
        result = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5, 0x672C}));
      }
      Gecode::rel(*this, left, right, Gecode::STRT_CAT, result);
    }

    UnicodeConcatSpace(UnicodeConcatSpace& other)
      : Gecode::Space(other) {
      left.update(*this, other.left);
      right.update(*this, other.right);
      result.update(*this, other.result);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeConcatSpace(*this);
    }
  };

  class UnicodeGConcatSpace : public Gecode::Space {
  public:
    Gecode::StringVar result;

    enum Mode { InferResult, FixedMatch, FixedMismatch };

    explicit UnicodeGConcatSpace(Mode mode)
      : result() {
      Gecode::StringVarArgs parts(3);
      parts[0] = Gecode::StringVar(*this, "A");
      parts[1] = Gecode::StringVar(
        *this, StringVal::from_symbols({0x65E5}));
      parts[2] = Gecode::StringVar(
        *this, StringVal::from_symbols({0x1F600}));

      if (mode == InferResult) {
        result = Gecode::StringVar(
          *this, symbol_set({'A', 0x65E5, 0x1F600}), 3, 3);
      } else if (mode == FixedMatch) {
        result = Gecode::StringVar(
          *this, StringVal::from_symbols({'A', 0x65E5, 0x1F600}));
      } else {
        result = Gecode::StringVar(
          *this, StringVal::from_symbols({'A', 0x65E5, 0x672C}));
      }
      Gecode::gconcat(*this, parts, result);
    }

    UnicodeGConcatSpace(UnicodeGConcatSpace& other)
      : Gecode::Space(other) {
      result.update(*this, other.result);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeGConcatSpace(*this);
    }
  };


  class UnicodeCharAtSpace : public Gecode::Space {
  public:
    Gecode::IntVar index;
    Gecode::StringVar result;

    explicit UnicodeCharAtSpace(int index_value)
      : index(*this, index_value, index_value),
        result(*this, symbol_set({'A', 0x65E5, 0x1F600}), 0, 1) {
      Gecode::StringVar source(
        *this, StringVal::from_symbols({'A', 0x65E5, 0x1F600}));
      Gecode::substr(*this, source, index, index, result);
    }

    UnicodeCharAtSpace(UnicodeCharAtSpace& other)
      : Gecode::Space(other) {
      index.update(*this, other.index);
      result.update(*this, other.result);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeCharAtSpace(*this);
    }
  };

  class UnicodeSubstringSpace : public Gecode::Space {
  public:
    Gecode::IntVar from;
    Gecode::IntVar to;
    Gecode::StringVar result;

    enum Mode { InferResult, InferBounds };

    explicit UnicodeSubstringSpace(Mode mode)
      : from(), to(), result() {
      Gecode::StringVar source(
        *this, StringVal::from_symbols({'A', 0x65E5, 0x1F600, 0x672C}));
      if (mode == InferResult) {
        from = Gecode::IntVar(*this, 2, 2);
        to = Gecode::IntVar(*this, 3, 3);
        result = Gecode::StringVar(
          *this, symbol_set({'A', 0x65E5, 0x1F600, 0x672C}), 0, 4);
      } else {
        from = Gecode::IntVar(*this, 1, 4);
        to = Gecode::IntVar(*this, 1, 4);
        result = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5, 0x1F600}));
      }
      Gecode::substr(*this, source, from, to, result);
    }

    UnicodeSubstringSpace(UnicodeSubstringSpace& other)
      : Gecode::Space(other) {
      from.update(*this, other.from);
      to.update(*this, other.to);
      result.update(*this, other.result);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeSubstringSpace(*this);
    }
  };

  class ByteSubstringUnicodeResultSpace : public Gecode::Space {
  public:
    Gecode::IntVar from;
    Gecode::IntVar to;
    Gecode::StringVar result;

    ByteSubstringUnicodeResultSpace(void)
      : from(*this, 1, 3), to(*this, 1, 3),
        result(*this, StringVal::from_symbols({0x65E5})) {
      Gecode::StringVar source(*this, "ABC");
      Gecode::substr(*this, source, from, to, result);
    }

    ByteSubstringUnicodeResultSpace(ByteSubstringUnicodeResultSpace& other)
      : Gecode::Space(other) {
      from.update(*this, other.from);
      to.update(*this, other.to);
      result.update(*this, other.result);
    }

    virtual Gecode::Space* copy(void) {
      return new ByteSubstringUnicodeResultSpace(*this);
    }
  };

  class UnicodeFindSpace : public Gecode::Space {
  public:
    Gecode::StringVar pattern;
    Gecode::StringVar haystack;
    Gecode::IntVar index;

    enum Mode {
      AssignedUnicode,
      BytePatternUnicodeHaystack,
      KnownUnicodePrefix,
      RemoveUnicodeNoMatch,
      InferUnicodePattern
    };

    explicit UnicodeFindSpace(Mode mode)
      : pattern(), haystack(), index() {
      if (mode == AssignedUnicode) {
        pattern = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5, 0x1F600}));
        haystack = Gecode::StringVar(
          *this, StringVal::from_symbols({'A', 0x65E5, 0x1F600, 0x672C}));
        index = Gecode::IntVar(*this, 0, 4);
      } else if (mode == BytePatternUnicodeHaystack) {
        pattern = Gecode::StringVar(*this, "A");
        haystack = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5, 'A', 0x1F600}));
        index = Gecode::IntVar(*this, 0, 3);
      } else if (mode == KnownUnicodePrefix) {
        pattern = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5, 0x1F600}));
        Gecode::String::NSBlocks blocks;
        blocks.push_back(Gecode::String::NSBlock(
          Gecode::String::NSIntSet(0x65E5), 1, 1));
        blocks.push_back(Gecode::String::NSBlock(
          Gecode::String::NSIntSet(0x1F600), 1, 1));
        blocks.push_back(Gecode::String::NSBlock(
          symbol_set({'a', 'b'}), 0, 1));
        haystack = Gecode::StringVar(*this, blocks, 2, 3);
        index = Gecode::IntVar(*this, 0, 3);
      } else if (mode == RemoveUnicodeNoMatch) {
        pattern = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5}));
        haystack = Gecode::StringVar(
          *this, two_symbols(0x65E5, 0x672C), 1, 1);
        index = Gecode::IntVar(*this, 0, 0);
      } else {
        pattern = Gecode::StringVar(
          *this, symbol_set({0x65E5, 0x1F600, 0x672C}), 2, 2);
        haystack = Gecode::StringVar(
          *this, StringVal::from_symbols({'A', 0x65E5, 0x1F600, 0x672C}));
        index = Gecode::IntVar(*this, 2, 2);
      }
      Gecode::find(*this, pattern, haystack, index);
    }

    UnicodeFindSpace(UnicodeFindSpace& other)
      : Gecode::Space(other) {
      pattern.update(*this, other.pattern);
      haystack.update(*this, other.haystack);
      index.update(*this, other.index);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeFindSpace(*this);
    }
  };

  class UnicodeContainsSpace : public Gecode::Space {
  public:
    Gecode::BoolVar result;

    UnicodeContainsSpace(void) : result(*this, 0, 1) {
      Gecode::StringVar haystack(
        *this, StringVal::from_symbols({'A', 0x65E5, 0x1F600}));
      Gecode::StringVar needle(
        *this, StringVal::from_symbols({0x65E5, 0x1F600}));
      Gecode::contains(*this, haystack, needle, result);
    }

    UnicodeContainsSpace(UnicodeContainsSpace& other)
      : Gecode::Space(other) {
      result.update(*this, other.result);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeContainsSpace(*this);
    }
  };

  Gecode::DFA unicode_literal_dfa(void) {
    return Gecode::DFA(0, {
      Gecode::DFA::Transition(0, 0x65E5, 1),
      Gecode::DFA::Transition(1, 0x1F600, 2)
    }, {2}, false);
  }

  Gecode::DFA unicode_class_dfa(void) {
    return Gecode::DFA(0, {
      Gecode::DFA::Transition(0, 0x65E5, 1),
      Gecode::DFA::Transition(0, 0x1F600, 1)
    }, {1}, false);
  }

  Gecode::DFA unicode_active_alphabet_star_dfa(void) {
    return Gecode::DFA(0, {
      Gecode::DFA::Transition(0, 0x65E5, 0),
      Gecode::DFA::Transition(0, 0x1F600, 0),
      Gecode::DFA::Transition(0, 0x10FFFF, 0)
    }, {0}, false);
  }

  class UnicodeRegularSpace : public Gecode::Space {
  public:
    enum Mode {
      AssignedLiteral,
      RejectedLiteral,
      PropagatedClass,
      FilterClass,
      ActiveAlphabetStar,
      MaximumScalar
    };

    Gecode::StringVar value;

    explicit UnicodeRegularSpace(Mode mode) : value() {
      if (mode == AssignedLiteral || mode == RejectedLiteral) {
        value = Gecode::StringVar(*this, StringVal::from_symbols(
          mode == AssignedLiteral
            ? std::initializer_list<StringSymbol>{0x65E5, 0x1F600}
            : std::initializer_list<StringSymbol>{0x65E5, 0x672C}));
        Gecode::extensional(*this, value, unicode_literal_dfa());
      } else if (mode == PropagatedClass) {
        value = Gecode::StringVar(
          *this, symbol_set({0x65E5, 0x1F600}), 1, 1);
        Gecode::extensional(*this, value, unicode_class_dfa());
        Gecode::StringVar selected(
          *this, StringVal::from_symbols({0x1F600}));
        Gecode::rel(*this, value, Gecode::STRT_EQ, selected);
      } else if (mode == FilterClass) {
        value = Gecode::StringVar(
          *this, symbol_set({0x65E5, 0x672C, 0x1F600}), 1, 1);
        Gecode::extensional(*this, value, unicode_class_dfa());
      } else if (mode == ActiveAlphabetStar) {
        value = Gecode::StringVar(*this, StringVal::from_symbols(
          {0x65E5, 0x1F600, 0x10FFFF, 0x65E5}));
        Gecode::extensional(
          *this, value, unicode_active_alphabet_star_dfa());
      } else {
        value = Gecode::StringVar(
          *this, StringVal::from_symbols({0x10FFFF}));
        Gecode::DFA maximum(0, {
          Gecode::DFA::Transition(0, 0x10FFFF, 1)
        }, {1}, false);
        Gecode::extensional(*this, value, maximum);
      }
    }

    UnicodeRegularSpace(UnicodeRegularSpace& other)
      : Gecode::Space(other) {
      value.update(*this, other.value);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeRegularSpace(*this);
    }
  };

  class UnicodeReifiedRegularSpace : public Gecode::Space {
  public:
    enum Mode { AssignedAccept, AssignedReject, PropagatedAccept };

    Gecode::StringVar value;
    Gecode::BoolVar result;

    explicit UnicodeReifiedRegularSpace(Mode mode)
      : value(), result(*this, 0, 1) {
      if (mode == PropagatedAccept) {
        value = Gecode::StringVar(
          *this, symbol_set({0x65E5, 0x1F600}), 1, 1);
        Gecode::extensional(
          *this, value, unicode_class_dfa(), result, Gecode::RM_EQV);
        Gecode::StringVar selected(
          *this, StringVal::from_symbols({0x1F600}));
        Gecode::rel(*this, value, Gecode::STRT_EQ, selected);
      } else {
        value = Gecode::StringVar(*this, StringVal::from_symbols(
          mode == AssignedAccept
            ? std::initializer_list<StringSymbol>{0x65E5, 0x1F600}
            : std::initializer_list<StringSymbol>{0x65E5, 0x672C}));
        Gecode::extensional(
          *this, value, unicode_literal_dfa(), result, Gecode::RM_EQV);
      }
    }

    UnicodeReifiedRegularSpace(UnicodeReifiedRegularSpace& other)
      : Gecode::Space(other) {
      value.update(*this, other.value);
      result.update(*this, other.result);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeReifiedRegularSpace(*this);
    }
  };

  class UnicodeGCCSpace : public Gecode::Space {
  public:
    Gecode::StringVar value;
    Gecode::IntVarArray counts;

    UnicodeGCCSpace(void)
      : value(*this, StringVal::from_symbols(
          {0x65E5, 0x1F600, 0x65E5, 0x10FFFF})),
        counts(*this, 4, 0, 4) {
      Gecode::IntArgs characters;
      characters << 0x65E5 << 0x1F600 << 0x10FFFF << 0x672C;
      Gecode::gcc(*this, value, characters, counts);
    }

    UnicodeGCCSpace(UnicodeGCCSpace& other)
      : Gecode::Space(other) {
      value.update(*this, other.value);
      counts.update(*this, other.counts);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeGCCSpace(*this);
    }
  };

  class UnicodeStrToNatSpace : public Gecode::Space {
  public:
    Gecode::StringVar value;
    Gecode::IntVar number;

    UnicodeStrToNatSpace(void)
      : value(*this, symbol_set({'7', 0x65E5}), 1, 1),
        number(*this, -1, 9) {
      Gecode::str2nat(*this, value, number);
      Gecode::StringVar unicode(
        *this, StringVal::from_symbols({0x65E5}));
      Gecode::rel(*this, value, Gecode::STRT_EQ, unicode);
    }

    UnicodeStrToNatSpace(UnicodeStrToNatSpace& other)
      : Gecode::Space(other) {
      value.update(*this, other.value);
      number.update(*this, other.number);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeStrToNatSpace(*this);
    }
  };

  class UnicodeReplaceSpace : public Gecode::Space {
  public:
    enum Mode {
      First,
      Last,
      All,
      AllEmptyQuery,
      AllVariableReplacement,
      ByteQueryUnicodeSource,
      MaximumScalar,
      KnownUnicodeOccurrence
    };

    Gecode::StringVar source;
    Gecode::StringVar result;
    Gecode::StringVar replacement;

    explicit UnicodeReplaceSpace(Mode mode)
      : source(), result(), replacement() {
      Gecode::StringVar query;
      if (mode == AllEmptyQuery) {
        source = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5, 0x1F600}));
        query = Gecode::StringVar(*this, StringVal());
        replacement = Gecode::StringVar(
          *this, StringVal::from_symbols({0x672C}));
        result = Gecode::StringVar(
          *this, symbol_set({0x65E5, 0x672C, 0x1F600}), 0, 8);
        Gecode::replace_all(*this, source, query, replacement, result);
      } else if (mode == AllVariableReplacement) {
        source = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5, 0x1F600, 0x65E5}));
        query = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5}));
        replacement = Gecode::StringVar(
          *this, symbol_set({0x672C, 0x10FFFF}), 1, 1);
        result = Gecode::StringVar(
          *this, StringVal::from_symbols({0x672C, 0x1F600, 0x672C}));
        Gecode::replace_all(*this, source, query, replacement, result);
      } else if (mode == ByteQueryUnicodeSource) {
        source = Gecode::StringVar(
          *this, StringVal::from_symbols({'A', 0x65E5, 'A'}));
        query = Gecode::StringVar(*this, StringVal::from_bytes("A"));
        replacement = Gecode::StringVar(
          *this, StringVal::from_symbols({0x1F600}));
        result = Gecode::StringVar(
          *this, symbol_set({'A', 0x65E5, 0x1F600}), 0, 8);
        Gecode::replace_all(*this, source, query, replacement, result);
      } else if (mode == MaximumScalar) {
        source = Gecode::StringVar(
          *this, StringVal::from_symbols({0x10FFFF, 0x65E5}));
        query = Gecode::StringVar(
          *this, StringVal::from_symbols({0x10FFFF}));
        replacement = Gecode::StringVar(
          *this, StringVal::from_symbols({0x1F600}));
        result = Gecode::StringVar(
          *this, symbol_set({0x65E5, 0x1F600, 0x10FFFF}), 0, 4);
        Gecode::replace(*this, source, query, replacement, result);
      } else if (mode == KnownUnicodeOccurrence) {
        Gecode::String::NSBlocks blocks;
        blocks.push_back(Gecode::String::NSBlock(
          Gecode::String::NSIntSet(0x65E5), 1, 1));
        blocks.push_back(Gecode::String::NSBlock(
          symbol_set({0x672C, 0x1F600}), 1, 1));
        source = Gecode::StringVar(*this, blocks, 2, 2);
        query = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5}));
        replacement = Gecode::StringVar(
          *this, StringVal::from_symbols({0x10FFFF}));
        result = Gecode::StringVar(
          *this, StringVal::from_symbols({0x10FFFF, 0x1F600}));
        Gecode::replace(*this, source, query, replacement, result);
      } else {
        source = Gecode::StringVar(*this, StringVal::from_symbols(
          {0x65E5, 0x1F600, 0x65E5, 0x1F600}));
        query = Gecode::StringVar(
          *this, StringVal::from_symbols({0x65E5, 0x1F600}));
        replacement = Gecode::StringVar(
          *this, StringVal::from_symbols({0x672C}));
        result = Gecode::StringVar(
          *this, symbol_set({0x65E5, 0x672C, 0x1F600}), 0, 8);
        if (mode == First)
          Gecode::replace(*this, source, query, replacement, result);
        else if (mode == Last)
          Gecode::replace_last(*this, source, query, replacement, result);
        else
          Gecode::replace_all(*this, source, query, replacement, result);
      }
    }

    UnicodeReplaceSpace(UnicodeReplaceSpace& other)
      : Gecode::Space(other) {
      source.update(*this, other.source);
      result.update(*this, other.result);
      replacement.update(*this, other.replacement);
    }

    virtual Gecode::Space* copy(void) {
      return new UnicodeReplaceSpace(*this);
    }
  };

}

int main(void) {
  assert(is_unicode_scalar_value(0));
  assert(is_unicode_scalar_value(0xD7FF));
  assert(!is_unicode_scalar_value(0xD800));
  assert(!is_unicode_scalar_value(0xDFFF));
  assert(is_unicode_scalar_value(0xE000));
  assert(is_unicode_scalar_value(0x10FFFF));
  assert(!is_unicode_scalar_value(-1));
  assert(!is_unicode_scalar_value(0x110000));

  const std::string japanese("\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E", 9);
  const StringVal decoded = StringVal::decode_utf8(japanese);
  assert(decoded.size() == 3);
  assert(decoded[0] == 0x65E5);
  assert(decoded[1] == 0x672C);
  assert(decoded[2] == 0x8A9E);
  assert(decoded.to_utf8() == japanese);

  const std::string emoji("\xF0\x9F\x98\x80", 4);
  const StringVal face = StringVal::decode_utf8(emoji);
  assert(face.size() == 1);
  assert(face[0] == 0x1F600);
  assert(face.to_utf8() == emoji);

  const std::string byte_sequence("\xE6\x97\xA5", 3);
  const StringVal bytes = StringVal::from_bytes(byte_sequence);
  assert(bytes.size() == 3);
  assert(bytes[0] == 0xE6);
  assert(bytes[1] == 0x97);
  assert(bytes[2] == 0xA5);
  assert(bytes.to_bytes() == byte_sequence);

  const StringVal boundaries = StringVal::from_symbols(
    {0, 0x7F, 0x80, 0x7FF, 0x800, 0xD7FF, 0xE000, 0xFFFF, 0x10000,
     0x10FFFF});
  assert(StringVal::decode_utf8(boundaries.to_utf8()) == boundaries);

  expect_bad_utf8(std::string("\x80", 1));
  expect_bad_utf8(std::string("\xC0\xAF", 2));
  expect_bad_utf8(std::string("\xC2", 1));
  expect_bad_utf8(std::string("\xE0\x80\x80", 3));
  expect_bad_utf8(std::string("\xED\xA0\x80", 3));
  expect_bad_utf8(std::string("\xF0\x80\x80\x80", 4));
  expect_bad_utf8(std::string("\xF4\x90\x80\x80", 4));
  expect_bad_utf8(std::string("\xF5\x80\x80\x80", 4));
  expect_bad_utf8(std::string("\xE2\x28\xA1", 3));

  expect_bad_symbol(-1);
  expect_bad_symbol(0xD800);
  expect_bad_symbol(0xDFFF);
  expect_bad_symbol(0x110000);

  bool rejected_non_byte = false;
  try {
    (void) StringVal::from_symbols({0x100}).to_bytes();
  } catch (const std::out_of_range&) {
    rejected_non_byte = true;
  }
  assert(rejected_non_byte);

  const std::vector<StringSymbol> block_symbols =
    {0x65E5, 0x65E5, 0x1F600, 0x10FFFF};
  Gecode::String::NSBlocks assigned_blocks(block_symbols);
  assert(assigned_blocks.val_symbols() ==
         StringVal::from_symbols(block_symbols));
  std::string assigned_block_bytes;
  assert(!assigned_blocks.try_val_bytes(assigned_block_bytes));

  Gecode::String::NSIntSet alternatives(0x65E5);
  alternatives.include(0x672C);
  Gecode::String::NSBlocks partial_blocks({
    Gecode::String::NSBlock(
      Gecode::String::NSIntSet(0x65E5), 2, 2),
    Gecode::String::NSBlock(
      Gecode::String::NSIntSet(0x1F600), 1, 2),
    Gecode::String::NSBlock(alternatives, 1, 1),
    Gecode::String::NSBlock(
      Gecode::String::NSIntSet(0x10FFFF), 1, 1)
  });
  assert(partial_blocks.known_pref_symbols() ==
         std::vector<StringSymbol>({0x65E5, 0x65E5, 0x1F600}));
  assert(partial_blocks.known_suff_symbols() ==
         std::vector<StringSymbol>({0x10FFFF}));

  FixedStringValSpace* unicode_space = new FixedStringValSpace(decoded);
  assert(unicode_space->value.assigned());
  assert(unicode_space->value.min_length() == 3);
  assert(unicode_space->value.max_length() == 3);
  assert(unicode_space->value.val_symbols() == decoded);
  assert(unicode_space->value.may_chars().in(0x65E5));
  assert(unicode_space->value.may_chars().in(0x672C));
  assert(unicode_space->value.may_chars().in(0x8A9E));

  bool rejected_legacy_value = false;
  try {
    (void) unicode_space->value.val();
  } catch (const Gecode::String::OutOfLimitsDS&) {
    rejected_legacy_value = true;
  }
  assert(rejected_legacy_value);

  FixedStringValSpace* unicode_clone =
    static_cast<FixedStringValSpace*>(unicode_space->clone());
  assert(unicode_clone->value.assigned());
  assert(unicode_clone->value.val_symbols() == decoded);
  delete unicode_clone;
  delete unicode_space;

  const StringVal supplementary =
    StringVal::from_symbols({0x1F600, 0x1F600, 0x10FFFF});
  FixedStringValSpace* supplementary_space =
    new FixedStringValSpace(supplementary);
  assert(supplementary_space->value.assigned());
  assert(supplementary_space->value.min_length() == 3);
  assert(supplementary_space->value.max_length() == 3);
  assert(supplementary_space->value.val_symbols() == supplementary);
  assert(supplementary_space->value.may_chars().in(0x1F600));
  assert(supplementary_space->value.may_chars().in(0x10FFFF));
  delete supplementary_space;

  FixedStringValSpace* byte_space =
    new FixedStringValSpace(StringVal::from_bytes("aab"));
  assert(byte_space->value.val() == "aab");
  assert(byte_space->value.val_symbols() == StringVal::from_bytes("aab"));
  delete byte_space;

  FixedStringValSpace* empty_space = new FixedStringValSpace(StringVal());
  assert(empty_space->value.assigned());
  assert(empty_space->value.min_length() == 0);
  assert(empty_space->value.max_length() == 0);
  assert(empty_space->value.val_symbols().empty());
  assert(empty_space->value.val().empty());
  delete empty_space;

  UnicodeEqSpace* equality = new UnicodeEqSpace();
  assert(equality->status() != Gecode::SS_FAILED);
  assert(equality->candidate.assigned());
  assert(equality->candidate.val_symbols() ==
         StringVal::from_symbols({0x65E5}));
  delete equality;

  UnicodeConcatSpace* infer_right =
    new UnicodeConcatSpace(UnicodeConcatSpace::InferRight);
  assert(infer_right->status() != Gecode::SS_FAILED);
  assert(infer_right->right.assigned());
  assert(infer_right->right.val_symbols() ==
         StringVal::from_symbols({0x672C}));
  delete infer_right;

  UnicodeConcatSpace* infer_left =
    new UnicodeConcatSpace(UnicodeConcatSpace::InferLeft);
  assert(infer_left->status() != Gecode::SS_FAILED);
  assert(infer_left->left.assigned());
  assert(infer_left->left.val_symbols() ==
         StringVal::from_symbols({0x65E5}));
  delete infer_left;

  UnicodeConcatSpace* infer_both =
    new UnicodeConcatSpace(UnicodeConcatSpace::InferBoth);
  assert(infer_both->status() != Gecode::SS_FAILED);
  assert(infer_both->left.assigned());
  assert(infer_both->right.assigned());
  assert(infer_both->left.val_symbols() ==
         StringVal::from_symbols({0x65E5}));
  assert(infer_both->right.val_symbols() ==
         StringVal::from_symbols({0x1F600}));
  delete infer_both;

  UnicodeConcatSpace* infer_result =
    new UnicodeConcatSpace(UnicodeConcatSpace::InferResult);
  assert(infer_result->status() != Gecode::SS_FAILED);
  assert(infer_result->result.assigned());
  assert(infer_result->result.val_symbols() ==
         StringVal::from_symbols({0x65E5, 0x1F600}));
  delete infer_result;

  UnicodeConcatSpace* invalid =
    new UnicodeConcatSpace(UnicodeConcatSpace::InvalidFixed);
  assert(invalid->status() == Gecode::SS_FAILED);
  delete invalid;

  UnicodeNqSpace* nq_different = new UnicodeNqSpace(false);
  assert(nq_different->status() != Gecode::SS_FAILED);
  delete nq_different;

  UnicodeNqSpace* nq_equal = new UnicodeNqSpace(true);
  assert(nq_equal->status() == Gecode::SS_FAILED);
  delete nq_equal;

  UnicodeIncreasingSpace* increasing = new UnicodeIncreasingSpace(
    StringVal::from_symbols({0x65E5, 0x672C, 0x1F600}), true);
  assert(increasing->status() != Gecode::SS_FAILED);
  delete increasing;

  UnicodeIncreasingSpace* not_increasing = new UnicodeIncreasingSpace(
    StringVal::from_symbols({0x1F600, 0x65E5}), true);
  assert(not_increasing->status() == Gecode::SS_FAILED);
  delete not_increasing;

  UnicodeIncreasingSpace* non_strict_equal = new UnicodeIncreasingSpace(
    StringVal::from_symbols({0x65E5, 0x65E5}), false);
  assert(non_strict_equal->status() != Gecode::SS_FAILED);
  delete non_strict_equal;

  UnicodeIncreasingSpace* strict_equal = new UnicodeIncreasingSpace(
    StringVal::from_symbols({0x65E5, 0x65E5}), true);
  assert(strict_equal->status() == Gecode::SS_FAILED);
  delete strict_equal;

  UnicodeRevSpace* reverse = new UnicodeRevSpace(false);
  assert(reverse->status() != Gecode::SS_FAILED);
  assert(reverse->result.assigned());
  assert(reverse->result.val_symbols() ==
         StringVal::from_symbols({0x1F600, 0x65E5, 'A'}));
  delete reverse;

  UnicodeRevSpace* reverse_mismatch = new UnicodeRevSpace(true);
  assert(reverse_mismatch->status() == Gecode::SS_FAILED);
  delete reverse_mismatch;

  UnicodePowSpace* power = new UnicodePowSpace(false);
  assert(power->status() != Gecode::SS_FAILED);
  assert(power->result.assigned());
  assert(power->result.val_symbols() == StringVal::from_symbols(
    {0x65E5, 0x1F600, 0x65E5, 0x1F600}));
  delete power;

  UnicodePowSpace* power_mismatch = new UnicodePowSpace(true);
  assert(power_mismatch->status() == Gecode::SS_FAILED);
  delete power_mismatch;

  UnicodeGConcatSpace* gconcat_result =
    new UnicodeGConcatSpace(UnicodeGConcatSpace::InferResult);
  assert(gconcat_result->status() != Gecode::SS_FAILED);
  assert(gconcat_result->result.assigned());
  assert(gconcat_result->result.val_symbols() ==
         StringVal::from_symbols({'A', 0x65E5, 0x1F600}));
  delete gconcat_result;

  UnicodeGConcatSpace* gconcat_match =
    new UnicodeGConcatSpace(UnicodeGConcatSpace::FixedMatch);
  assert(gconcat_match->status() != Gecode::SS_FAILED);
  delete gconcat_match;

  UnicodeGConcatSpace* gconcat_mismatch =
    new UnicodeGConcatSpace(UnicodeGConcatSpace::FixedMismatch);
  assert(gconcat_mismatch->status() == Gecode::SS_FAILED);
  delete gconcat_mismatch;

  UnicodeCharAtSpace* char_at = new UnicodeCharAtSpace(2);
  assert(char_at->status() != Gecode::SS_FAILED);
  assert(char_at->result.assigned());
  assert(char_at->result.val_symbols() ==
         StringVal::from_symbols({0x65E5}));
  delete char_at;

  UnicodeCharAtSpace* char_at_empty = new UnicodeCharAtSpace(4);
  assert(char_at_empty->status() != Gecode::SS_FAILED);
  assert(char_at_empty->result.assigned());
  assert(char_at_empty->result.val_symbols().empty());
  delete char_at_empty;

  UnicodeSubstringSpace* substring_result =
    new UnicodeSubstringSpace(UnicodeSubstringSpace::InferResult);
  assert(substring_result->status() != Gecode::SS_FAILED);
  assert(substring_result->result.assigned());
  assert(substring_result->result.val_symbols() ==
         StringVal::from_symbols({0x65E5, 0x1F600}));
  delete substring_result;

  UnicodeSubstringSpace* substring_bounds =
    new UnicodeSubstringSpace(UnicodeSubstringSpace::InferBounds);
  assert(substring_bounds->status() != Gecode::SS_FAILED);
  assert(substring_bounds->from.assigned() && substring_bounds->from.val() == 2);
  assert(substring_bounds->to.assigned() && substring_bounds->to.val() == 3);
  delete substring_bounds;

  ByteSubstringUnicodeResultSpace* byte_unicode_substring =
    new ByteSubstringUnicodeResultSpace;
  assert(byte_unicode_substring->status() == Gecode::SS_FAILED);
  delete byte_unicode_substring;

  UnicodeFindSpace* find_assigned =
    new UnicodeFindSpace(UnicodeFindSpace::AssignedUnicode);
  assert(find_assigned->status() != Gecode::SS_FAILED);
  assert(find_assigned->index.assigned() && find_assigned->index.val() == 2);
  delete find_assigned;

  UnicodeFindSpace* find_byte_pattern =
    new UnicodeFindSpace(UnicodeFindSpace::BytePatternUnicodeHaystack);
  assert(find_byte_pattern->status() != Gecode::SS_FAILED);
  assert(find_byte_pattern->index.assigned() &&
         find_byte_pattern->index.val() == 2);
  delete find_byte_pattern;

  UnicodeFindSpace* find_prefix =
    new UnicodeFindSpace(UnicodeFindSpace::KnownUnicodePrefix);
  assert(find_prefix->status() != Gecode::SS_FAILED);
  assert(find_prefix->index.assigned() && find_prefix->index.val() == 1);
  delete find_prefix;

  UnicodeFindSpace* find_no_match =
    new UnicodeFindSpace(UnicodeFindSpace::RemoveUnicodeNoMatch);
  assert(find_no_match->status() != Gecode::SS_FAILED);
  assert(find_no_match->haystack.assigned());
  assert(find_no_match->haystack.val_symbols() ==
         StringVal::from_symbols({0x672C}));
  delete find_no_match;

  UnicodeFindSpace* find_pattern =
    new UnicodeFindSpace(UnicodeFindSpace::InferUnicodePattern);
  assert(find_pattern->status() != Gecode::SS_FAILED);
  assert(find_pattern->pattern.assigned());
  assert(find_pattern->pattern.val_symbols() ==
         StringVal::from_symbols({0x65E5, 0x1F600}));
  delete find_pattern;

  UnicodeContainsSpace* contains = new UnicodeContainsSpace;
  assert(contains->status() != Gecode::SS_FAILED);
  assert(contains->result.assigned() && contains->result.val() == 1);
  delete contains;

  UnicodeRegularSpace* regular_literal =
    new UnicodeRegularSpace(UnicodeRegularSpace::AssignedLiteral);
  assert(regular_literal->status() != Gecode::SS_FAILED);
  delete regular_literal;

  UnicodeRegularSpace* rejected_regular_literal =
    new UnicodeRegularSpace(UnicodeRegularSpace::RejectedLiteral);
  assert(rejected_regular_literal->status() == Gecode::SS_FAILED);
  delete rejected_regular_literal;

  UnicodeRegularSpace* propagated_regular_class =
    new UnicodeRegularSpace(UnicodeRegularSpace::PropagatedClass);
  assert(propagated_regular_class->status() != Gecode::SS_FAILED);
  assert(propagated_regular_class->value.val_symbols() ==
         StringVal::from_symbols({0x1F600}));
  delete propagated_regular_class;

  UnicodeRegularSpace* filtered_regular_class =
    new UnicodeRegularSpace(UnicodeRegularSpace::FilterClass);
  assert(filtered_regular_class->status() != Gecode::SS_FAILED);
  assert(filtered_regular_class->value.may_chars().in(0x65E5));
  assert(filtered_regular_class->value.may_chars().in(0x1F600));
  assert(!filtered_regular_class->value.may_chars().in(0x672C));
  delete filtered_regular_class;

  UnicodeRegularSpace* regular_active_alphabet =
    new UnicodeRegularSpace(UnicodeRegularSpace::ActiveAlphabetStar);
  assert(regular_active_alphabet->status() != Gecode::SS_FAILED);
  delete regular_active_alphabet;

  UnicodeRegularSpace* regular_maximum =
    new UnicodeRegularSpace(UnicodeRegularSpace::MaximumScalar);
  assert(regular_maximum->status() != Gecode::SS_FAILED);
  delete regular_maximum;

  UnicodeReifiedRegularSpace* reified_regular_accept =
    new UnicodeReifiedRegularSpace(
      UnicodeReifiedRegularSpace::AssignedAccept);
  assert(reified_regular_accept->status() != Gecode::SS_FAILED);
  assert(reified_regular_accept->result.assigned() &&
         reified_regular_accept->result.val() == 1);
  delete reified_regular_accept;

  UnicodeReifiedRegularSpace* reified_regular_reject =
    new UnicodeReifiedRegularSpace(
      UnicodeReifiedRegularSpace::AssignedReject);
  assert(reified_regular_reject->status() != Gecode::SS_FAILED);
  assert(reified_regular_reject->result.assigned() &&
         reified_regular_reject->result.val() == 0);
  delete reified_regular_reject;

  UnicodeReifiedRegularSpace* reified_regular_propagated =
    new UnicodeReifiedRegularSpace(
      UnicodeReifiedRegularSpace::PropagatedAccept);
  assert(reified_regular_propagated->status() != Gecode::SS_FAILED);
  assert(reified_regular_propagated->result.assigned() &&
         reified_regular_propagated->result.val() == 1);
  delete reified_regular_propagated;

  UnicodeStrToNatSpace* unicode_str_to_nat =
    new UnicodeStrToNatSpace;
  assert(unicode_str_to_nat->status() != Gecode::SS_FAILED);
  assert(unicode_str_to_nat->value.assigned());
  assert(unicode_str_to_nat->number.assigned() &&
         unicode_str_to_nat->number.val() == -1);
  delete unicode_str_to_nat;

  UnicodeGCCSpace* unicode_gcc = new UnicodeGCCSpace;
  assert(unicode_gcc->status() != Gecode::SS_FAILED);
  assert(unicode_gcc->counts[0].assigned() &&
         unicode_gcc->counts[0].val() == 2);
  assert(unicode_gcc->counts[1].assigned() &&
         unicode_gcc->counts[1].val() == 1);
  assert(unicode_gcc->counts[2].assigned() &&
         unicode_gcc->counts[2].val() == 1);
  assert(unicode_gcc->counts[3].assigned() &&
         unicode_gcc->counts[3].val() == 0);
  delete unicode_gcc;

  UnicodeReplaceSpace* replace_first =
    new UnicodeReplaceSpace(UnicodeReplaceSpace::First);
  assert(replace_first->status() != Gecode::SS_FAILED);
  assert(replace_first->result.val_symbols() ==
         StringVal::from_symbols({0x672C, 0x65E5, 0x1F600}));
  delete replace_first;

  UnicodeReplaceSpace* replace_last =
    new UnicodeReplaceSpace(UnicodeReplaceSpace::Last);
  assert(replace_last->status() != Gecode::SS_FAILED);
  assert(replace_last->result.val_symbols() ==
         StringVal::from_symbols({0x65E5, 0x1F600, 0x672C}));
  delete replace_last;

  UnicodeReplaceSpace* replace_every =
    new UnicodeReplaceSpace(UnicodeReplaceSpace::All);
  assert(replace_every->status() != Gecode::SS_FAILED);
  assert(replace_every->result.val_symbols() ==
         StringVal::from_symbols({0x672C, 0x672C}));
  delete replace_every;

  UnicodeReplaceSpace* replace_empty =
    new UnicodeReplaceSpace(UnicodeReplaceSpace::AllEmptyQuery);
  assert(replace_empty->status() != Gecode::SS_FAILED);
  assert(replace_empty->result.val_symbols() == StringVal::from_symbols(
    {0x672C, 0x65E5, 0x672C, 0x1F600, 0x672C}));
  delete replace_empty;

  UnicodeReplaceSpace* replace_variable =
    new UnicodeReplaceSpace(UnicodeReplaceSpace::AllVariableReplacement);
  assert(replace_variable->status() != Gecode::SS_FAILED);
  assert(replace_variable->replacement.assigned());
  assert(replace_variable->replacement.val_symbols() ==
         StringVal::from_symbols({0x672C}));
  delete replace_variable;

  UnicodeReplaceSpace* replace_byte_query =
    new UnicodeReplaceSpace(UnicodeReplaceSpace::ByteQueryUnicodeSource);
  assert(replace_byte_query->status() != Gecode::SS_FAILED);
  assert(replace_byte_query->result.val_symbols() ==
         StringVal::from_symbols({0x1F600, 0x65E5, 0x1F600}));
  delete replace_byte_query;

  UnicodeReplaceSpace* replace_maximum =
    new UnicodeReplaceSpace(UnicodeReplaceSpace::MaximumScalar);
  assert(replace_maximum->status() != Gecode::SS_FAILED);
  assert(replace_maximum->result.val_symbols() ==
         StringVal::from_symbols({0x1F600, 0x65E5}));
  delete replace_maximum;

  UnicodeReplaceSpace* replace_known_occurrence =
    new UnicodeReplaceSpace(UnicodeReplaceSpace::KnownUnicodeOccurrence);
  assert(replace_known_occurrence->status() != Gecode::SS_FAILED);
  assert(replace_known_occurrence->source.assigned());
  assert(replace_known_occurrence->source.val_symbols() ==
         StringVal::from_symbols({0x65E5, 0x1F600}));
  delete replace_known_occurrence;

  return 0;
}
