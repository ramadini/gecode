#define DEBUG

#include <iostream>
#include <cassert>
#include <gecode/string.hh>

using namespace Gecode;
using namespace String;
using std::cerr;
using std::endl;

class StrTest : public Space {

public:
  StrTest() {};
  virtual Space* copy() {
    return new StrTest();
  }
 
  void test01() {
    cerr << "\n*** Test 01 ***" << endl;
    cerr << "Max. alphabet size: " << MAX_ALPHABET_SIZE << '\n';
    cerr << "Max. string length: " << MAX_STRING_LENGTH << '\n';
    CharSet s0, s1(*this), s2(*this, 3, 10), s3(*this, 'a', 'e'), 
                s4(*this, 100, 200), s5(*this, IntSet({2, 8, 3, 90}));
    cerr << "|" << s0 << "| = " << s0.size() << "\n";
    cerr << "|" << s1 << "| = " << s1.size() << "\n";
    assert (s0.empty() && s1.size() == MAX_ALPHABET_SIZE);
    cerr << "|" << s2 << "| = " << s2.size() << "\n";
    cerr << "|" << s3 << "| = " << s3.size() << "\n";
    assert (s2.size() == 8 && s3.size() == 5);
    cerr << "|" << s4 << "| = " << s4.size() << "\n";
    cerr << "|" << s5 << "| = " << s5.size() << "\n";
    assert ((s4.size() - 1) / 25 == s5.size());
    assert (s1.isUniverse() && s1.contains(s4) && !s4.contains(s3));
    assert (s3.equals(CharSet(*this, 97, 101)) && s3.disjoint(s5));
  }
};

int main() {
  (new StrTest())->test01();
  return 0;
}
