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

  return 0;
}
