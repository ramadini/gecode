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
optional_chars(Space& home, int max_length) {
  String::NSBlocks domain(1,
    String::NSBlock(String::NSIntSet('a', 'b'), 0, max_length));
  return StringVar(home, domain, 0, max_length);
}

static StringVar
optional_char(Space& home, char value) {
  String::NSBlocks domain(1,
    String::NSBlock(String::NSIntSet(value), 0, 1));
  return StringVar(home, domain, 0, 1);
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

class FindFixpointModel : public Space {
public:
  StringVar pattern;
  StringVar haystack;
  IntVar index;

  FindFixpointModel(void)
    : pattern(optional_chars(*this, 2)),
      haystack(optional_chars(*this, 2)),
      index(*this, 1, 2) {
    (void) new (*this) CountingFind(*this, pattern, haystack, index);
  }

  FindFixpointModel(FindFixpointModel& other) : Space(other) {
    pattern.update(*this, other.pattern);
    haystack.update(*this, other.haystack);
    index.update(*this, other.index);
  }

  virtual Space* copy(void) {
    return new FindFixpointModel(*this);
  }
};

class EmptyPatternModel : public Space {
public:
  StringVar pattern;
  IntVar index;

  EmptyPatternModel(void)
    : pattern(optional_char(*this, 'a')),
      index(*this, 1, 1) {
    find(*this, pattern, StringVar(*this, "b"), index);
  }

  EmptyPatternModel(EmptyPatternModel& other) : Space(other) {
    pattern.update(*this, other.pattern);
    index.update(*this, other.index);
  }

  virtual Space* copy(void) {
    return new EmptyPatternModel(*this);
  }
};

class PredicateModel : public Space {
public:
  enum Kind {
    CONTAINS,
    STARTS_WITH,
    ENDS_WITH
  };

  StringVar x;
  StringVar y;
  BoolVar result;

  PredicateModel(Kind kind, const char* xv, const char* yv)
    : x(*this, xv), y(*this, yv), result(*this, 0, 1) {
    switch (kind) {
    case CONTAINS:
      contains(*this, x, y, result);
      break;
    case STARTS_WITH:
      startswith(*this, x, y, result);
      break;
    case ENDS_WITH:
      endswith(*this, x, y, result);
      break;
    }
  }

  PredicateModel(PredicateModel& other) : Space(other) {
    x.update(*this, other.x);
    y.update(*this, other.y);
    result.update(*this, other.result);
  }

  virtual Space* copy(void) {
    return new PredicateModel(*this);
  }
};

class ImpossibleContainsModel : public Space {
public:
  ImpossibleContainsModel(void) {
    contains(*this, StringVar(*this, ""), StringVar(*this, "a"));
  }

  ImpossibleContainsModel(ImpossibleContainsModel& other) : Space(other) {}

  virtual Space* copy(void) {
    return new ImpossibleContainsModel(*this);
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

  CountingFind::propagations = 0;
  FindFixpointModel* fixpoint = new FindFixpointModel;
  assert(fixpoint->status() != SS_FAILED);
  assert(CountingFind::propagations == 1);
  delete fixpoint;

  EmptyPatternModel* forced_empty = new EmptyPatternModel;
  assert(forced_empty->status() != SS_FAILED);
  assert(forced_empty->pattern.assigned());
  assert(forced_empty->pattern.val() == "");
  assert(forced_empty->index.assigned() && forced_empty->index.val() == 1);
  delete forced_empty;

  FindLengthModel* length_event = new FindLengthModel;
  assert(length_event->status() != SS_FAILED);
  assert(length_event->haystack.assigned());
  assert(length_event->haystack.val() == "b");
  assert(length_event->haystack_length.assigned());
  assert(length_event->haystack_length.val() == 1);
  delete length_event;

  PredicateModel* empty_contains =
    new PredicateModel(PredicateModel::CONTAINS, "", "");
  assert(empty_contains->status() != SS_FAILED);
  assert(empty_contains->result.assigned() && empty_contains->result.val() == 1);
  delete empty_contains;

  PredicateModel* empty_prefix =
    new PredicateModel(PredicateModel::STARTS_WITH, "", "");
  assert(empty_prefix->status() != SS_FAILED);
  assert(empty_prefix->result.assigned() && empty_prefix->result.val() == 1);
  delete empty_prefix;

  PredicateModel* long_prefix =
    new PredicateModel(PredicateModel::STARTS_WITH, "", "a");
  assert(long_prefix->status() != SS_FAILED);
  assert(long_prefix->result.assigned() && long_prefix->result.val() == 0);
  delete long_prefix;

  PredicateModel* empty_suffix =
    new PredicateModel(PredicateModel::ENDS_WITH, "", "");
  assert(empty_suffix->status() != SS_FAILED);
  assert(empty_suffix->result.assigned() && empty_suffix->result.val() == 1);
  delete empty_suffix;

  PredicateModel* long_suffix =
    new PredicateModel(PredicateModel::ENDS_WITH, "", "a");
  assert(long_suffix->status() != SS_FAILED);
  assert(long_suffix->result.assigned() && long_suffix->result.val() == 0);
  delete long_suffix;

  ImpossibleContainsModel* impossible = new ImpossibleContainsModel;
  assert(impossible->status() == SS_FAILED);
  delete impossible;
  return 0;
}
