#define DEBUG

#include <iostream>
#include <cassert>
#include <gecode/string.hh>
#include <gecode/string/eq.hh>
#include <gecode/string/nq.hh>
#include <gecode/string/re-eq.hh>
#include <gecode/string/concat.hh>
#include <gecode/int/bool.hh>
#include <gecode/string/branch.hh>
#include <gecode/string/gcc.hh>
#include <gecode/string/find.hh>

using namespace Gecode;
using namespace String;
using Gecode::String::DashedString;
using Gecode::String::Branch::None_LLLL;
using Gecode::Int::BoolView;

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
      std::cerr << "x[0]: " << x[0] << std::endl;
    }
    assert (x[0].pdomain()->val() == "da");
    assert (brancher.status(*this));
    while (!x[1].assigned()) {
         c = brancher.choice(*this);
         brancher.commit(*this, *c, 0);
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
    for (auto c : s)
      vx.push_back(NSBlock(c, 0, 1));
    vx.push_back(NSBlock('=', 1, 1));
    DashedString x(*this, vx, 0, 10000);
    NSBlocks vxx({NSBlock(NSIntSet(0, 1000), 390, 390)});
    DashedString xx(*this, vxx, 0, 10000);
    assert (x.equate(*this, xx));
//     std::cerr << "x = " << x << std::endl;
    NSBlocks vy("8");
    s = "%N:K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-'GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=";
    for (auto c : s)
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
      for (auto c : s)
         yy.push_back(NSBlock(c, 0, 1));
      yy.push_back(NSBlock('=', 1, 1));
      yy.push_back(NSBlock('8', 1, 1));
      s = "%N:K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-'GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=";
      for (auto c : s)
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
     for (auto c : s)
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
     for (auto c : s)
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
    ViewArray<Gecode::Int::IntView> N(*this, 3);
    N[0] = IntVar(*this, 3, 5);
    N[1] = IntVar(*this, 5, 6);
    N[2] = IntVar(*this, 1, 3);
    class gcc : public GCC {
      public:
        gcc(Home h,
            StringView x, const vec2& a, ViewArray<Gecode::Int::IntView>& c):
              GCC(h, x, a, c) {};
    };
    std::cerr << "D(x) :: " << x << '\n';
    std::cerr << "D(y) :: " << y << '\n';
    std::cerr << "A = " << A << '\n';
    std::cerr << "N = [" << N[0] << ", " << N[1] << ", " << N[2] << "]\n";
    int n = A.size();
    vec2 S(n);
    for (int i = 0; i < n; ++i)
      S[i] = std::make_pair(A[i], i);
    std::sort(S.begin(), S.end());
    assert(gcc(*this, x, S, N).propagate(*this, 0) == ES_FIX);
    assert(gcc(*this, y, S, N).propagate(*this, 0) == __ES_SUBSUMED);
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
    DashedString::_DEEP_CHECK = true;
    bool b = check_sweep<DSBlock, DSBlocks, DSBlock, DSBlocks>(
      x.blocks(), y.blocks()
    );
    assert (!b);
    std::cerr << "===== UNSATISFIABLE =====" << std::endl;
  }

};

int main() {
  (new StrTest())->test01();
  (new StrTest())->test02();
  (new StrTest())->test03();
  (new StrTest())->test04();
  (new StrTest())->test05();
  (new StrTest())->test06();
  (new StrTest())->test07();
  (new StrTest())->test08();
  (new StrTest())->test09();
  (new StrTest())->test10();
  (new StrTest())->test11();
  (new StrTest())->test12();
  (new StrTest())->test13();
  (new StrTest())->test14();
  (new StrTest())->test15();
  (new StrTest())->test16();
  (new StrTest())->test17();
  (new StrTest())->test18();
  (new StrTest())->test19();
  (new StrTest())->test20();
  (new StrTest())->test21();
  (new StrTest())->test22();
  return 0;
}
