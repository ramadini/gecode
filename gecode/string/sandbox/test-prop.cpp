#define DEBUG

/* Testing dashed string propagation */

#include <iostream>
#include <cassert>
#include <gecode/int.hh>
#include <gecode/string.hh>
#include <gecode/string/rel.hh>
#include <gecode/string/int.hh>
#include <gecode/string/rel-op.hh>

using namespace Gecode;
using namespace String;
using namespace Rel;
using namespace RelOp;
using namespace Gecode::Int;
using namespace String::Int;
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
    class E : public Eq<StringView,StringView> {
    public:
      E(Home h, StringView x, StringView y) : Eq(h, x, y) {};
    };
    assert(E(*this, x, y).propagate(*this, 0) == ES_OK);
    assert(x.varimp()->dom().equals(y.varimp()->dom()));
    StringVar z(*this, Block(*this, CharSet(*this), 3, 12));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    std::cerr << "Propagating Eq: y = " << y << "  vs  z = " << z << "\n";
    assert(E(*this, y, z).propagate(*this, 0) == ES_OK);
    std::cerr << "z = " << z << std::endl;
    StringView vz(z);
    assert(vz[0].lb() == 3 && vz[0].ub() == 12);
    StringVar t(*this, Block(*this, CharSet(*this), 12, 12));
    std::cerr << "Propagating Eq: t = " << t << "  vs  z = " << z << "\n";
    assert(E(*this, t, z).propagate(*this, 0) == ES_OK);
    std::cerr << "Propagating Eq: x = " << x << "  vs  z = " << z << "\n";
    assert(E(*this, x, z).propagate(*this, 0) == __ES_SUBSUMED);
    std::cerr << "x = " << x << "\n";
    assert (x.val() == str2vec(w) && w == vec2str(x.val()));
  }
  
  void test02() {
    cerr << "\n*** Test 02 ***" << endl;
    Block b[2];
    b[0].update(*this, Block(*this, CharSet(*this, 'f', 'g'), 0, 3));
    b[1].update(*this, Block(*this, CharSet(*this, 'o'), 1, 2));
    DashedString d(*this, DashedString(*this, b, 2));
    StringVar x(*this, d);
    StringView vx(x);
    class L : public Length<StringView> {
    public:
      L(Home h, StringView x, IntView n) : Length(h, x, n) {};
    };
    std::cerr << "x = " << x << "\n";
    assert(L(*this, x, IntVar(*this,3,8)).propagate(*this, 0) == ES_FIX);
    std::cerr << "After |x| in 3..8, x = " << x << "\n";
    assert (vx[0].lb() == 1);
    assert(L(*this, x, IntVar(*this,2,2)).propagate(*this, 0) == ES_FAILED);
    assert(L(*this, x, IntVar(*this,3,3)).propagate(*this, 0) == __ES_SUBSUMED);
    std::cerr << "After |x| = 3, x = " << x << "\n";
    assert (vx[0].ub() == 2);
    StringVar y(*this, DashedString(*this, Block('f')));
    class E : public Eq<StringView,StringView> {
    public:
      E(Home h, StringView x, StringView y) : Eq(h, x, y) {};
    };
    assert(E(*this, x, y).propagate(*this, 0) == ES_FAILED);
    b[0].update(*this, Block(*this, CharSet(*this, 'g'), 2, 5));
    StringVar z(*this, DashedString(*this, b, 2));
    assert(E(*this, x, z).propagate(*this, 0) == __ES_SUBSUMED);
    std::cerr << "After equate(x, {g}^(2,5) + {o}^(1,2)), x = " << x << "\n";
    std::vector<int> v = x.val();
    assert (v[0] == 'g' && v[1] == v[0] && v[2] == 'o' && v.size() == 3);
  }
  
  void test03() {
    cerr << "\n*** Test 03 ***" << endl;
    Block b[2];
    b[0].update(*this, Block(*this, CharSet(*this,IntSet({'h','l','e'})), 2,4));
    b[1].update(*this, Block(*this, CharSet(*this, 'o'), 1, 2));
    DashedString d(*this, DashedString(*this, b, 2));
    StringVar x(*this, d);
    std::cerr << "eq(" << x << ", hello)\n";
    eq(*this, x, str2vec("hello"));
    assert (vec2str(x.val()) == "hello");
    std::cerr << "x = " << x << "\n";
    ConstStringView vx(*this, &x.val()[0], 5);
    StringVar y(*this, Block(*this, CharSet(*this,IntSet({'h','l','e'})), 2,6));
    StringView vy(y);
    assert (!check_equate_x(vx, vy));
    assert (!check_equate_x(vy, vx));
    assert (equate_x(*this, vy, vx) == ES_FAILED);
    vector<int> w = str2vec(
      ":?@NbT;^AZR3IuW3ee:)DpBr%&C]=x=BqcG8[Pe.Uj` ]c4]?e]qCu|B,LSV!W(e: "
    );
    ConstStringView vz(*this, &w[0], w.size());
    b[0].update(*this, Block(*this, CharSet(*this), 3, 31));
    b[1].update(*this, Block(*this, CharSet(*this, 0, 1000), 2, 50));
    d.update(*this, DashedString(*this, b, 2));
    StringVar t(*this, d);
    std::cerr << "t = " << t << "\n";
    std::cerr << "z = " << vz << "\n";
    cerr << "Equate t = " << t << "  vs  z = " << vz << "\n";
    StringView vt(t);
    assert (equate_x(*this, vt, vz) == ME_STRING_CARD);
    std::cerr << "After equate, t = " << t << "\n";
    assert (vt.size() == 16+1+35   && vt[0].val()[0] == ':');
    assert (vt[15].val()[0] == '3' && !vt[16].isFixed());
    assert (vt[17].val()[0] == 'B' && vt[vt.size()-1].val()[0] == ' ');
  }
  
  void test04() {
    cerr << "\n*** Test 04 ***" << endl;
    Block b[2];
    b[0].update(*this, Block(*this, CharSet(*this,IntSet({'h','l','e'})), 2,4));
    b[1].update(*this, Block(*this, CharSet(*this, 'o'), 1, 2));
    StringVar x(*this, DashedString(*this, b, 2));
    vector<int> w = str2vec(
      ":?@NbT;^AZR3IuW3ee:)DpBr%&C]=x=BqcG8[Pe.Uj` ]c4]?e]qCu|B,LSV!W(e: "
    );
    StringView vx(x);
    ConstStringView vw(*this, &w[0], w.size());
    ConcatView<StringView,ConstStringView> vxw(vx, vw);
    vxw.fwd_iterator();
    vxw.bwd_iterator();
    StringVar y(*this);
    StringView vy(y);
    std::cerr << "x = " << x << "\n";
    std::cerr << "y = " << y << "\n";
    std::cerr << "w = " << vw << "\n";
    class C : public Concat<StringView,StringView,ConstStringView> {
    public:
      C(Home h, StringView x, StringView y, ConstStringView z) 
      : Concat(h, x, y, z) {};
    };
    assert(C(*this, vx, vx, vw).propagate(*this, 0) == ES_FAILED);
    assert(C(*this, vx, vy, vw).propagate(*this, 0) == ES_FAILED);
    assert(C(*this, vy, vx, vw).propagate(*this, 0) == ES_FAILED);
    // FIXME: Fix this.
    vy.update(*this, DashedString(*this));
    assert(C(*this, vy, vy, vw).propagate(*this, 0) == ES_OK);
  }
  
};

int main() {
  StrTest* home = new StrTest();
  home->test01();
  home->test02();
  home->test03();
  home->test04();
  cerr << "\n----- test-prop.cpp passes -----\n\n";
  return 0;
}



