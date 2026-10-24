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

#include <memory>
#include <set>
#include <vector>

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

  /// Space for assigned scalar-value checks
  class ScalarValueSpace : public Gecode::Space {
  public:
    ScalarValueSpace(void) {}
    ScalarValueSpace(ScalarValueSpace& s) : Gecode::Space(s) {}
    virtual Gecode::Space* copy(void) {
      return new ScalarValueSpace(*this);
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
      ScalarValueSpace home;
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
                         : "String::Rel::ScalarIncreasing"),
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
  LexExtrema lex_extrema;
  ScalarOrder scalar_increasing(false);
  ScalarOrder scalar_decreasing(true);

}}}

// STATISTICS: test-string
