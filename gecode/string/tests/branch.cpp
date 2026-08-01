#include <gecode/search.hh>
#include <gecode/string.hh>

#include <cassert>

using namespace Gecode;

class DegreeZeroModel : public Space {
public:
  StringVarArray strings;

  DegreeZeroModel(int brancher, bool multi_block) {
    StringVar empty(*this, "");
    assert(empty.domain().min_str().empty());
    StringVarArgs variables;
    if (multi_block) {
      String::NSBlocks domain;
      domain.emplace_back(String::NSIntSet('a'), 0, 2);
      domain.emplace_back(String::NSIntSet('b'), 1, 3);
      variables << StringVar(*this, domain, 4, 5)
                << StringVar(*this, domain, 4, 5);
    } else {
      variables << StringVar(*this, String::NSIntSet('a', 'b'), 1, 1)
                << StringVar(*this, String::NSIntSet('a', 'b'), 1, 1);
    }
    strings = StringVarArray(*this, variables);
    assert(strings[1].domain().min_str() == (multi_block ? "aabb" : "a"));
    switch (brancher) {
    case 0:
      blockmin_llll(*this, strings);
      break;
    case 1:
      blockmin_lllm(*this, strings);
      break;
    default:
      lenblockmin_lllm(*this, strings);
      break;
    }
  }

  DegreeZeroModel(DegreeZeroModel& other) : Space(other) {
    strings.update(*this, other.strings);
  }

  virtual Space* copy(void) {
    return new DegreeZeroModel(*this);
  }
};

int
main(void) {
  for (int brancher = 0; brancher < 3; ++brancher) {
    for (int multi_block = 0; multi_block < 2; ++multi_block) {
      DegreeZeroModel* model = new DegreeZeroModel(brancher, multi_block != 0);
      DFS<DegreeZeroModel> search(model);
      delete model;
      DegreeZeroModel* solution = search.next();
      assert(solution != nullptr);
      for (const StringVar& variable : solution->strings) {
        assert(variable.assigned());
        const std::string value = variable.val();
        if (multi_block) {
          const std::string::size_type split = value.find('b');
          assert(value.size() >= 4 && value.size() <= 5);
          assert(split <= 2);
          assert(value.find_first_not_of('a') == split);
          assert(value.find_first_not_of('b', split) == std::string::npos);
          assert(value.size() - split >= 1 && value.size() - split <= 3);
        } else {
          assert(value == "a" || value == "b");
        }
      }
      delete solution;
    }
  }
  return 0;
}