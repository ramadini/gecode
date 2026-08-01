#include <gecode/minimodel.hh>
#include <gecode/string.hh>
#include <gecode/string/find.hh>

#include <cassert>

using namespace Gecode;

static StringVar
repeated_chars(Space& home, int length) {
  String::NSBlocks domain(1,
    String::NSBlock(String::NSIntSet('a', 'b'), length, length));
  return StringVar(home, domain, length, length);
}

static StringVar
optional_a_then_b(Space& home) {
  String::NSBlocks domain;
  domain.push_back(String::NSBlock(String::NSIntSet('a'), 0, 1));
  domain.push_back(String::NSBlock(String::NSIntSet('b'), 1, 1));
  return StringVar(home, domain, 1, 2);
}

class FindModel : public Space {
public:
  StringVar haystack;
  IntVar index;

  FindModel(void)
    : haystack(repeated_chars(*this, 3)),
      index(*this, 0, 0) {
    find(*this, StringVar(*this, "a"), haystack, index);
  }

  FindModel(FindModel& other) : Space(other) {
    haystack.update(*this, other.haystack);
    index.update(*this, other.index);
  }

  virtual Space* copy(void) {
    return new FindModel(*this);
  }
};

class CountingFind : public String::Find {
public:
  static int propagations;

  CountingFind(Home home, String::StringView pattern,
               String::StringView haystack, Int::IntView index)
    : Find(home, pattern, haystack, index) {}

  virtual ExecStatus propagate(Space& home, const ModEventDelta& med) {
    ++propagations;
    return Find::propagate(home, med);
  }
};

int CountingFind::propagations = 0;

class FindEventModel : public Space {
public:
  IntVar index;

  FindEventModel(void) : index(*this, 0, 2) {
    StringVar pattern(repeated_chars(*this, 1));
    StringVar haystack(repeated_chars(*this, 2));
    (void) new (*this) CountingFind(*this, pattern, haystack, index);
  }

  FindEventModel(FindEventModel& other) : Space(other) {
    index.update(*this, other.index);
  }

  virtual Space* copy(void) {
    return new FindEventModel(*this);
  }
};

class FindLengthModel : public Space {
public:
  StringVar haystack;
  IntVar haystack_length;

  FindLengthModel(void)
    : haystack(optional_a_then_b(*this)), haystack_length(*this, 1, 2) {
    length(*this, haystack, haystack_length);
    find(*this, StringVar(*this, "a"), haystack, IntVar(*this, 0, 0));
  }

  FindLengthModel(FindLengthModel& other) : Space(other) {
    haystack.update(*this, other.haystack);
    haystack_length.update(*this, other.haystack_length);
  }

  virtual Space* copy(void) {
    return new FindLengthModel(*this);
  }
};

int
main(void) {
  FindModel* zero = new FindModel;
  assert(zero->status() != SS_FAILED);
  assert(zero->haystack.assigned());
  assert(zero->haystack.val() == "bbb");
  delete zero;

  CountingFind::propagations = 0;
  FindEventModel* events = new FindEventModel;
  assert(events->status() != SS_FAILED);
  assert(events->index.in(1));
  int before = CountingFind::propagations;
  rel(*events, events->index, IRT_NQ, 1);
  assert(events->status() != SS_FAILED);
  assert(CountingFind::propagations > before);
  delete events;

  FindLengthModel* length_event = new FindLengthModel;
  assert(length_event->status() != SS_FAILED);
  assert(length_event->haystack.assigned());
  assert(length_event->haystack.val() == "b");
  assert(length_event->haystack_length.assigned());
  assert(length_event->haystack_length.val() == 1);
  delete length_event;
  return 0;
}