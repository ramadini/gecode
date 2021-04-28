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
    Region r;
    {
      Gecode::Set::GLBndSet* ps = r.alloc<Gecode::Set::GLBndSet>(1);
      ps->init(*this);
      Gecode::Set::SetDelta d;
      int m = b1.baseMin();
      ps->include(*this, m, m, d);
      m = b2.baseMin();
      ps->include(*this, m, m, d);
      b4.baseIntersect(*this, *ps);
      b4.lb(*this, 5);
      b4.ub(*this, 5);
      assert (b4.val() == std::vector<int>({'a', 'a', 'a', 'a', 'a'}));      
      ps->include(*this, 'H', 'H', d);
      b6.baseExclude(*this, *ps);
      bool ok = false;
      try {
        b6.lb(*this, 1);
      }
      catch (const IllegalOperation& e) {
        cerr << e.what() << '\n';
        ok = true;
        b6.updateCard(*this, 3, 4);
      }
      assert (ok);
      b3.nullify(*this);
      cerr << b3 << ' ' << b4 << ' ' << b6 << endl;
      r.free();
    }
  }
  
  void test03() {
    cerr << "\n*** Test 03 ***" << endl;
    DashedString d0(*this), d1(*this, Block('r', 3));
    cerr << "log(||" << d0 << "||) = " << d0.logdim() << "\n";
    cerr << "log(||" << d1 << "||) = " << d1.logdim() << "\n";
    assert (d0.isUniverse() && d0[0].isUniverse() && d1.isFixed());
    assert (d1.val() == std::vector<int>({'r', 'r', 'r'}));
    int n = 8, i = 0;
    Block b[n];
    b[i++].update(*this, Block(*this, CharSet(*this, IntSet({'B','b'})), 1 ,1));
    b[i++].update(*this, Block(*this, CharSet(*this, IntSet({'o'})), 1, 3));
    b[i++].update(*this, Block(*this, CharSet(*this, IntSet({'o'})), 1, 1));
    b[i++].update(*this, Block());
    b[i++].update(*this, Block('m'));
    b[i++].update(*this, Block(*this, CharSet(*this, IntSet({'!'})), 0, 1));
    b[i++].update(*this, Block(*this, CharSet(*this, IntSet({'!'})), 0, 0));
    b[i++].update(*this, Block(*this, CharSet(*this, IntSet({'!'})), 0, 2));
    DashedString d2(*this, b, n);
    cerr << d2 << '\n';
    assert (d2.min_length() == 4 && d2.size() == 4 && d2.max_length() == 9);
    assert (d2.logdim() - log(24) < DBL_EPSILON);
    Block b0[2];
    b0[0].update(*this, Block(*this, CharSet(*this, 'r','t'), 1 ,3));
    b0[1].update(*this, Block(*this, CharSet(*this, IntSet({'a','d'})), 0 ,2));
    d0.update(*this, DashedString(*this, b0, 2));
    assert (d0.contains(d1));
    cerr << d0 << '\n';
    d1.nullify(*this);
    assert (d1.isNull() && d1.val() == std::vector<int>(0));
  }
  
  void test04() {
    cerr << "\n*** Test 04 ***" << endl;
    Block bv[5];
    bv[0].update(*this, Block(*this, CharSet(*this, 'a'), 2 ,3));
    bv[1].update(*this, Block(*this, CharSet(*this, 'c'), 1 ,2));
    bv[2].update(*this, Block('b'));
    bv[3].update(*this, Block(*this, CharSet(*this, 'c'), 0 ,2));
    bv[4].update(*this, Block(*this, CharSet(*this, 'a'), 3, 4));
    StringVar y(*this, DashedString(*this, bv, 5));
    StringView vy(y);
    StringView::SweepFwdIterator fwd_it0(vy, Position(0,1));
    Block b(*this, CharSet(*this, 'a', 'b'), 3 ,4);
    cerr << "Pushing forward " << b << " in " << y << " from " << *fwd_it0;
    Position p = push<StringView::SweepFwdIterator>(b, fwd_it0);
    cerr << "\n...ESP = " << p << ", EEP = " << *fwd_it0 << endl;
    assert (p == Position(2,0) && *fwd_it0 == Position(4,2));
    
    b.update(*this, Block(*this, CharSet(*this, 'c', 'z'), 3 ,4));
    StringView::SweepFwdIterator fwd_it1 = vy.fwd_iterator();
    cerr << "Pushing forward " << b << " in " << y << " from " << *fwd_it1;
    p = push<StringView::SweepFwdIterator>(b, fwd_it1);
    cerr << "\n...ESP = " << p << ", EEP = " << *fwd_it1 << endl;
    assert (p == *fwd_it1 && p == Position(y.varimp()->size(), 0));
    
    b.update(*this, Block(*this, CharSet(*this, 'c', 'd'), 1 ,2));
    StringView::SweepBwdIterator bwd_it0 = vy.bwd_iterator();
    cerr << "Pushing backward " << b << " in " << y << " from " << *bwd_it0;
    p = push<StringView::SweepBwdIterator>(b, bwd_it0);
    cerr << "\n...LSP = " << *bwd_it0 << ", LEP = " << p << endl;
    assert (*bwd_it0 == Position(3,1) && p == Position(4,0));
    
    b.update(*this, Block('b',2));
    StringView::SweepBwdIterator bwd_it1(vy, Position(4,4));
    cerr << "Pushing backward " << b << " in " << y << " from " << *bwd_it1;
    p = push<StringView::SweepBwdIterator>(b, bwd_it1);
    cerr << "\n...LSP = " << *bwd_it1 << ", LEP = " << p << endl;
    assert (p == *bwd_it1 && p == Position(0,0));
    
  }
  
   void test05() {
    cerr << "\n*** Test 05 ***" << endl;
    Block bv[5];
    bv[0].update(*this, Block(*this, CharSet(*this, 'a'), 2 ,3));
    bv[1].update(*this, Block(*this, CharSet(*this, 'c'), 1 ,2));
    bv[2].update(*this, Block('b'));
    bv[3].update(*this, Block(*this, CharSet(*this, 'c'), 0 ,2));
    bv[4].update(*this, Block(*this, CharSet(*this, 'a'), 3, 4));
    StringVar y(*this, DashedString(*this, bv, 5));
    StringView vy(y);
    StringView::SweepFwdIterator fwd_it0(vy, Position(0,1));
    Block b(*this, CharSet(*this, 'a', 'b'), 3 ,4);
    cerr << "Streching forward " << b << " in " << y << " from " << *fwd_it0;
    stretch<StringView::SweepFwdIterator>(b, fwd_it0);
    cerr << "\n...to " << *fwd_it0 << endl;
    assert (*fwd_it0 == Position(1,0));

    StringView::SweepFwdIterator fwd_it1(vy, Position(1,1));
    cerr << "Streching forward " << b << " in " << y << " from " << *fwd_it1;
    stretch<StringView::SweepFwdIterator>(b, fwd_it1);
    cerr << "\n...to " << *fwd_it1 << endl;
    assert (*fwd_it1 == Position(5,0));

    b.update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, 3));
    StringView::SweepBwdIterator bwd_it0(vy, Position(3, 1));
    cerr << "Stretching backward " << b << " in " << y << " from " << *bwd_it0;
    stretch<StringView::SweepBwdIterator>(b, bwd_it0);
    cerr << "\n...to " << *bwd_it0 << endl;
    assert (*bwd_it0 == Position(0,1));
        
    StringView::SweepBwdIterator bwd_it1 = vy.bwd_iterator();
    cerr << "Stretching backward " << b << " in " << y << " from " << *bwd_it1;
    stretch<StringView::SweepBwdIterator>(b, bwd_it1);
    cerr << "\n...to " << *bwd_it1 << endl;
    assert (*bwd_it1 == Position(3,0));
  }
  
  void test06() {
    cerr << "\n*** Test 06 ***" << endl;
    Block bv[5];
    bv[0].update(*this, Block(*this, CharSet(*this, IntSet({'B','b'})), 1 , 1));
    bv[1].update(*this, Block(*this, CharSet(*this, 'o'), 2 ,4));
    bv[2].update(*this, Block('m'));
    bv[3].update(*this, Block(*this, CharSet(*this, '!'), 0 ,3));
    StringVar y(*this, DashedString(*this, bv, 5));
    StringView vy(y);
    StringView::SweepFwdIterator fwd_it0(vy, Position(2,0));
    Block b(*this, CharSet(*this, 'm'), 1 ,2);
    cerr << "Pushing forward " << b << " in " << y << " from " << *fwd_it0;
    Position p = push<StringView::SweepFwdIterator>(b, fwd_it0);
    cerr << "\n...ESP = " << p << ", EEP = " << *fwd_it0 << endl;
    assert (p == Position(2,0) && *fwd_it0 == Position(3,0));
    
    StringView::SweepFwdIterator fwd_it1(vy, Position(2,1));
    cerr << "Pushing forward " << b << " in " << y << " from " << *fwd_it1;
    p = push<StringView::SweepFwdIterator>(b, fwd_it1);
    cerr << "\n...ESP = " << p << ", EEP = " << *fwd_it1 << endl;
    assert (p == *fwd_it1 && p == Position(4,0));
    
    b.update(*this, Block(*this, CharSet(*this, IntSet({'o','m','g'})), 1, 8));
    StringView::SweepFwdIterator fwd_it2(vy, Position(1,1));
    cerr << "Streching forward " << b << " in " << y << " from " << *fwd_it2;
    stretch<StringView::SweepFwdIterator>(b, fwd_it2);
    cerr << "\n...to " << *fwd_it2 << endl;
    assert (*fwd_it2 == Position(4,0));
    
    StringView::SweepBwdIterator bwd_it0(vy, Position(1,1));
    cerr << "Streching backward " << b << " in " << y << " from " << *bwd_it0;
    stretch<StringView::SweepBwdIterator>(b, bwd_it0);
    cerr << "\n...to " << *bwd_it0 << endl;
    assert (*bwd_it0 == Position(1,0));
    
    StringView::SweepBwdIterator bwd_it1(vy, *bwd_it0);
    cerr << "Pushing backward " << b << " in " << y << " from " << *bwd_it1;
    p = push<StringView::SweepBwdIterator>(b, bwd_it1);
    cerr << "\n...LSP = " << *bwd_it1 << ", LEP = " << p << endl;
    assert (*bwd_it1 == p && p == Position(0,0));
  }
  
  void test07() {
    cerr << "\n*** Test 07 ***" << endl;
    Block bv[4];
    bv[0].update(*this, Block(*this, CharSet(*this, IntSet({'B','b'})), 1, 1));
    bv[1].update(*this, Block(*this, CharSet(*this, 'o'), 2, 10));
    bv[2].update(*this, Block('m'));
    bv[3].update(*this, Block(*this, CharSet(*this, '!'), 3, 10));
    StringVar x(*this, DashedString(*this, bv, 4));
    StringVar y(*this, Block(*this, CharSet(*this,IntSet({'b','o','m'})),0,4));    
    StringView vx(x), vy(y);
    int n = vx.size();
    Matching m[n];
    cerr << "Init. x = " << x << "  vs  y = " << y << "\n";
    bool b = init_x<StringView,StringView>(*this, vx, vy, m);
    assert(b);    
    for (int i = 0; i < n; ++i)
      cerr << "ESP[ " << vx[i] << " ] = " << m[i].ESP << ", "
           << "LEP[ " << vx[i] << " ] = " << m[i].LEP << "\n";
    cerr << "Init. y = " << y << "  vs  x = " << x << "\n";
    b = init_x<StringView,StringView>(*this, vy, vx, m);
    assert(!b);
    cerr << "Failed!\n";
    bv[3].update(*this, Block(*this, CharSet(*this, '!'), 0, 3));
    StringVar z(*this, DashedString(*this, bv, 3));
    StringView vz(z);
    b = init_x<StringView,StringView>(*this, vy, vz, m);
    cerr << "Init. y = " << y << "  vs  z = " << z << "\n";
    assert(b);
    n = vy.size();
    for (int i = 0; i < n; ++i)
      cerr << "ESP[ " << vy[i] << " ] = " << m[i].ESP << ", "
           << "LEP[ " << vy[i] << " ] = " << m[i].LEP << "\n";
  }
  
  void test08() {
    cerr << "\n*** Test 08 ***" << endl;
    Block bv[4];
    bv[0].update(*this, Block(*this, CharSet(*this, IntSet({'B','b'})), 1, 1));
    bv[1].update(*this, Block(*this, CharSet(*this, 'o'), 2, 4));
    bv[2].update(*this, Block('m'));
    bv[3].update(*this, Block(*this, CharSet(*this, '!'), 0, 3));
    StringVar x(*this, DashedString(*this, bv, 4));
    StringVar y(*this, Block(*this, CharSet(*this,IntSet({'b','o','m'})),0,4));    
    StringView vx(x), vy(y);    
    cerr << "Sweep x = " << x << "  vs  y = " << y << "\n";
    int n = vx.size(), k = 0;
    Matching mx[n];
    assert(sweep_x(*this, vx, vy, mx, k));
    for (int i = 0; i < n; ++i)
      cerr << "Block " << vx[i] << ":\t"
           << "ESP: " << mx[i].ESP << ", "
           << "EEP: " << mx[i].EEP << ", "
           << "LSP: " << mx[i].LSP << ", "
           << "LEP: " << mx[i].LEP << "\n";
    assert (mx[0].LEP == Position(0,1) && mx[1].EEP == Position(0,3));
    assert (mx[2].ESP == Position(0,3) && mx[3].LSP == Position(1,0));
    cerr << "Sweep y = " << y << "  vs  x = " << x << "\n";    
    n = vy.size();
    Matching my[n];
    assert(sweep_x(*this, vy, vx, my, k));
    for (int i = 0; i < n; ++i)
      cerr << "Block " << vy[i] << ":\t"
           << "ESP: " << my[i].ESP << ", "
           << "EEP: " << my[i].EEP << ", "
           << "LSP: " << my[i].LSP << ", "
           << "LEP: " << my[i].LEP << "\n";
    assert (my[0].LSP == Position(0,0) && my[0].EEP == Position(4,0));
    
  }
  
  void test09() {
    cerr << "\n*** Test 09 ***" << endl;
    Block bv[4];
    bv[0].update(*this, Block(*this, CharSet(*this, IntSet({'B','b'})), 1, 1));
    bv[1].update(*this, Block(*this, CharSet(*this, 'o'), 2, 4));
    bv[2].update(*this, Block('m'));
    bv[3].update(*this, Block(*this, CharSet(*this, '!'), 0, 3));
    StringVar x(*this, DashedString(*this, bv, 4));
    StringVar y(*this, Block(*this, CharSet(*this,IntSet({'b','o','m'})),0,4));    
    StringView vx(x), vy(y);    
    cerr << "Equate x = " << x << "  vs  y = " << y << "\n";
    assert(equate_x(*this, vx, vy) == ME_STRING_VAL);
    cerr << "After equate: x = " << x << "  vs  y = " << y << "\n";
    assert(x.val() == std::vector<int>({'b', 'o', 'o', 'm'}));
    cerr << "Equate y = " << y << "  vs  x = " << x << "\n";
    assert(equate_x(*this, vy, vx) == ME_STRING_VAL);
    cerr << "After equate: y = " << y << "  vs  x = " << x << "\n";
    assert(x.val() == y.val());
    
  }
  
  void test10() {
    cerr << "\n*** Test 10 ***" << endl;
    Block bv[9];
    bv[0].update(*this, Block(*this, CharSet(*this, 'B'), 0, 1));
    bv[1].update(*this, Block(*this, CharSet(*this, '='), 0, 1));
    bv[2].update(*this, Block(*this, CharSet(*this, 'C'), 0, 1));
    bv[3].update(*this, Block(*this, CharSet(*this, ' '), 0, 1));
    bv[4].update(*this, Block(*this, CharSet(*this, '='), 0, 1));
    bv[5].update(*this, Block(*this, CharSet(*this, ' '), 0, 1));
    bv[6].update(*this, Block(*this, CharSet(*this, 'B'), 0, 1));
    bv[7].update(*this, Block(*this, CharSet(*this, '='), 0, 1));
    bv[8].update(*this, Block(*this, CharSet(*this, 'C'), 1, 1));
    StringVar x(*this, DashedString(*this, bv, 9));
    StringVar y(*this, Block(*this, CharSet(*this, '=')));    
    StringView vx(x), vy(y);    
    cerr << "Equate x = " << x << "  vs  y = " << y << "\n";
    assert(equate_x(*this, vx, vy) == ME_STRING_FAILED);
    assert(equate_x(*this, vy, vx) == ME_STRING_FAILED);
    cerr << "Failed!\n";
    //TODO:
    
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
  cerr << "\n----- test0.cpp passes -----\n\n";
  return 0;
}



