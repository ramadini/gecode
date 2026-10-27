/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "test/string.hh"

#include <gecode/string/extensional.hh>
#include <gecode/string/extensional/parse-reg.hpp>

#include <memory>
#include <set>
#include <utility>

namespace Test { namespace String { namespace Extensional {

  /// Test regular-language membership and its reified variants
  class Regular : public Test {
  public:
    enum Language {
      EVEN_B,
      A_STAR_B
    };

  private:
    Language language;

    Gecode::DFA automaton(void) const {
      using Gecode::DFA;
      switch (language) {
      case EVEN_B:
        return DFA(0, {
          DFA::Transition(0, 'a', 0),
          DFA::Transition(0, 'b', 1),
          DFA::Transition(1, 'a', 1),
          DFA::Transition(1, 'b', 0)
        }, {0});
      case A_STAR_B:
        return DFA(0, {
          DFA::Transition(0, 'a', 0),
          DFA::Transition(0, 'b', 1)
        }, {1});
      default:
        GECODE_NEVER;
      }
      return DFA();
    }

  public:
    Regular(const std::string& name, Language language0)
      : Test("Extensional::" + name, 1, "ab", 3, true),
        language(language0) {}

    virtual bool solution(const Assignment& a) const {
      const std::string& x = a[0];
      switch (language) {
      case EVEN_B: {
        unsigned int count = 0;
        for (std::string::const_iterator c = x.begin(); c != x.end(); ++c)
          if (*c == 'b')
            ++count;
        return (count % 2) == 0;
      }
      case A_STAR_B:
        if (x.empty() || (x[x.size() - 1] != 'b'))
          return false;
        for (std::string::size_type i = 0; i + 1 < x.size(); ++i)
          if (x[i] != 'a')
            return false;
        return true;
      default:
        return false;
      }
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::extensional(home, x[0], automaton());
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x,
                      Gecode::Reify r) {
      Gecode::DFA d = automaton();
      Gecode::extensional(home, x[0], d, r.var(), r.mode());
    }
  };

  Regular even_b("EvenB", Regular::EVEN_B);
  Regular a_star_b("AStarB", Regular::A_STAR_B);

  /// Space for the DFA reification truth table
  class DfaReificationSpace : public Gecode::Space {
  public:
    Gecode::StringVar x;
    Gecode::BoolVar b;

    DfaReificationSpace(int symbol, int control, Gecode::ReifyMode mode,
                        bool assigned)
      : b(*this, control < 0 ? 0 : control, control < 0 ? 1 : control) {
      using namespace Gecode;
      const Gecode::String::StringVal value =
        Gecode::String::StringVal::from_symbols({symbol});
      if (assigned) {
        x = StringVar(*this, value);
      } else {
        Gecode::String::NSIntSet alphabet('a');
        alphabet.add('b');
        alphabet.add('c');
        alphabet.add(0x100);
        alphabet.add(0x101);
        alphabet.add(0x102);
        x = StringVar(*this, alphabet, 1, 1);
      }
      const DFA d(0, {
        DFA::Transition(0, 'a', 1),
        DFA::Transition(0, 'c', 0),
        DFA::Transition(0, 0x100, 1),
        DFA::Transition(0, 0x102, 0)
      }, {1});
      extensional(*this, x, d, b, mode);
      if (!assigned)
        rel(*this, x, STRT_EQ, StringVar(*this, value));
    }

    DfaReificationSpace(DfaReificationSpace& s) : Gecode::Space(s) {
      x.update(*this, s.x);
      b.update(*this, s.b);
    }

    virtual Gecode::Space* copy(void) {
      return new DfaReificationSpace(*this);
    }
  };

  /// Check both assignment orders, including the byte and scalar fast paths
  class DfaReification : public Base {
  private:
    bool assigned;
  public:
    DfaReification(bool assigned0)
      : Base(std::string("String::Extensional::DfaReification::") +
             (assigned0 ? "Assigned" : "Deferred")),
        assigned(assigned0) {}

    virtual bool run(void) {
      using namespace Gecode;
      const int symbols[] = {'a', 'b', 'c', 0x100, 0x101, 0x102};
      const ReifyMode modes[] = {RM_EQV, RM_IMP, RM_PMI};
      for (int symbol : symbols) {
        const bool accepted = (symbol == 'a') || (symbol == 0x100);
        for (ReifyMode mode : modes) {
          const bool allow_zero = (mode == RM_IMP) || !accepted;
          const bool allow_one = (mode == RM_PMI) || accepted;
          for (int control = -1; control <= 1; ++control) {
            DfaReificationSpace s(symbol, control, mode, assigned);
            const bool failed = s.status() == SS_FAILED;
            const bool contradiction =
              ((control == 0) && !allow_zero) ||
              ((control == 1) && !allow_one);
            const int min = control < 0 ? (allow_zero ? 0 : 1) : control;
            const int max = control < 0 ? (allow_one ? 1 : 0) : control;
            if ((failed != contradiction) ||
                (!failed && ((s.b.min() != min) || (s.b.max() != max)))) {
              if (opt.log)
                olog << "DFA reification: symbol=" << symbol
                     << " mode=" << mode << " control=" << control
                     << " assigned=" << assigned << std::endl;
              return false;
            }
          }
        }
      }
      return true;
    }
  };

  DfaReification dfa_reification_assigned(true);
  DfaReification dfa_reification_deferred(false);

  /// Reify membership while both one-character strings are still possible
  class RegexReificationSpace : public Gecode::Space {
  public:
    Gecode::StringVar x;
    Gecode::BoolVar b;

    RegexReificationSpace(const std::string& regex, int control,
                          Gecode::ReifyMode mode)
      : x(*this, Gecode::String::NSIntSet('a', 'b'), 1, 1),
        b(*this, control < 0 ? 0 : control, control < 0 ? 1 : control) {
      using namespace Gecode;
      extensional(*this, x, regex, b, mode);
      branch(*this, StringVarArgs() << x, STRING_VAR_NONE(), STRING_VAL_LLLL());
      branch(*this, BoolVarArgs() << b, BOOL_VAR_NONE(), BOOL_VAL_MIN());
    }

    RegexReificationSpace(RegexReificationSpace& s) : Gecode::Space(s) {
      x.update(*this, s.x);
      b.update(*this, s.b);
    }

    virtual Gecode::Space*
    copy(void) {
      return new RegexReificationSpace(*this);
    }
  };

  /// Compare mode inference, exact solutions, and assignments after a fixpoint
  class RegexReification : public Base {
  private:
    std::string regex;
    std::set<std::string> members;

    typedef std::set<std::pair<std::string, int>> Solutions;

    static bool
    later_assignments(RegexReificationSpace& root, const Solutions& expected) {
      using namespace Gecode;
      for (const char* word : {"a", "b"})
        for (int control = 0; control <= 1; ++control)
          for (bool boolean_first : {false, true}) {
            std::unique_ptr<RegexReificationSpace> s(
                static_cast<RegexReificationSpace*>(root.clone()));
            if (boolean_first)
              rel(*s, s->b, IRT_EQ, control);
            else
              rel(*s, s->x, STRT_EQ, StringVar(*s, word));
            if (s->status() != SS_FAILED) {
              if (boolean_first)
                rel(*s, s->x, STRT_EQ, StringVar(*s, word));
              else
                rel(*s, s->b, IRT_EQ, control);
            }
            const bool allowed = expected.count(std::make_pair(word, control));
            if ((s->status() != SS_FAILED) != allowed)
              return false;
          }
      return true;
    }

  public:
    RegexReification(const std::string& name, const std::string& pattern,
                     std::set<std::string> accepted)
      : Base("String::Extensional::RegexReification::" + name),
        regex(pattern), members(accepted) {}

    virtual bool
    run(void) {
      using namespace Gecode;
      for (ReifyMode mode : {RM_EQV, RM_IMP, RM_PMI})
        for (int control = -1; control <= 1; ++control) {
          Solutions expected;
          bool allow_zero = false;
          bool allow_one = false;
          for (const char* word : {"a", "b"}) {
            const bool member = members.count(word);
            for (int value = 0; value <= 1; ++value) {
              const bool allowed = mode == RM_EQV   ? (bool(value) == member)
                                   : mode == RM_IMP ? (!value || member)
                                                    : (!member || value);
              if (allowed && ((control < 0) || (control == value))) {
                expected.insert(std::make_pair(word, value));
                allow_zero |= value == 0;
                allow_one |= value == 1;
              }
            }
          }
          RegexReificationSpace root(regex, control, mode);
          const bool failed = root.status() == SS_FAILED;
          if (failed != expected.empty())
            return false;
          if (!failed && ((root.b.min() != (allow_zero ? 0 : 1)) ||
                          (root.b.max() != (allow_one ? 1 : 0)) ||
                          !later_assignments(root, expected)))
            return false;
          Search::Options options;
          options.threads = 1;
          DFS<RegexReificationSpace> search(&root, options);
          Solutions actual;
          while (RegexReificationSpace* solution = search.next()) {
            std::unique_ptr<RegexReificationSpace> s(solution);
            if (!s->x.assigned() || !s->b.assigned() ||
                !actual.insert(std::make_pair(s->x.val(), s->b.val())).second)
              return false;
          }
          if (actual != expected) {
            if (opt.log)
              olog << "Regex reification: regex=" << regex << " mode=" << mode
                   << " control=" << control << std::endl;
            return false;
          }
        }
      return true;
    }
  };

  RegexReification regex_all_accepted("AllAccepted", "[ab]", {"a", "b"});
  RegexReification regex_all_rejected("AllRejected", "c", {});
  RegexReification regex_mixed("Mixed", "a", {"a"});

  class RegexFixpointSpace : public Gecode::Space {
  public:
    Gecode::StringVar x;

    RegexFixpointSpace(int control, Gecode::ReifyMode mode) {
      using namespace Gecode;
      using namespace Gecode::String;
      NSBlocks blocks({NSBlock(NSIntSet('a', 'b'), 0, 1),
                       NSBlock(NSIntSet('a', 'c'), 0, 1),
                       NSBlock(NSIntSet('b', 'c'), 0, 1)});
      x = StringVar(*this, blocks, 2, 2);
      extensional(*this, x, "[bc].", BoolVar(*this, control, control), mode);
    }

    RegexFixpointSpace(RegexFixpointSpace& s) : Gecode::Space(s) {
      x.update(*this, s.x);
    }

    virtual Gecode::Space*
    copy(void) {
      return new RegexFixpointSpace(*this);
    }
  };

  class RegexFixpoint : public Base {
  public:
    RegexFixpoint()
      : Base("String::Extensional::RegexReification::Fixpoint") {}

    virtual bool
    run(void) {
      using namespace Gecode;
      const std::set<std::string> domain = {"aa", "ab", "ac", "ba",
                                            "bb", "bc", "cb", "cc"};
      for (ReifyMode mode : {RM_EQV, RM_IMP, RM_PMI})
        for (int control : {1, 0}) {
          RegexFixpointSpace root(control, mode);
          if (root.status() == SS_FAILED || root.x.min_length() != 2 ||
              root.x.max_length() != 2)
            return false;
          for (const char* word : {"aa", "ab", "ac", "ba", "bb", "bc", "ca",
                                   "cb", "cc", "", "b", "bbb"}) {
            std::unique_ptr<RegexFixpointSpace> s(
                static_cast<RegexFixpointSpace*>(root.clone()));
            const std::string value(word);
            const bool member = value.size() == 2 && value[0] != 'a';
            const bool allowed = mode == RM_EQV   ? (bool(control) == member)
                                 : mode == RM_IMP ? (!control || member)
                                                  : (!member || control);
            rel(*s, s->x, STRT_EQ, StringVar(*s, value));
            if ((s->status() != SS_FAILED) !=
                (domain.count(value) && allowed)) {
              if (opt.log)
                olog << "Regex fixpoint: word=" << value << " mode=" << mode
                     << " control=" << control << std::endl;
              return false;
            }
          }
        }
      return true;
    }
  };

  RegexFixpoint regex_fixpoint;

  /// Enumerate a structured domain through either public regex overload
  class StarSpace : public Gecode::Space {
  public:
    Gecode::StringVar x;

    StarSpace(Gecode::String::NSBlocks blocks, int min, int max,
              const std::string& regex, bool decomposed)
      : x(*this, blocks, min, max) {
      if (decomposed) {
        Gecode::VarArgs auxiliaries;
        Gecode::extensional(*this, x, regex, auxiliaries);
      } else {
        Gecode::extensional(*this, x, regex);
      }
      Gecode::branch(*this, Gecode::StringVarArgs() << x,
                     Gecode::STRING_VAR_NONE(), Gecode::STRING_VAL_LLLL());
    }

    StarSpace(StarSpace& s) : Gecode::Space(s) {
      x.update(*this, s.x);
    }

    virtual Gecode::Space* copy(void) {
      return new StarSpace(*this);
    }
  };

  /// Compare exact solutions with the original block and language semantics
  class StarDecomposition : public Base {
  private:
    int a_min;
    int b_min;
    int min_length;
    int max_length;
    std::string alphabet;

    typedef std::set<std::string> Words;

    static bool collect_solutions(StarSpace& root, Words& words) {
      Gecode::Search::Options options;
      options.threads = 1;
      Gecode::DFS<StarSpace> search(&root, options);
      while (StarSpace* solution = search.next()) {
        std::unique_ptr<StarSpace> s(solution);
        if (!s->x.assigned() || !words.insert(s->x.val()).second)
          return false;
      }
      return true;
    }

  public:
    StarDecomposition(const std::string& name, int a_min0, int b_min0,
                      int min, int max, const std::string& alphabet0)
      : Base("String::Extensional::StarDecomposition::" + name),
        a_min(a_min0), b_min(b_min0), min_length(min), max_length(max),
        alphabet(alphabet0) {}

    virtual bool run(void) {
      Gecode::String::NSBlocks blocks;
      blocks.emplace_back(Gecode::String::NSIntSet('a'), a_min, 1);
      blocks.emplace_back(Gecode::String::NSIntSet('b'), b_min, 1);
      const std::string regex = alphabet.size() == 1 ? alphabet + "*" :
        "[" + alphabet + "]*";
      Words expected;
      for (int a = a_min; a <= 1; ++a)
        for (int b = b_min; b <= 1; ++b) {
          const std::string word = std::string(a, 'a') + std::string(b, 'b');
          if ((a + b >= min_length) && (a + b <= max_length) &&
              (word.find_first_not_of(alphabet) == std::string::npos))
            expected.insert(word);
        }
      for (bool decomposed : {false, true}) {
        StarSpace root(blocks, min_length, max_length, regex, decomposed);
        Words actual;
        if (!collect_solutions(root, actual) || (actual != expected)) {
          if (opt.log)
            olog << "Star decomposition: regex=" << regex
                 << " decomposed=" << decomposed << std::endl;
          return false;
        }
      }
      return true;
    }
  };

  StarDecomposition star_optional("OptionalIncompatible", 1, 0, 1, 2, "a");
  StarDecomposition star_bounds("BlockBounds", 0, 0, 0, 2, "ab");
  StarDecomposition star_aggregate("AggregateBounds", 0, 0, 1, 1, "ab");
  StarDecomposition star_aggregate_incompatible(
    "AggregateIncompatible", 0, 0, 2, 2, "a");
  StarDecomposition star_mandatory("MandatoryIncompatible", 0, 1, 1, 2, "a");
  StarDecomposition star_epsilon("Epsilon", 0, 0, 0, 2, "a");
  StarDecomposition star_only_epsilon("OnlyEpsilon", 0, 0, 0, 2, "c");

  /// Keep every reported decomposition handle alive across propagation/search.
  class UnionSpace : public Gecode::Space {
  public:
    Gecode::StringVar x;
    Gecode::StringVarArray strings;
    Gecode::IntVarArray integers;
    Gecode::BoolVarArray booleans;

    UnionSpace(Gecode::String::NSBlocks blocks, int min, int max,
               const std::string& pattern, bool explicit_post,
               const Gecode::String::StringVal* value = nullptr,
               bool assign_first = false)
      : x(*this, blocks, min, max) {
      using namespace Gecode;
      if (value != nullptr && assign_first)
        rel(*this, x, STRT_EQ, StringVar(*this, *value));
      VarArgs variables;
      if (explicit_post)
        extensional(*this, x, pattern, variables);
      else
        extensional(*this, x, pattern);
      strings = StringVarArray(*this, variables.sva);
      integers = IntVarArray(*this, variables.iva);
      booleans = BoolVarArray(*this, variables.bva);
      if (value != nullptr && !assign_first && status() != SS_FAILED)
        rel(*this, x, STRT_EQ, StringVar(*this, *value));
      branch(*this, StringVarArgs() << x, STRING_VAR_NONE(), STRING_VAL_LLLL());
      branch(*this, strings, STRING_VAR_NONE(), STRING_VAL_LLLL());
      branch(*this, integers, INT_VAR_NONE(), INT_VAL_MIN());
      branch(*this, booleans, BOOL_VAR_NONE(), BOOL_VAL_MIN());
    }

    UnionSpace(UnionSpace& s) : Gecode::Space(s) {
      x.update(*this, s.x);
      strings.update(*this, s.strings);
      integers.update(*this, s.integers);
      booleans.update(*this, s.booleans);
    }

    virtual Gecode::Space*
    copy(void) {
      return new UnionSpace(*this);
    }
  };

  /// Compare bounded star languages after assigning all introduced variables.
  class BoundedStar : public Base {
  private:
    std::string regex;
    int upper;
    std::set<std::string> expected;
    bool compare_ordinary;
    int other_symbol;

  public:
    BoundedStar(const std::string& name, const std::string& pattern, int max,
                std::set<std::string> words, bool ordinary = true,
                int symbol = 'b')
      : Base("String::Extensional::BoundedStar::" + name),
        regex(pattern), upper(max), expected(std::move(words)),
        compare_ordinary(ordinary), other_symbol(symbol) {}

    virtual bool
    run(void) {
      using namespace Gecode;
      using namespace Gecode::String;
      NSIntSet alphabet('a');
      alphabet.include(NSIntSet(other_symbol));
      const NSBlocks blocks({NSBlock(alphabet, 0, upper)});
      for (bool explicit_post : {true, false}) {
        if (!explicit_post && !compare_ordinary)
          continue;
        const StringVal a = StringVal::decode_utf8("a");
        for (bool assign_first : {true, false}) {
          UnionSpace assigned(blocks, 0, upper, regex, explicit_post, &a,
                              assign_first);
          if ((assigned.status() != SS_FAILED) != (expected.count("a") != 0))
            return false;
        }
        UnionSpace root(blocks, 0, upper, regex, explicit_post);
        Search::Options options;
        options.threads = 1;
        DFS<UnionSpace> search(&root, options);
        std::set<std::string> actual;
        while (UnionSpace* solution = search.next()) {
          std::unique_ptr<UnionSpace> s(solution);
          if (!s->x.assigned() || !s->strings.assigned() ||
              !s->integers.assigned() || !s->booleans.assigned())
            return false;
          actual.insert(s->x.val_symbols().to_utf8());
        }
        if (actual != expected) {
          if (opt.log)
            olog << "Bounded star: regex=" << regex
                 << " explicit_post=" << explicit_post << std::endl;
          return false;
        }
      }
      return true;
    }
  };

  BoundedStar star_exact_pair("ExactPair", "[a,2,2]*", 4, {"", "aa", "aaaa"});
  BoundedStar star_class_pair("ClassPair", "[ab,2,2]*", 3,
                              {"", "aa", "ab", "ba", "bb"});
  BoundedStar star_two_or_three("TwoOrThree", "[a,2,3]*", 4,
                                {"", "aa", "aaa", "aaaa"});
  BoundedStar star_bounded_concat("BoundedConcat", "([a,1,2]a)*", 4,
                                  {"", "aa", "aaa", "aaaa"});
  BoundedStar star_empty_intersection("EmptyIntersection", "([a,2,2]&[a,3,3])*",
                                      4, {""});
  BoundedStar star_zero_repeat("ZeroRepeat", "[a,0,0]*", 2, {""});
  BoundedStar star_large_positive("LargePositive", "[ab,1,65535]*", 2,
                                  {"", "a", "b", "aa", "ab", "ba", "bb"},
                                  false);
  BoundedStar star_large_optional("LargeOptional", "[ab,0,65535]*", 2,
                                  {"", "a", "b", "aa", "ab", "ba", "bb"},
                                  false);
  BoundedStar star_symbol_intersection("SymbolIntersection", "([ab]&a)*", 2,
                                       {"", "a", "aa"}, false);
  BoundedStar star_scalar_pair("ScalarPair", "[^a,2,2]*", 4,
                               {"", u8"\u0100\u0100",
                                u8"\u0100\u0100\u0100\u0100"},
                               true, 0x100);

  /// Exact union languages, including inactive empty languages and old stars.
  class ExplicitUnion : public Base {
  public:
    enum Group { BOUNDS, COMPOSITION, SCALARS, COMPATIBILITY };

  private:
    Group group;
    typedef std::set<std::string> Words;

    static bool
    collect(UnionSpace& root, Words& words,
            bool inactive_literal_star = false) {
      using namespace Gecode;
      Search::Options options;
      options.threads = 1;
      DFS<UnionSpace> search(&root, options);
      while (UnionSpace* solution = search.next()) {
        std::unique_ptr<UnionSpace> s(solution);
        if (!s->x.assigned() || !s->strings.assigned() ||
            !s->integers.assigned() || !s->booleans.assigned())
          return false;
        words.insert(s->x.val_symbols().to_utf8());
        if (inactive_literal_star) {
          // In (ab)*|c the selector is positive; only the old Star repetition
          // handle can be zero. It must still be reported for the c solution.
          bool zero_repetitions = false;
          for (int i = 0; i < s->integers.size(); ++i)
            zero_repetitions |= s->integers[i].val() == 0;
          if (!zero_repetitions)
            return false;
        }
      }
      return true;
    }

    bool
    check(const std::string& pattern, const Gecode::String::NSBlocks& blocks,
          int min, int max, const Words& expected, bool compare_ordinary = true,
          bool inactive_literal_star = false) {
      using namespace Gecode;
      for (bool explicit_post : {true, false}) {
        if (!explicit_post && !compare_ordinary)
          continue;
        UnionSpace root(blocks, min, max, pattern, explicit_post);
        Words actual;
        if (root.status() != SS_FAILED) {
          // The public text overload has already destroyed its parser AST.
          std::unique_ptr<UnionSpace> clone(
              static_cast<UnionSpace*>(root.clone()));
          if (!collect(*clone, actual, inactive_literal_star && explicit_post))
            return false;
        }
        if (actual != expected) {
          if (opt.log)
            olog << "Explicit union: pattern=" << pattern
                 << " explicit_post=" << explicit_post << std::endl;
          return false;
        }
      }
      return true;
    }

    bool
    assigned_membership(void) {
      using namespace Gecode;
      using namespace Gecode::String;
      const NSBlocks blocks({NSBlock(NSIntSet('a', 'b'), 0, 2)});
      for (const char* pattern : {"a|bb", "bb|a"})
        for (const char* word : {"", "a", "b", "aa", "ab", "ba", "bb"})
          for (bool assign_first : {true, false})
            for (bool explicit_post : {true, false}) {
              const StringVal value = StringVal::decode_utf8(word);
              UnionSpace root(blocks, 0, 2, pattern, explicit_post, &value,
                              assign_first);
              Words actual;
              if (!collect(root, actual))
                return false;
              const Words expected =
                  (value.to_utf8() == "a" || value.to_utf8() == "bb")
                      ? Words({word})
                      : Words();
              if (actual != expected)
                return false;
            }
      return true;
    }

  public:
    ExplicitUnion(const std::string& name, Group group0)
      : Base("String::Extensional::C12::" + name),
        group(group0) {}

    virtual bool
    run(void) {
      using namespace Gecode::String;
      const NSBlocks ab({NSBlock(NSIntSet('a', 'b'), 0, 2)});
      const NSBlocks abc({NSBlock(NSIntSet('a', 'c'), 0, 2)});
      const NSBlocks only_a({NSBlock(NSIntSet('a'), 0, 2)});
      switch (group) {
      case BOUNDS: {
        for (const char* pattern : {"a|bb", "bb|a"})
          if (!check(pattern, ab, 1, 1, {"a"}) ||
              !check(pattern, ab, 2, 2, {"bb"}) ||
              !check(pattern, ab, 0, 2, {"a", "bb"}) ||
              !check(pattern, only_a, 1, 2, {"a"}))
            return false;
        const NSBlocks structured(
            {NSBlock(NSIntSet('a'), 0, 1), NSBlock(NSIntSet('b'), 0, 2)});
        return check("a|bb", structured, 2, 2, {"bb"}) && assigned_membership();
      }
      case COMPOSITION:
        return check("a|[b,1,1]", only_a, 0, 2, {"a"}) &&
               check("[b,1,1]|()", only_a, 0, 0, {""}) &&
               check("[b,1,1]|a", only_a, 0, 0, {}) &&
               check("[b,0,1]|a", only_a, 0, 1, {"", "a"}) &&
               check("()|a", only_a, 0, 1, {"", "a"}) &&
               check("(a&b)|a", only_a, 0, 1, {"a"}) &&
               check("a|((bb|b)c)", only_a, 1, 1, {"a"});
      case SCALARS: {
        NSIntSet alphabet(0);
        alphabet.include(NSIntSet('a'));
        alphabet.include(NSIntSet(0x1F600));
        const NSBlocks scalar({NSBlock(alphabet, 0, 2)});
        return check("\\u{1F600}|aa", scalar, 1, 1, {"\xF0\x9F\x98\x80"}) &&
               check("\\0|aa", scalar, 1, 1, {std::string(1, '\0')});
      }
      case COMPATIBILITY: {
        const NSBlocks only_c({NSBlock(NSIntSet('c'), 0, 1)});
        return check("([ab]&a)|b", ab, 1, 1, {"a", "b"}, false) &&
               check("(([ab]&a)c)|bb", abc, 2, 2, {"ac", "bb"}, false) &&
               check("([ab]&a)*|b", ab, 0, 2, {"", "a", "aa", "b"}, false) &&
               check("[a,1,501]|b", ab, 1, 1, {"a", "b"}, false) &&
               check("(ab)*|c", only_c, 1, 1, {"c"}, true, true);
      }
      }
      GECODE_NEVER;
      return false;
    }
  };

  ExplicitUnion union_bounds("Bounds", ExplicitUnion::BOUNDS);
  ExplicitUnion union_composition("Composition", ExplicitUnion::COMPOSITION);
  ExplicitUnion union_scalars("Scalars", ExplicitUnion::SCALARS);
  ExplicitUnion union_compatibility("Compatibility",
                                    ExplicitUnion::COMPATIBILITY);

  /// A singleton union requires both alternatives to denote the same word.
  class SingletonIntersection : public Base {
  public:
    SingletonIntersection()
      : Base("String::Extensional::SingletonIntersection") {}

    virtual bool
    run(void) {
      using namespace Gecode::String;
      for (const char* pattern : {"a|()", "()|a", "a|b"}) {
        const auto expression = RegExParser(pattern).parse();
        try {
          expression->get_string();
          return false;
        } catch (const std::runtime_error&) {
        }
      }
      const struct {
        const char* pattern;
        const char* word;
      } singletons[] = {
          {"a|a", "a"}, {"()|()", ""}, {"ab|ab", "ab"}, {"[a,2,2]|aa", "aa"}};
      for (const auto& test : singletons)
        if (RegExParser(test.pattern).parse()->get_string() != test.word)
          return false;

      // Ordinary nested posting must diagnose unsupported intersection rather
      // than compile a|epsilon as if it were the singleton a and accept b.
      try {
        StarSpace root(NSBlocks({NSBlock(NSIntSet('b'), 1, 1)}), 1, 1,
                       "((a|())&a)b", false);
        return false;
      } catch (const std::runtime_error& error) {
        if (std::string(error.what()) !=
            "General symbolic REG intersection is not implemented")
          return false;
      }

      const struct {
        const char* pattern;
        std::set<std::string> words;
        bool explicit_post;
      } supported[] = {{"(a&a)b", {"ab"}, false},
                       {"((a|a)&a)b", {"ab"}, false},
                       {"(()&())b", {"b"}, false},
                       {"(a&b)b", {}, false},
                       {"(a|())&a", {"a"}, false},
                       {"([ab]&a)|b", {"a", "b"}, true},
                       {"(([ab]&a)c)|bb", {"ac", "bb"}, true}};
      for (const auto& test : supported) {
        UnionSpace root(NSBlocks({NSBlock(NSIntSet('a', 'c'), 0, 2)}), 0, 2,
                        test.pattern, test.explicit_post);
        Gecode::Search::Options options;
        options.threads = 1;
        Gecode::DFS<UnionSpace> search(&root, options);
        std::set<std::string> actual;
        while (UnionSpace* solution = search.next()) {
          std::unique_ptr<UnionSpace> s(solution);
          if (!s->x.assigned() || !s->strings.assigned() ||
              !s->integers.assigned() || !s->booleans.assigned())
            return false;
          actual.insert(s->x.val_symbols().to_utf8());
        }
        if (actual != test.words) {
          if (opt.log)
            olog << "Singleton intersection: regex=" << test.pattern
                 << " explicit_post=" << test.explicit_post << std::endl;
          return false;
        }
      }
      return true;
    }
  };

  SingletonIntersection singleton_intersection;

  /// Enumerate the complete bounded language over one byte or scalar symbol.
  class EmptyLanguage : public Base {
  private:
    std::string regex;
    std::set<int> lengths;
    bool explicit_post;
    int symbol;

  public:
    EmptyLanguage(const std::string& name, const std::string& pattern,
                  std::set<int> expected, bool both_overloads = false,
                  int character = 'a')
      : Base("String::Extensional::C13::" + name),
        regex(pattern), lengths(expected), explicit_post(both_overloads),
        symbol(character) {}

    virtual bool
    run(void) {
      using Gecode::String::NSBlock;
      using Gecode::String::NSBlocks;
      using Gecode::String::NSIntSet;
      using Gecode::String::StringVal;
      std::set<StringVal> expected;
      for (int length : lengths)
        expected.insert(
            StringVal::from_symbols(std::vector<int>(length, symbol)));
      for (bool decomposed : {false, true}) {
        if (decomposed && !explicit_post)
          continue;
        StarSpace root(NSBlocks({NSBlock(NSIntSet(symbol), 0, 2)}), 0, 2, regex,
                       decomposed);
        Gecode::Search::Options options;
        options.threads = 1;
        Gecode::DFS<StarSpace> search(&root, options);
        std::set<StringVal> actual;
        while (StarSpace* solution = search.next()) {
          std::unique_ptr<StarSpace> s(solution);
          if (!s->x.assigned() || !actual.insert(s->x.val_symbols()).second)
            return false;
        }
        if (actual != expected) {
          if (opt.log)
            olog << "Empty language: regex=" << regex
                 << " decomposed=" << decomposed << std::endl;
          return false;
        }
      }
      return true;
    }
  };

  EmptyLanguage empty_union("Union", "a|[^a]", {1});
  EmptyLanguage empty_class("Class", "[^a]", {});
  EmptyLanguage empty_concat("Concat", "a[^a]", {});
  EmptyLanguage empty_zero("ZeroRepeat", "[^a,0,0]", {0}, true);
  EmptyLanguage empty_optional("OptionalRepeat", "[^a,0,2]", {0}, true);
  EmptyLanguage empty_positive("PositiveRepeat", "[^a,1,2]", {}, true);
  EmptyLanguage empty_star("Star", "([^a])*", {0});
  EmptyLanguage empty_union_star("UnionStar", "(a|[^a])*", {0, 1, 2});
  EmptyLanguage empty_zero_concat("ConcatZeroRepeat", "a[^a,0,0]", {1});
  EmptyLanguage empty_scalar_union("ScalarUnion", "\\u0100|[^\\u0100]", {1},
                                   false, 0x100);
  EmptyLanguage empty_scalar_class("ScalarClass", "[^\\u0100]", {}, false,
                                   0x100);
  EmptyLanguage empty_intersection("SingletonIntersection", "(a&b)|a", {1});

  /// Check compiler contracts also used by Match, without its posting path.
  class EmptyCompilation : public Base {
  public:
    EmptyCompilation()
      : Base("String::Extensional::C13::Compilation") {}

    virtual bool
    run(void) {
      using namespace Gecode::String;
      for (bool active : {false, true}) {
        const NSIntSet alphabet = active ? NSIntSet('a') : NSIntSet();
        for (bool starred : {false, true}) {
          const auto expression =
              RegExParser(starred ? "([^a])*" : "[^a]").parse();
          // Bare, prefix (pattern followed by sigma*), and full context.
          for (int context = 0; context < 3; ++context) {
            TrimDFAHandle dfa(
                expression->symbolic_dfa(alphabet, context == 2, context != 0));
            if (dfa->accepting_states().empty() == starred)
              return false;
            for (int length = 0; length <= 2; ++length) {
              const StringVal word =
                  StringVal::from_symbols(std::vector<int>(length, 'a'));
              const bool expected =
                  starred && (length == 0 || (active && context != 0));
              if (dfa->accepted(word) != expected)
                return false;
            }
          }
        }
      }
      const struct {
        const char* pattern;
        std::set<int> lengths;
      } legacy_cases[] = {{"[b-a]", {}},      {"a|[b-a]", {1}},
                          {"a[b-a]", {}},     {"([b-a])*", {0}},
                          {"[b-a,0,2]", {0}}, {"[b-a,1,2]", {}}};
      for (const auto& test : legacy_cases) {
        const auto expression = RegExParser(test.pattern).parse();
        TrimDFAHandle dfa(new trimDFA(expression->dfa()));
        for (int length = 0; length <= 2; ++length)
          if (dfa->accepted(std::string(length, 'a')) !=
              (test.lengths.count(length) != 0))
            return false;
      }
      // An empty operand must not hide an unsupported intersection.
      for (const char* pattern : {"[^a]((a|b)&a)", "[^a]|((a|b)&a)"}) {
        const auto expression = RegExParser(pattern).parse();
        try {
          TrimDFAHandle dfa(expression->symbolic_dfa(NSIntSet('a')));
          return false;
        } catch (const std::runtime_error& error) {
          if (std::string(error.what()) !=
              "General symbolic REG intersection is not implemented")
            return false;
        }
      }
      return true;
    }
  };

  EmptyCompilation empty_compilation;

  class EmptyReificationSpace : public Gecode::Space {
  public:
    EmptyReificationSpace(const std::string& pattern, int length, int control,
                          Gecode::ReifyMode mode) {
      using namespace Gecode;
      StringVar x(*this, std::string(length, 'a'));
      BoolVar b(*this, control, control);
      extensional(*this, x, pattern, b, mode);
    }

    EmptyReificationSpace(EmptyReificationSpace& s) : Gecode::Space(s) {}

    virtual Gecode::Space*
    copy(void) {
      return new EmptyReificationSpace(*this);
    }
  };

  /// Assigned strings isolate the compiled-language contract from C11.
  class EmptyReification : public Base {
  public:
    EmptyReification()
      : Base("String::Extensional::C13::Reification") {}

    virtual bool
    run(void) {
      using namespace Gecode;
      const char* patterns[] = {"[^a]", "[^a,0,2]", "a|[^a]"};
      for (int pattern = 0; pattern < 3; ++pattern)
        for (int length = 0; length <= 2; ++length) {
          const bool member =
              (pattern == 1 && length == 0) || (pattern == 2 && length == 1);
          for (ReifyMode mode : {RM_EQV, RM_IMP, RM_PMI})
            for (int control = 0; control <= 1; ++control) {
              const bool expected = mode == RM_EQV   ? (bool(control) == member)
                                    : mode == RM_IMP ? (!control || member)
                                                     : (!member || control);
              EmptyReificationSpace s(patterns[pattern], length, control, mode);
              if ((s.status() != SS_FAILED) != expected) {
                if (opt.log)
                  olog << "Empty-language reification: regex="
                       << patterns[pattern] << " length=" << length
                       << " mode=" << mode << " control=" << control
                       << std::endl;
                return false;
              }
            }
        }
      return true;
    }
  };

  EmptyReification empty_reification;

  /// Class literals leave grouping balance to the surrounding expression.
  class ClassParentheses : public Base {
  public:
    ClassParentheses()
      : Base("String::Extensional::ClassParentheses") {}

    virtual bool
    run(void) {
      using namespace Gecode::String;
      const struct {
        const char* pattern;
        int symbol;
      } cases[] = {{"[(]", '('},   {"[)]", ')'},   {"([(])", '('},
                   {"([)])", ')'}, {"[\\(]", '('}, {"[\\)]", ')'},
                   {"\\(", '('},   {"\\)", ')'},   {"(a)", 'a'}};
      for (const auto& test : cases)
        for (int symbol : {'(', ')', 'a'})
          for (bool decomposed : {false, true}) {
            StarSpace root(NSBlocks({NSBlock(NSIntSet(symbol), 1, 1)}), 1, 1,
                           test.pattern, decomposed);
            if ((root.status() != Gecode::SS_FAILED) !=
                (symbol == test.symbol)) {
              if (opt.log)
                olog << "Class parentheses: regex=" << test.pattern
                     << " symbol=" << symbol << " decomposed=" << decomposed
                     << std::endl;
              return false;
            }
          }
      const struct {
        const char* pattern;
        const char* diagnostic;
      } malformed[] = {{"(a", "Unexpected end of regular expression"},
                       {"a)", "Ill-formed expression"}};
      for (const auto& test : malformed) {
        try {
          RegExParser(test.pattern).parse();
          return false;
        } catch (const std::runtime_error& error) {
          if (std::string(error.what()) != test.diagnostic)
            return false;
        }
      }
      return true;
    }
  };

  ClassParentheses class_parentheses;

  class Regex : public Test {
  public:
    Regex(void) : Test("Extensional::Regex", 1, "ab", 3, true) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string& value = assignment[0];
      if (value.empty() || (value[value.size() - 1] != 'b'))
        return false;
      for (std::string::size_type i = 0; i + 1 < value.size(); ++i)
        if (value[i] != 'a')
          return false;
      return true;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::extensional(home, x[0], "a*b");
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x,
                      Gecode::Reify r) {
      Gecode::extensional(home, x[0], "a*b", r.var(), r.mode());
    }
  };

  class FiniteLanguage : public Test {
  public:
    FiniteLanguage(void) : Test("Extensional::FiniteLanguage", 1, "ab", 2) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string& value = assignment[0];
      return (value == "") || (value == "a") || (value == "bb");
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::extensional(home, x[0],
        std::vector<std::string>{"", "a", "bb"});
    }
  };

  Regex regex;
  FiniteLanguage finite_language;

  class CollectionSpace : public Gecode::Space {
  public:
    CollectionSpace(int overload, bool empty, bool member, bool failed) {
      using namespace Gecode;
      using Gecode::String::StringVal;
      const std::string bytes("\xC4\x80");
      const StringVal value = overload == 2 ? StringVal::from_bytes(bytes)
                                            : StringVal::from_symbols({0x100});
      StringVar x(*this, member ? value : StringVal::from_symbols({'b'}));
      StringVarArgs variables;
      std::vector<StringVal> values;
      std::vector<std::string> strings;
      if (!empty) {
        variables << StringVar(*this, value);
        values.push_back(value);
        strings.push_back(bytes);
      }
      if (failed)
        fail();
      switch (overload) {
      case 0:
        extensional(*this, x, variables);
        break;
      case 1:
        extensional(*this, x, values);
        break;
      case 2:
        extensional(*this, x, strings);
        break;
      }
    }

    CollectionSpace(CollectionSpace& s) : Gecode::Space(s) {}

    virtual Gecode::Space*
    copy(void) {
      return new CollectionSpace(*this);
    }
  };

  // Empty membership fails normally; singleton membership keeps scalar/byte
  // semantics, and every collection overload tolerates an already failed space.
  class Collections : public Base {
  public:
    Collections()
      : Base("String::Extensional::Collections") {}

    virtual bool
    run(void) {
      for (int overload = 0; overload < 3; ++overload)
        for (bool empty : {true, false})
          for (bool member : {true, false})
            for (bool failed : {false, true}) {
              CollectionSpace s(overload, empty, member, failed);
              const bool expected = !empty && member && !failed;
              if ((s.status() != Gecode::SS_FAILED) != expected)
                return false;
            }
      return true;
    }
  };

  Collections collections;

}}}

// STATISTICS: test-string
