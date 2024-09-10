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
    NSBlocks v;
    v.push_back(NSBlock(NSIntSet('c'), 1, 2));
    v.push_back(NSBlock(NSIntSet('a'), 2, 4));
    v.push_back(NSBlock(NSIntSet('b'), 1, 3));
    v.push_back(NSBlock(NSIntSet('d'), 3, 4));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 0, 100);
    double lx = x.domain().logdim();
    class match : public Match {
    public:
      match(Home h, StringView x, IntView i, stringDFA* R, stringDFA* R1, int r)
        : Match(h, x, i, R, R1, r) {};
    };
    string re = "a*b";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    String::RegEx* regex = RegExParser(".*(" + re + ").*").parse();    
    stringDFA* R = new stringDFA(regex->dfa());
    stringDFA* R1 = new stringDFA(RegExParser("(" + re + ").*").parse()->dfa());
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert(match(*this, x, i, R, R1, 1).propagate(*this, 0) == ES_FIX);
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    assert (lx == x.domain().logdim());
    assert (i.min() == 2 && i.max() == 9);
  }

  void test02() {
    std::cerr << "\n*** Test 02 ***" << std::endl;
    NSBlocks v;
    v.push_back(NSBlock(NSIntSet('c'), 1, 2));
    v.push_back(NSBlock(NSIntSet('a'), 2, 4));
    v.push_back(NSBlock(NSIntSet('b'), 1, 3));
    v.push_back(NSBlock(NSIntSet('d'), 3, 4));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 0, 100);
    double lx = x.domain().logdim();
    class match : public MatchNew {
    public:
      match(Home h, StringView x, IntView i, int r,
            stringDFA* R, stringDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "a*b";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    String::RegEx* regex = RegExParser(".*(" + re + ").*").parse();   
    stringDFA* R = new stringDFA(regex->dfa());
    stringDFA* R1 = new stringDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(R1->toMatchNFA(x.may_chars()));
    assert(match(*this, x, i, 1, R1, R, R2).propagate(*this, 0) == ES_FIX);
    std::cerr << "===== After i = match(x, R) =====\n" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    assert (lx == x.domain().logdim());
    assert (i.min() == 2 && i.max() == 3);
  }

  void test03() {
    std::cerr << "\n*** Test 03 ***" << std::endl;
    NSBlocks v;
    v.push_back(NSBlock(NSIntSet('a', 'b'), 2, 2));
    NSIntSet S('a');
    S.add('d');
    v.push_back(NSBlock(S, 1, 3));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 0, 100);
    double lx = x.domain().logdim();
    class match : public Match {
    public:
      match(Home h, StringView x, IntView i, stringDFA* R, stringDFA* R1, int r)
        : Match(h, x, i, R, R1, r) {};
    };
    string re = "(ab)*(c|ad)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    String::RegEx* regex = RegExParser(".*(" + re + ").*").parse();    
    stringDFA* R = new stringDFA(regex->dfa());
    stringDFA* R1 = new stringDFA(RegExParser("(" + re + ").*").parse()->dfa());
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert(match(*this, x, i, R, R1, 1).propagate(*this, 0) == ES_FIX);
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    assert (lx == x.domain().logdim());
    assert (i.min() == 0 && i.max() == 5);
  }

  void test04() {
    std::cerr << "\n*** Test 04 ***" << std::endl;
    NSBlocks v;
    v.push_back(NSBlock(NSIntSet('a', 'b'), 2, 2));
    NSIntSet S('a');
    S.add('d');
    v.push_back(NSBlock(S, 1, 3));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 0, 100);
    double lx = x.domain().logdim();
    class match : public MatchNew {
    public:
      match(Home h, StringView x, IntView i, int r,
            stringDFA* R, stringDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "(ab)*(c|ad)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    String::RegEx* regex = RegExParser(".*(" + re + ").*").parse();   
    stringDFA* R = new stringDFA(regex->dfa());
    stringDFA* R1 = new stringDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(R1->toMatchNFA(x.may_chars()));
    assert(match(*this, x, i, 1, R1, R, R2).propagate(*this, 0) == ES_FIX);
    std::cerr << "===== After i = match(x, R) =====\n" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    assert (lx == x.domain().logdim());
    assert (i.min() == 0 && i.max() == 4);
  }

  void test05() {
    std::cerr << "\n*** Test 05 ***" << std::endl;
    NSBlocks v;
    v.push_back(NSBlock(NSIntSet('a', 'b'), 2, 2));
    v.push_back(NSBlock(NSIntSet('d'), 1, 3));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 0, 100);
    double lx = x.domain().logdim();
    class match : public Match {
    public:
      match(Home h, StringView x, IntView i, stringDFA* R, stringDFA* R1, int r)
        : Match(h, x, i, R, R1, r) {};
    };
    string re = "(ab)*(c|ad)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    String::RegEx* regex = RegExParser(".*(" + re + ").*").parse();    
    stringDFA* R = new stringDFA(regex->dfa());
    stringDFA* R1 = new stringDFA(RegExParser("(" + re + ").*").parse()->dfa());
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert(match(*this, x, i, R, R1, 1).propagate(*this, 0) == ES_FIX);
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    assert (lx == x.domain().logdim());
    assert (i.min() == 0 && i.max() == 5);
  }

  void test06() {
    std::cerr << "\n*** Test 06 ***" << std::endl;
    NSBlocks v;
    v.push_back(NSBlock(NSIntSet('a', 'b'), 2, 2));
    v.push_back(NSBlock(NSIntSet('d'), 1, 3));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 0, 100);
    double lx = x.domain().logdim();
    class match : public MatchNew {
    public:
      match(Home h, StringView x, IntView i, int r,
            stringDFA* R, stringDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "(ab)*(c|ad)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    String::RegEx* regex = RegExParser(".*(" + re + ").*").parse();   
    stringDFA* R = new stringDFA(regex->dfa());
    stringDFA* R1 = new stringDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(R1->toMatchNFA(x.may_chars()));
    assert(match(*this, x, i, 1, R1, R, R2).propagate(*this, 0) == ES_FIX);
    std::cerr << "===== After i = match(x, R) =====\n" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    assert (lx == x.domain().logdim());
    assert (i.min() == 0 && !i.in(1) && i.max() == 2);
  }

  void test07() {
    std::cerr << "\n*** Test 07 ***" << std::endl;
    NSBlocks v;
    v.push_back(NSBlock(NSIntSet('a'), 1, 5));
    v.push_back(NSBlock(NSIntSet('c','d'), 1, 3));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 0, 100);
    double lx = x.domain().logdim();
    class match : public Match {
    public:
      match(Home h, StringView x, IntView i, stringDFA* R, stringDFA* R1, int r)
        : Match(h, x, i, R, R1, r) {};
    };
    string re = "(ab)*(c|ad)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    String::RegEx* regex = RegExParser(".*(" + re + ").*").parse();    
    stringDFA* R = new stringDFA(regex->dfa());
    stringDFA* R1 = new stringDFA(RegExParser("(" + re + ").*").parse()->dfa());
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert(match(*this, x, i, R, R1, 1).propagate(*this, 0) == ES_FIX);
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    assert (lx == x.domain().logdim());
    assert (i.min() == 1 && i.max() == 8);
  }

  void test08() {
    std::cerr << "\n*** Test 08 ***" << std::endl;
    NSBlocks v;
    v.push_back(NSBlock(NSIntSet('a'), 1, 5));
    v.push_back(NSBlock(NSIntSet('c','d'), 1, 3));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 0, 100);
    double lx = x.domain().logdim();
    class match : public MatchNew {
    public:
      match(Home h, StringView x, IntView i, int r,
            stringDFA* R, stringDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "(ab)*(c|ad)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    String::RegEx* regex = RegExParser(".*(" + re + ").*").parse();   
    stringDFA* R = new stringDFA(regex->dfa());
    stringDFA* R1 = new stringDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(R1->toMatchNFA(x.may_chars()));
    assert(match(*this, x, i, 1, R1, R, R2).propagate(*this, 0) == ES_FIX);
    std::cerr << "===== After i = match(x, R) =====\n" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    assert (lx == x.domain().logdim());
    assert (i.min() == 1 && i.max() == 8);
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
  return 0;
}
