#include <gecode/minimodel.hh>
#include <gecode/string.hh>

#include <cassert>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace Gecode;

class CharAtModel : public Space {
public:
  IntVar index;
  StringVar result;

  CharAtModel(const std::string& source, int lower, int upper,
              const std::string* expected = nullptr)
    : index(*this, lower, upper), result(*this) {
    StringVar input(*this, source);
    substr(*this, input, index, index, result);
    if (expected != nullptr)
      rel(*this, result, STRT_EQ, StringVar(*this, *expected));
    branch(*this, index, INT_VAL_MIN());
  }

  CharAtModel(const std::string& source, const IntSet& indices,
              const std::string& expected)
    : index(*this, indices), result(*this) {
    StringVar input(*this, source);
    substr(*this, input, index, index, result);
    rel(*this, result, STRT_EQ, StringVar(*this, expected));
    branch(*this, index, INT_VAL_MIN());
  }

  CharAtModel(CharAtModel& other) : Space(other) {
    index.update(*this, other.index);
    result.update(*this, other.result);
  }

  virtual Space* copy(void) {
    return new CharAtModel(*this);
  }
};

class VariableSourceCharAtModel : public Space {
public:
  StringVar source;
  IntVar index;
  StringVar result;

  VariableSourceCharAtModel(const std::vector<std::string>& sources,
                            int lower, int upper)
    : source(*this), index(*this, lower, upper), result(*this) {
    extensional(*this, source, sources);
    substr(*this, source, index, index, result);
    StringVarArgs strings;
    strings << source << result;
    blockmin_lllm(*this, strings);
    branch(*this, index, INT_VAL_MIN());
  }

  VariableSourceCharAtModel(VariableSourceCharAtModel& other)
    : Space(other) {
    source.update(*this, other.source);
    index.update(*this, other.index);
    result.update(*this, other.result);
  }

  virtual Space* copy(void) {
    return new VariableSourceCharAtModel(*this);
  }
};

class AliasedCharAtModel : public Space {
public:
  StringVar value;
  IntVar index;

  AliasedCharAtModel(const std::string& source, int lower, int upper)
    : value(*this, source), index(*this, lower, upper) {
    substr(*this, value, index, index, value);
    branch(*this, index, INT_VAL_MIN());
  }

  AliasedCharAtModel(AliasedCharAtModel& other) : Space(other) {
    value.update(*this, other.value);
    index.update(*this, other.index);
  }

  virtual Space* copy(void) {
    return new AliasedCharAtModel(*this);
  }
};

static std::string
expected_at(const std::string& source, int index) {
  return index < 1 || index > static_cast<int>(source.size())
    ? "" : std::string(1, source[index - 1]);
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

int
main(void) {
  std::vector<std::string> sources;
  generate_strings(sources, "", 3);

  for (const std::string& source : sources) {
    int lower = -2;
    int upper = static_cast<int>(source.size()) + 2;
    std::map<int, std::string> observed;
    CharAtModel* model = new CharAtModel(source, lower, upper);
    DFS<CharAtModel> search(model);
    delete model;
    while (CharAtModel* solution = search.next()) {
      assert(solution->index.assigned());
      assert(solution->result.assigned());
      observed[solution->index.val()] = solution->result.val();
      delete solution;
    }
    assert(observed.size() == static_cast<unsigned int>(upper - lower + 1));
    for (int index = lower; index <= upper; ++index)
      assert(observed[index] == expected_at(source, index));

    const std::string results[] = {"", "a", "b"};
    for (const std::string& expected : results) {
      CharAtModel* constrained =
        new CharAtModel(source, lower, upper, &expected);
      DFS<CharAtModel> constrained_search(constrained);
      delete constrained;
      while (CharAtModel* solution = constrained_search.next()) {
        assert(expected_at(source, solution->index.val()) == expected);
        delete solution;
      }
    }

    std::set<int> aliased_indices;
    AliasedCharAtModel* aliased =
      new AliasedCharAtModel(source, lower, upper);
    DFS<AliasedCharAtModel> aliased_search(aliased);
    delete aliased;
    while (AliasedCharAtModel* solution = aliased_search.next()) {
      aliased_indices.insert(solution->index.val());
      delete solution;
    }
    for (int index = lower; index <= upper; ++index) {
      bool expected = source == expected_at(source, index);
      assert((aliased_indices.count(index) != 0) == expected);
    }
  }

  int sparse_values[] = {-2, 1, 2, 3, 5};
  IntSet sparse_indices(sparse_values, 5);
  CharAtModel* sparse = new CharAtModel("aba", sparse_indices, "a");
  DFS<CharAtModel> sparse_search(sparse);
  delete sparse;
  std::set<int> sparse_solutions;
  while (CharAtModel* solution = sparse_search.next()) {
    sparse_solutions.insert(solution->index.val());
    delete solution;
  }
  assert(sparse_solutions == std::set<int>({1, 3}));

  int lower = -2;
  int upper = 5;
  VariableSourceCharAtModel* variable =
    new VariableSourceCharAtModel(sources, lower, upper);
  DFS<VariableSourceCharAtModel> variable_search(variable);
  delete variable;
  std::set<std::tuple<std::string, int, std::string> > observed;
  while (VariableSourceCharAtModel* solution = variable_search.next()) {
    observed.insert(std::make_tuple(solution->source.val(),
                                    solution->index.val(),
                                    solution->result.val()));
    delete solution;
  }
  std::set<std::tuple<std::string, int, std::string> > expected;
  for (const std::string& source : sources)
    for (int index = lower; index <= upper; ++index)
      expected.insert(std::make_tuple(source, index,
                                      expected_at(source, index)));
  assert(observed == expected);
  return 0;
}