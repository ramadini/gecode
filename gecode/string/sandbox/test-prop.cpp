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
  
  string
  vec2str(const std::vector<int>& v) {
    int n = v.size();
    string w;
    for (int i = 0; i < n; ++i)
      w += v[i];
    return w;
  }
 
  void test01() {
    cerr << "\n*** Test 01 ***" << endl;
    StringVar x(*this);    
    string w = "Hello World!";
    int n = w.size();
    Block b[n];
    str2blocks(w, b, n, 0, 1);
    StringVar y(*this, DashedString(*this, b, n));
    std::cerr << "Propagating Eq: x = " << x << "  vs  y = " << y << "\n";
    class Eq0 : public Eq<StringView,StringView> {
    public:
      Eq0(Home h, StringView x, StringView y) : Eq(h, x, y) {};
    };
    assert(Eq0(*this, x, y).propagate(*this, 0) == ES_OK);
    assert(x.varimp()->dom().equals(y.varimp()->dom()));
    StringVar z(*this, Block(*this, CharSet(*this), 3, 12));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    std::cerr << "Propagating Eq: y = " << y << "  vs  z = " << z << "\n";
    assert(Eq0(*this, y, z).propagate(*this, 0) == ES_OK);
    std::cerr << "z = " << z << std::endl;
    StringView vz(z);
    assert(vz[0].lb() == 3 && vz[0].ub() == 12);
    StringVar t(*this, Block(*this, CharSet(*this), 12, 12));
    std::cerr << "Propagating Eq: t = " << t << "  vs  z = " << z << "\n";
    assert(Eq0(*this, t, z).propagate(*this, 0) == ES_OK);
    std::cerr << "Propagating Eq: x = " << x << "  vs  z = " << z << "\n";
    assert(Eq0(*this, x, z).propagate(*this, 0) == __ES_SUBSUMED);
    std::cerr << "x = " << x << "\n";
    assert (x.val() == str2vec(w) && w == vec2str(x.val()));
  }
};

int main() {
  StrTest* home = new StrTest();
  home->test01();
  cerr << "\n----- test-prop.cpp passes -----\n\n";
  return 0;
}



