#include <gecode/string.hh>

#include <cassert>
#include <string>

using namespace Gecode;

class IncreasingModel : public Space {
public:
  StringVar string;

  IncreasingModel(const std::string& value, bool strict)
    : string(*this, value) {
    rel(*this, string, strict ? STRT_INCLT : STRT_INCLQ);
  }

  IncreasingModel(IncreasingModel& other) : Space(other) {
    string.update(*this, other.string);
  }

  virtual Space* copy(void) {
    return new IncreasingModel(*this);
  }
};

static bool
accepted(const std::string& value, bool strict) {
  IncreasingModel model(value, strict);
  return model.status() != SS_FAILED;
}

int
main(void) {
  assert(accepted("", true));
  assert(accepted("", false));
  assert(accepted("a", true));
  assert(accepted("a", false));
  assert(accepted("ab", true));
  assert(accepted("ab", false));
  assert(!accepted("aa", true));
  assert(accepted("aa", false));
  assert(!accepted("ba", true));
  assert(!accepted("ba", false));
  return 0;
}
