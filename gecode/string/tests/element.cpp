#include <gecode/minimodel.hh>
#include <gecode/string.hh>

#include <cassert>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace Gecode;

class ElementModel : public Space {
public:
  IntVar index;
  StringVar result;

  ElementModel(const std::vector<std::string>& values,
               int lower, int upper, const std::string* expected = nullptr)
    : index(*this, lower, upper), result(*this) {
    post(values, expected);
  }

  ElementModel(const std::vector<std::string>& values,
               const IntSet& indices, const std::string* expected = nullptr)
    : index(*this, indices), result(*this) {
    post(values, expected);
  }

  ElementModel(ElementModel& other) : Space(other) {
    index.update(*this, other.index);
    result.update(*this, other.result);
  }

  virtual Space* copy(void) {
    return new ElementModel(*this);
  }

private:
  void post(const std::vector<std::string>& values,
            const std::string* expected) {
    StringVarArgs candidates;
    for (const std::string& value : values)
      candidates << StringVar(*this, value);
    element(*this, candidates, index, result);
    if (expected != nullptr)
      rel(*this, result, STRT_EQ, StringVar(*this, *expected));
    branch(*this, index, INT_VAL_MIN());
  }
};

class HoleModel : public Space {
public:
  IntVar index;
  StringVar result;

  HoleModel(void) : index(*this, 1, 3), result(*this) {
    StringVarArgs candidates;
    candidates << StringVar(*this, "a")
               << StringVar(*this, "b")
               << StringVar(*this, "c");
    element(*this, candidates, index, result);
  }

  HoleModel(HoleModel& other) : Space(other) {
    index.update(*this, other.index);
    result.update(*this, other.result);
  }

  virtual Space* copy(void) {
    return new HoleModel(*this);
  }
};

class AliasedElementModel : public Space {
public:
  IntVar index;
  StringVar result;

  AliasedElementModel(void) : index(*this, 1, 2), result(*this, "a") {
    StringVarArgs candidates;
    candidates << result << StringVar(*this, "b");
    element(*this, candidates, index, result);
    branch(*this, index, INT_VAL_MIN());
  }

  AliasedElementModel(AliasedElementModel& other) : Space(other) {
    index.update(*this, other.index);
    result.update(*this, other.result);
  }

  virtual Space* copy(void) {
    return new AliasedElementModel(*this);
  }
};

class EmptyElementModel : public Space {
public:
  EmptyElementModel(void) {
    StringVarArgs candidates;
    IntVar index(*this, 0, 1);
    StringVar result(*this);
    element(*this, candidates, index, result);
  }

  EmptyElementModel(EmptyElementModel& other) : Space(other) {}

  virtual Space* copy(void) {
    return new EmptyElementModel(*this);
  }
};

typedef std::pair<int, std::string> Solution;

static std::set<Solution>
solve(ElementModel* model) {
  DFS<ElementModel> search(model);
  delete model;
  std::set<Solution> solutions;
  while (ElementModel* solution = search.next()) {
    assert(solution->index.assigned());
    assert(solution->result.assigned());
    solutions.insert(std::make_pair(solution->index.val(),
                                    solution->result.val()));
    delete solution;
  }
  return solutions;
}

int
main(void) {
  const std::vector<std::string> values = {"", "a", "bb", "a"};
  const std::set<Solution> expected = {
    Solution(1, ""), Solution(2, "a"),
    Solution(3, "bb"), Solution(4, "a")
  };
  assert(solve(new ElementModel(values, -2, 6)) == expected);

  int sparse_values[] = {1, 3, 4};
  IntSet sparse_indices(sparse_values, 3);
  const std::set<Solution> sparse_expected = {
    Solution(1, ""), Solution(3, "bb"), Solution(4, "a")
  };
  assert(solve(new ElementModel(values, sparse_indices)) == sparse_expected);

  const std::string constrained_value = "a";
  const std::set<Solution> constrained_expected = {
    Solution(2, "a"), Solution(4, "a")
  };
  assert(solve(new ElementModel(values, 1, 4, &constrained_value)) ==
         constrained_expected);

  HoleModel* hole = new HoleModel;
  assert(hole->status() != SS_FAILED);
  assert(hole->result.may_chars().contains(String::char2int('b')));
  rel(*hole, hole->index, IRT_NQ, 2);
  assert(hole->status() != SS_FAILED);
  assert(!hole->result.may_chars().contains(String::char2int('b')));
  delete hole;

  AliasedElementModel* aliased = new AliasedElementModel;
  DFS<AliasedElementModel> aliased_search(aliased);
  delete aliased;
  std::set<int> aliased_indices;
  while (AliasedElementModel* solution = aliased_search.next()) {
    aliased_indices.insert(solution->index.val());
    delete solution;
  }
  assert(aliased_indices == std::set<int>({1}));

  EmptyElementModel* empty = new EmptyElementModel;
  assert(empty->status() == SS_FAILED);
  delete empty;
  return 0;
}