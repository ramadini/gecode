#define DEBUG

/* Testing dashed string reverse equation and other string propagators */

#include <iostream>
#include <cassert>
#include <gecode/int.hh>
#include <gecode/string.hh>
#include <gecode/string/rel.hh>
#include <gecode/string/int.hh>
#include <gecode/string/rel-op.hh>
#include <gecode/string/branch.hh>

using namespace Gecode;
using namespace String;
using namespace Rel;
using namespace RelOp;
using namespace Branch;
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
  
  std::vector<int>
  rev(const std::vector<int>& v) {
    std::vector<int> w(v);
    std::reverse(w.begin(), w.end());
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
    assert(E(*this, x, y).propagate(*this, 0) == ES_FIX);
    assert(x.varimp()->dom().equals(y.varimp()->dom()));
    StringVar z(*this, Block(*this, CharSet(*this), 3, 12));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    std::cerr << "Propagating Eq: y = " << y << "  vs  z = " << z << "\n";
    assert(E(*this, y, z).propagate(*this, 0) == ES_FIX);
    std::cerr << "z = " << z << std::endl;
    StringView vz(z);
    assert(vz[0].lb() == 3 && vz[0].ub() == 12);
    StringVar t(*this, Block(*this, CharSet(*this), 12, 12));
    std::cerr << "Propagating Eq: t = " << t << "  vs  z = " << z << "\n";
    assert(E(*this, t, z).propagate(*this, 0) == ES_FIX);
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
    
    class E : public Eq<StringView,ConstStringView> {
    public:
      E(Home h, StringView x, ConstStringView w) : Eq(h, x, w) {};
    };
    vector<int> w = str2vec("hello");
    ConstStringView vw(*this, &w[0], w.size());
    assert(E(*this, x, vw).propagate(*this, 0) == __ES_SUBSUMED);
    assert (vec2str(x.val()) == "hello");
    std::cerr << "x = " << x << "\n";
    ConstStringView vx(*this, &x.val()[0], 5);
    StringVar y(*this, Block(*this, CharSet(*this,IntSet({'h','l','e'})), 2,6));
    StringView vy(y);
    assert (!check_equate_x(vx, vy));
    assert (!check_equate_x(vy, vx));
    assert (vy.equate(*this, vx) == ES_FAILED);
    w = str2vec(
      ":?@NbT;^AZR3IuW3ee:)DpBr%&C]=x=BqcG8[Pe.Uj` ]c4]?e]qCu|B,LSV!W(e: "
    );
    ConstStringView vz(*this, &w[0], w.size());
    Block c[2];
    c[0].update(*this, Block(*this, CharSet(*this), 3, 31));
    c[1].update(*this, Block(*this, CharSet(*this, 0, 1000), 2, 50));
    d.update(*this, DashedString(*this, c, 2));
    StringVar t(*this, d);
    std::cerr << "t = " << t << "\n";
    std::cerr << "z = " << vz << "\n";
    cerr << "Equate t = " << t << "  vs  z = " << vz << "\n";
    StringView vt(t);
    assert (vt.equate(*this, vz) == ME_STRING_CARD);
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
    string w =":?@NbT;^AZR3IuW3ee:)DpBr%&C]=x=BqcG8[Pe.Uj` ]c4]?e]qCu|B,LSV!W(e: ";
    StringView vx(x);
    Block bw[w.size()];
    str2blocks(w, bw, w.size());
    StringView vw(StringVar(*this,DashedString(*this, bw, w.size())));
    ConcatView vxw(vx, vw);
    vxw.fwd_iterator();
    vxw.bwd_iterator();
    StringVar y(*this);
    StringView vy(y);
    std::cerr << "x = " << x << "\n";
    std::cerr << "y = " << y << "\n";
    std::cerr << "w = " << vw << "\n";
    class C : public Concat<StringView,StringView,StringView> {
    public:
      C(Home h, StringView x, StringView y, StringView z) 
      : Concat(h, x, y, z) {};
    };
    assert(C(*this, vx, vx, vw).propagate(*this, 0) == ES_FAILED);
    std::cerr << "xx = w unsat!\n";
    assert(C(*this, vx, vy, vw).propagate(*this, 0) == ES_FAILED);
    std::cerr << "xy = w unsat!\n";
    assert(C(*this, vy, vx, vw).propagate(*this, 0) == ES_FAILED);
    std::cerr << "yx = w unsat!\n";
    StringView vt(*this);
    assert(C(*this, vt, vt, vw).propagate(*this, 0) == ES_FIX);
    std::cerr << "vt = " << vt << '\n';
    assert (vt.min_length() == 0 && vt.max_length() == (int) w.size());
  }
  void test05() {
    cerr << "\n*** Test 05 ***" << endl;
    IntSet s({'B', 'C', ' ', '='});
    Block bx[9];
    bx[0].update(*this, Block('A'));
    bx[1].update(*this, Block('='));
    bx[2].update(*this, Block('B'));
    bx[3].update(*this, Block(*this, CharSet(*this, s), 0, 5));
    bx[4].update(*this, Block('C'));
    bx[5].update(*this, Block('='));
    bx[6].update(*this, Block(*this, CharSet(*this, s), 0, 5));
    bx[4].update(*this, Block('C'));
    StringVar x(*this, DashedString(*this, bx, 9));
    StringView vx(x);
    string w = "A=B=C = B=C"; 
    Block by[w.size()];
    str2blocks(w, by, w.size());
    StringVar y(*this, DashedString(*this, by, w.size()));
    StringView vy(y);
    ReverseView ry(y);
    cerr << "Equate x = " << x << "  vs  y = " << ry << "\n";
    assert (vx.equate(*this, vy) == ME_STRING_FAILED);
    cerr << "Unsat!\n";
    cerr << "Equate y = " << ry << "  vs  x = " << x << "\n";
    assert (!check_equate_x(vx, ry));
    cerr << "Unsat!\n";
    assert(vx.isOK() && ry.isOK());
  }
  
  void test06() {
    cerr << "\n*** Test 06 ***" << endl;
    string wx = "axb_xyz";
    string wy = "...yx.._";
    int nx = wx.size(), ny = wy.size();
    Block bx[nx], by[ny];
    str2blocks(wx, bx, nx);
    str2blocks(wy, by, ny);
    for (int i = 0; i < nx; ++i)
      if (wx[i] == '_')
        bx[i].update(*this, Block(*this, CharSet(*this), 1, 1));
      else if (wx[i] == '.')
        bx[i].update(*this, Block(*this, CharSet(*this), 0, 1));
    StringVar x(*this, DashedString(*this, bx, nx));
    StringView vx(x);
    for (int i = 0; i < ny; ++i)
      if (wy[i] == '_')
        by[i].update(*this, Block(*this, CharSet(*this), 1, 1));
      else if (wy[i] == '.')
        by[i].update(*this, Block(*this, CharSet(*this), 0, 1));
    StringVar y(*this, DashedString(*this, by, ny));
    StringView vy(y);
    ReverseView ry(vy);
    cerr << "Equate x = " << x << "  vs " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_FAILED);
    cerr << "Unsat!\n";
    assert (!check_equate_x(vx, ry));
  }
  
  void test07() {
    cerr << "\n*** Test 07 ***" << endl;
    StringVar x(*this, Block(*this, CharSet(*this, 'a', 'b'), 0, 4));
    Block by[2];
    by[1].update(*this,  Block(*this, CharSet(*this, 'a', 'b'), 0, 2));
    by[0].update(*this,  Block('a', 3));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView ry(vy);
    cerr << "Equate x = " << x << "  vs " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_CARD);
    cerr << "After equate: x = " << x << "\n";
    assert (vx[0].baseEquals(vy[1]) && vx[0].ub() == 1 && vx[1].equals(vy[0]));
    assert(vx.isOK() && ry.isOK());
  }
  
  void test08() {
    std::cerr << "\n*** Test 08 ***" << std::endl;
    int n = 1;
    Block bx[3];
    bx[0].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, 30*n));
    bx[1].update(*this, Block('d', 5*n));
    bx[2].update(*this, Block(*this, CharSet(*this, 'c', 'f'), 0, 2*n));
    Block by[3];
    by[1].update(*this, Block(*this, CharSet(*this, 'b', 'd'), 26*n, 26*n));
    by[0].update(*this, Block('f', n));
    StringVar x(*this, DashedString(*this, bx, 3));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView ry(vy);
    cerr << "Equate x = " << x << "  vs " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_CARD);
    cerr << "After equate: x = " << x << "\n";
    assert (vx.size() == 4 && vx[3].val()[0] == 'f');
    assert(vx.isOK() && ry.isOK());
  }
  
  void test09() {
    std::cerr << "\n*** Test 09 ***" << std::endl;
    Block bx[2];
    bx[0].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, 300));
    bx[1].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 1));
    Block by[2];
    by[1].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, 300));
    by[0].update(*this, Block(*this, CharSet(*this, 'b', 'c'), 0, 200));
    StringVar x(*this, DashedString(*this, bx, 2));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView ry(vy);
    cerr << "Equate x = " << x << "  vs " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_NONE);
    cerr << "After equate: x = " << x << "\n";
    assert(vx.isOK() && ry.isOK());
  }
  
  void test10() {
    std::cerr << "\n*** Test 10 ***" << std::endl;
    Block bx[3];
    bx[0].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 1));
    bx[1].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 1));
    bx[2].update(*this, Block(*this, CharSet(*this, 'c', 'd'), 1, 1));
    Block by[2];
    by[1].update(*this, Block(*this, CharSet(*this, 'a', 'a'), 0, 2));
    by[0].update(*this, Block(*this, CharSet(*this, 'd', 'd'), 1, 1));
    StringVar x(*this, DashedString(*this, bx, 3));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView ry(vy);
    cerr << "Equate x = " << x << "  vs " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_VAL);
    cerr << "After equate: x = " << x << "\n";
    ReverseView rx(vx);
    cerr << "Equate y = " << y << "  vs  x = " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_VAL);
    cerr << "After equate: y = " << y << " \n";
    assert (x.val() == rev(y.val()) && y.val() == rev(x.val()));
    assert(vx.isOK() && vy.isOK());
  }
  
  void test11() {
    std::cerr << "\n*** Test 11 ***" << std::endl;
    Block bx[3];
    bx[2].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 1));
    bx[1].update(*this, Block(*this, CharSet(*this, 'e', 'f'), 1, 1));
    bx[0].update(*this, Block(*this, CharSet(*this, 'c', 'd'), 1, 1));
    Block by[2];
    by[0].update(*this, Block(*this, CharSet(*this, 'a', 'd'), 0, 2));
    by[1].update(*this, Block(*this, CharSet(*this, 'd', 'd'), 1, 1));
    StringVar x(*this, DashedString(*this, bx, 3));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    cerr << "Equate x = " << x << "  vs  y = " << y << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_FAILED);
    cerr << "Unsat!\n";
    cerr << "Equate y = " << y << "  vs  x = " << x << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_FAILED);
    cerr << "Unsat!\n";
    assert (!check_equate_x(vx, ry) && !check_equate_x(vy, rx));
    assert(rx.isOK() && ry.isOK());
  }
  
  void test12() {
    std::cerr << "\n*** Test 12 ***" << std::endl;
    Block bx[3];
    bx[2].update(*this, Block(*this, CharSet(*this, 'x', 'z'), 1, 1));
    bx[1].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 0, 2));
    bx[0].update(*this, Block('y'));
    Block by[4];
    by[0].update(*this, Block(*this, CharSet(*this, 'x', 'z'), 1, 1));
    by[1].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, 1));
    by[2].update(*this, Block(*this, CharSet(*this, 'b', 'c'), 1, 1));
    by[3].update(*this, Block(*this, CharSet(*this, 'x', 'z'), 1, 1));
    StringVar x(*this, DashedString(*this, bx, 3));
    StringVar y(*this, DashedString(*this, by, 4));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    cerr << "Equate x = " << x << "  vs " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_CARD);
    cerr << "After equate: x = " << x << "\n";
    assert (vx[2].ub() == 1 && vx[1].val()[0] == 'b' && vx[0].val()[0] == 'y');
    cerr << "Equate y = " << vy << "  vs " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_BASE);
    cerr << "After equate: x = " << x << "\n";
    assert (vx.varimp()->dom().contains_rev(vy.varimp()->dom()));
    assert (vy.varimp()->dom().contains_rev(vx.varimp()->dom()));
    assert(vx.isOK() && vy.isOK());
  }
  
  void test13() {
    std::cerr << "\n*** Test 13 ***" << std::endl;
    Block bx[3];
    bx[2].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 0, 2));
    bx[1].update(*this, Block(*this, CharSet(*this, 'a', 'd'), 1, 1));
    bx[0].update(*this, Block(*this, CharSet(*this, 'f', 'h'), 0, 2));
    Block by[2];
    by[1].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 1));
    by[0].update(*this, Block(*this, CharSet(*this, 'c', 'd'), 1, 1));
    StringVar x(*this, DashedString(*this, bx, 3));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
//    cerr << "Equate x = " << x << "  vs  y = " << ry << "\n";
//    assert (vx.equate(*this, vy) == ME_STRING_CARD);
//    cerr << "After equate: x = " << x << "  vs  y = " << y << "\n";
//    assert (vx.varimp()->dom().contains(vy.varimp()->dom()));
//    assert (vy.varimp()->dom().contains(vx.varimp()->dom()));
    cerr << "Equate y = " << y << "  vs " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_FAILED);
    cerr << "Unsat!\n";
    cerr << "Equate x = " << y << "  vs  " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_FAILED);
    cerr << "Unsat!\n";
    assert(rx.isOK() && ry.isOK());
  }
  
  void test14() {
    std::cerr << "\n*** Test 14 ***" << std::endl;
    Block bx[2];
    bx[0].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, 500));
    bx[1].update(*this, Block(*this, CharSet(*this, 'd', 'e'), 0, 300));
    Block by[2];
    by[1].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 0, 200));
    by[0].update(*this, Block(*this, CharSet(*this, 'c', 'd'), 0, 900));
    StringVar x(*this, DashedString(*this, bx, 2));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    cerr << "Equate x = " << x << "  vs " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_BASE);
    cerr << "After equate: x = " << x << "\n";
    assert(vx[1].baseMax() == vx[1].baseMin() && vx[1].baseMax() == 'd');
    cerr << "Equate y = " << y << "  vs  " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_CARD);
    cerr << "After equate: y = " << y << "\n";
    assert(ry[1].ub() == 800);
    assert(rx.isOK() && ry.isOK());
  }
  
  void test15() {
    std::cerr << "\n*** Test 15 ***" << std::endl;
    Block bx[4];
    bx[0].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, 400));
    bx[1].update(*this, Block(*this, CharSet(*this, 'b', 'c'), 3, 3));
    bx[2].update(*this, Block(*this, CharSet(*this, 'b', 'c'), 0, 97));
    bx[3].update(*this, Block(*this, CharSet(*this, 
                                              IntSet({'a','d','e',})), 0, 300));
    Block by[2];
    by[1].update(*this, Block(*this, CharSet(*this, 
                                              IntSet({'a','b','d',})), 0, 900));
    by[0].update(*this, Block(*this, CharSet(*this, 'c', 'd'), 0, 900));
    StringVar x(*this, DashedString(*this, bx, 4));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    cerr << "Equate x = " << x << "  vs " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_BASE);
    cerr << "After equate: x = " << x << "\n";     
    assert (vx[2].baseMin() == 'a' && vx[2].baseMax() == 'd');
    cerr << "Equate y = " << y << "  vs " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_CARD);
    cerr << "After equate: y = " << y << "\n";
    assert(ry[1].ub() == 800);
    assert(rx.isOK() && ry.isOK());
  }
  
  void test16() {
    std::cerr << "\n*** Test 16 ***" << std::endl;
    StringVar x(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, 400));
    StringVar y(*this, Block(*this, CharSet(*this, 'd'), 0, 900));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    cerr << "Equate x = " << x << "  vs  " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_VAL);
    cerr << "After equate: x = " << x << "\n";
    cerr << "Equate y = " << y << "  vs  x = " << x << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_VAL);
    cerr << "After equate: y = " << y << "\n";
    assert (x.val().empty() == y.val().empty());
  }
  
  void test17() {
    std::cerr << "\n*** Test 17 ***" << std::endl;
    int u = String::Limits::MAX_STRING_LENGTH;
    Block bx[2];
    bx[1].update(*this, Block(*this, CharSet(*this, 'b'), 0, u));
    bx[0].update(*this, Block(*this, CharSet(*this, 'c', 'd'), 0, u));
    Block by[2];
    by[0].update(*this, Block(*this, CharSet(*this, 'b', 'c'), 0, 2000));
    StringVar x(*this, DashedString(*this, bx, 2));
    StringVar y(*this, DashedString(*this, by, 1));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    cerr << "Equate x = " << x << "  vs  " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_CARD);
    cerr << "After equate: x = " << x << "\n";
    cerr << "Equate y = " << y << "  vs  " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_CARD);
    cerr << "After equate: y = " << y << "\n";
    assert (vx.size() == 2 && vy.size() == 2);
    assert (vy.varimp()->dom().equals_rev(vx.varimp()->dom()));
    assert(vx.isOK() && vy.isOK());
  }
  
  void test18() {
    std::cerr << "\n*** Test 18 ***" << std::endl;
    int M = String::Limits::MAX_STRING_LENGTH;
    StringVar x(*this, Block(*this, CharSet(*this, 'a', 'b'), 0, M));
    Block by[2];
    by[1].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, M));
    by[0].update(*this, Block('b'));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    cerr << "Equate x = " << x << "  vs  " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_CARD);
    cerr << "After equate: x = " << x << "\n";
    cerr << "Equate y = " << y << "  vs  " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_BASE);
    cerr << "After equate: y = " << y << "\n";
    assert (vx.varimp()->dom().contains_rev(vy.varimp()->dom()));
    assert (vy.varimp()->dom().contains_rev(vx.varimp()->dom()));
    assert(vx.isOK() && vy.isOK());
  }
  
  void test19() {
    std::cerr << "\n*** Test 19 ***" << std::endl;
    Block bx[3];
    bx[2].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 0, 6));
    bx[1].update(*this, Block('c'));
    bx[0].update(*this, Block('d'));
    Block by[2];
    by[0].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, 5));
    by[1].update(*this, Block(*this, CharSet(*this, 'd'), 0, 1));
    StringVar x(*this, DashedString(*this, bx, 3));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    cerr << "Equate x = " << x << "  vs  " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_CARD);
    cerr << "After equate: x = " << x << "\n";
    cerr << "Equate y = " << y << "  vs  " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_CARD);
    cerr << "After equate: y = " << y << "\n";
    assert(rx.isOK() && ry.isOK());
  }
  
  void test20() {
    std::cerr << "\n*** Test 20 ***" << std::endl;
    StringVar x(*this, 
                 DashedString(*this, Block(*this, CharSet(*this, 'a'), 0, 6)));
    Block by[3];
    by[2].update(*this, Block(*this, CharSet(*this, 'a'), 0, 5));
    by[1].update(*this, Block('a'));
    by[0].update(*this, Block(*this, CharSet(*this, 'a'), 0, 5));
    StringVar y(*this, DashedString(*this, by, 3));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    cerr << "Equate x = " << x << "  vs  " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_CARD);
    cerr << "After equate: x = " << x << "\n";
    cerr << "Equate y = " << y << "  vs  " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_CARD);
    cerr << "After equate: y = " << y << "\n";
    assert (vx.varimp()->dom().contains(vy.varimp()->dom()));
    assert (vy.varimp()->dom().contains(vx.varimp()->dom()));
    assert(vx.isOK() && vy.isOK());
  }
  
};

int main() {
  StrTest* home = new StrTest();
  home->test01();
  home->test02();
  home->test03();
  home->test04();
  home->test05();
  home->test06();
  home->test07();
  home->test08();
  home->test09();
  home->test10();
  home->test11();
  home->test12();
  home->test13();
  home->test14();
  home->test15();
  home->test16();
  home->test17();
  home->test18();
  home->test19();
  home->test20();
  delete home;
  cerr << "\n----- str_test3 passes -----\n\n";
  return 0;
}



