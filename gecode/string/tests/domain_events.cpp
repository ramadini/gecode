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

static_assert(
  !std::is_constructible<
    String::StringVarImp::DomainState, String::StringVarImp&
  >::value,
  "Domain refinement transactions must only be created by StringVarImp");

template<class T>
class HasRawStringNotify {
private:
  template<class U>
  static auto test(int) -> decltype(
    std::declval<U&>().notify(std::declval<Space&>(), 0, 0),
    std::true_type());

  template<class>
  static std::false_type test(...);

public:
  static const bool value = decltype(test<T>(0))::value;
};

static_assert(
  !HasRawStringNotify<String::StringVarImp>::value,
  "Raw string-domain notification must remain internal");

class CountingDomain : public UnaryPropagator
  <String::StringView, String::PC_STRING_DOM> {
private:
  int index;

public:
  static int propagations[2];

  CountingDomain(Home home, String::StringView string, int index0 = 0)
    : UnaryPropagator<String::StringView, String::PC_STRING_DOM>
      (home, string), index(index0) {}

  CountingDomain(Space& home, CountingDomain& other)
    : UnaryPropagator<String::StringView, String::PC_STRING_DOM>
      (home, other), index(other.index) {}

  virtual Actor* copy(Space& home) {
    return new (home) CountingDomain(home, *this);
  }

  virtual ExecStatus propagate(Space&, const ModEventDelta& med) {
    (void) med;
    ++propagations[index];
    return ES_FIX;
  }
};

int CountingDomain::propagations[2] = {0, 0};

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

static void
check_combined_notifications(void) {
  EventSpace home;
  StringVar left(home, String::NSIntSet('a', 'b'), 1, 1);
  StringVar right(home, String::NSIntSet('b', 'c'), 1, 1);
  (void) new (home) CountingDomain(home, left, 0);
  (void) new (home) CountingDomain(home, right, 1);

  assert(home.status() != SS_FAILED);
  int left_before = CountingDomain::propagations[0];
  int right_before = CountingDomain::propagations[1];

  String::StringView left_view(left);
  String::StringView right_view(right);
  assert(!me_failed(left_view.eq(home, right_view)));
  assert(home.status() != SS_FAILED);
  assert(CountingDomain::propagations[0] > left_before);
  assert(CountingDomain::propagations[1] > right_before);
}

int
main(void) {
  check_mod_events();
  check_combined_notifications();

  CountingDomain::propagations[0] = 0;
  RegexPostingModel model;
  assert(model.status() != SS_FAILED);
  int before = CountingDomain::propagations[0];

  model.restrict_to_a_star();
  assert(model.status() != SS_FAILED);
  assert(CountingDomain::propagations[0] > before);
  assert(String::StringView(model.string).may_chars() == String::NSIntSet('a'));
  assert(model.string.min_length() == 0 && model.string.max_length() == 2);
  return 0;
}
