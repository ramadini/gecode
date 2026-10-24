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

#include <cmath>
#include <memory>
#include <set>

namespace Test { namespace String { namespace Rel {

  /// Test an increasing or decreasing string relation
  class Unary : public Test {
  private:
    Gecode::StringRelType relation;
  public:
    Unary(Gecode::StringRelType relation0)
      : Test("Rel::" + str(relation0), 1, "ac", 3),
        relation(relation0) {}

    virtual bool solution(const Assignment& a) const {
      const std::string& x = a[0];
      const bool increasing =
        (relation == Gecode::STRT_INCLT) ||
        (relation == Gecode::STRT_INCLQ);
      const bool strict =
        (relation == Gecode::STRT_INCLT) ||
        (relation == Gecode::STRT_DECLT);
      for (std::string::size_type i = 1; i < x.size(); ++i) {
        if (strict && (x[i - 1] == x[i]))
          return false;
        if (increasing ? (x[i - 1] > x[i]) : (x[i - 1] < x[i]))
          return false;
      }
      return true;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::rel(home, x[0], relation);
    }
  };

  class Domain : public Test {
  public:
    Domain(void) : Test("Rel::Domain", 1, "ab", 2) {}

    virtual bool solution(const Assignment& assignment) const {
      return assignment[0].empty() || (assignment[0] == "a");
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::String::NSBlocks domain;
      domain.push_back(Gecode::String::NSBlock(
        Gecode::String::NSIntSet('a'), 0, 1));
      Gecode::rel(home, x[0], Gecode::STRT_DOM, domain, 0, 1);
    }
  };

  /// Space exposing a lexicographic upper-bound regression
  class LexExtremaSpace : public Gecode::Space {
  public:
    Gecode::StringVar x;
    Gecode::StringVar y;

    LexExtremaSpace(void) : x(*this, "bd"), y(*this) {
      Gecode::String::NSIntSet b('b');
      Gecode::String::NSIntSet ac('a');
      ac.add('c');
      std::vector<Gecode::String::NSBlock> blocks;
      blocks.push_back(Gecode::String::NSBlock(b, 0, 1));
      blocks.push_back(Gecode::String::NSBlock(ac, 1, 1));
      Gecode::String::NSBlocks domain(std::move(blocks));
      Gecode::rel(*this, y, Gecode::STRT_DOM, domain, 1, 2);
      Gecode::rel(*this, x, Gecode::STRT_LEXLT, y);
    }

    LexExtremaSpace(LexExtremaSpace& s) : Gecode::Space(s) {
      x.update(*this, s.x);
      y.update(*this, s.y);
    }

    virtual Gecode::Space* copy(void) {
      return new LexExtremaSpace(*this);
    }
  };

  /// Check lexicographic extrema across optional heterogeneous blocks
  class LexExtrema : public Base {
  public:
    LexExtrema(void) : Base("String::Rel::LexExtrema") {}

    virtual bool run(void) {
      LexExtremaSpace s;
      if (s.status() == Gecode::SS_FAILED)
        return false;
      Gecode::StringVar value(s, "c");
      Gecode::rel(s, s.y, Gecode::STRT_EQ, value);
      return (s.status() != Gecode::SS_FAILED) &&
        s.y.assigned() && (s.y.val() == "c");
    }
  };

  /// Test a binary string relation, with optional aliasing
  class Binary : public Test {
  private:
    Gecode::StringRelType relation;
    bool shared;
  public:
    Binary(Gecode::StringRelType relation0, bool shared0)
      : Test("Rel::" + str(relation0) + (shared0 ? "::XX" : "::XY"),
             shared0 ? 1 : 2, "ac", 2, true),
        relation(relation0), shared(shared0) {}

    virtual bool solution(const Assignment& a) const {
      const std::string& x = a[0];
      const std::string& y = a[shared ? 0 : 1];
      switch (relation) {
      case Gecode::STRT_EQ:    return x == y;
      case Gecode::STRT_NQ:    return x != y;
      case Gecode::STRT_LEXLT: return x < y;
      case Gecode::STRT_LEXLQ: return x <= y;
      default:                 return false;
      }
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::rel(home, x[0], relation, x[shared ? 0 : 1]);
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x,
                      Gecode::Reify r) {
      Gecode::rel(home, x[0], relation, x[shared ? 0 : 1], r);
    }
  };

  /// Space for direct domain and public posting checks
  class ContractSpace : public Gecode::Space {
  public:
    ContractSpace(void) {}
    ContractSpace(ContractSpace& s) : Gecode::Space(s) {}
    virtual Gecode::Space* copy(void) {
      return new ContractSpace(*this);
    }
  };

  /// Enumerate an open scalar domain after posting a monotone relation
  class ScalarOrderSpace : public Gecode::Space {
  public:
    Gecode::StringVarArray x;

    ScalarOrderSpace(int first, int last, bool strict, bool decreasing)
      : x(*this, 1) {
      x[0] = Gecode::StringVar(
        *this, Gecode::String::NSIntSet(first, last), 0, 2);
      Gecode::rel(*this, x[0], decreasing
        ? (strict ? Gecode::STRT_DECLT : Gecode::STRT_DECLQ)
        : (strict ? Gecode::STRT_INCLT : Gecode::STRT_INCLQ));
      Gecode::branch(*this, Gecode::StringVarArgs(x),
                     Gecode::STRING_VAR_NONE(), Gecode::STRING_VAL_LLLL());
    }

    ScalarOrderSpace(ScalarOrderSpace& s) : Gecode::Space(s) {
      x.update(*this, s.x);
    }

    virtual Gecode::Space* copy(void) {
      return new ScalarOrderSpace(*this);
    }
  };

  /// Compare assigned and searched values with scalar-vector ordering
  class ScalarOrder : public Base {
  private:
    typedef std::vector<Gecode::String::StringSymbol> Word;
    typedef std::set<Word> Words;

    bool decreasing;

    static bool ordered(const Word& word, bool strict, bool decreasing) {
      for (Word::size_type i = 1; i < word.size(); ++i)
        if ((strict && word[i - 1] == word[i]) ||
            (decreasing ? word[i - 1] < word[i] : word[i - 1] > word[i]))
          return false;
      return true;
    }

    static bool assigned_matches(const Word& word, bool strict,
                                 bool decreasing) {
      ContractSpace home;
      Gecode::StringVar x(
        home, Gecode::String::StringVal::from_symbols(word));
      Gecode::rel(home, x, decreasing
        ? (strict ? Gecode::STRT_DECLT : Gecode::STRT_DECLQ)
        : (strict ? Gecode::STRT_INCLT : Gecode::STRT_INCLQ));
      return (home.status() != Gecode::SS_FAILED) ==
        ordered(word, strict, decreasing);
    }

    static bool search_matches(int first, int last, bool strict,
                               bool decreasing, const Words& expected) {
      ScalarOrderSpace root(first, last, strict, decreasing);
      if (root.status() == Gecode::SS_FAILED)
        return false;
      Gecode::Search::Options options;
      options.threads = 1;
      Gecode::DFS<ScalarOrderSpace> search(&root, options);
      Words actual;
      while (ScalarOrderSpace* solution = search.next()) {
        std::unique_ptr<ScalarOrderSpace> s(solution);
        if (!s->x[0].assigned())
          return false;
        const Gecode::String::StringVal value = s->x[0].val_symbols();
        const Word word(value.begin(), value.end());
        if (!actual.insert(word).second)
          return false;
      }
      return actual == expected;
    }

  public:
    ScalarOrder(bool decreasing0)
      : Base(decreasing0 ? "String::Rel::ScalarDecreasing"
                         : "String::Rel::ScalarIncreasing", TestTags::check()),
        decreasing(decreasing0) {}

    virtual bool run(void) {
      for (int first : {0x7F, 0x100, 0x10FFFE}) {
        std::vector<Word> candidates(1);
        for (int a = first; a <= first + 1; ++a) {
          candidates.push_back(Word{a});
          for (int b = first; b <= first + 1; ++b)
            candidates.push_back(Word{a, b});
        }
        for (bool strict : {false, true}) {
          Words expected;
          for (const Word& word : candidates) {
            if (!assigned_matches(word, strict, decreasing))
              return false;
            if (ordered(word, strict, decreasing))
              expected.insert(word);
          }
          if (!search_matches(first, first + 1, strict, decreasing, expected))
            return false;
        }
      }
      return true;
    }
  };

  /// Two optional runs with a separate aggregate length restriction
  Gecode::String::NSBlocks
  optional_runs(int first='a', int second='b') {
    Gecode::String::NSBlocks blocks;
    blocks.emplace_back(Gecode::String::NSIntSet(first), 0, 2);
    blocks.emplace_back(Gecode::String::NSIntSet(second), 0, 2);
    return blocks;
  }

  /// Check public and low-level construction before normalization
  class Construction : public Base {
  private:
    template<class Action>
    static bool rejects(bool limits, const Action& construct) {
      try {
        construct();
      } catch (const Gecode::String::OutOfLimitsDS&) {
        return limits;
      } catch (const Gecode::String::EmptyDS&) {
        return !limits;
      }
      return false;
    }

    static bool empty(const Gecode::StringVar& x) {
      return x.assigned() && x.val().empty() &&
        x.min_length() == 0 && x.max_length() == 0 &&
        x.domain().is_normalized();
    }

  public:
    Construction(void) : Base("String::Rel::Construction", TestTags::check()) {}

    virtual bool run(void) {
      using namespace Gecode;
      const int limit = Gecode::String::DashedString::_MAX_STR_LENGTH;
      const int bounds[][2] = {
        {2, 1}, {-1, 1}, {0, -1}, {limit + 1, limit + 1}, {0, limit + 1}
      };
      const Gecode::String::NSIntSet alphabet('a');
      for (const auto& bound : bounds) {
        const int l = bound[0], u = bound[1];
        const bool limits = l < 0 || u < 0 || l > limit || u > limit;
        ContractSpace home;
        Gecode::String::NSBlocks blocks = optional_runs();
        Gecode::String::NSBlocks malformed(1);
        malformed[0].S = alphabet;
        malformed[0].l = l;
        malformed[0].u = u;
        Gecode::String::DSBlock block;
        block.S.init(home, alphabet);
        block.l = l;
        block.u = u;
        if (!rejects(limits, [&] { StringVar x(home, l, u); }) ||
            !rejects(limits, [&] { StringVar x(home, alphabet, l, u); }) ||
            !rejects(limits, [&] {
              StringVar x(home, l, u, Gecode::String::StringAlphabet::ascii());
            }) ||
            !rejects(limits, [&] { StringVar x(home, blocks, l, u); }) ||
            !rejects(limits, [&] { StringVar x(home, malformed, 0, limit); }) ||
            !rejects(limits, [&] {
              Gecode::String::DashedString x(home, block);
            }))
          return false;
      }
      ContractSpace home;
      Gecode::String::NSBlocks no_blocks;
      Gecode::String::NSBlocks blocks = optional_runs();
      if (!rejects(false, [&] { StringVar x(home, no_blocks, 0, 0); }) ||
          !rejects(false, [&] { StringVar x(home, blocks, 5, 5); }))
        return false;
      Gecode::String::NSBlocks mandatory(1);
      mandatory[0].l = mandatory[0].u = 1;
      if (!rejects(false, [&] { StringVar x(home, mandatory, 0, limit); }) ||
          !rejects(false, [&] {
            StringVar x(home, Gecode::String::NSIntSet(), 1, 2);
          }))
        return false;
      Gecode::String::DSBlock mandatory_block;
      mandatory_block.l = mandatory_block.u = 1;
      if (!rejects(false, [&] {
            Gecode::String::DashedString x(home, mandatory_block);
          }))
        return false;
      for (int second : {'a', 'b'}) {
        Gecode::String::NSBlocks too_long;
        too_long.emplace_back(alphabet, limit, limit);
        too_long.emplace_back(Gecode::String::NSIntSet(second), 1, 1);
        if (!rejects(false, [&] { StringVar x(home, too_long, 0, limit); }))
          return false;
      }
      const std::string oversized(limit + 1, 'a');
      if (!rejects(true, [&] { StringVar x(home, oversized); }) ||
          !rejects(true, [&] {
            StringVar x(home, Gecode::String::StringVal::from_bytes(oversized));
          }))
        return false;
      StringVar zero(home, 0, 0);
      StringVar no_symbols(home, Gecode::String::NSIntSet(), 0, 2);
      StringVar value(home, std::string());
      StringVar scalar(home, Gecode::String::StringVal());
      StringVar aggregate_zero(home, blocks, 0, 0);
      Gecode::String::NSBlocks optional_empty(1);
      optional_empty[0].u = 2;
      StringVar block_zero(home, optional_empty, 0, 2);
      if (!empty(zero) || !empty(no_symbols) || !empty(value) ||
          !empty(scalar) || !empty(aggregate_zero) || !empty(block_zero))
        return false;
      Gecode::String::NSBlocks one_block;
      one_block.emplace_back(alphabet, 0, 3);
      StringVar narrowed(home, one_block, 1, 2);
      StringVar boundary(home, alphabet, limit, limit);
      StringVar boundary_value(home, std::string(limit, 'a'));
      return narrowed.min_length() == 1 && narrowed.max_length() == 2 &&
        narrowed.domain().at(0).l == 1 && narrowed.domain().at(0).u == 2 &&
        narrowed.domain().is_normalized() && boundary.assigned() &&
        boundary.max_length() == limit && boundary.domain().is_normalized() &&
        boundary_value.assigned() && boundary_value.max_length() == limit;
    }
  };

  /// Keep aggregate restrictions on direct tells and concrete operations
  class AggregateBounds : public Base {
  private:
    static Gecode::ModEvent tell(Gecode::Space& home, Gecode::StringVar x,
                                const std::string& value, int route);
    static Gecode::ModEvent tell(Gecode::Space& home, Gecode::StringVar x,
                                const Gecode::String::NSBlocks& blocks);
    static Gecode::ModEvent reverse(Gecode::Space& home, Gecode::StringVar x,
                                   Gecode::StringVar y);
    static Gecode::ModEvent concat(Gecode::Space& home,
                                  Gecode::StringVar result,
                                  Gecode::StringVar x, Gecode::StringVar y);

  public:
    AggregateBounds(void)
      : Base("String::Rel::AggregateBounds", TestTags::check()) {}

    virtual bool run(void) {
      using namespace Gecode;
      for (int route = 0; route < 3; ++route) {
        {
          ContractSpace home;
          Gecode::String::NSBlocks blocks = optional_runs();
          StringVar x(home, blocks, 2, 2);
          if (!me_failed(tell(home, x, "a", route)) || x.assigned() ||
              x.min_length() != 2 || x.max_length() != 2 ||
              !x.domain().equals(blocks) || !x.domain().is_normalized())
            return false;
        }
        for (const std::string& valid : {"aa", "ab", "bb"}) {
          ContractSpace home;
          Gecode::String::NSBlocks blocks = optional_runs();
          StringVar x(home, blocks, 2, 2);
          if (me_failed(tell(home, x, valid, route)) ||
              !x.assigned() || x.val() != valid)
            return false;
        }
      }
      for (bool reject : {false, true}) {
        ContractSpace home;
        Gecode::String::NSBlocks blocks = optional_runs(0x100, 0x101);
        StringVar x(home, blocks, 2, 2);
        Gecode::String::NSBlocks value;
        value.emplace_back(Gecode::String::NSIntSet(0x100), 1, 1);
        if (!reject)
          value.emplace_back(Gecode::String::NSIntSet(0x101), 1, 1);
        if (me_failed(tell(home, x, value)) != reject)
          return false;
        if (reject) {
          if (x.min_length() != 2 || x.max_length() != 2 ||
              !x.domain().equals(blocks))
            return false;
        } else if (!x.assigned() || x.val_symbols() !=
                   Gecode::String::StringVal::from_symbols({0x100, 0x101})) {
          return false;
        }
      }
      for (bool reject : {false, true}) {
        ContractSpace home;
        Gecode::String::NSBlocks blocks = optional_runs();
        StringVar x(home, blocks, 2, 2);
        Gecode::String::NSBlocks replacement = optional_runs();
        replacement[0].u = 1;
        if (me_failed(tell(home, x, replacement)) ||
            x.min_length() != 2 || x.max_length() != 2 ||
            !x.domain().is_normalized())
          return false;
        const std::string value = reject ? "a" : "ab";
        if (me_failed(tell(home, x, value, 0)) != reject)
          return false;
        if (!reject && (!x.assigned() || x.val() != value))
          return false;
      }
      for (bool direct : {false, true}) {
        for (int operation = 0; operation < 5; ++operation) {
          for (const std::string& value : {"a", "ab"}) {
            ContractSpace home;
            Gecode::String::NSBlocks blocks = optional_runs();
            StringVar x(home, blocks, 2, 2);
            const bool is_reverse = operation == 1 || operation == 2;
            StringVar concrete(home, is_reverse && value == "ab" ? "ba" : value);
            ModEvent event = Gecode::String::ME_STRING_NONE;
            if (operation == 0) {
              if (direct)
                event = tell(home, x, value, 0);
              else
                rel(home, x, STRT_EQ, concrete);
            } else if (operation == 1) {
              if (direct)
                event = reverse(home, concrete, x);
              else
                rel(home, x, STRT_REV, concrete);
            } else if (operation == 2) {
              if (direct)
                event = reverse(home, x, concrete);
              else
                rel(home, concrete, STRT_REV, x);
            } else if (operation == 3) {
              StringVar prefix(home, "c"), result(home, "c" + value);
              if (direct)
                event = concat(home, result, prefix, x);
              else
                rel(home, prefix, x, STRT_CAT, result);
            } else {
              StringVar suffix(home, "c"), result(home, value + "c");
              if (direct)
                event = concat(home, result, x, suffix);
              else
                rel(home, x, suffix, STRT_CAT, result);
            }
            const bool failed = direct ? me_failed(event) :
              home.status() == SS_FAILED;
            if (failed != (value.size() == 1))
              return false;
            if (direct && failed && (x.min_length() != 2 ||
                x.max_length() != 2 || !x.domain().equals(blocks)))
              return false;
            if (!failed && (!x.assigned() || x.val() != value))
              return false;
          }
        }
      }
      return true;
    }
  };

  // Keep each direct operation's forced-inline implementation in its own frame.
  Gecode::ModEvent
  AggregateBounds::tell(Gecode::Space& home, Gecode::StringVar x,
                        const std::string& value, int route) {
    if (route == 0)
      return x.varimp()->eq(home, value);
    if (route == 1)
      return x.varimp()->eq(home,
        Gecode::String::StringVal::from_bytes(value));
    const Gecode::String::NSBlocks blocks(value);
    return tell(home, x, blocks);
  }

  Gecode::ModEvent
  AggregateBounds::tell(Gecode::Space& home, Gecode::StringVar x,
                        const Gecode::String::NSBlocks& blocks) {
    return x.varimp()->dom(home, blocks);
  }

  Gecode::ModEvent
  AggregateBounds::reverse(Gecode::Space& home, Gecode::StringVar x,
                           Gecode::StringVar y) {
    return x.varimp()->rev(home, y.varimp());
  }

  Gecode::ModEvent
  AggregateBounds::concat(Gecode::Space& home, Gecode::StringVar result,
                          Gecode::StringVar x, Gecode::StringVar y) {
    return result.varimp()->concat(home, x.varimp(), y.varimp());
  }

  /// Compare the block-product estimate with tiny, unambiguous languages
  class SizeEstimate : public Base {
  private:
    static bool close(double actual, unsigned int expected) {
      return std::abs(actual - expected) <= 1e-10 * (expected + 1);
    }

  public:
    SizeEstimate(void) : Base("String::Rel::SizeEstimate", TestTags::check()) {}

    virtual bool run(void) {
      using namespace Gecode;
      ContractSpace home;
      StringVar empty(home, "");
      StringVar assigned(home,
        Gecode::String::StringVal::from_symbols({0x100, 0x101}));
      StringVar single(home, Gecode::String::NSIntSet('a', 'b'), 0, 2);
      Gecode::String::NSBlocks blocks;
      blocks.emplace_back(Gecode::String::NSIntSet('a', 'b'), 0, 1);
      blocks.emplace_back(Gecode::String::NSIntSet('c'), 0, 2);
      StringVar multiple(home, blocks, 0, 3);
      unsigned int single_count = 0, multiple_count = 0;
      for (Assignment a(1, "abc", 3); a(); ++a) {
        const std::string& value = a[0];
        if (value.size() <= 2 && value.find('c') == std::string::npos)
          ++single_count;
        const std::string::size_type prefix = value.find_first_of('c');
        const std::string::size_type run =
          prefix == std::string::npos ? value.size() : prefix;
        if (run <= 1 && value.size() - run <= 2 &&
            value.find_first_not_of('c', run) == std::string::npos)
          ++multiple_count;
      }
      return close(empty.size(), 1) && close(assigned.size(), 1) &&
        close(single.size(), single_count) && single_count == 7 &&
        close(multiple.size(), multiple_count) && multiple_count == 9;
    }
  };

  /// Check ownership and range counts across ordinary set copies and moves
  class NSIntSetCopy : public Base {
  private:
    static bool expected(const Gecode::String::NSIntSet& symbols) {
      return symbols.consistent() && symbols.size() == 6 &&
        symbols.length() == 3 && symbols.toString() == "{1..2,5,9..11}";
    }

  public:
    NSIntSetCopy(void) : Base("String::Rel::NSIntSetCopy", TestTags::check()) {}

    virtual bool run(void) {
      using Gecode::String::NSIntSet;
      NSIntSet source(1,2);
      source.add(5);
      source.include(NSIntSet(9,11));
      {
        NSIntSet copy(source);
        NSIntSet assigned(20);
        assigned = copy;
        NSIntSet& alias = copy;
        copy = alias;
        if (!expected(source) || !expected(copy) || !expected(assigned) ||
            source.first() == copy.first() || copy.first() == assigned.first())
          return false;
        NSIntSet moved(std::move(copy));
        if (!expected(moved) || !copy.empty() || !copy.consistent())
          return false;
        assigned = std::move(moved);
        if (!expected(assigned) || !moved.empty() || !moved.consistent())
          return false;
        assigned.add(20);
        if (!expected(source) || !assigned.consistent() || assigned.size() != 7)
          return false;
        NSIntSet empty;
        assigned = empty;
        if (!assigned.empty() || !assigned.consistent())
          return false;
        NSIntSet empty_copy(empty);
        if (!empty_copy.empty() || !empty_copy.consistent())
          return false;
      }
      return expected(source);
    }
  };

  Binary eq_xy(Gecode::STRT_EQ, false);
  Binary nq_xy(Gecode::STRT_NQ, false);
  Binary lt_xy(Gecode::STRT_LEXLT, false);
  Binary lq_xy(Gecode::STRT_LEXLQ, false);
  Binary eq_xx(Gecode::STRT_EQ, true);
  Binary nq_xx(Gecode::STRT_NQ, true);
  Binary lt_xx(Gecode::STRT_LEXLT, true);
  Binary lq_xx(Gecode::STRT_LEXLQ, true);
  Unary inc_lt(Gecode::STRT_INCLT);
  Unary inc_lq(Gecode::STRT_INCLQ);
  Unary dec_lt(Gecode::STRT_DECLT);
  Unary dec_lq(Gecode::STRT_DECLQ);
  Domain domain;
  NSIntSetCopy ns_int_set_copy;
  LexExtrema lex_extrema;
  Construction construction;
  AggregateBounds aggregate_bounds;
  SizeEstimate size_estimate;
  ScalarOrder scalar_increasing(false);
  ScalarOrder scalar_decreasing(true);

}}}

// STATISTICS: test-string
