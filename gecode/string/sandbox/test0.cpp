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
      b1.includeBaseIn(*this, *ps);
      b2.includeBaseIn(*this, *ps);
      b4.baseIntersect(*this, *ps);
      b4.lb(*this, 5);
      b4.ub(*this, 5);
      assert (b4.val() == std::vector<int>({'a', 'a', 'a', 'a', 'a'}));
      Gecode::Set::SetDelta d;
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
    std::vector<Block> b(n);
    b[i++].update(*this, Block(*this, CharSet(*this, IntSet({'B','b'})), 1 ,1));
    b[i++].update(*this, Block(*this, CharSet(*this, IntSet({'o'})), 1, 3));
    b[i++].update(*this, Block(*this, CharSet(*this, IntSet({'o'})), 1, 1));
    b[i++].update(*this, Block());
    b[i++].update(*this, Block('m'));
    b[i++].update(*this, Block(*this, CharSet(*this, IntSet({'!'})), 0, 1));
    b[i++].update(*this, Block(*this, CharSet(*this, IntSet({'!'})), 0, 0));
    b[i++].update(*this, Block(*this, CharSet(*this, IntSet({'!'})), 0, 2));
    DashedString d2(*this, b);
    cerr << d2 << '\n';
    assert (d2.min_length() == 4 && d2.size() == 4 && d2.max_length() == 9);
    assert (d2.logdim() - log(24) < DBL_EPSILON);
    b = std::vector<Block>(2);
    b[0].update(*this, Block(*this, CharSet(*this, 'r','t'), 1 ,3));
    b[1].update(*this, Block(*this, CharSet(*this, IntSet({'a','d'})), 0 ,2));
    d0.update(*this, DashedString(*this, b));
    assert (d0.contains(d1));
    cerr << d0 << '\n';
    d1.nullify(*this);
    assert (d1.isNull() && d1.val() == std::vector<int>(0));
  }
  
  void test04() {
    cerr << "\n*** Test 04 ***" << endl;
    std::vector<Block> bv(5);
    bv[0].update(*this, Block(*this, CharSet(*this, 'a'), 2 ,3));
    bv[1].update(*this, Block(*this, CharSet(*this, 'c'), 1 ,2));
    bv[2].update(*this, Block('b'));
    bv[3].update(*this, Block(*this, CharSet(*this, 'c'), 0 ,2));
    bv[4].update(*this, Block(*this, CharSet(*this, 'a'), 3, 4));
    StringVar y(*this, DashedString(*this, bv));
    StringView vy(y);
    StringView::SweepFwdIterator fwd_it0 =
      StringView::SweepFwdIterator(vy, Position(0,1));
    Block b(*this, CharSet(*this, 'a', 'b'), 3 ,4);
    cerr << "Pushing forward " << b << " in " << y << " from " << *fwd_it0;
    Position p = push<StringView::SweepFwdIterator>(b, fwd_it0);
    cerr << "\n...ESP = " << p << ", EEP = " << *fwd_it0 << endl;
    assert (p == Position(2,0) && *fwd_it0 == Position(4,2));
    
    b.update(*this, Block(*this, CharSet(*this, 'c', 'z'), 3 ,4));
    StringView::SweepFwdIterator fwd_it1 = vy.sweep_fwd_iterator();
    cerr << "Pushing forward " << b << " in " << y << " from " << *fwd_it1;
    p = push<StringView::SweepFwdIterator>(b, fwd_it1);
    cerr << "\n...ESP = " << p << ", EEP = " << *fwd_it1 << endl;
    assert (p == *fwd_it1 && p == Position(y.varimp()->size(), 0));
    
    b.update(*this, Block(*this, CharSet(*this, 'c', 'd'), 1 ,2));
    StringView::PushBwdIterator bwd_it0 = vy.push_bwd_iterator();
    cerr << "Pushing backward " << b << " in " << y << " from " << *bwd_it0;
    p = push<StringView::PushBwdIterator>(b, bwd_it0);
    cerr << "\n...LSP = " << *bwd_it0 << ", LEP = " << p << endl;
    assert (*bwd_it0 == Position(3,1) && p == Position(3,2));
    
    b.update(*this, Block('b',2));
    StringView::PushBwdIterator bwd_it1 =
      StringView::PushBwdIterator(vy, Position(4,4));
    cerr << "Pushing backward " << b << " in " << y << " from " << *bwd_it1;
    p = push<StringView::PushBwdIterator>(b, bwd_it1);
    cerr << "\n...LSP = " << *bwd_it1 << ", LEP = " << p << endl;
    assert (p == *bwd_it1 && p == Position(-1, 0));
    
  }
  
   void test05() {
    cerr << "\n*** Test 05 ***" << endl;
    std::vector<Block> bv(5);
    bv[0].update(*this, Block(*this, CharSet(*this, 'a'), 2 ,3));
    bv[1].update(*this, Block(*this, CharSet(*this, 'c'), 1 ,2));
    bv[2].update(*this, Block('b'));
    bv[3].update(*this, Block(*this, CharSet(*this, 'c'), 0 ,2));
    bv[4].update(*this, Block(*this, CharSet(*this, 'a'), 3, 4));
    StringVar y(*this, DashedString(*this, bv));
    StringView vy(y);
    StringView::SweepFwdIterator fwd_it0 =
      StringView::SweepFwdIterator(vy, Position(0,1));
    Block b(*this, CharSet(*this, 'a', 'b'), 3 ,4);
    cerr << "Streching forward " << b << " in " << y << " from " << *fwd_it0;
    stretch<StringView::SweepFwdIterator>(b, fwd_it0);
    cerr << "\n...to " << *fwd_it0 << endl;
    assert (*fwd_it0 == Position(1,0));

    StringView::SweepFwdIterator fwd_it1 =
      StringView::SweepFwdIterator(vy, Position(1,1));
    cerr << "Streching forward " << b << " in " << y << " from " << *fwd_it1;
    stretch<StringView::SweepFwdIterator>(b, fwd_it1);
    cerr << "\n...to " << *fwd_it1 << endl;
    assert (*fwd_it1 == Position(5,0));

    b.update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, 3));
    StringView::StretchBwdIterator bwd_it0 =
      StringView::StretchBwdIterator(vy, Position(3, 1));
    cerr << "Stretching backward " << b << " in " << y << " from " << *bwd_it0;
    stretch<StringView::StretchBwdIterator>(b, bwd_it0);
    cerr << "\n...to " << *bwd_it0 << endl;
    assert (*bwd_it0 == Position(0,1));
        
    StringView::StretchBwdIterator bwd_it1 = vy.stretch_bwd_iterator();
    cerr << "Stretching backward " << b << " in " << y << " from " << *bwd_it1;
    stretch<StringView::StretchBwdIterator>(b, bwd_it1);
    cerr << "\n...to " << *bwd_it1 << endl;
    assert (*bwd_it1 == Position(2,1));
  }
  
  void test06() {
    cerr << "\n*** Test 06 ***" << endl;
    std::vector<Block> bv(5);
    bv[0].update(*this, Block(*this, CharSet(*this, IntSet({'B','b'})), 1 , 1));
    bv[1].update(*this, Block(*this, CharSet(*this, 'o'), 2 ,4));
    bv[2].update(*this, Block('m'));
    bv[3].update(*this, Block(*this, CharSet(*this, '!'), 0 ,3));
    StringVar y(*this, DashedString(*this, bv));
    StringView vy(y);
    StringView::SweepFwdIterator fwd_it0 =
      StringView::SweepFwdIterator(vy, Position(2,0));
    Block b(*this, CharSet(*this, 'm'), 1 ,2);
    cerr << "Pushing forward " << b << " in " << y << " from " << *fwd_it0;
    Position p = push<StringView::SweepFwdIterator>(b, fwd_it0);
    cerr << "\n...ESP = " << p << ", EEP = " << *fwd_it0 << endl;
    assert (p == Position(2,0) && *fwd_it0 == Position(3,0));
    
    StringView::SweepFwdIterator fwd_it1 =
      StringView::SweepFwdIterator(vy, Position(2,1));
    cerr << "Pushing forward " << b << " in " << y << " from " << *fwd_it1;
    p = push<StringView::SweepFwdIterator>(b, fwd_it1);
    cerr << "\n...ESP = " << p << ", EEP = " << *fwd_it1 << endl;
    assert (p == *fwd_it1 && p == Position(4,0));
    
    b.update(*this, Block(*this, CharSet(*this, IntSet({'o','m','g'})), 1, 8));
    StringView::SweepFwdIterator fwd_it2 =
      StringView::SweepFwdIterator(vy, Position(1,1));
    cerr << "Streching forward " << b << " in " << y << " from " << *fwd_it2;
    stretch<StringView::SweepFwdIterator>(b, fwd_it2);
    cerr << "\n...to " << *fwd_it2 << endl;
    assert (Position(3,3).equiv(*fwd_it2, vy[(*fwd_it2).idx-1].ub()));
    
    StringView::StretchBwdIterator bwd_it0 =
      StringView::StretchBwdIterator(vy, Position(1,1));
    cerr << "Streching backward " << b << " in " << y << " from " << *bwd_it0;
    stretch<StringView::StretchBwdIterator>(b, bwd_it0);
    cerr << "\n...to " << *bwd_it0 << endl;
    assert (*bwd_it0 == Position(0,1));
    
    StringView::PushBwdIterator bwd_it1 =
      StringView::PushBwdIterator(vy, *bwd_it0);
    cerr << "Pushing backward " << b << " in " << y << " from " << *bwd_it1;
    p = push<StringView::PushBwdIterator>(b, bwd_it1);
    cerr << "\n...LSP = " << *bwd_it1 << ", LEP = " << p << endl;
    assert (*bwd_it1 == p && p < Position(0,0));
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
  cerr << "\n----- test0.cpp passes -----\n\n";
  return 0;
}



