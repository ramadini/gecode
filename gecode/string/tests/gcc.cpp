#include <gecode/search.hh>
#include <gecode/string.hh>

#include <cassert>
#include <set>
#include <string>

using namespace Gecode;

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
  return 0;
}
