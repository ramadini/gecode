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
    try {
      CharSet s(*this, IntSet({2, 8, -3, 90}));
    }
    catch (const OutOfLimits& e) {
      cerr << e.what() << '\n';
      try {
        CharSet s(*this, IntSet({2, MAX_ALPHABET_SIZE, 3, 90}));
      }
      catch (const OutOfLimits& e) {
        cerr << e.what() << '\n';
      }
      catch (...) {
        assert (0);
      }
    }
    catch (...) {
      assert (0);
    }
  }
  
  void test02() {
    cerr << "\n*** Test 02 ***" << endl;
    Block b0, b1('a'), b2(' ', 10), b3(*this),
          b4(*this, CharSet(*this, IntSet({'a', 'e', 'i', 'o', 'u', 'y'}))),
          b5(*this, CharSet(*this, 32, 60), 0, 0),
          b6(*this, CharSet(*this, 'G', 'H'), 2, 5);
    cerr << "log(||" << b0 << "||) = " << b0.logdim() << "\n";
    cerr << "log(||" << b1 << "||) = " << b1.logdim() << "\n";
    assert (b0.isNull() && b1.isFixed() && b1.ub() == 1);
    cerr << "log(||" << b2 << "||) = " << b2.logdim() << "\n";
    cerr << "log(||" << b3 << "||) = " << b3.logdim() << "\n";
    assert (b2.isFixed() && b3.isUniverse());
    for (int v : b2.val()) assert (v == ' ');
    cerr << "log(||" << b4 << "||) = " << b4.logdim() << "\n";
    cerr << "log(||" << b5 << "||) = " << b5.logdim() << "\n";
    cerr << "log(||" << b6 << "||) = " << b6.logdim() << "\n";
    assert (b5.baseEquals(b0) && b4.contains(b1));
    b0.update(*this, b2);
    assert (b0.lb() == 10 && b5.isNull() && b4.baseDisjoint(b6));
    // TODO: val, fix, lb, ub, baseIntersect, baseExclude, nullify,
    // updateCard
  }
  
  
  
};

int main() {
  StrTest* home = new StrTest();
  home->test01();
  home->test02();
  return 0;
}
