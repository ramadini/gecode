#define DEBUG

/* Testing dashed string propagation */

#include <iostream>
#include <cassert>
#include <gecode/string.hh>

using namespace Gecode;
using namespace String;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

class StrTest : public Space {

public:
  StrTest() {};
  virtual Space* copy() {
    return new StrTest();
  }
  
  void
  str2blocks(const string& w, Block* bv, int n, int l = 1, int u = 1) {
    for (int i = 0; i < n; ++i)
      bv[i].update(*this, Block(*this, CharSet(*this, w[i]), l, u));
  }
 
  void test01() {
    cerr << "\n*** Test 01 ***" << endl;
    
  }
};

int main() {
  StrTest* home = new StrTest();
  home->test01();
  cerr << "\n----- test-prop.cpp passes -----\n\n";
  return 0;
}



