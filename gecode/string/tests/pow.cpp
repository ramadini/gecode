#include <gecode/string.hh>

#include <cassert>

using namespace Gecode;

class AliasIdentityModel : public Space {
public:
  StringVar value;
  IntVar exponent;

  AliasIdentityModel(int minimum = 1, int maximum = 1)
    : value(*this, String::NSIntSet('a', 'b'), 1, 1),
      exponent(*this, minimum, maximum) {
    pow(*this, value, exponent, value);
  }

  AliasIdentityModel(AliasIdentityModel& other) : Space(other) {
    value.update(*this, other.value);
    exponent.update(*this, other.exponent);
  }

  virtual Space* copy(void) {
    return new AliasIdentityModel(*this);
  }
};

int
main(void) {
  AliasIdentityModel* model = new AliasIdentityModel;
  assert(model->status() != SS_FAILED);
  assert(model->value.min_length() == 1);
  assert(model->value.max_length() == 1);
  assert(model->value.may_chars().contains(String::char2int('a')));
  assert(model->value.may_chars().contains(String::char2int('b')));
  delete model;

  AliasIdentityModel* mixed = new AliasIdentityModel(1, 2);
  assert(mixed->status() != SS_FAILED);
  assert(mixed->value.min_length() == 1);
  AliasIdentityModel* exponent_one =
    static_cast<AliasIdentityModel*>(mixed->clone());
  rel(*exponent_one, exponent_one->exponent, IRT_EQ, 1);
  assert(exponent_one->status() != SS_FAILED);
  AliasIdentityModel* exponent_two =
    static_cast<AliasIdentityModel*>(mixed->clone());
  rel(*exponent_two, exponent_two->exponent, IRT_EQ, 2);
  assert(exponent_two->status() == SS_FAILED);
  delete exponent_two;
  delete exponent_one;
  delete mixed;

  AliasIdentityModel* impossible = new AliasIdentityModel(2, 2);
  assert(impossible->status() == SS_FAILED);
  delete impossible;
  return 0;
}