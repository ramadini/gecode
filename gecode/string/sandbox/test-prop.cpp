#define DEBUG

/* Testing dashed string propagation */

#include <iostream>
#include <cassert>
#include <gecode/string.hh>
#include <gecode/string/rel.hh>

using namespace Gecode;
using namespace String;
using namespace Rel;
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
  
  std::vector<int>
  str2vec(const string& w) {
    int n = w.size();
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i)
      v[i] = w[i];
    return v;
  }
 
  void test01() {
    cerr << "\n*** Test 01 ***" << endl;
    StringVar x(*this);//, Block(*this, CharSet(*this), 3, 10));
    string w = "Hello World!";
    int n = w.size();
    Block b[n];
    str2blocks(w, b, n, 0, 1);
    StringVar y(*this, DashedString(*this, b, n));
    std::cerr << "Propagating Eq: x = " << x << ", y = " << y << "\n";
    class Eq0 : public Eq<StringView,StringView> {
    public:
      Eq0(Home h, StringView x, StringView y) : Eq(h, x, y) {};
    };
    Eq0 eq(*this, x, y);
    assert(eq.propagate(*this, 0) == ES_OK);
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
  }
};

int main() {
  StrTest* home = new StrTest();
  home->test01();
  cerr << "\n----- test-prop.cpp passes -----\n\n";
  return 0;
}



