#define DEBUG

#include <iostream>
#include <cassert>
#include <utility>
#include <gecode/string.hh>
#include <gecode/string/eq.hh>
#include <gecode/string/nq.hh>
#include <gecode/string/re-eq.hh>
#include <gecode/string/concat.hh>
#include <gecode/int/bool.hh>
#include <gecode/string/branch.hh>
#include <gecode/string/find.hh>
#include <gecode/string/match.hh>

using namespace Gecode;
using namespace String;
using Gecode::String::DashedString;
using Gecode::String::Branch::None_LLLL;
using Gecode::Int::BoolView;
using Gecode::Int::IntView;

class StrTest : public Space {

public:
  StrTest() {};
  virtual Space* copy() {
    return new StrTest();
  }
  
  NSBlocks
  ns_blocks(DashedString& x) {
  int n = x.length();
    NSBlocks v(n);
    for (int i = 0; i < n; ++i)
      v[i] = NSBlock(x.at(i));
    return v;
  }
  NSBlocks
  ns_blocks(const DashedString& x) {
  int n = x.length();
    NSBlocks v(n);
    for (int i = 0; i < n; ++i)
      v[i] = NSBlock(x.at(i));
    return v;
  }

  void test01() {
    std::cerr << "\n*** Test 01 ***" << std::endl;
    StringVar x(*this);
    assert (x.min_length() == 0);
    assert (x.max_length() == DashedString::_MAX_STR_LENGTH);
    assert (x.must_chars().size() == 0);
    assert (x.may_chars().size() == DashedString::_MAX_STR_ALPHA + 1);
    StringVar y(*this, "Hello World!");
    std::cerr << y.val() << std::endl;
    assert (y.assigned() && y.val() == "Hello World!" && y.domain().logdim() == 0);
    NSIntSet s('b');
    s.include('B');
    NSBlocks v({
      NSBlock(s, 1, 1),
      NSBlock(NSIntSet('o'), 2, 10),
      NSBlock(NSIntSet('m'), 1, 1),
      NSBlock(NSIntSet('!'), 3, 10),
    });
    StringVar z(*this, v, 0, 100);
    NSIntSet t;
    t.include('o');
    t.include('m');
    t.include('!');
    assert(z.must_chars() == t);
    //assert(z.size() == 2 * 9 * 8);
    StringView w(z);
    assert(*w.pdomain() == z.domain());
    class eq : public Eq {
    public:
      eq(Home h, StringView x, StringView y) :
        Eq(h, x, y) {};
    };
    assert(eq(*this, x, y).propagate(*this, 0) == __ES_SUBSUMED);
    eq(*this, x, y).propagate(*this, 0);
    std::cerr << x << std::endl;
    assert (x.val() == "Hello World!");

    class nq : public Nq {
    public:
      nq(Home h, StringView x, StringView y) :
        Nq(h, x, y) {};
    };
    assert(nq(*this, x, z).propagate(*this, 0) == __ES_SUBSUMED);
    std::cerr << z << std::endl;
    assert (!z.assigned());
    assert(nq(*this, x, y).propagate(*this, 0) == ES_FAILED);

    typedef Gecode::Int::BoolView BoolView;
    class eqv : public ReEq<BoolView, RM_EQV> {
    public:
      eqv(Home h, StringView x, StringView y, BoolView b) :
        ReEq<BoolView, RM_EQV>(h, x, y, b) {};
    };
    class imp : public ReEq<BoolView, RM_IMP> {
    public:
       imp(Home h, StringView x, StringView y, BoolView b) :
          ReEq<BoolView, RM_IMP>(h, x, y, b) {};
    };
    class pmi : public ReEq<BoolView, RM_PMI> {
    public:
       pmi(Home h, StringView x, StringView y, BoolView b) :
          ReEq<BoolView, RM_PMI>(h, x, y, b) {};
    };
    BoolVar b0(*this, 0, 1);
    assert(eqv(*this, x, y, b0).propagate(*this, 0) == __ES_SUBSUMED);
    assert(b0.val() == 1);
    BoolVar b1(*this, 0, 1);
    assert(imp(*this, y, z, b1).propagate(*this, 0) == __ES_SUBSUMED);
    assert(b1.val() == 0);
    BoolVar b2(*this, 0, 1);
    assert(pmi(*this, z, y, b2).propagate(*this, 0) == __ES_SUBSUMED);
    assert(pmi(*this, z, StringVar(*this), b2).propagate(*this, 0) == ES_FIX);
    assert(!b2.assigned());
  }

  void test02() {
    std::cerr << "\n*** Test 02 ***" << std::endl;
    ViewArray<String::StringView> x(*this, 2);
    NSBlocks v({
      NSBlock(NSIntSet('d'), 1),
      NSBlock(NSIntSet('a', 'b'), 1, 2),
      NSBlock(NSIntSet('c'), 0, 2)
    });
    x[0] = StringVar(*this, v, 0, 100);
    x[1] = StringVar(*this);
    using Gecode::String::Branch::SizeMin_LLUL;
    None_LLLL brancher(*this, x);
    const Choice* c;
    while (!x[0].assigned()) {
      c = brancher.choice(*this);
      std::cerr << "Here!\n";
      brancher.commit(*this, *c, 0);
      delete c;
      std::cerr << "x[0]: " << x[0] << std::endl;
    }
    assert (x[0].pdomain()->val() == "da");
    assert (brancher.status(*this));
    while (!x[1].assigned()) {
         c = brancher.choice(*this);
         brancher.commit(*this, *c, 0);
         delete c;
         std::cerr << "x[1]: " << x[1] << std::endl;
      }
    assert (x[1].pdomain()->val() == "");
  }

  void test03() {
    std::cerr << "\n*** Test 03 ***" << std::endl;
    StringVar x(*this);
    StringVar y(*this, "lo!");
    StringVar z(*this, "Hello!");
    class cat : public Concat {
    public:
       cat(Home h, StringView x, StringView y, StringView z) :
          Concat(h, x, y, z) {};
    };
    assert(cat(*this, x, y, z).propagate(*this, 0) == __ES_SUBSUMED);
    std::cerr<<"x = "<<x<<std::endl;
    std::cerr<<"y = "<<y<<std::endl;
    std::cerr<<"z = "<<z<<std::endl;
  }

  void test04() {
    std::cerr << "\n*** Test 04 ***" << std::endl;
    NSBlocks dx({
      NSBlock(NSIntSet('a'), 0, 2),
      NSBlock(NSIntSet('d', 'd'), 1, 1)
    });
    StringVar x(*this, dx, 0, 100);
    NSBlocks dy(1, NSBlock(NSIntSet('d'), 0, 3));
    StringVar y(*this, dy, 0, 100);
    NSBlocks dz({
       NSBlock(NSIntSet('a', 'c'), 1, 1),
       NSBlock(NSIntSet('d', 'd'), 1, 2)
    });
    StringVar z(*this, dz, 0, 100);
    class cat : public Concat {
    public:
       cat(Home h, StringView x, StringView y, StringView z) :
          Concat(h, x, y, z) {};
    };
    assert(cat(*this, x, y, z).propagate(*this, 0) == ES_FIX);
    std::cerr<<"x = "<<x<<std::endl;
    std::cerr<<"y = "<<y<<std::endl;
    std::cerr<<"z = "<<z<<std::endl;
  }

  void test05() {
    std::cerr << "\n*** Test 05 ***" << std::endl;
    StringVar x(*this, "Hello!");
    StringVar y(*this);
    StringVar z(*this, "lo!");
    class cat : public Concat {
    public:
       cat(Home h, StringView x, StringView y, StringView z) :
          Concat(h, x, y, z) {};
    };
    std::cerr<<"x = "<<x<<std::endl;
    std::cerr<<"y = "<<y<<std::endl;
    std::cerr<<"z = "<<z<<std::endl;
    assert(cat(*this, x, y, z).propagate(*this, 0) == ES_FAILED);
  }

  void test06() {
    std::cerr << "\n*** Test 06 ***" << std::endl;
    NSBlocks vx({
       NSBlock(NSIntSet('B'), 0, 1),
       NSBlock(NSIntSet('='), 0, 1),
       NSBlock(NSIntSet('C'), 0, 1),
       NSBlock(NSIntSet(' '), 0, 1),
       NSBlock(NSIntSet('='), 0, 1),
       NSBlock(NSIntSet(' '), 0, 1),
       NSBlock(NSIntSet('B'), 0, 1),
       NSBlock(NSIntSet('='), 0, 1),
       NSBlock(NSIntSet('C'), 1, 1),
    });
    NSBlocks vy({NSBlock(NSIntSet(0, 1000), 2, 7)});
    DashedString x(*this, vx, 2, 100);
    DashedString z(*this, vy, 2, 100);
    assert (x.equate(*this, z));
    vy.at(0).l = 1;
    DashedString y(*this, vy, 2, 10000);
    std::cerr << "x = " << x << ' ' << x.min_length() << std::endl;
    std::cerr << "y = " << y << ' ' << y.min_length() << std::endl;
    assert (x.equate(*this, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << ' ' << x.min_length() << std::endl;
    std::cerr << "y = " << y << ' ' << y.min_length() << std::endl;
    assert (x.min_length() == y.min_length() && y.min_length() == 2);
  }

  void test07() {
    std::cerr << "\n*** Test 07 ***" << std::endl;
    NSBlocks vx({
       NSBlock(NSIntSet('z'), 1, 1),
       NSBlock(NSIntSet('b'), 1, 1),
       NSBlock(NSIntSet(0, 1000), 0, 9)
    });
    NSIntSet s('a');
    s.add('c');
    s.add('z');
    NSBlocks vy({
       NSBlock(NSIntSet(0, 1000), 0, 10),
       NSBlock(s, 1, 1),
       NSBlock(NSIntSet('a', 'c'), 0, 10)
    });
    DashedString x(*this, vx, 0, 100);
    DashedString y(*this, vy, 0, 100);
    std::cerr << "x = " << x << ' ' << x.min_length() << std::endl;
    std::cerr << "y = " << y << ' ' << y.min_length() << std::endl;
    assert (x.equate(*this, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << ' ' << x.min_length() << std::endl;
    std::cerr << "y = " << y << ' ' << y.min_length() << std::endl;
    assert (ns_blocks(x) == vx && ns_blocks(y) == vy);
  }

  void test08() {
    std::cerr << "\n*** Test 08 ***" << std::endl;
    NSBlocks vx({
       NSBlock(NSIntSet('A'), 1, 1),
       NSBlock(NSIntSet('='), 0, 1),
       NSBlock(NSIntSet('B'), 0, 1),
       NSBlock(NSIntSet('='), 0, 1),
       NSBlock(NSIntSet('C'), 0, 1),
       NSBlock(NSIntSet(' '), 0, 1),
       NSBlock(NSIntSet('='), 0, 1),
       NSBlock(NSIntSet(' '), 0, 1),
       NSBlock(NSIntSet('B'), 0, 1),
    });
    DashedString x(*this, vx, 0, 10000);
    NSBlocks vxx({NSBlock(NSIntSet(0, 1000), 6, 6)});
    DashedString xx(*this, vxx, 0, 10000);
    assert (x.equate(*this, xx));
    DashedString y(*this, vx, 0, 10000);
    NSBlocks vyy({NSBlock(NSIntSet(0, 1000), 2, 9)});
    DashedString yy(*this, vyy, 0, 10000);
    assert (y.equate(*this, yy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (x.equate(*this, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (x.min_length() == y.min_length() &&
            x.max_length() == y.max_length());
  }

  void test09() {
    std::cerr << "\n*** Test 09 ***" << std::endl;
    NSBlocks vx("A;CW7.MC3ER88MWZPRP.9H@A3?(2-UL3S3-3EG<;MQ@4TW6%*,FKYZ;J3XMF9?<F9>F%I)*HTAX3)7?'/0X*19<D1T)A><#V$V4UL7$@D W$,U5&GPFA(MH;.Z-N7/FBT7H0L5/;(#$S<LFZ6(SY5H6#YY/VD.=CUJG.5<7?O%W1N@PTHD3;A3.A4X%GN3Y(/&FFQ2#MK&/)WM&:>=23WNH;Q72P YHOAM++MROZPIOJ=))4MR7?&D;=N/&RR(6E7ZB,$?<<0GIE51P8%NV:J");
    string s = "K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-''GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8";
    for (auto& c : s)
      vx.push_back(NSBlock(c, 0, 1));
    vx.push_back(NSBlock('=', 1, 1));
    DashedString x(*this, vx, 0, 10000);
    NSBlocks vxx({NSBlock(NSIntSet(0, 1000), 390, 390)});
    DashedString xx(*this, vxx, 0, 10000);
    assert (x.equate(*this, xx));
//     std::cerr << "x = " << x << std::endl;
    NSBlocks vy("8");
    s = "%N:K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-'GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=";
    for (auto& c : s)
      vy.push_back(NSBlock(c, 0, 1));
    vy.concat(NSBlocks("*9.63TS$1BY#<VP+%U.N#%,0@/JH$0X>A<VLG'O@M-II%;D-XXU5,,O+Q8&YWO%DX.SS/AADJG-$.7*FX+R7UEW(E(S5EY:;<M%> CKL#DBB"), vy);
    DashedString y(*this, vy, 0, 10000);
    NSBlocks vyy({NSBlock(NSIntSet(0, 1000), 110, 110)});
    DashedString yy(*this, vyy, 0, 10000);
    assert (y.equate(*this, yy));
//     std::cerr << "y = " << y << std::endl;
    ConcatView xy(x, y);
    DashedString z(*this, "A;CW7.MC3ER88MWZPRP.9H@A3?(2-UL3S3-3EG<;MQ@4TW6%*,FKYZ;J3XMF9?<F9>F%I)*HTAX3)7?'/0X*19<D1T)A><#V$V4UL7$@D W$,U5&GPFA(MH;.Z-N7/FBT7H0L5/;(#$S<LFZ6(SY5H6#YY/VD.=CUJG.5<7?O%W1N@PTHD3;A3.A4X%GN3Y(/&FFQ2#MK&/)WM&:>=23WNH;Q72P YHOAM++MROZPIOJ=))4MR7?&D;=N/&RR(6E7ZB,$?<<0GIE51P8%NV:JK+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-''GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=*9.63TS$1BY#<VP+%U.N#%,0@/JH$0X>A<VLG'O@M-II%;D-XXU5,,O+Q8&YWO%DX.SS/AADJG-$.7*FX+R7UEW(E(S5EY:;<M%> CKL#DBB");
//     std::cerr << "z = " << z << std::endl;
    assert (!sweep_concat(*this, xy, x, y, z));
  }

   void test10() {
      std::cerr << "\n*** Test 10 ***" << std::endl;
      NSBlocks yy("A;CW7.MC3ER88MWZPRP.9H@A3?(2-UL3S3-3EG<;MQ@4TW6%*,FKYZ;J3XMF9?<F9>F%I)*HTAX3)7?'/0X*19<D1T)A><#V$V4UL7$@D W$,U5&GPFA(MH;.Z-N7/FBT7H0L5/;(#$S<LFZ6(SY5H6#YY/VD.=CUJG.5<7?O%W1N@PTHD3;A3.A4X%GN3Y(/&FFQ2#MK&/)WM&:>=23WNH;Q72P YHOAM++MROZPIOJ=))4MR7?&D;=N/&RR(6E7ZB,$?<<0GIE51P8%NV:J");
      string s = "K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-''GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8";
      for (auto& c : s)
         yy.push_back(NSBlock(c, 0, 1));
      yy.push_back(NSBlock('=', 1, 1));
      yy.push_back(NSBlock('8', 1, 1));
      s = "%N:K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-'GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=";
      for (auto& c : s)
         yy.push_back(NSBlock(c, 0, 1));
      yy.concat(NSBlocks("*9.63TS$1BY#<VP+%U.N#%,0@/JH$0X>A<VLG'O@M-II%;D-XXU5,,O+Q8&YWO%DX.SS/AADJG-$.7*FX+R7UEW(E(S5EY:;<M%> CKL#DBB"), yy);
      DashedString x(*this, "A;CW7.MC3ER88MWZPRP.9H@A3?(2-UL3S3-3EG<;MQ@4TW6%*,FKYZ;J3XMF9?<F9>F%I)*HTAX3)7?'/0X*19<D1T)A><#V$V4UL7$@D W$,U5&GPFA(MH;.Z-N7/FBT7H0L5/;(#$S<LFZ6(SY5H6#YY/VD.=CUJG.5<7?O%W1N@PTHD3;A3.A4X%GN3Y(/&FFQ2#MK&/)WM&:>=23WNH;Q72P YHOAM++MROZPIOJ=))4MR7?&D;=N/&RR(6E7ZB,$?<<0GIE51P8%NV:JK+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-''GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=*9.63TS$1BY#<VP+%U.N#%,0@/JH$0X>A<VLG'O@M-II%;D-XXU5,,O+Q8&YWO%DX.SS/AADJG-$.7*FX+R7UEW(E(S5EY:;<M%> CKL#DBB");
      DashedString y(*this, yy, 0, 10000);
      std::cerr << "y = " << y << std::endl;
      assert (!sweep_equate(*this, x, y) && !sweep_equate(*this, x, y));
      std::cerr << "===== After Equate =====" << std::endl;
      std::cerr << "x = " << x << '\n';
      std::cerr << "y = " << y << " ("
                  << y.min_length() << ", " << y.max_length() << ")\n";
   }

   void test11() {
     std::cerr << "\n*** Test 11 ***" << std::endl;
     NSBlocks vx("+C<*@?OB+>9MW?,2U','/YBRO%ZAFAZ;+*");
     string s = "=UOL1%!'Z7*I ";
     for (auto& c : s)
       vx.push_back(NSBlock(c, 0, 1));
     NSBlocks vy({
       NSBlock(NSIntSet(0, 1000), 1, 49),
       NSBlock(NSIntSet(' '), 0, 48)
     });
     DashedString x(*this, vx, 0, 10000);
     DashedString y(*this, vy, 0, 10000);
     std::cerr << "x = " << x << '\n';
     std::cerr << "y = " << y << '\n';
     assert (sweep_equate(*this, x, y));
     std::cerr << "===== After Equate =====" << std::endl;
     std::cerr << "x = " << x << '\n';
     std::cerr << "y = " << y << '\n';
     assert (ns_blocks(x) == vx && y.max_length() == x.max_length() + 13);
   }

   void test12() {
     std::cerr << "\n*** Test 12 ***" << std::endl;
     NSBlocks vx("$");
     string s = "==2V=< =2V";
     for (auto& c : s)
       if (c != ' ')
         vx.push_back(NSBlock(c, 0, 1));
       else
         vx.push_back(NSBlock(' ', 0, 11));
     NSBlocks vy(vx);
     vy.push_back(NSBlock(' ', 0, 11));
     DashedString x(*this, vx, 0, 10000);
     DashedString y(*this, vy, 0, 10000);
     std::cerr << "x = " << x << '\n';
     std::cerr << "y = " << y << '\n';
     assert (sweep_equate(*this, x, y));
     std::cerr << "===== After Equate =====" << std::endl;
     std::cerr << "x = " << x << '\n';
     std::cerr << "y = " << y << '\n';
     assert (ns_blocks(x) == vx &&  ns_blocks(y) == vy);
   }

  void test13() {
    std::cerr << "\n*** Test 13 ***" << std::endl;
    DashedString x(*this, "ciao");
    DashedString y(*this, "cina");
    assert ( x.lex(*this, y, false) && y.lex(*this, y, false));
    assert (!x.lex(*this, x, true) && x.lex(*this, y, true));
    NSBlocks vx({
      NSBlock(NSIntSet('b', 'c'), 2, 2),
      NSBlock(NSIntSet('c', 'd'), 1, 1),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'b'), 2, 2),
      NSBlock(NSIntSet('a', 'c'), 1, 1),
    });
    x.update(*this, vx);
    y.update(*this, vy);
    NSBlocks vz({
      NSBlock(NSIntSet('a', 'b'), 2, 2),
      NSBlock(NSIntSet('a', 'c'), 1, 1),
     NSBlock(NSIntSet('a'), 3, 3),
     NSBlock(NSIntSet('f', 'z'), 1, 1)
    });
    DashedString z(*this, vz, 0, 10000);
    std::cerr << "x = " << x << '\n';
    std::cerr << "y = " << y << '\n';
    std::cerr << "z = " << z << '\n';
    assert (x.lex(*this, y, false));
    assert (!x.lex(*this, y, true) && !y.lex(*this, x, true));
    assert (x.val() == y.val() && x.val() == "bbc");
    assert (y.lex(*this, z, true) && y.lex(*this, z, false));
    vz[0].S.remove('a');
    vz[1].S = NSIntSet('c');
    assert (ns_blocks(z) == vz);
    std::cerr << "===== After x <= y && y <= z =====" << std::endl;
    std::cerr << "x = " << x << '\n';
    std::cerr << "y = " << y << '\n';
    std::cerr << "z = " << z << '\n';
    vx = NSBlocks({
      NSBlock(NSIntSet('Z', 'Z'), 1, 1),
      NSBlock(NSIntSet('E', 'E'), 1, 1),
      NSBlock(NSIntSet('A', 'Z'), 0, 9),
    });
    vy = NSBlocks({
      NSBlock(NSIntSet('Z', 'Z'), 1, 1),
      NSBlock(NSIntSet('A', 'Z'), 1, 1),
      NSBlock(NSIntSet('A', 'Z'), 0, 9),
    });
    DashedString x1(*this, vx, 0, 10000);
    DashedString y1(*this, vy, 0, 10000);
    std::cerr << "x1 = " << x1 << '\n';
    std::cerr << "y1 = " << y1 << '\n';
    assert (x1.lex(*this, y1, true) && y1.at(1).S.min() == 'E');
    std::cerr << "x1 = " << x1 << '\n';
    std::cerr << "y1 = " << y1 << '\n';
  }

  void test14() {
    std::cerr << "\n*** Test 14 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('d', 'e'), 0, 2),
      NSBlock(NSIntSet('c'), 1, 3),
      NSBlock(NSIntSet('a', 'd'), 3, 5),
      NSBlock(NSIntSet('h', 'n'), 0, 2),
      NSBlock(NSIntSet('a', 'b'), 0, 1),
      NSBlock(NSIntSet('g'), 0, 3),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'b'), 1, 2),
      NSBlock(NSIntSet('x', 'y'), 2, 3),
      NSBlock(NSIntSet('a', 'b'), 1, 2),
      NSBlock(NSIntSet('a', 'c'), 0, 10),
    });
    DashedString x(*this, vx, 0, 10000);
    DashedString y(*this, vy, 0, 10000);
    std::cerr << "x = " << x << '\n';
    std::cerr << "y = " << y << '\n';
    int fst = 1, lst = y.max_length();
    assert (y.find(*this, x, fst, lst, true) && fst == 5 && lst == 14);
    std::cerr << "===== After n = y.find(x), n > 0 =====" << std::endl;
    std::cerr << "x = " << x << '\n';
    std::cerr << "y = " << y << '\n';
    std::cerr << "n :: [" << fst << ", " << lst << "]\n";
  }

  void test15() {
    std::cerr << "\n*** Test 15 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('d', 'e'), 0, 2),
      NSBlock(NSIntSet('c'), 1, 3),
      NSBlock(NSIntSet('a', 'b'), 3, 5),
      NSBlock(NSIntSet('h', 'n'), 0, 2),
      NSBlock(NSIntSet('a', 'b'), 0, 1),
      NSBlock(NSIntSet('g'), 0, 3),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'b'), 1, 2),
      NSBlock(NSIntSet('x', 'y'), 2, 3),
      NSBlock(NSIntSet('a', 'b'), 1, 2),
      NSBlock(NSIntSet('b', 'd'), 0, 4),
    });
    StringVar x(*this, vx, 0, 10000);
    StringVar y(*this, vy, 0, 10000);
    IntVar n(*this, 1, y.max_length());
    std::cerr << "D(x) :: " << x << '\n';
    std::cerr << "D(y) :: " << y << '\n';
    std::cerr << "D(n) :: " << n << '\n';
    class find : public Find {
    public:
      find(Home h, StringView x, StringView y, Gecode::Int::IntView n) :
        Find(h, x, y, n) {};
    };
    assert(find(*this, x, y, n).propagate(*this, 0) == ES_FIX);
    assert (n.min() == 5 && n.max() == 8 && x.val() == "cbbb");
    std::cerr << "===== After n = y.find(x), n >= 0 =====" << std::endl;
    std::cerr << "D(x) :: " << x << '\n';
    std::cerr << "D(y) :: " << y << '\n';
    std::cerr << "D(n) :: " << n << '\n';
  }

  void test16() {
    std::cerr << "\n*** Test 16 ***" << std::endl;
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'z'), 0, 3),
      NSBlock(NSIntSet('b', 'b'), 2, 2),
      NSBlock(NSIntSet('a', 'a'), 1, 2),
      NSBlock(NSIntSet('c', 'c'), 5, 8),
      NSBlock(NSIntSet('a', 'z'), 1, 2),
    });
    StringVar x(*this, "accc");
    StringVar y(*this, vy, 0, 10000);
    IntVar n(*this, 10, 50);
    std::cerr << "D(x) :: " << x << '\n';
    std::cerr << "D(y) :: " << y << '\n';
    std::cerr << "D(n) :: " << n << '\n';
    class find : public Find {
    public:
      find(Home h, StringView x, StringView y, Gecode::Int::IntView n) :
        Find(h, x, y, n) {};
    };
    assert(find(*this, x, y, n).propagate(*this, 0) == ES_FAILED);
    std::cerr << "===== UNSATISFIABLE =====" << std::endl;
  }

  void test17() {
    std::cerr << "\n*** Test 17 ***" << std::endl;
    NSBlocks vy({
      NSBlock(NSIntSet('b', 'c'), 0, 12),
      NSBlock(NSIntSet('a', 'a'), 3, 4),
      NSBlock(NSIntSet('d', 'd'), 1, 2),
      NSBlock(NSIntSet('b', 'c'), 2, 4),
      NSBlock(NSIntSet('a', 'a'), 5, 5),
      NSBlock(NSIntSet('b', 'b'), 2, 3),
      NSBlock(NSIntSet('a', 'c'), 0, 8),
    });
    StringVar x(*this, "abb");
    StringVar y(*this, vy, 0, 10000);
    IntVar n(*this, 0, 100);
    std::cerr << "D(x) :: " << x << '\n';
    std::cerr << "D(y) :: " << y << '\n';
    std::cerr << "D(n) :: " << n << '\n';
    class find : public Find {
    public:
      find(Home h, StringView x, StringView y, Gecode::Int::IntView n) :
      Find(h, x, y, n) {};
    };
    assert(find(*this, x, y, n).propagate(*this, 0) == ES_FIX);
    std::cerr << "D(x) :: " << x << '\n';
    std::cerr << "D(y) :: " << y << '\n';
    std::cerr << "D(n) :: " << n << '\n';
    assert(n.min() == 7 && n.max() == 27);
  }

  void test18() {
    std::cerr << "\n*** Test 18 ***" << std::endl;
    NSBlocks v({
      NSBlock(NSIntSet('a'), 5, 5),
      NSBlock(NSIntSet('a', 'c'), 0, 10),
    NSBlock(NSIntSet('b'), 5, 5)
    });
    StringVar x(*this, v, 0, 10000);
    StringVar y(*this, "ababacbbcabaa");
    IntArgs A;
    A << 'b' << 'a' << 'c';
    IntVarArgs N;
    N << IntVar(*this, 3, 5)
      << IntVar(*this, 5, 6)
      << IntVar(*this, 1, 3);
    std::cerr << "D(x) :: " << x << '\n';
    std::cerr << "D(y) :: " << y << '\n';
    std::cerr << "A = " << A << '\n';
    std::cerr << "N = [" << N[0] << ", " << N[1] << ", " << N[2] << "]\n";
    gcc(*this, x, A, N);
    assert(status() != SS_FAILED);
    gcc(*this, y, A, N);
    assert(status() != SS_FAILED);
    assert(N[0].val() == 5 && N[1].val() == 6 && N[2].val() == 2);
    v[1].S.remove('b');
    v[1].l = 1;
    v[1].u = 4;
    std::cerr << "===== GCC(x/y, ['a', 'b', 'c'], N) =====" << std::endl;
    std::cerr << "D(x) :: " << x << '\n';
    std::cerr << "D(y) :: " << y << '\n';
    std::cerr << "A = " << A << '\n';
    std::cerr << "N = [" << N[0] << ", " << N[1] << ", " << N[2] << "]\n";
  }

  void test19() {
    std::cerr << "\n*** Test 19 ***" << std::endl;
    NSBlocks v({
      NSBlock(NSIntSet('c', 'z'), 0, 3),
      NSBlock(NSIntSet('b', 'm'), 3, 6),
      NSBlock(NSIntSet('a', 'd'), 0, 5),
      NSBlock(NSIntSet('a', 'h'), 0, 2),
    });
    DashedString x(*this, v, 0, 10000);
    std::cerr << "x = " << x << '\n';
    assert(x.increasing(*this, true));
    v.at(0).S = NSIntSet('c', 'l');
    v.at(2).u = 0;
    v.at(3).S = NSIntSet('e', 'h');
    v.normalize();
    assert(ns_blocks(x) == v);
    std::cerr << "===== strict_increasing(x) =====" << std::endl;
    std::cerr << "x = " << x << '\n';
  }

  void test20() {
    std::cerr << "\n*** Test 20 ***" << std::endl;
    NSBlocks v({
      NSBlock(NSIntSet('0', '3'), 4, 7)
    });
    StringVar x(*this, "123");
    StringVar y(*this, v, 0, 10000);
    IntVar n(*this, 2, 2);
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'a'), 1, 1),
      NSBlock(NSIntSet('b', 'b'), 2, 2)
    });
    NSBlocks vy({
      NSBlock(NSIntSet('b', 'c'), 0, 12),
      NSBlock(NSIntSet('a', 'a'), 3 , 3),
      NSBlock(NSIntSet('d', 'd'), 1, 2),
      NSBlock(NSIntSet('b', 'c'), 2, 4),
      NSBlock(NSIntSet('a', 'a'), 5, 5),
      NSBlock(NSIntSet('b', 'b'), 3, 3),
      NSBlock(NSIntSet('a', 'c'), 0, 8),
    });
    StringVar x1(*this, vx, 0, 10000);
    StringVar y1(*this, vy, 0, 10000);
    IntVar n1(*this, 0, 38);
    std::cerr << "D(x) :: " << x << '\n';
    std::cerr << "D(y) :: " << y << '\n';
    std::cerr << "D(n) :: " << n << '\n';
    class find : public Find {
    public:
      find(Home h, StringView x, StringView y, Gecode::Int::IntView n) :
        Find(h, x, y, n) {};
    };
    assert(find(*this, x, y, n).propagate(*this, 0) == ES_FIX);
    assert(find(*this, x1, y1, n1).propagate(*this, 0) == ES_FIX);
    assert(ns_blocks(y.domain()).slice(1, 4).known());
    std::cerr << "D(y) :: " << y << '\n';
    std::cerr << "n1 :: " << n1 << '\n';
  }
  
  void test21() {
    std::cerr << "\n*** Test 21 ***" << std::endl;
    NSIntSet s('B', 'C');
    s.add(' ');
    s.add('=');    
    NSBlocks vx({
      NSBlock(NSIntSet('A', 'A'), 1, 1),
      NSBlock(NSIntSet('=', '='), 1, 1),
      NSBlock(NSIntSet('B', 'B'), 1, 1),
      NSBlock(s, 0, 5),
      NSBlock(NSIntSet('C', 'C'), 1, 1),
      NSBlock(NSIntSet('=', '='), 1, 1),
      NSBlock(s, 0, 5),
      NSBlock(NSIntSet('C', 'C'), 1, 1),
    });
    DashedString x(*this, vx, 0, 100);
    DashedString y(*this, "A=B=C = B=C");
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (!sweep_equate(*this, x, y));
    std::cerr << "===== UNSATISFIABLE =====" << std::endl;
  }

  void test22() {
    std::cerr << "\n*** Test 22 ***" << std::endl;
    NSBlocks v("axb");
    v.extend(NSBlocks({NSBlock(NSIntSet::top(), 1, 1)}));
    v.extend(NSBlocks("xyz"));    
    NSBlocks w(NSBlocks({NSBlock(NSIntSet::top(), 1, 3)}));
    w.extend(NSBlocks("xy"));
    w.extend(NSBlocks({NSBlock(NSIntSet::top(), 0, 3)}));    
    DashedString x(*this, v, 0, 10);
    DashedString y(*this, w, 0, 10);
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (!sweep_equate(*this, x, y));
    // FIXME: x and y are not equatable.
    bool b = check_sweep<DSBlock, DSBlocks, DSBlock, DSBlocks>(
      x.blocks(), y.blocks()
    );
    assert (b);
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
  }

  void test23() {
    std::cerr << "\n*** Test 23 ***" << std::endl;
    NSBlocks v;
    v.push_back(NSBlock(NSIntSet('d'), 2, 3));
    v.push_back(NSBlock(NSIntSet('c'), 0, 5));
    v.push_back(NSBlock(NSIntSet('a', 'b'), 3, 5));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 10, 100);
    class match : public Match {
    public:
      match(Home h, StringView x, IntView i, trimDFA* R, trimDFA* R1, int r)
        : Match(h, x, i, R, R1, r) {};
    };
    string re = "(ab|c)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    assert((new (*this) match(*this, x, i, R, R1, 1))->propagate(*this, 0) == ES_FAILED);
    std::cerr << "===== i = match(x, R) UNSATISFIABLE =====\n" << std::endl;
  }
  
  void test24() {
    std::cerr << "\n*** Test 24 ***" << std::endl;
    NSBlocks v;
    NSIntSet s0('b'); s0.add('d');
    NSIntSet s1('a'); s1.add('c');
    v.push_back(NSBlock(s0, 2, 5));
    v.push_back(NSBlock(s1, 0, 3));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 0, 100);
    class match : public Match {
    public:
      match(Home h, StringView x, IntView i, trimDFA* R, trimDFA* R1, int r)
        : Match(h, x, i, R, R1, r) {};
    };
    string re = "(ab|c)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    double lx = x.domain().logdim();
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert((new (*this) match(*this, x, i, R, R1, 1))->propagate(*this, 0) == ES_FIX);
    std::cerr << "x = " << x << std::endl;
    assert(x.domain().logdim() == lx);
    std::cerr << "i = " << i << std::endl;
    for (int j = 0; j < 9; ++j)
      assert ( j == 0 || j > 2 ? i.in(j) : !i.in(j) );
  }
  
  void test25() {
    std::cerr << "\n*** Test 25 ***" << std::endl;
    NSBlocks v;
    NSIntSet s0('b'); s0.add('d');
    NSIntSet s1('a'); s1.add('c');
    v.push_back(NSBlock(s0, 2, 5));
    v.push_back(NSBlock(s1, 0, 3));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 1, 3);
    class match : public Match {
    public:
      match(Home h, StringView x, IntView i, trimDFA* R, trimDFA* R1, int r)
        : Match(h, x, i, R, R1, r) {};
    };
    string re = "(ab|c)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert((new (*this) match(*this, x, i, R, R1, 1))->propagate(*this, 0) == ES_FIX);
    std::cerr << "x = " << x << std::endl;
    assert(i.val() == 3 && x.domain().at(1).val() == "c");
  }
  
  void test26() {
    std::cerr << "\n*** Test 26 ***" << std::endl;
    NSBlocks v;
    v.push_back(NSBlock(NSIntSet('d'), 2, 3));
    v.push_back(NSBlock(NSIntSet('c'), 0, 5));
    v.push_back(NSBlock(NSIntSet('d', 'e'), 3, 8));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 10, 100);
    class match : public MatchNew {
    public:
      match(Home h, StringView x, IntView i, int r,
            trimDFA* R, trimDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "(ab|c)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(*R1, x.may_chars());
    assert((new (*this) match(*this, x, i, 1, R1, R, R2))->propagate(*this, 0) == ES_FAILED);
    std::cerr << "===== i = match(x, R) UNSATISFIABLE =====\n" << std::endl;
  }
  
  void test27() {
    std::cerr << "\n*** Test 27 ***" << std::endl; //FIXME: How match find it UNSATISFIABLE?
    NSBlocks v;
    v.push_back(NSBlock(NSIntSet('d'), 2, 3));
    v.push_back(NSBlock(NSIntSet('c'), 0, 5));
    v.push_back(NSBlock(NSIntSet('a', 'b'), 3, 5));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 10, 100);
    class match : public MatchNew {
    public:
      match(Home h, StringView x, IntView i, int r,
            trimDFA* R, trimDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "(ab|c)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(*R1, x.may_chars());
    assert((new (*this) match(*this, x, i, 1, R1, R, R2))->propagate(*this, 0) == ES_FAILED);
    std::cerr << "===== i = match(x, R) UNSATISFIABLE =====\n" << std::endl;
  }
  
  void test28() {
    std::cerr << "\n*** Test 28 ***" << std::endl;
    NSBlocks v;
    NSIntSet s0('b'); s0.add('d');
    NSIntSet s1('a'); s1.add('c');
    v.push_back(NSBlock(s0, 2, 5));
    v.push_back(NSBlock(s1, 0, 3));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 0, 100);
    class match : public MatchNew {
    public:
      match(Home h, StringView x, IntView i, int r,
            trimDFA* R, trimDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "(ab|c)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(*R1, x.may_chars());
    double lx = x.domain().logdim();
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert((new (*this) match(*this, x, i, 1, R1, R, R2))->propagate(*this, 0) == ES_FIX);
    std::cerr << "x = " << x << std::endl;
    assert(x.domain().logdim() == lx);
    std::cerr << "i = " << i << std::endl;
    for (int j = 0; j < 9; ++j)
      assert ( j == 0 || j > 2 ? i.in(j) : !i.in(j) );
  }
  
  void test29() {
    std::cerr << "\n*** Test 29 ***" << std::endl;
    NSBlocks v;
    NSIntSet s0('b'); s0.add('d');
    NSIntSet s1('a'); s1.add('c');
    v.push_back(NSBlock(s0, 2, 5));
    v.push_back(NSBlock(s1, 0, 3));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 1, 3);
    class match : public MatchNew {
    public:
      match(Home h, StringView x, IntView i, int r,
            trimDFA* R, trimDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "(ab|c)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(*R1, x.may_chars());
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert((new (*this) match(*this, x, i, 1, R1, R, R2))->propagate(*this, 0) == ES_FIX);
    std::cerr << "x = " << x << std::endl;
    assert(i.val() == 3 && x.domain().at(1).val() == "c");
  }
  
  void test30() {
    std::cerr << "\n*** Test 30 ***" << std::endl;
    NSBlocks v;
    v.push_back(NSBlock(NSIntSet('d'), 2, 3));
    v.push_back(NSBlock(NSIntSet('c'), 0, 5));
    v.push_back(NSBlock(NSIntSet('d', 'e'), 3, 8));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 10, 100);
    class match : public MatchNew {
    public:
      match(Home h, StringView x, IntView i, int r,
            trimDFA* R, trimDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "(ab|c)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(*R1, x.may_chars());
    assert((new (*this) match(*this, x, i, 1, R1, R, R2))->propagate(*this, 0) == ES_FAILED);
    std::cerr << "===== i = match(x, R) UNSATISFIABLE =====\n" << std::endl;
  }

  void test31() {
    std::cerr << "\n*** Test 31 ***" << std::endl;
    DSBlocks blocks(*this, NSBlocks("ac"));
    DSBlock middle(*this, NSBlock(NSIntSet('b'), 2, 3));
    blocks.insert(*this, 1, middle);

    middle.S.update(*this, 'z');
    middle.l = middle.u = 1;

    blocks.insert(*this, 0, DSBlock(*this, NSBlock(NSIntSet('x'), 1, 1)));
    blocks.insert(*this, blocks.length(),
                  DSBlock(*this, NSBlock(NSIntSet('y'), 1, 1)));

    assert(blocks.length() == 5);
    assert(blocks.at(0) == NSBlock(NSIntSet('x'), 1, 1));
    assert(blocks.at(1) == NSBlock(NSIntSet('a'), 1, 1));
    assert(blocks.at(2) == NSBlock(NSIntSet('b'), 2, 3));
    assert(blocks.at(3) == NSBlock(NSIntSet('c'), 1, 1));
    assert(blocks.at(4) == NSBlock(NSIntSet('y'), 1, 1));
  }

  void test32() {
    std::cerr << "\n*** Test 32 ***" << std::endl;
    NSIntSet values;
    values.add('a');
    values.add('c');
    values.add('e');
    DSIntSet set(*this, values);

    set.remove(*this, 'c');
    NSIntSet remaining;
    remaining.add('a');
    remaining.add('e');
    assert(set == remaining);
    assert(set.size() == 2);
    assert(set.min() == 'a' && set.max() == 'e');

    set.remove(*this, 'a');
    assert(set.size() == 1);
    assert(set.min() == 'e' && set.max() == 'e');

    set.remove(*this, 'e');
    assert(set.empty());
    assert(set.ranges() == NULL);
  }

  void test33() {
    std::cerr << "\n*** Test 33 ***" << std::endl;
    NSBlock null_with_chars(NSIntSet('x'), 2, 3);
    null_with_chars.u = 0;
    NSBlocks blocks({
      null_with_chars,
      NSBlock(NSIntSet('a'), 1, 2),
      null_with_chars,
      NSBlock(NSIntSet('a'), 3, 4),
      NSBlock(),
      NSBlock(NSIntSet('b'), DashedString::_MAX_STR_LENGTH - 2,
              DashedString::_MAX_STR_LENGTH - 1),
      NSBlock(NSIntSet('b'), 10, 20)
    });

    blocks.normalize();
    assert(blocks.length() == 2);
    assert(blocks.at(0) == NSBlock(NSIntSet('a'), 6, 6));
    assert(blocks.at(1) == NSBlock(NSIntSet('b'),
                                  DashedString::_MAX_STR_LENGTH,
                                  DashedString::_MAX_STR_LENGTH));

    NSBlocks empty({NSBlock(), null_with_chars});
    empty.normalize();
    assert(empty.length() == 1 && empty.at(0).null());
    assert(empty.at(0).S.empty() && empty.at(0).l == 0 && empty.at(0).u == 0);
  }

  void test34() {
    std::cerr << "\n*** Test 34 ***" << std::endl;
    NSBlocks left("ab");
    NSBlocks right("cd");
    left.concat(right, right);
    assert(right.val() == "abcd");

    NSBlocks same("ab");
    same.concat(same, same);
    assert(same.val() == "abab");

    NSBlocks extended("ab");
    extended.extend(extended);
    assert(extended.val() == "abab");

    NSBlocks prefixed("ab");
    prefixed.push_front(prefixed.back());
    assert(prefixed.val() == "bab");
  }

  void test35() {
    std::cerr << "\n*** Test 35 ***" << std::endl;
    const int max = DashedString::_MAX_STR_ALPHA;

    NSIntSet upper(max - 2, max);
    upper.shift(1);
    assert(upper.consistent());
    assert(upper.length() == 1 && upper.size() == 2);
    assert(upper.min() == max - 1 && upper.max() == max);

    NSIntSet lower(0, 2);
    lower.shift(-1);
    assert(lower.consistent());
    assert(lower.length() == 1 && lower.size() == 2);
    assert(lower.min() == 0 && lower.max() == 1);

    NSIntSet sparse(1);
    sparse.add(max - 1);
    sparse.shift(2);
    assert(sparse.consistent());
    assert(sparse.length() == 1 && sparse.size() == 1);
    assert(sparse.min() == 3 && sparse.max() == 3);

    NSIntSet removed(0, 1);
    removed.shift(-2);
    assert(removed.consistent() && removed.empty());

    NSIntSet letters('A', 'Z');
    letters.shift(32);
    assert(letters == NSIntSet('a', 'z'));
    letters.shift(-32);
    assert(letters == NSIntSet('A', 'Z'));
  }

  void test36() {
    std::cerr << "\n*** Test 36 ***" << std::endl;
    const int max = DashedString::_MAX_STR_ALPHA;

    NSIntSet edge(0);
    edge.include(NSIntSet(2, max));
    NSIntSet edge_comp = edge.comp();
    assert(edge_comp.consistent());
    assert(edge_comp == NSIntSet(1));

    NSIntSet middle(2, 3);
    NSIntSet middle_comp = middle.comp();
    assert(middle_comp.consistent());
    assert(middle_comp.length() == 2);
    assert(middle_comp.size() == max - 3 + 2);
    assert(middle_comp.first()->l == 0 && middle_comp.first()->u == 1);
    assert(middle_comp.last()->l == 4 && middle_comp.last()->u == max);

    NSIntSet all = NSIntSet::top();
    assert(all.comp().empty());
    NSIntSet empty;
    assert(empty.comp() == all);

    all.exclude(edge);
    assert(all.consistent());
    assert(all == NSIntSet(1));
  }

  void test37() {
    std::cerr << "\n*** Test 37 ***" << std::endl;
    std::string bytes;
    bytes += static_cast<char>(0x80);
    bytes += static_cast<char>(0x80);
    bytes += static_cast<char>(0xff);

    NSBlocks blocks(bytes);
    assert(blocks.length() == 2);
    assert(blocks.at(0) == NSBlock(NSIntSet(0x80), 2, 2));
    assert(blocks.at(1) == NSBlock(NSIntSet(0xff), 1, 1));
    assert(blocks.val() == bytes);

    const signed char first = static_cast<signed char>(0x80);
    const signed char last = static_cast<signed char>(0xff);
    assert(NSIntSet(first) == NSIntSet(0x80));
    assert(NSIntSet(first, last) == NSIntSet(0x80, 0xff));
  }

  void test38() {
    std::cerr << "\n*** Test 38 ***" << std::endl;
    const int universe = 6;
    for (int left = 0; left < (1 << universe); ++left) {
      for (int right = 0; right < (1 << universe); ++right) {
        NSIntSet values;
        NSIntSet removed;
        for (int value = 0; value < universe; ++value) {
          if (left & (1 << value))
            values.add(value);
          if (right & (1 << value))
            removed.add(value);
        }
        values.exclude(removed);
        assert(values.consistent());
        int expected_size = 0;
        for (int value = 0; value < universe; ++value) {
          expected_size += static_cast<bool>(
            (left & ~right) & (1 << value));
          assert(values.in(value) ==
                 static_cast<bool>((left & ~right) & (1 << value)));
        }
        assert(values.size() == expected_size);
      }
    }

    NSIntSet self(1, 3);
    self.exclude(self);
    assert(self.consistent() && self.empty());
  }

  void test39() {
    std::cerr << "\n*** Test 39 ***" << std::endl;
    StringVar x(*this, NSIntSet('a', 'b'), 1, 2);

    BoolVar eqv(*this, 0, 1);
    assert((ReEq<BoolView, RM_EQV>::post(*this, x, x, eqv) == ES_OK));
    assert(eqv.one());

    BoolVar imp(*this, 0, 1);
    assert((ReEq<BoolView, RM_IMP>::post(*this, x, x, imp) == ES_OK));
    assert(!imp.assigned());

    BoolVar pmi(*this, 0, 1);
    assert((ReEq<BoolView, RM_PMI>::post(*this, x, x, pmi) == ES_OK));
    assert(pmi.one());
  }

  void test40() {
    std::cerr << "\n*** Test 40 ***" << std::endl;

    StringVar b(*this, NSIntSet('a', 'z'), 1, 1);
    StringVar d(*this, NSIntSet('a', 'z'), 1, 1);
    StringVar f(*this, NSIntSet('a', 'z'), 1, 1);
    StringVarArgs high_arity;
    high_arity << StringVar(*this, "a") << b << StringVar(*this, "c")
               << d << StringVar(*this, "e") << f << StringVar(*this, "g");
    gconcat(*this, high_arity, StringVar(*this, "abcdefg"));

    StringVar repeated(*this, NSIntSet('x', 'y'), 1, 1);
    StringVarArgs repeated_args;
    repeated_args << repeated << StringVar(*this, "-") << repeated;
    gconcat(*this, repeated_args, StringVar(*this, "x-x"));

    StringVar self(*this, NSIntSet('a', 'b'), 0, 3);
    StringVarArgs self_args;
    self_args << StringVar(*this, "") << self << StringVar(*this, "");
    gconcat(*this, self_args, self);

    StringVar split(*this, NSIntSet('a', 'b'), 2, 2);
    StringVarArgs split_args;
    split_args << StringVar(*this, "<") << split << StringVar(*this, ">");
    gconcat(*this, split_args, StringVar(*this, "<ab>"));
    assert(status() != SS_FAILED);
    assert(split.assigned() && split.val() == "ab");

    assert(status() != SS_FAILED);
    assert(b.assigned() && b.val() == "b");
    assert(d.assigned() && d.val() == "d");
    assert(f.assigned() && f.val() == "f");
    assert(repeated.assigned() && repeated.val() == "x");
  }

  void test41() {
    std::cerr << "\n*** Test 41 ***" << std::endl;

    ViewArray<StringView> selected(*this, 1);
    selected[0] = StringVar(*this, NSIntSet('a', 'b'), 1, 1);
    None_LLLL selected_brancher(*this, selected);
    const Choice* selected_choice = selected_brancher.choice(*this);
    assert(!me_failed(selected[0].eq(*this, "a")));
    assert(selected_brancher.commit(*this, *selected_choice, 0) == ES_OK);
    assert(selected_brancher.commit(*this, *selected_choice, 1) == ES_FAILED);
    delete selected_choice;

    ViewArray<StringView> excluded(*this, 1);
    excluded[0] = StringVar(*this, NSIntSet('a', 'b'), 1, 1);
    None_LLLL excluded_brancher(*this, excluded);
    const Choice* excluded_choice = excluded_brancher.choice(*this);
    assert(!me_failed(excluded[0].eq(*this, "b")));
    assert(excluded_brancher.commit(*this, *excluded_choice, 0) == ES_FAILED);
    assert(excluded_brancher.commit(*this, *excluded_choice, 1) == ES_OK);
    delete excluded_choice;

    ViewArray<StringView> length(*this, 1);
    length[0] = StringVar(*this, NSIntSet('a'), 0, 2);
    None_LLLL length_brancher(*this, length);
    const Choice* length_choice = length_brancher.choice(*this);
    assert(!me_failed(length[0].eq(*this, "a")));
    assert(length_brancher.commit(*this, *length_choice, 0) == ES_FAILED);
    assert(length_brancher.commit(*this, *length_choice, 1) == ES_OK);
    delete length_choice;
  }

  void test42() {
    std::cerr << "\n*** Test 42 ***" << std::endl;
    const int universe = 6;
    const auto from_mask = [universe](int mask) {
      NSIntSet values;
      for (int value = 0; value < universe; ++value)
        if (mask & (1 << value))
          values.add(value);
      return values;
    };
    for (int left = 0; left < (1 << universe); ++left) {
      for (int right = 0; right < (1 << universe); ++right) {
        NSIntSet left_set = from_mask(left);
        NSIntSet right_set = from_mask(right);

        NSIntSet united(left_set);
        united.include(right_set);
        assert(united.consistent());

        NSIntSet intersected(left_set);
        intersected.intersect(right_set);
        assert(intersected.consistent());

        for (int value = 0; value < universe; ++value) {
          assert(united.in(value) ==
                 static_cast<bool>((left | right) & (1 << value)));
          assert(intersected.in(value) ==
                 static_cast<bool>((left & right) & (1 << value)));
        }
        assert(left_set.disjoint(right_set) == ((left & right) == 0));
        assert(left_set.contains(right_set) == ((right & ~left) == 0));
      }
    }
  }

  void test43() {
    std::cerr << "\n*** Test 43 ***" << std::endl;

    NSIntSet source(1);
    source.add(3);
    NSRange* ranges = source.first();
    NSIntSet moved(std::move(source));
    assert(source.empty());
    assert(moved.consistent() && moved.first() == ranges);

    NSIntSet assigned(5);
    assigned = std::move(moved);
    assert(moved.empty());
    assert(assigned.consistent() && assigned.first() == ranges);

    NSBlocks blocks;
    blocks.reserve(1);
    blocks.push_back(NSBlock(assigned, 1, 1));
    ranges = blocks.front().S.first();
    blocks.push_back(NSBlock(NSIntSet(2), 1, 1));
    assert(blocks.front().S.consistent());
    assert(blocks.front().S.first() == ranges);
  }

  void test44() {
    std::cerr << "\n*** Test 44 ***" << std::endl;

    DSIntSet empty;
    assert(empty.toIntSet().size() == 0);

    NSIntSet source(1, 2);
    source.add(4);
    source.add(6);
    source.add(7);
    DSIntSet stored(*this, source);
    IntSet converted = stored.toIntSet();
    assert(converted.ranges() == 3);
    assert(converted.size() == 5);
    assert(converted.min(0) == 1 && converted.max(0) == 2);
    assert(converted.min(1) == 4 && converted.max(1) == 4);
    assert(converted.min(2) == 6 && converted.max(2) == 7);
  }

  void test45() {
    std::cerr << "\n*** Test 45 ***" << std::endl;

    NSIntSet unknown('d');
    unknown.add('e');
    NSBlocks blocks({
      NSBlock(),
      NSBlock(NSIntSet(0x80), 2, 2),
      NSBlock(NSIntSet('b'), 1, 3),
      NSBlock(NSIntSet('c'), 1, 1),
      NSBlock(unknown, 1, 1),
      NSBlock(NSIntSet('f'), 2, 4),
      NSBlock(NSIntSet(0xff), 1, 1),
      NSBlock()
    });
    string expected_pref(2, static_cast<char>(0x80));
    expected_pref += 'b';
    string expected_suff("ff");
    expected_suff += static_cast<char>(0xff);

    assert(blocks.known_pref() == expected_pref);
    assert(blocks.known_suff() == expected_suff);

    blocks.normalize();
    DashedString dashed(*this, blocks, 0, DashedString::_MAX_STR_LENGTH);
    assert(dashed.known_pref() == expected_pref);
    assert(dashed.known_suff() == expected_suff);
  }

  void test46() {
    std::cerr << "\n*** Test 46 ***" << std::endl;

    StringVar x(*this, NSIntSet('a', 'b'), 0, 3);
    StringVarImp* variable = x.varimp();
    assert(variable->lb(*this, 1) == ME_STRING_LEN);
    assert(variable->lb(*this, 1) == ME_STRING_NONE);
    assert(variable->ub(*this, 2) == ME_STRING_LEN);
    assert(variable->ub(*this, 2) == ME_STRING_NONE);
  }

};

int main() {
  const auto run = [](void (StrTest::*test)(void)) {
    StrTest* space = new StrTest();
    (space->*test)();
    delete space;
  };
  run(&StrTest::test01);
  run(&StrTest::test02);
  run(&StrTest::test03);
  run(&StrTest::test04);
  run(&StrTest::test05);
  run(&StrTest::test06);
  run(&StrTest::test07);
  run(&StrTest::test08);
  run(&StrTest::test09);
  run(&StrTest::test10);
  run(&StrTest::test11);
  run(&StrTest::test12);
  run(&StrTest::test13);
  run(&StrTest::test14);
  run(&StrTest::test15);
  run(&StrTest::test16);
  run(&StrTest::test17);
  run(&StrTest::test18);
  run(&StrTest::test19);
  run(&StrTest::test20);
  run(&StrTest::test21);
  run(&StrTest::test22);
  run(&StrTest::test23);
  run(&StrTest::test24);
  run(&StrTest::test25);
  run(&StrTest::test26);
  run(&StrTest::test27);
  run(&StrTest::test28);
  run(&StrTest::test29);
  run(&StrTest::test30);
  run(&StrTest::test31);
  run(&StrTest::test32);
  run(&StrTest::test33);
  run(&StrTest::test34);
  run(&StrTest::test35);
  run(&StrTest::test36);
  run(&StrTest::test37);
  run(&StrTest::test38);
  run(&StrTest::test39);
  run(&StrTest::test40);
  run(&StrTest::test41);
  run(&StrTest::test42);
  run(&StrTest::test43);
  run(&StrTest::test44);
  run(&StrTest::test45);
  run(&StrTest::test46);
  return 0;
}
