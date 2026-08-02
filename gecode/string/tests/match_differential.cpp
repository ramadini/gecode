#include <gecode/search.hh>
#include <gecode/string.hh>

#include <cassert>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace Gecode;
using namespace Gecode::String;

namespace {

  typedef std::pair<std::string, int> Solution;

  class CountingDomain : public UnaryPropagator
    <StringView, PC_STRING_DOM> {
  public:
    static int propagations;
    static int assigned_propagations;

    CountingDomain(Home home, StringView text)
      : UnaryPropagator<StringView, PC_STRING_DOM>(home, text) {}

    CountingDomain(Space& home, CountingDomain& other)
      : UnaryPropagator<StringView, PC_STRING_DOM>(home, other) {}

    virtual Actor* copy(Space& home) {
      return new (home) CountingDomain(home, *this);
    }

    virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
      ++propagations;
      if (!x0.assigned())
        return ES_FIX;
      ++assigned_propagations;
      return home.ES_SUBSUMED(*this);
    }
  };

  int CountingDomain::propagations = 0;
  int CountingDomain::assigned_propagations = 0;

  StringVar
  make_text(Space& home) {
    NSBlocks domain(1, NSBlock(NSIntSet('a', 'b'), 1, 4));
    return StringVar(home, domain, 1, 4);
  }

  StringVar
  make_single_text(Space& home) {
    NSBlocks domain(1, NSBlock(NSIntSet('a', 'b'), 1, 1));
    return StringVar(home, domain, 1, 1);
  }

  class MatchModel : public Space {
  public:
    StringVar text;
    IntVar index;

    MatchModel(const std::string& pattern, bool use_new)
      : text(make_text(*this)),
        index(*this, 0, 5) {
      if (use_new)
        match_new(*this, text, pattern, index);
      else
        match(*this, text, pattern, index);
      branch(*this, index, INT_VAL_MIN());
      StringVarArgs strings;
      strings << text;
      blockmin_lllm(*this, strings);
    }

    MatchModel(MatchModel& model) : Space(model) {
      text.update(*this, model.text);
      index.update(*this, model.index);
    }

    virtual Space* copy(void) {
      return new MatchModel(*this);
    }
  };

  class PostingFailureModel : public Space {
  public:
    StringVar text;
    IntVar index;

    PostingFailureModel(bool use_new)
      : text(*this, "a"), index(*this, 2, 2) {
      if (use_new)
        match_new(*this, text, "a", index);
      else
        match(*this, text, "a", index);
    }

    PostingFailureModel(PostingFailureModel& model) : Space(model) {
      text.update(*this, model.text);
      index.update(*this, model.index);
    }

    virtual Space* copy(void) {
      return new PostingFailureModel(*this);
    }
  };

  class NotificationModel : public Space {
  public:
    StringVar text;
    IntVar index;

    NotificationModel(void)
      : text(make_single_text(*this)), index(*this, 0, 0) {
      (void) new (*this) CountingDomain(*this, text);
    }

    NotificationModel(NotificationModel& model) : Space(model) {
      text.update(*this, model.text);
      index.update(*this, model.index);
    }

    virtual Space* copy(void) {
      return new NotificationModel(*this);
    }

    void refine(void) {
      NSBlocks domain(1, NSBlock(NSIntSet('a'), 1, 1));
      assert(!me_failed(StringView(text).varimp()->refine(*this, domain)));
    }
  };

  std::set<Solution>
  actual_solutions(const std::string& pattern, bool use_new) {
    MatchModel* model = new MatchModel(pattern, use_new);
    DFS<MatchModel> search(model);
    delete model;

    std::set<Solution> actual;
    while (MatchModel* solution = search.next()) {
      assert(solution->text.assigned() && solution->index.assigned());
      actual.insert(Solution(solution->text.val(), solution->index.val()));
      delete solution;
    }
    return actual;
  }

}

int
main(void) {
  PostingFailureModel legacy_failure(false);
  PostingFailureModel new_failure(true);
  assert(legacy_failure.status() == SS_FAILED);
  assert(new_failure.status() == SS_FAILED);

  CountingDomain::propagations = 0;
  CountingDomain::assigned_propagations = 0;
  NotificationModel initial;
  assert(initial.status() != SS_FAILED);
  NotificationModel* notification =
    static_cast<NotificationModel*>(initial.clone());
  const int before = CountingDomain::propagations;
  notification->refine();
  assert(notification->status() != SS_FAILED);
  assert(CountingDomain::propagations > before);
  assert(CountingDomain::assigned_propagations > 0);
  delete notification;

  const std::vector<std::string> patterns = {
    "a", "b", "ab", "aa", "a|b", "(ab|b)", "a*b"
  };

  for (const std::string& pattern : patterns) {
    const std::set<Solution> legacy = actual_solutions(pattern, false);
    const std::set<Solution> modern = actual_solutions(pattern, true);
    if (legacy != modern) {
      std::cerr << "Mismatch for pattern " << pattern
                << ": legacy " << legacy.size()
                << ", new " << modern.size() << std::endl;
      return 1;
    }
  }

  std::cout << "Match differential tests passed" << std::endl;
  return 0;
}