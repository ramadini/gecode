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

#include <algorithm>
#include <memory>
#include <set>

namespace Test { namespace String { namespace GCC {

  class Counts : public Test {
  private:
    int a;
    int b;

    static std::string name(int a0, int b0) {
      return "Counts::" + std::to_string(a0) + "A" +
        std::to_string(b0) + "B";
    }

  public:
    Counts(int a0, int b0)
      : Test("GCC::" + name(a0, b0), 1, "abc", 3), a(a0), b(b0) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string& value = assignment[0];
      return static_cast<int>(
        std::count(value.begin(), value.end(), 'a')) == a &&
        static_cast<int>(
          std::count(value.begin(), value.end(), 'b')) == b;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::IntArgs characters;
      characters << 'a' << 'b';
      Gecode::IntVarArgs counts;
      counts << Gecode::IntVar(home, a, a) << Gecode::IntVar(home, b, b);
      Gecode::gcc(home, x[0], characters, counts);
    }
  };

  Counts no_a_or_b(0, 0);
  Counts one_a(1, 0);

  /// Space for direct GCC posting and structured-domain search
  class GCCSpace : public Gecode::Space {
  public:
    Gecode::StringVar x;

    GCCSpace(void) : x(*this) {}
    GCCSpace(GCCSpace& s) : Gecode::Space(s) { x.update(*this, s.x); }
    virtual Gecode::Space*
    copy(void) {
      return new GCCSpace(*this);
    }
  };

  /// A singleton block contributes only its own covered symbol
  class StructuredCounts : public Base {
  private:
    int offset;

  public:
    StructuredCounts(bool scalar)
      : Base(scalar ? "String::GCC::StructuredCounts::Scalars"
                    : "String::GCC::StructuredCounts::Bytes"),
        offset(scalar ? 0x100 : 0) {}

    virtual bool
    run(void) {
      using Gecode::String::StringVal;
      const StringVal expected =
          StringVal::from_symbols({'a' + offset, 'b' + offset});
      for (bool assigned : {false, true}) {
        GCCSpace root;
        if (assigned) {
          root.x = Gecode::StringVar(root, expected);
        } else {
          Gecode::String::NSBlocks blocks;
          blocks.emplace_back(Gecode::String::NSIntSet('a' + offset), 1, 2);
          blocks.emplace_back(Gecode::String::NSIntSet('b' + offset), 1, 2);
          root.x = Gecode::StringVar(root, blocks, 2, 4);
        }
        Gecode::IntArgs cover;
        cover << 'a' + offset << 'b' + offset;
        Gecode::IntVarArgs counts;
        counts << Gecode::IntVar(root, 1, 1) << Gecode::IntVar(root, 1, 1);
        Gecode::gcc(root, root.x, cover, counts);
        Gecode::branch(root, Gecode::StringVarArgs() << root.x,
                       Gecode::STRING_VAR_NONE(), Gecode::STRING_VAL_LLLL());
        Gecode::Search::Options options;
        options.threads = 1;
        Gecode::DFS<GCCSpace> search(&root, options);
        std::set<StringVal> actual;
        while (GCCSpace* solution = search.next()) {
          std::unique_ptr<GCCSpace> s(solution);
          if (!s->x.assigned() || !actual.insert(s->x.val_symbols()).second)
            return false;
        }
        if (actual != std::set<StringVal>{expected})
          return false;
      }
      return true;
    }
  };

  StructuredCounts structured_bytes(false);
  StructuredCounts structured_scalars(true);

  /// Duplicate cover entries constrain the same occurrence quantity
  class DuplicateCounts : public Test {
  private:
    bool conflicting;

  public:
    DuplicateCounts(bool conflicting0)
      : Test(conflicting0 ? "GCC::DuplicateCounts::Conflicting"
                          : "GCC::DuplicateCounts::Consistent",
             1, "ab", 1, false),
        conflicting(conflicting0) {}

    virtual bool
    solution(const Assignment& assignment) const {
      return !conflicting && assignment[0] == "a";
    }

    virtual void
    post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::IntArgs cover;
      cover << 'a' << 'a';
      Gecode::IntVarArgs counts;
      const int second = conflicting ? 0 : 1;
      counts << Gecode::IntVar(home, 1, 1)
             << Gecode::IntVar(home, second, second);
      Gecode::gcc(home, x[0], cover, counts);
    }
  };

  DuplicateCounts duplicate_consistent(false);
  DuplicateCounts duplicate_conflicting(true);

  /// Duplicate counts stay equal when constrained after GCC posting
  class DuplicateCountEquality : public Base {
  public:
    DuplicateCountEquality(void)
      : Base("String::GCC::DuplicateCounts::Equality") {}

    virtual bool
    run(void) {
      GCCSpace home;
      home.x =
          Gecode::StringVar(home, Gecode::String::NSIntSet('a', 'b'), 2, 2);
      Gecode::IntArgs cover;
      cover << 'a' << 'a';
      Gecode::IntVarArgs counts;
      counts << Gecode::IntVar(home, 0, 2) << Gecode::IntVar(home, 0, 2);
      Gecode::gcc(home, home.x, cover, counts);
      Gecode::rel(home, counts[1], Gecode::IRT_EQ, 1);
      return home.status() != Gecode::SS_FAILED && counts[0].assigned() &&
             counts[0].val() == 1 && counts[1].assigned() &&
             counts[1].val() == 1;
    }
  };

  DuplicateCountEquality duplicate_equality;

  /// Reject both malformed shapes before inspecting cover or count entries
  class ShapeMismatch : public Base {
  private:
    bool counts_longer;

  public:
    ShapeMismatch(bool counts_longer0)
      : Base(counts_longer0 ? "String::GCC::Shape::CountsLonger"
                            : "String::GCC::Shape::CountsShorter"),
        counts_longer(counts_longer0) {}

    virtual bool
    run(void) {
      GCCSpace home;
      home.x = Gecode::StringVar(home, "a");
      Gecode::IntArgs cover;
      cover << 'a' << 'b';
      Gecode::IntVarArgs counts;
      for (int i = 0; i < (counts_longer ? 3 : 1); ++i)
        counts << Gecode::IntVar(home, 1, 1);
      try {
        Gecode::gcc(home, home.x, cover, counts);
      } catch (const Gecode::Int::ArgumentSizeMismatch&) {
        return true;
      }
      return false;
    }
  };

  ShapeMismatch counts_shorter(false);
  ShapeMismatch counts_longer(true);

  class MustChars : public Test {
  public:
    MustChars(void) : Test("GCC::MustChars", 1, "abc", 3) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string& value = assignment[0];
      return value.find('a') != std::string::npos;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::String::NSIntSet characters('a');
      Gecode::must_chars(home, x[0], characters);
    }
  };

  MustChars must_chars;

  class EmptyMustChars : public Test {
  public:
    EmptyMustChars(void) : Test("GCC::MustChars::Empty", 1, "abc", 3) {}

    virtual bool solution(const Assignment&) const {
      return true;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::String::NSIntSet characters;
      Gecode::must_chars(home, x[0], characters);
    }
  };

  EmptyMustChars empty_must_chars;

}}}

// STATISTICS: test-string
