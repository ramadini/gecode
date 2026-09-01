#include <gecode/search.hh>
#include <gecode/string.hh>
#include <gecode/string/match.hh>

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

  int min_length(const NSBlocks& domain);
  int max_length(const NSBlocks& domain);

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
  make_single_text(Space& home) {
    NSBlocks domain(1, NSBlock(NSIntSet('a', 'b'), 1, 1));
    return StringVar(home, domain, 1, 1);
  }

  StringVar
  make_text(Space& home, const NSBlocks& source,
            int min_length, int max_length) {
    NSBlocks domain(source);
    return StringVar(home, domain, min_length, max_length);
  }

  class MatchModel : public Space {
  public:
    StringVar text;
    IntVar index;

    MatchModel(const std::string& pattern, MatchImplementation implementation,
               const NSBlocks& domain, int min_length, int max_length)
      : text(make_text(*this, domain, min_length, max_length)),
        index(*this, 0, max_length + 1) {
      if (implementation == MATCH_AUTO)
        match(*this, text, pattern, index);
      else if (post_match(*this, text, pattern, index,
                          implementation) == ES_FAILED)
          fail();
      StringVarArgs strings;
      strings << text;
      blockmin_lllm(*this, strings);
      branch(*this, index, INT_VAL_MIN());
    }

    MatchModel(MatchModel& model) : Space(model) {
      text.update(*this, model.text);
      index.update(*this, model.index);
    }

    virtual Space* copy(void) {
      return new MatchModel(*this);
    }
  };

  class IncrementalModel : public Space {
  public:
    StringVar text;
    IntVar index;

    IncrementalModel(const std::string& pattern,
                     MatchImplementation implementation,
                     const NSBlocks& domain)
      : text(make_text(*this, domain, min_length(domain), max_length(domain))),
        index(*this, 0, max_length(domain) + 1) {
      if (implementation == MATCH_AUTO)
        match(*this, text, pattern, index);
      else if (post_match(*this, text, pattern, index,
                          implementation) == ES_FAILED)
        fail();
    }

    IncrementalModel(IncrementalModel& model) : Space(model) {
      text.update(*this, model.text);
      index.update(*this, model.index);
    }

    virtual Space* copy(void) {
      return new IncrementalModel(*this);
    }

    void constrain(const std::string& value, int position) {
      if (me_failed(Gecode::Int::IntView(index).eq(*this, position))) {
        fail();
        return;
      }
      NSBlocks exact(value);
      if (me_failed(StringView(text).varimp()->refine(*this, exact)))
        fail();
    }
  };

  class PostingFailureModel : public Space {
  public:
    StringVar text;
    IntVar index;

    PostingFailureModel(MatchImplementation implementation)
      : text(*this, "a"), index(*this, 2, 2) {
      if (post_match(*this, text, "a", index, implementation) == ES_FAILED)
        fail();
    }

    PostingFailureModel(PostingFailureModel& model) : Space(model) {
      text.update(*this, model.text);
      index.update(*this, model.index);
    }

    virtual Space* copy(void) {
      return new PostingFailureModel(*this);
    }
  };

  class InvalidPatternModel : public Space {
  public:
    StringVar text;
    IntVar index;

    InvalidPatternModel(void)
      : text(*this, "a"), index(*this, 0, 1) {
      match(*this, text, "a*", index);
    }

    InvalidPatternModel(InvalidPatternModel& model) : Space(model) {
      text.update(*this, model.text);
      index.update(*this, model.index);
    }

    virtual Space* copy(void) {
      return new InvalidPatternModel(*this);
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

  int
  min_length(const NSBlocks& domain) {
    int length = 0;
    for (const NSBlock& block : domain)
      length += block.l;
    return length;
  }

  int
  max_length(const NSBlocks& domain) {
    int length = 0;
    for (const NSBlock& block : domain)
      length += block.u;
    return length;
  }

  void enumerate_blocks(const NSBlocks&, unsigned int, std::string&,
                        std::set<std::string>&);

  void
  enumerate_characters(const NSBlocks& domain, unsigned int block,
                       int remaining, std::string& value,
                       std::set<std::string>& values) {
    if (remaining == 0) {
      enumerate_blocks(domain, block + 1, value, values);
      return;
    }
    for (NSIntSet::iterator character(domain[block].S); character();
         ++character) {
      value.push_back(static_cast<char>(*character));
      enumerate_characters(domain, block, remaining - 1, value, values);
      value.erase(value.size() - 1);
    }
  }

  void
  enumerate_blocks(const NSBlocks& domain, unsigned int block,
                   std::string& value, std::set<std::string>& values) {
    if (block == domain.size()) {
      values.insert(value);
      return;
    }
    for (int count = domain[block].l; count <= domain[block].u; ++count)
      enumerate_characters(domain, block, count, value, values);
  }

  int
  oracle_index(const std::string& value, const trimDFA& pattern) {
    for (std::string::size_type start = 0; start < value.size(); ++start)
      for (std::string::size_type length = 1;
           start + length <= value.size(); ++length)
        if (pattern.accepted(value.substr(start, length)))
          return static_cast<int>(start) + 1;
    return 0;
  }

  std::set<Solution>
  expected_solutions(const std::string& pattern, const NSBlocks& domain) {
    trimDFA automaton(RegExParser(pattern).parse()->dfa());
    std::set<std::string> values;
    std::string value;
    enumerate_blocks(domain, 0, value, values);
    std::set<Solution> expected;
    for (const std::string& word : values)
      expected.insert(Solution(word, oracle_index(word, automaton)));
    return expected;
  }

  std::set<Solution>
  actual_solutions(const std::string& pattern,
                   MatchImplementation implementation,
                   const NSBlocks& domain) {
    MatchModel* model = new MatchModel(
      pattern, implementation, domain, min_length(domain), max_length(domain));
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

  bool
  supports_incrementally(const std::string& pattern,
                         MatchImplementation implementation,
                         const NSBlocks& domain, const Solution& solution) {
    IncrementalModel initial(pattern, implementation, domain);
    if (initial.status() == SS_FAILED)
      return false;
    IncrementalModel* constrained =
      static_cast<IncrementalModel*>(initial.clone());
    constrained->constrain(solution.first, solution.second);
    const bool supported = constrained->status() != SS_FAILED;
    delete constrained;
    return supported;
  }

}

int
main(void) {
  PostingFailureModel dfa_failure(MATCH_DFA);
  PostingFailureModel nfa_failure(MATCH_NFA);
  PostingFailureModel auto_failure(MATCH_AUTO);
  assert(dfa_failure.status() == SS_FAILED);
  assert(nfa_failure.status() == SS_FAILED);
  assert(auto_failure.status() == SS_FAILED);

  InvalidPatternModel invalid_pattern;
  assert(invalid_pattern.status() == SS_FAILED);

  assert(match_default_implementation(
    RegExParser("a").parse()->dfa()) == MATCH_DFA);
  assert(match_default_implementation(
    RegExParser("(a|a)").parse()->dfa()) == MATCH_DFA);
  assert(match_default_implementation(
    RegExParser("(ab|ab)").parse()->dfa()) == MATCH_DFA);
  assert(match_default_implementation(
    RegExParser("(a|b)").parse()->dfa()) == MATCH_NFA);
  assert(match_default_implementation(
    RegExParser("a*b").parse()->dfa()) == MATCH_NFA);

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
    "a", "b", "ab", "aa", "(a|a)", "a|b", "(ab|b)", "a*b",
    "(a|b)*ab", "a(a|b)*b"
  };

  std::vector<NSBlocks> domains;
  domains.push_back(
    NSBlocks(1, NSBlock(NSIntSet('a', 'b'), 0, 4)));

  NSBlocks optional;
  optional.push_back(NSBlock(NSIntSet('a'), 0, 1));
  optional.push_back(NSBlock(NSIntSet('b'), 0, 1));
  optional.push_back(NSBlock(NSIntSet('a', 'b'), 1, 2));
  domains.push_back(optional);

  NSBlocks affixes;
  affixes.push_back(NSBlock(NSIntSet('a'), 1, 1));
  affixes.push_back(NSBlock(NSIntSet('a', 'b'), 0, 2));
  affixes.push_back(NSBlock(NSIntSet('b'), 1, 1));
  domains.push_back(affixes);

  for (const NSBlocks& domain : domains) {
    for (const std::string& pattern : patterns) {
      const std::set<Solution> expected = expected_solutions(pattern, domain);
      const std::set<Solution> dfa =
        actual_solutions(pattern, MATCH_DFA, domain);
      const std::set<Solution> nfa =
        actual_solutions(pattern, MATCH_NFA, domain);
      const std::set<Solution> automatic =
        actual_solutions(pattern, MATCH_AUTO, domain);
      if (dfa != expected || nfa != expected || automatic != expected) {
        std::cerr << "Mismatch for pattern " << pattern
                  << ": expected " << expected.size()
                  << ", DFA " << dfa.size()
                  << ", NFA " << nfa.size()
                  << ", automatic " << automatic.size() << std::endl;
        for (const Solution& solution : expected)
          if (dfa.find(solution) == dfa.end() ||
              nfa.find(solution) == nfa.end() ||
              automatic.find(solution) == automatic.end())
            std::cerr << "Missing: <" << solution.first << ">, "
                      << solution.second << std::endl;
        return 1;
      }
      for (const Solution& solution : expected) {
        if (!supports_incrementally(pattern, MATCH_DFA, domain, solution) ||
            !supports_incrementally(pattern, MATCH_NFA, domain, solution) ||
            !supports_incrementally(pattern, MATCH_AUTO, domain, solution)) {
          std::cerr << "Incremental propagation lost <" << solution.first
                    << ">, " << solution.second << " for pattern "
                    << pattern << std::endl;
          return 1;
        }
      }
    }
  }

  std::cout << "Match differential tests passed" << std::endl;
  return 0;
}
