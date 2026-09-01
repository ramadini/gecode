#include <gecode/search.hh>
#include <gecode/string.hh>

#include <cassert>
#include <set>
#include <string>

using namespace Gecode;

static String::NSIntSet
unsorted_domain(void) {
  String::NSIntSet domain(1, 3);
  domain.remove(2);
  return domain;
}

class CountCharacterCollisionModel : public Space {
public:
  StringVar value;

  CountCharacterCollisionModel(void)
    : value(*this, String::NSIntSet(1, 2), 3, 3) {
    IntArgs characters;
    characters << 1 << 2;
    IntVarArgs counts;
    counts << IntVar(*this, 2, 2) << IntVar(*this, 1, 1);
    gcc(*this, value, characters, counts);

    StringVarArgs variables;
    variables << value;
    sizemin_llul(*this, variables);
  }

  CountCharacterCollisionModel(CountCharacterCollisionModel& other)
    : Space(other) {
    value.update(*this, other.value);
  }

  virtual Space* copy(void) {
    return new CountCharacterCollisionModel(*this);
  }
};

class UnsortedCharactersModel : public Space {
public:
  StringVar value;

  UnsortedCharactersModel(void)
    : value(*this, unsorted_domain(), 1, 1) {
    IntArgs characters;
    characters << 2 << 1;
    IntVarArgs counts;
    counts << IntVar(*this, 0, 0) << IntVar(*this, 1, 1);
    gcc(*this, value, characters, counts);

    StringVarArgs variables;
    variables << value;
    sizemin_llul(*this, variables);
  }

  UnsortedCharactersModel(UnsortedCharactersModel& other)
    : Space(other) {
    value.update(*this, other.value);
  }

  virtual Space* copy(void) {
    return new UnsortedCharactersModel(*this);
  }
};

static std::string
symbols(unsigned char first, unsigned char second, unsigned char third) {
  std::string value;
  value.push_back(static_cast<char>(first));
  value.push_back(static_cast<char>(second));
  value.push_back(static_cast<char>(third));
  return value;
}

int
main(void) {
  std::set<std::string> expected;
  expected.insert(symbols(1, 1, 2));
  expected.insert(symbols(1, 2, 1));
  expected.insert(symbols(2, 1, 1));

  CountCharacterCollisionModel* model = new CountCharacterCollisionModel;
  DFS<CountCharacterCollisionModel> search(model);
  delete model;

  std::set<std::string> actual;
  while (CountCharacterCollisionModel* solution = search.next()) {
    assert(solution->value.assigned());
    actual.insert(solution->value.val());
    delete solution;
  }
  assert(actual == expected);

  UnsortedCharactersModel* unsorted_model = new UnsortedCharactersModel;
  DFS<UnsortedCharactersModel> unsorted_search(unsorted_model);
  delete unsorted_model;

  UnsortedCharactersModel* unsorted_solution = unsorted_search.next();
  assert(unsorted_solution != NULL);
  assert(unsorted_solution->value.assigned());
  assert(unsorted_solution->value.val() ==
    std::string(1, static_cast<char>(1)));
  delete unsorted_solution;
  assert(unsorted_search.next() == NULL);
  return 0;
}
