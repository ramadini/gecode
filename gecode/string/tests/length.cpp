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

static StringVar
two_optional_characters(Space& home) {
  String::NSBlocks domain;
  domain.push_back(String::NSBlock(String::NSIntSet('a'), 0, 1));
  domain.push_back(String::NSBlock(String::NSIntSet('b'), 0, 1));
  return StringVar(home, domain, 0, 2);
}

static StringVar
hole_length_string(Space& home) {
  String::NSBlocks domain(1,
    String::NSBlock(String::NSIntSet('a'), 2, 3));
  return StringVar(home, domain, 2, 3);
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

  LengthEventModel(int fixed_length)
    : string(two_optional_characters(*this)),
      string_length(*this, fixed_length, fixed_length) {
    (void) new (*this) CountingLength(*this, string, string_length);
  }

  LengthEventModel(const IntSet& lengths)
    : string(hole_length_string(*this)), string_length(*this, lengths) {
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

  void restrict_max_length(int length) {
    ModEvent me = String::StringView(string).ub(*this, length);
    assert(me != String::ME_STRING_FAILED);
  }

  void restrict_integer_min(int length) {
    rel(*this, string_length, IRT_GQ, length);
  }

  void restrict_integer_max(int length) {
    rel(*this, string_length, IRT_LQ, length);
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

  LengthEventModel* string_max_change = new LengthEventModel;
  assert(string_max_change->status() != SS_FAILED);
  before = CountingLength::propagations;
  string_max_change->restrict_max_length(2);
  assert(string_max_change->status() != SS_FAILED);
  assert(CountingLength::propagations == before + 1);
  assert(string_max_change->string_length.max() == 2);

  LengthEventModel* integer_max_change = new LengthEventModel;
  assert(integer_max_change->status() != SS_FAILED);
  before = CountingLength::propagations;
  integer_max_change->restrict_integer_max(2);
  assert(integer_max_change->status() != SS_FAILED);
  assert(CountingLength::propagations == before + 1);
  assert(integer_max_change->string.max_length() == 2);

  LengthEventModel* optional_characters = new LengthEventModel(1);
  assert(optional_characters->status() != SS_FAILED);
  assert(optional_characters->string.min_length() == 1);
  assert(optional_characters->string.max_length() == 1);

  int sparse_values[] = {1, 3};
  LengthEventModel* sparse_length =
    new LengthEventModel(IntSet(sparse_values, 2));
  assert(sparse_length->status() != SS_FAILED);
  assert(sparse_length->string_length.assigned());
  assert(sparse_length->string_length.val() == 3);
  assert(sparse_length->string.min_length() == 3);
  assert(sparse_length->string.max_length() == 3);

  delete sparse_length;
  delete optional_characters;
  delete integer_max_change;
  delete string_max_change;
  delete integer_change;
  delete clone;
  delete model;
  return 0;
}