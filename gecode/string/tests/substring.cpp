#include <gecode/minimodel.hh>
#include <gecode/string.hh>

#include <cassert>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace Gecode;

class SubstringModel : public Space {
public:
  IntVar from;
  IntVar to;
  StringVar result;

  SubstringModel(const std::string& source, int lower, int upper,
                 const std::string* expected = nullptr)
    : from(*this, lower, upper), to(*this, lower, upper), result(*this) {
    StringVar input(*this, source);
    substr(*this, input, from, to, result);
    if (expected != nullptr)
      rel(*this, result, STRT_EQ, StringVar(*this, *expected));
    branch(*this, from, INT_VAL_MIN());
    branch(*this, to, INT_VAL_MIN());
  }

  SubstringModel(const std::string& source, const IntSet& from_domain,
                 const IntSet& to_domain)
    : from(*this, from_domain), to(*this, to_domain), result(*this) {
    StringVar input(*this, source);
    substr(*this, input, from, to, result);
    branch(*this, from, INT_VAL_MIN());
    branch(*this, to, INT_VAL_MIN());
  }

  SubstringModel(SubstringModel& other) : Space(other) {
    from.update(*this, other.from);
    to.update(*this, other.to);
    result.update(*this, other.result);
  }

  virtual Space* copy(void) {
    return new SubstringModel(*this);
  }
};

class AliasedSubstringModel : public Space {
public:
  StringVar value;
  IntVar from;
  IntVar to;

  AliasedSubstringModel(const std::string& source, int lower, int upper)
    : value(*this, source), from(*this, lower, upper),
      to(*this, lower, upper) {
    substr(*this, value, from, to, value);
    branch(*this, from, INT_VAL_MIN());
    branch(*this, to, INT_VAL_MIN());
  }

  AliasedSubstringModel(AliasedSubstringModel& other) : Space(other) {
    value.update(*this, other.value);
    from.update(*this, other.from);
    to.update(*this, other.to);
  }

  virtual Space* copy(void) {
    return new AliasedSubstringModel(*this);
  }
};

static std::string
expected_substring(const std::string& source, int from, int to) {
  int start = std::max(1, from);
  int end = std::min(static_cast<int>(source.size()), to);
  return end < start ? "" : source.substr(start - 1, end - start + 1);
}

static void
generate_strings(std::vector<std::string>& strings, std::string prefix,
                 int remaining) {
  strings.push_back(prefix);
  if (remaining == 0)
    return;
  generate_strings(strings, prefix + 'a', remaining - 1);
  generate_strings(strings, prefix + 'b', remaining - 1);
}

typedef std::tuple<int, int, std::string> Solution;

static std::set<Solution>
solve(SubstringModel* model) {
  DFS<SubstringModel> search(model);
  delete model;
  std::set<Solution> solutions;
  while (SubstringModel* solution = search.next()) {
    assert(solution->from.assigned());
    assert(solution->to.assigned());
    assert(solution->result.assigned());
    solutions.insert(std::make_tuple(solution->from.val(),
                                     solution->to.val(),
                                     solution->result.val()));
    delete solution;
  }
  return solutions;
}

int
main(void) {
  std::vector<std::string> sources;
  generate_strings(sources, "", 3);

  for (const std::string& source : sources) {
    int lower = -2;
    int upper = static_cast<int>(source.size()) + 2;
    std::set<Solution> expected;
    std::set<std::string> results;
    for (int from = lower; from <= upper; ++from) {
      for (int to = lower; to <= upper; ++to) {
        std::string value = expected_substring(source, from, to);
        expected.insert(std::make_tuple(from, to, value));
        results.insert(value);
      }
    }
    assert(solve(new SubstringModel(source, lower, upper)) == expected);

    for (const std::string& result : results) {
      std::set<Solution> constrained_expected;
      for (const Solution& solution : expected)
        if (std::get<2>(solution) == result)
          constrained_expected.insert(solution);
      assert(solve(new SubstringModel
        (source, lower, upper, &result)) == constrained_expected);
    }

    std::set<std::pair<int, int> > aliased_expected;
    for (int from = lower; from <= upper; ++from)
      for (int to = lower; to <= upper; ++to)
        if (expected_substring(source, from, to) == source)
          aliased_expected.insert(std::make_pair(from, to));
    AliasedSubstringModel* aliased =
      new AliasedSubstringModel(source, lower, upper);
    DFS<AliasedSubstringModel> aliased_search(aliased);
    delete aliased;
    std::set<std::pair<int, int> > aliased_observed;
    while (AliasedSubstringModel* solution = aliased_search.next()) {
      aliased_observed.insert
        (std::make_pair(solution->from.val(), solution->to.val()));
      delete solution;
    }
    assert(aliased_observed == aliased_expected);
  }

  int from_values[] = {-2, 1, 3, 5};
  int to_values[] = {-1, 2, 4};
  IntSet from_domain(from_values, 4);
  IntSet to_domain(to_values, 3);
  std::set<Solution> sparse_expected;
  for (int from : from_values)
    for (int to : to_values)
      sparse_expected.insert(std::make_tuple
        (from, to, expected_substring("abc", from, to)));
  assert(solve(new SubstringModel
    ("abc", from_domain, to_domain)) == sparse_expected);
  return 0;
}