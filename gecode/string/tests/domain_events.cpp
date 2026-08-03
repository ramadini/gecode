#include <gecode/string.hh>

#include <cassert>
#include <type_traits>
#include <utility>

using namespace Gecode;

static_assert(
  std::is_same<
    decltype(std::declval<String::StringView>().domain()),
    const String::DashedString&
  >::value,
  "StringView domain access must remain read-only");

class CountingDomain : public UnaryPropagator
  <String::StringView, String::PC_STRING_DOM> {
public:
  static int propagations;

  CountingDomain(Home home, String::StringView string)
    : UnaryPropagator<String::StringView, String::PC_STRING_DOM>
      (home, string) {}

  CountingDomain(Space& home, CountingDomain& other)
    : UnaryPropagator<String::StringView, String::PC_STRING_DOM>
      (home, other) {}

  virtual Actor* copy(Space& home) {
    return new (home) CountingDomain(home, *this);
  }

  virtual ExecStatus propagate(Space&, const ModEventDelta& med) {
    (void) med;
    ++propagations;
    return ES_FIX;
  }
};

int CountingDomain::propagations = 0;

static StringVar
star_source(Space& home) {
  String::NSBlocks domain;
  domain.push_back(
    String::NSBlock(String::NSIntSet('a', 'b'), 0, 1));
  domain.push_back(
    String::NSBlock(String::NSIntSet('a', 'c'), 0, 1));
  return StringVar(home, domain, 0, 2);
}

class EventSpace : public Space {
public:
  EventSpace(void) {}
  EventSpace(EventSpace& other) : Space(other) {}
  virtual Space* copy(void) {
    return new EventSpace(*this);
  }
};

class RegexPostingModel : public Space {
public:
  StringVar string;

  RegexPostingModel(void)
    : string(star_source(*this)) {
    (void) new (*this) CountingDomain(*this, string);
  }

  RegexPostingModel(RegexPostingModel& other) : Space(other) {
    string.update(*this, other.string);
  }

  virtual Space* copy(void) {
    return new RegexPostingModel(*this);
  }

  void restrict_to_a_star(void) {
    VarArgs auxiliaries;
    extensional(*this, string, "a*", auxiliaries);
  }
};

static void
check_mod_events(void) {
  {
    EventSpace home;
    StringVar string(home, String::NSIntSet('a', 'c'), 1, 2);
    String::NSBlocks same(1,
      String::NSBlock(String::NSIntSet('a', 'c'), 1, 2));
    assert(String::StringView(string).dom(home, same) ==
           String::ME_STRING_NONE);

    String::NSBlocks restricted(1,
      String::NSBlock(String::NSIntSet('a', 'b'), 1, 2));
    assert(String::StringView(string).dom(home, restricted) ==
           String::ME_STRING_DOM);
  }
  {
    EventSpace home;
    StringVar string(home, String::NSIntSet('a', 'b'), 0, 3);
    assert(String::StringView(string).lb(home, 1) == String::ME_STRING_LEN);
  }
  {
    EventSpace home;
    StringVar string(home, String::NSIntSet('a', 'b'), 1, 1);
    assert(String::StringView(string).eq(home, "a") == String::ME_STRING_VAL);
  }
  {
    EventSpace home;
    StringVar string(home, String::NSIntSet('a', 'b'), 1, 1);
    String::NSBlocks disjoint(1,
      String::NSBlock(String::NSIntSet('c'), 1, 1));
    assert(String::StringView(string).dom(home, disjoint) ==
           String::ME_STRING_FAILED);
  }
}

int
main(void) {
  check_mod_events();

  CountingDomain::propagations = 0;
  RegexPostingModel model;
  assert(model.status() != SS_FAILED);
  int before = CountingDomain::propagations;

  model.restrict_to_a_star();
  assert(model.status() != SS_FAILED);
  assert(CountingDomain::propagations > before);
  assert(String::StringView(model.string).may_chars() == String::NSIntSet('a'));
  assert(model.string.min_length() == 0 && model.string.max_length() == 2);
  return 0;
}
