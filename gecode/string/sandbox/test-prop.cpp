#define DEBUG

/* Testing dashed string propagation */

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
    std::cerr << "xx = w unsat!\n";
    assert(C(*this, vx, vy, vw).propagate(*this, 0) == ES_FAILED);
    std::cerr << "xy = w unsat!\n";
    assert(C(*this, vy, vx, vw).propagate(*this, 0) == ES_FAILED);
    std::cerr << "yx = w unsat!\n";
    StringView vt(*this);
    assert(C(*this, vt, vt, vw).propagate(*this, 0) == ES_OK);
    std::cerr << "vt = " << vt << '\n';
    assert (vt.min_length() == 0 && vt.max_length() == (int) w.size());
  }
  
  void test05() {
    cerr << "\n*** Test 05 ***" << endl;
    Block b[2];
    b[0].update(*this, Block(*this, CharSet(*this, 'a'), 0,2));
    b[1].update(*this, Block('d'));
    StringVar x(*this, DashedString(*this, b, 2));
    StringVar y(*this, DashedString(*this,Block(*this,CharSet(*this,'d'),0,3)));
    b[0].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 1,1));
    b[1].update(*this, Block(*this, CharSet(*this, 'd'), 1, 2));
    StringVar z(*this, DashedString(*this, b, 2));
    class C : public Concat<StringView,StringView,StringView> {
    public:
      C(Home h, StringView x, StringView y, StringView z) 
      : Concat(h, x, y, z) {};
    };
    StringView vx(x), vy(y), vz(z);
    std::cerr << "x = " << x << "\n";
    std::cerr << "y = " << y << "\n";
    std::cerr << "z = " << z << "\n";
    assert(C(*this, vx, vy, vz).propagate(*this, 0) == ES_OK);
    std::cerr << "After z = xy:\n";
    std::cerr << "x = " << x << "\n";
    std::cerr << "y = " << y << "\n";
    std::cerr << "z = " << z << "\n";
    assert (vx.val() == str2vec("ad") && vy.size() == 1 && vy[0].ub() == 1);
    assert (vz[0].val()[0] == 'a' && vz[1].baseEquals(vy[0]));
    assert(C(*this, vz, vy, vx).propagate(*this, 0) == __ES_SUBSUMED);
    std::cerr << "After x = zy:\n";
    std::cerr << "x = " << x << "\n";
    std::cerr << "y = " << y << "\n";
    std::cerr << "z = " << z << "\n";
    assert(C(*this, vy, vy, vz).propagate(*this, 0) == ES_FAILED);
    
  }


  void test06() {
    cerr << "\n*** Test 06 ***" << endl;
    string w1 = "A;CW7.MC3ER88MWZPRP.9H@A3?(2-UL3S3-3EG<;MQ@4TW6%*,FKYZ;J3XMF9?<F9>F%I)*HTAX3)7?'/0X*19<D1T)A><#V$V4UL7$@D W$,U5&GPFA(MH;.Z-N7/FBT7H0L5/;(#$S<LFZ6(SY5H6#YY/VD.=CUJG.5<7?O%W1N@PTHD3;A3.A4X%GN3Y(/&FFQ2#MK&/)WM&:>=23WNH;Q72P YHOAM++MROZPIOJ=))4MR7?&D;=N/&RR(6E7ZB,$?<<0GIE51P8%NV:J";
    string w2 = "K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-''GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8";
    int n = w1.size() + w2.size() + 1;
    Block bx[n];
    int i = 0;
    for (auto c : w1)
      bx[i++].update(*this, Block(c));
    for (auto c : w2)
      bx[i++].update(*this, Block(*this, CharSet(*this, c), 0, 1));
    bx[i++].update(*this, Block('='));
    assert (i == n && n == 392);
    StringVar x(*this, DashedString(*this, bx, n));
    StringVar xx(*this, Block(*this, CharSet(*this, 0, 1000), 390, 390));
    StringView vx(x), vxx(xx);
    assert (vx.equate(*this, vxx) == ME_STRING_NONE);
    assert (vxx.equate(*this, vx) == ME_STRING_BASE);
    w1 = "%N:K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-'GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=";
    w2 = "*9.63TS$1BY#<VP+%U.N#%,0@/JH$0X>A<VLG'O@M-II%;D-XXU5,,O+Q8&YWO%DX.SS/AADJG-$.7*FX+R7UEW(E(S5EY:;<M%> CKL#DBB";
    n = w1.size() + w2.size() + 1;
    Block by[n];
    by[0].update(*this, Block('8'));
    i = 1;
    for (auto c : w1)
      by[i++].update(*this, Block(*this, CharSet(*this, c), 0, 1));
    for (auto c : w2)
      by[i++].update(*this, Block(c));
    assert (i == n && n == 226);
    StringVar y(*this, DashedString(*this, by, n));
    StringVar yy(*this, Block(*this, CharSet(*this, 0, 1000), 110, 110));
    StringView vy(y), vyy(yy);
    assert (vy.equate(*this, vyy) == ME_STRING_NONE);
    assert (vyy.equate(*this, vy) == ME_STRING_BASE);
    ConcatView<StringView,StringView> xy(vx,vy);
    std::vector<int> vw = str2vec("A;CW7.MC3ER88MWZPRP.9H@A3?(2-UL3S3-3EG<;MQ@4TW6%*,FKYZ;J3XMF9?<F9>F%I)*HTAX3)7?'/0X*19<D1T)A><#V$V4UL7$@D W$,U5&GPFA(MH;.Z-N7/FBT7H0L5/;(#$S<LFZ6(SY5H6#YY/VD.=CUJG.5<7?O%W1N@PTHD3;A3.A4X%GN3Y(/&FFQ2#MK&/)WM&:>=23WNH;Q72P YHOAM++MROZPIOJ=))4MR7?&D;=N/&RR(6E7ZB,$?<<0GIE51P8%NV:JK+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-''GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=*9.63TS$1BY#<VP+%U.N#%,0@/JH$0X>A<VLG'O@M-II%;D-XXU5,,O+Q8&YWO%DX.SS/AADJG-$.7*FX+R7UEW(E(S5EY:;<M%> CKL#DBB");
    ConstStringView w(*this, &vw[0], vw.size());
    std::cerr << "xy = " << xy << "\n";
    std::cerr << "w = " << w << "\n";
    assert (!check_equate_x(xy,w) && check_equate_x(w,xy));
    class C : public Concat<StringView,StringView,ConstStringView> {
    public:
      C(Home h, StringView x, StringView y, ConstStringView z) 
      : Concat(h, x, y, z) {};
    };
    assert(C(*this, x, y, w).propagate(*this, 0) == ES_FAILED);
    std::cerr << "Unsat!\n";
  }
  
  void test07() {
    cerr << "\n*** Test 07 ***" << endl;
    string w1 = "A;CW7.MC3ER88MWZPRP.9H@A3?(2-UL3S3-3EG<;MQ@4TW6%*,FKYZ;J3XMF9?<F9>F%I)*HTAX3)7?'/0X*19<D1T)A><#V$V4UL7$@D W$,U5&GPFA(MH;.Z-N7/FBT7H0L5/;(#$S<LFZ6(SY5H6#YY/VD.=CUJG.5<7?O%W1N@PTHD3;A3.A4X%GN3Y(/&FFQ2#MK&/)WM&:>=23WNH;Q72P YHOAM++MROZPIOJ=))4MR7?&D;=N/&RR(6E7ZB,$?<<0GIE51P8%NV:J";
    string w2 = "K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-''GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8";
    string w3 = "%N:K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-'GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=";
    string w4 = "*9.63TS$1BY#<VP+%U.N#%,0@/JH$0X>A<VLG'O@M-II%;D-XXU5,,O+Q8&YWO%DX.SS/AADJG-$.7*FX+R7UEW(E(S5EY:;<M%> CKL#DBB";
    int n = w1.size() + w2.size() + w3.size() + w4.size() + 2;
    Block by[n];
    int i = 0;
    for (auto c : w1)
      by[i++].update(*this, Block(c));
    for (auto c : w2)
      by[i++].update(*this, Block(*this, CharSet(*this, c), 0, 1));
    by[i++].update(*this, Block('='));
    by[i++].update(*this, Block('8'));
    for (auto c : w3)
      by[i++].update(*this, Block(*this, CharSet(*this, c), 0, 1));
    for (auto c : w4)
      by[i++].update(*this, Block(c));
    StringVar y(*this, DashedString(*this, by, i));
    StringView vy(y);
    std::vector<int> w = str2vec("A;CW7.MC3ER88MWZPRP.9H@A3?(2-UL3S3-3EG<;MQ@4TW6%*,FKYZ;J3XMF9?<F9>F%I)*HTAX3)7?'/0X*19<D1T)A><#V$V4UL7$@D W$,U5&GPFA(MH;.Z-N7/FBT7H0L5/;(#$S<LFZ6(SY5H6#YY/VD.=CUJG.5<7?O%W1N@PTHD3;A3.A4X%GN3Y(/&FFQ2#MK&/)WM&:>=23WNH;Q72P YHOAM++MROZPIOJ=))4MR7?&D;=N/&RR(6E7ZB,$?<<0GIE51P8%NV:JK+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-''GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=*9.63TS$1BY#<VP+%U.N#%,0@/JH$0X>A<VLG'O@M-II%;D-XXU5,,O+Q8&YWO%DX.SS/AADJG-$.7*FX+R7UEW(E(S5EY:;<M%> CKL#DBB");
    ConstStringView vw(*this, &w[0], w.size());
    std::cerr << "y = " << vy << "\n";
    std::cerr << "w = " << vw << "\n";
    class E : public Eq<StringView,ConstStringView> {
    public:
      E(Home h, StringView x, ConstStringView y) : Eq(h, x, y) {};
    };
    assert(E(*this, vy, vw).propagate(*this, 0) == ES_FAILED);
    std::cerr << "Unsat!\n";
  }

  void test08() {
    cerr << "\n*** Test 08 ***" << endl;
    ViewArray<StringView> vx(*this, 2);
    Block d[3];
    d[0].update(*this, 'd');
    d[1].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 2));
    d[2].update(*this, Block(*this, CharSet(*this, 'c'), 0, 2));
    vx[0] = StringVar(*this, DashedString(*this, d, 3));
    vx[1] = StringVar(*this);
    using Gecode::String::Branch::None_LLLL;
//    using Gecode::String::Branch::Block_MinDim_LSLM;
    None_LLLL brancher(*this, vx);
    std::cerr << "x = " << vx[0] << std::endl;
    int i = 0;
    while (!vx[0].assigned()) {
      const Choice* c = brancher.choice(*this);
      brancher.commit(*this, *c, 0);
      std::cerr << "After commit " << ++i << ": " << vx[0] << std::endl;
    }
    
  }
//std::cerr << "\n*** Test 02 ***" << std::endl;
//    ViewArray<String::StringView> x(*this, 2);
//    NSBlocks v({
//      NSBlock(NSIntSet('d'), 1),
//      NSBlock(NSIntSet('a', 'b'), 1, 2),
//      NSBlock(NSIntSet('c'), 0, 2)
//    });
//    x[0] = StringVar(*this, v, 0, 100);
//    x[1] = StringVar(*this);
//    using Gecode::String::Branch::SizeMin_LLUL;
//    None_LLLL brancher(*this, x);
//    const Choice* c;
//    while (!x[0].assigned()) {
//      c = brancher.choice(*this);
//      std::cerr << "Here!\n";
//      brancher.commit(*this, *c, 0);
//      std::cerr << "x[0]: " << x[0] << std::endl;
//    }
//    assert (x[0].pdomain()->val() == "da");
//    assert (brancher.status(*this));
//    while (!x[1].assigned()) {
//         c = brancher.choice(*this);
//         brancher.commit(*this, *c, 0);
//         std::cerr << "x[1]: " << x[1] << std::endl;
//      }
//    assert (x[1].pdomain()->val() == "");

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
  cerr << "\n----- test-prop.cpp passes -----\n\n";
  return 0;
}



