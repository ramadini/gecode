#include <cassert>
#include <gecode/string.hh>

using namespace Gecode;
using namespace Gecode::String;

class ReRegTest : public Space {
private:
  StringVar accepted;
  StringVar rejected;

public:
  ReRegTest(void)
    : accepted(*this, NSIntSet('a', 'b'), 1, 1),
      rejected(*this, NSIntSet('a', 'b'), 1, 1) {
    DFA accepts_a(0, {DFA::Transition(0, 'a', 1)}, {1}, false);
    BoolVar true_control(*this, 1, 1);
    BoolVar false_control(*this, 0, 0);
    extensional(*this, accepted, accepts_a, true_control, RM_EQV);
    extensional(*this, rejected, accepts_a, false_control, RM_EQV);
  }

  ReRegTest(ReRegTest& test) : Space(test) {
    accepted.update(*this, test.accepted);
    rejected.update(*this, test.rejected);
  }

  virtual Space* copy(void) {
    return new ReRegTest(*this);
  }

  void check(void) {
    assert(status() == SS_SOLVED);
    assert(accepted.val() == "a");
    assert(rejected.val() == "b");
  }
};

int main(void) {
  ReRegTest test;
  test.check();
  return 0;
}