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
      match(Home h, StringView x, IntView i, trimDFA* R, trimDFA* R1, int r)
        : Match(h, x, i, R, R1, r) {};
    };
    string re = "a*b";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert((new (*this) match(*this, x, i, R, R1, 1))->propagate(*this, 0) == ES_FIX);
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
            trimDFA* R, trimDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "a*b";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(*R1, x.may_chars());
    assert((new (*this) match(*this, x, i, 1, R1, R, R2))->propagate(*this, 0) == ES_FIX);
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
      match(Home h, StringView x, IntView i, trimDFA* R, trimDFA* R1, int r)
        : Match(h, x, i, R, R1, r) {};
    };
    string re = "(ab)*(c|ad)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert((new (*this) match(*this, x, i, R, R1, 1))->propagate(*this, 0) == ES_FIX);
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
            trimDFA* R, trimDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "(ab)*(c|ad)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(*R1, x.may_chars());
    assert((new (*this) match(*this, x, i, 1, R1, R, R2))->propagate(*this, 0) == ES_FIX);
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
      match(Home h, StringView x, IntView i, trimDFA* R, trimDFA* R1, int r)
        : Match(h, x, i, R, R1, r) {};
    };
    string re = "(ab)*(c|ad)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert((new (*this) match(*this, x, i, R, R1, 1))->propagate(*this, 0) == ES_FIX);
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
            trimDFA* R, trimDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "(ab)*(c|ad)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(*R1, x.may_chars());
    assert((new (*this) match(*this, x, i, 1, R1, R, R2))->propagate(*this, 0) == ES_FIX);
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
      match(Home h, StringView x, IntView i, trimDFA* R, trimDFA* R1, int r)
        : Match(h, x, i, R, R1, r) {};
    };
    string re = "(ab)*(c|ad)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert((new (*this) match(*this, x, i, R, R1, 1))->propagate(*this, 0) == ES_FIX);
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
            trimDFA* R, trimDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "(ab)*(c|ad)";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(*R1, x.may_chars());
    assert((new (*this) match(*this, x, i, 1, R1, R, R2))->propagate(*this, 0) == ES_FIX);
    std::cerr << "===== After i = match(x, R) =====\n" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    assert (lx == x.domain().logdim());
    assert (i.min() == 1 && i.max() == 8);
  }
  
  void test09() {
    std::cerr << "\n*** Test 09 ***" << std::endl;
    NSBlocks v;
    NSIntSet s('b'); s.add('d');
    v.push_back(NSBlock(s, 1, 1));
    v.push_back(NSBlock(NSIntSet('c'), 1, 1));
    v.push_back(NSBlock(NSIntSet('b', 'd'), 10, 10));
    v.push_back(NSBlock(NSIntSet('a'), 1, 1));
    v.push_back(NSBlock(NSIntSet('d', 'e'), 0, 10));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 0, 100);
    class match : public MatchNew {
    public:
      match(Home h, StringView x, IntView i, int r,
            trimDFA* R, trimDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };
    string re = "ca";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    matchNFA* R2 = new matchNFA(*R1, x.may_chars());
    assert((new (*this) match(*this, x, i, 2, R1, R, R2))->propagate(*this, 0) == ES_FIX);
    std::cerr << "===== After i = match(x, R) =====\n" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    assert (i.size() == 2);
    assert (i.min() == 0 && i.max() == 12);
  }
  
  void test10() {
    std::cerr << "\n*** Test 10 ***" << std::endl;
    NSBlocks v;
    NSIntSet s('b'); s.add('d');
    v.push_back(NSBlock(s, 1, 1));
    v.push_back(NSBlock(NSIntSet('c'), 1, 1));
    v.push_back(NSBlock(NSIntSet('b', 'd'), 10, 10));
    v.push_back(NSBlock(NSIntSet('a'), 1, 1));
    v.push_back(NSBlock(NSIntSet('d', 'e'), 0, 10));
    StringVar x(*this, v, 0, 100);
    IntVar i(*this, 0, 100);
    class match : public Match {
    public:
      match(Home h, StringView x, IntView i, trimDFA* R, trimDFA* R1, int r)
        : Match(h, x, i, R, R1, r) {};
    };
    string re = "ca";
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    std::cerr << "R = " << re << std::endl;
    std::unique_ptr<RegEx> regex = RegExParser(".*(" + re + ").*").parse();
    trimDFA* R = new trimDFA(regex->dfa());
    trimDFA* R1 = new trimDFA(RegExParser("(" + re + ").*").parse()->dfa());
    std::cerr << "===== After i = match(x, R) =====" << std::endl;
    assert((new (*this) match(*this, x, i, R, R1, 2))->propagate(*this, 0) == ES_FIX);
    std::cerr << "x = " << x << std::endl;
    std::cerr << "i = " << i << std::endl;
    assert (i.size() == 21);
    assert (i.min() == 0 && i.max() == 22 && !i.in(1) && !i.in(2));
  }

  void test11() {
    std::cerr << "\n*** Test 11 ***" << std::endl;

    trimDFA accepts_empty(RegExParser("a*").parse()->dfa());
    assert(accepts_empty.min_word_length() == 0);
    NSBlocks empty;
    assert(Reg::propagate_blocks(*this, empty, &accepts_empty) == ES_FIX);

    trimDFA finite(RegExParser("(ab|cde)").parse()->dfa());
    assert(finite.min_word_length() == 2);
    assert(Reg::propagate_blocks(*this, empty, &finite) == ES_FAILED);

    DFA unreachable(0, {
      DFA::Transition(0, 'a', 0),
      DFA::Transition(1, 'b', 1)
    }, {1}, false);
    trimDFA no_word(unreachable);
    assert(no_word.min_word_length() == 0);
  }

  void test12() {
    std::cerr << "\n*** Test 12 ***" << std::endl;

    trimDFA dfa(RegExParser("a").parse()->dfa());
    int next = dfa.search(0, 'a');

    DSIntSet accepted(*this, NSIntSet('a'));
    assert(dfa.neighbot(0, accepted) == NSIntSet(next));

    DSIntSet partial(*this, NSIntSet('a', 'b'));
    assert(dfa.neighbot(0, partial).empty());

    DSBlock optional_second(*this, NSBlock(NSIntSet('a'), 1, 2));
    assert(dfa.reach_all(optional_second, NSIntSet(0)).empty());

    trimDFA variable_length(RegExParser("(a|aa)").parse()->dfa());
    int after_one = variable_length.search(0, 'a');
    int after_two = variable_length.search(after_one, 'a');
    NSIntSet optional_states(after_one);
    optional_states.add(after_two);
    assert(variable_length.reach_all(optional_second, NSIntSet(0)) ==
      optional_states);

    trimDFA branching(RegExParser("(a|bc)").parse()->dfa());
    NSIntSet next_states;
    next_states.add(branching.search(0, 'a'));
    next_states.add(branching.search(0, 'b'));
    DSIntSet complete(*this, NSIntSet('a', 'b'));
    assert(branching.neighbot(0, complete) == next_states);
  }

  void test13() {
    std::cerr << "\n*** Test 13 ***" << std::endl;

    DFA base = RegExParser("(a|bc)").parse()->dfa();
    trimDFA trimmed(base);
    NSIntSet alphabet('a', 'c');
    compDFA from_dfa(base, alphabet);
    compDFA from_trimmed(trimmed, alphabet);
    compDFA* complete[] = {&from_dfa, &from_trimmed};

    for (compDFA* dfa : complete) {
      for (int q = 0; q < dfa->n_states; ++q) {
        NSIntSet labels;
        for (const auto& transition : dfa->delta[q]) {
          assert(!transition.first.empty());
          labels.include(transition.first);
        }
        assert(labels == alphabet);
      }
      assert(dfa->accepted("a"));
      assert(dfa->accepted("bc"));
      assert(!dfa->accepted("b"));
      assert(!dfa->accepted("c"));

      dfa->negate();
      assert(!dfa->accepted("a"));
      assert(!dfa->accepted("bc"));
      assert(dfa->accepted(""));
      assert(dfa->accepted("b"));
      assert(dfa->accepted("c"));
      assert(dfa->search(dfa->q_bot, 'a') == dfa->q_bot);
    }

    DFA disjoint_base = RegExParser("z").parse()->dfa();
    trimDFA disjoint_trimmed(disjoint_base);
    NSIntSet singleton_alphabet('a');
    compDFA disjoint_from_dfa(disjoint_base, singleton_alphabet);
    compDFA disjoint_from_trimmed(disjoint_trimmed, singleton_alphabet);
    compDFA* disjoint[] = {&disjoint_from_dfa, &disjoint_from_trimmed};

    for (compDFA* dfa : disjoint) {
      assert(dfa->q_bot >= 0);
      assert(dfa->search(0, 'a') == dfa->q_bot);
      dfa->negate();
      assert(dfa->accepted("a"));
    }
  }

  void test14() {
    std::cerr << "\n*** Test 14 ***" << std::endl;

    class match : public MatchNew {
    public:
      match(Home h, StringView x, IntView i, int r,
            trimDFA* R, trimDFA* R1, matchNFA* R2)
        : MatchNew(h, x, i, r, R, R1, R2) {};
    };

    NSBlocks blocks;
    blocks.push_back(NSBlock(NSIntSet('a', 'b'), 1, 1));

    StringVar must_x(*this, blocks, 0, 1);
    IntVar must_i(*this, 0, 1);
    trimDFA* must_full = new trimDFA(
      RegExParser(".*((a|b)).*").parse()->dfa());
    trimDFA* must_pref = new trimDFA(
      RegExParser("((a|b)).*").parse()->dfa());
    matchNFA* must_nfa = new matchNFA(*must_pref, must_x.may_chars());
    assert((new (*this) match(*this, must_x, must_i, 1,
      must_pref, must_full, must_nfa))->propagate(*this, 0) != ES_FAILED);
    assert(must_i.assigned() && must_i.val() == 1);

    StringVar may_x(*this, blocks, 0, 1);
    IntVar may_i(*this, 0, 1);
    trimDFA* may_full = new trimDFA(
      RegExParser(".*(a).*").parse()->dfa());
    trimDFA* may_pref = new trimDFA(
      RegExParser("(a).*").parse()->dfa());
    matchNFA* may_nfa = new matchNFA(*may_pref, may_x.may_chars());
    assert((new (*this) match(*this, may_x, may_i, 1,
      may_pref, may_full, may_nfa))->propagate(*this, 0) == ES_FIX);
    assert(may_i.size() == 2 && may_i.in(0) && may_i.in(1));
  }

  void test15() {
    std::cerr << "\n*** Test 15 ***" << std::endl;

    StringVar x(*this, "b");
    IntVar no_match(*this, 0, 0);
    match_new(*this, x, "a", no_match);
    assert(status() == SS_SOLVED);
  }

  void test16() {
    std::cerr << "\n*** Test 16 ***" << std::endl;

    StringVar x(*this, "b");
    IntVar match_at_one(*this, 1, 1);
    match_new(*this, x, "a", match_at_one);
    assert(status() == SS_FAILED);
  }

  void test17() {
    std::cerr << "\n*** Test 17 ***" << std::endl;

    NSBlocks blocks;
    blocks.push_back(NSBlock(NSIntSet('a'), 2, 4));
    blocks.push_back(NSBlock(NSIntSet('b'), 1, 1));
    StringVar x(*this, blocks, 0, 5);
    const DashedString& domain = x.domain();

    NSBlocks prefix = domain.prefix(0, 3);
    assert(prefix.size() == 1 && prefix[0].S == NSIntSet('a'));
    assert(prefix[0].l == 2 && prefix[0].u == 3);

    prefix = domain.prefix(2, 0);
    assert(prefix == blocks);

    NSBlocks suffix = domain.suffix(0, 1);
    assert(suffix.size() == 2 && suffix[0].S == NSIntSet('a'));
    assert(suffix[0].l == 1 && suffix[0].u == 3);
    assert(suffix[1].S == NSIntSet('b'));

    suffix = domain.suffix(0, 4);
    assert(suffix.size() == 1 && suffix[0].S == NSIntSet('b'));
    assert(suffix[0].l == 1 && suffix[0].u == 1);
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
  return 0;
}
