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

  return 0;
}
