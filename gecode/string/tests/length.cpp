#include <gecode/minimodel.hh>
#include <gecode/string.hh>
#include <gecode/string/length.hh>

#include <cassert>

using namespace Gecode;

static StringVar
variable_length_string(Space& home) {
  String::NSBlocks domain(1,
    String::NSBlock(String::NSIntSet('a', 'c'), 1, 3));
  return StringVar(home, domain, 1, 3);
}

class CountingLength : public String::Length {
public:
  static int propagations;

  CountingLength(Home home, String::StringView string,
                 Int::IntView length)
    : Length(home, string, length) {}

  CountingLength(Space& home, CountingLength& other)
    : Length(home, other) {}

  virtual Actor* copy(Space& home) {
    return new (home) CountingLength(home, *this);
  }

  virtual ExecStatus propagate(Space& home, const ModEventDelta& med) {
    ++propagations;
    return Length::propagate(home, med);
  }
};

int CountingLength::propagations = 0;

class LengthEventModel : public Space {
public:
  StringVar string;
  IntVar string_length;

  LengthEventModel(void)
    : string(variable_length_string(*this)), string_length(*this, 1, 3) {
    (void) new (*this) CountingLength(*this, string, string_length);
  }

  LengthEventModel(LengthEventModel& other) : Space(other) {
    string.update(*this, other.string);
    string_length.update(*this, other.string_length);
  }

  virtual Space* copy(void) {
    return new LengthEventModel(*this);
  }

  void restrict_characters(char maximum) {
    String::NSBlocks domain(1,
      String::NSBlock(String::NSIntSet('a', maximum), 1, 3));
    ModEvent me = String::StringView(string).dom(*this, domain);
    assert(me != String::ME_STRING_FAILED);
  }

  void restrict_min_length(int length) {
    ModEvent me = String::StringView(string).lb(*this, length);
    assert(me != String::ME_STRING_FAILED);
  }

  void restrict_integer_min(int length) {
    rel(*this, string_length, IRT_GQ, length);
  }
};

int
main(void) {
  CountingLength::propagations = 0;
  LengthEventModel* model = new LengthEventModel;
  assert(model->status() != SS_FAILED);

  int before = CountingLength::propagations;
  model->restrict_characters('b');
  assert(model->status() != SS_FAILED);
  assert(CountingLength::propagations == before);

  LengthEventModel* clone = static_cast<LengthEventModel*>(model->clone());
  before = CountingLength::propagations;
  clone->restrict_characters('a');
  assert(clone->status() != SS_FAILED);
  assert(CountingLength::propagations == before);

  clone->restrict_min_length(2);
  assert(clone->status() != SS_FAILED);
  assert(CountingLength::propagations > before);
  assert(clone->string_length.min() == 2);

  LengthEventModel* integer_change = new LengthEventModel;
  assert(integer_change->status() != SS_FAILED);
  before = CountingLength::propagations;
  integer_change->restrict_integer_min(2);
  assert(integer_change->status() != SS_FAILED);
  assert(CountingLength::propagations == before + 1);
  assert(integer_change->string.min_length() == 2);

  delete integer_change;
  delete clone;
  delete model;
  return 0;
}