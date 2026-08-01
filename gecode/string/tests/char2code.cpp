#include <gecode/minimodel.hh>
#include <gecode/string.hh>
#include <gecode/string/char2code.hh>

#include <cassert>

using namespace Gecode;

static StringVar
character_string(Space& home, int minimum, int maximum, bool optional) {
  int min_length = optional ? 0 : 1;
  String::NSBlocks domain(1,
    String::NSBlock(String::NSIntSet(minimum, maximum), min_length, 1));
  return StringVar(home, domain, min_length, 1);
}

static StringVar
multi_block_string(Space& home) {
  String::NSBlocks domain;
  domain.push_back(String::NSBlock(String::NSIntSet('a'), 0, 1));
  domain.push_back(String::NSBlock(String::NSIntSet('b'), 0, 1));
  return StringVar(home, domain, 0, 1);
}

class CountingDomain : public UnaryPropagator
  <String::StringView, String::PC_STRING_DOM> {
public:
  static int propagations;

  CountingDomain(Home home, String::StringView string)
    : UnaryPropagator<String::StringView, String::PC_STRING_DOM>(home, string) {}

  CountingDomain(Space& home, CountingDomain& other)
    : UnaryPropagator<String::StringView, String::PC_STRING_DOM>(home, other) {}

  virtual Actor* copy(Space& home) {
    return new (home) CountingDomain(home, *this);
  }

  virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
    ++propagations;
    return x0.assigned() ? home.ES_SUBSUMED(*this) : ES_FIX;
  }
};

int CountingDomain::propagations = 0;

class Char2CodeModel : public Space {
public:
  StringVar string;
  IntVar code;

  Char2CodeModel(int minimum = 'a', int maximum = 'b', bool optional = false)
    : string(character_string(*this, minimum, maximum, optional)),
      code(*this, -1, maximum) {
    char2code(*this, string, code);
    (void) new (*this) CountingDomain(*this, string);
  }

  Char2CodeModel(Char2CodeModel& other) : Space(other) {
    string.update(*this, other.string);
    code.update(*this, other.code);
  }

  virtual Space* copy(void) {
    return new Char2CodeModel(*this);
  }

  void assign_code(int value) {
    rel(*this, code, IRT_EQ, value);
  }
};

class MultiBlockModel : public Space {
public:
  StringVar string;
  IntVar code;

  MultiBlockModel(void)
    : string(multi_block_string(*this)), code(*this, -1, 'b') {
    char2code(*this, string, code);
  }

  MultiBlockModel(MultiBlockModel& other) : Space(other) {
    string.update(*this, other.string);
    code.update(*this, other.code);
  }

  virtual Space* copy(void) {
    return new MultiBlockModel(*this);
  }
};

int
main(void) {
  CountingDomain::propagations = 0;
  Char2CodeModel* notification = new Char2CodeModel;
  assert(notification->status() != SS_FAILED);
  Char2CodeModel* clone = static_cast<Char2CodeModel*>(notification->clone());
  int before = CountingDomain::propagations;
  clone->assign_code('a');
  assert(clone->status() != SS_FAILED);
  assert(clone->string.assigned() && clone->string.val() == "a");
  assert(CountingDomain::propagations > before);
  delete clone;
  delete notification;

  Char2CodeModel* empty = new Char2CodeModel('a', 'b', true);
  assert(empty->status() != SS_FAILED);
  assert(empty->string.min_length() == 0 && empty->string.max_length() == 1);
  assert(empty->code.in(-1) && empty->code.in('a') && empty->code.in('b'));
  empty->assign_code(-1);
  assert(empty->status() != SS_FAILED);
  assert(empty->string.assigned() && empty->string.val().empty());
  delete empty;

  Char2CodeModel* zero = new Char2CodeModel(0, 0);
  zero->assign_code(0);
  assert(zero->status() != SS_FAILED);
  assert(zero->string.assigned());
  assert(zero->string.val().size() == 1 && zero->string.val()[0] == '\0');
  delete zero;

  MultiBlockModel* multi_block = new MultiBlockModel;
  rel(*multi_block, multi_block->code, IRT_EQ, 'b');
  assert(multi_block->status() != SS_FAILED);
  assert(multi_block->string.assigned() && multi_block->string.val() == "b");
  delete multi_block;
  return 0;
}