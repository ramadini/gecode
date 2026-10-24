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

#include <gecode/string/branch.hh>

#include <memory>
#include <set>
#include <vector>

namespace Test { namespace String { namespace Branch {

  using namespace Gecode;
  using Gecode::String::NSBlocks;
  using Gecode::String::NSIntSet;
  using Gecode::String::StringSymbol;
  using Gecode::String::StringVal;

  typedef std::vector<StringSymbol> Word;
  typedef std::set<Word> Words;

  struct Selector {
    StringVarBranch vars;
    StringValBranch vals;
  };

  const Selector selectors[] = {
    {STRING_VAR_NONE(), STRING_VAL_LLLL()},
    {STRING_VAR_LENMIN(), STRING_VAL_LLLL()},
    {STRING_VAR_LENMAX(), STRING_VAL_LLLL()},
    {STRING_VAR_SIZEMIN(), STRING_VAL_LLUL()},
    {STRING_VAR_BLOCKMIN(), STRING_VAL_LLLL()},
    {STRING_VAR_BLOCKMIN(), STRING_VAL_LLLM()},
    {STRING_VAR_LENBLOCKMIN(), STRING_VAL_LLLM()}
  };

  /// Keep saved choices compatible while changing only the selected domain
  class ReplaySpace : public Space {
  public:
    StringVarArray x;

    ReplaySpace(NSBlocks domain, int lower, int upper, int selector,
                int mandatory_symbol = -1)
      : x(*this, mandatory_symbol < 0 ? 1 : 2) {
      x[0] = StringVar(*this, domain, lower, upper);
      if (mandatory_symbol >= 0)
        x[1] = StringVar(*this, StringVal::from_symbols({mandatory_symbol}));
      if (selector >= 0)
        branch(*this, StringVarArgs(x), selectors[selector].vars,
               selectors[selector].vals);
    }

    ReplaySpace(ReplaySpace& s) : Space(s) {
      x.update(*this, s.x);
    }

    virtual Space* copy(void) {
      return new ReplaySpace(*this);
    }
  };

  /// Compare scalar solutions and reject duplicate projections
  static bool
  solutions_match(ReplaySpace& root, const Words& expected,
                  unsigned int c_d = 1, unsigned int a_d = 1) {
    Search::Options options;
    options.threads = 1;
    options.c_d = c_d;
    options.a_d = a_d;
    DFS<ReplaySpace> search(&root, options);
    Words actual;
    while (ReplaySpace* solution = search.next()) {
      std::unique_ptr<ReplaySpace> s(solution);
      if (!s->x[0].assigned())
        return false;
      const StringVal value = s->x[0].val_symbols();
      Word word;
      for (StringVal::size_type i = 0; i < value.size(); ++i)
        word.push_back(value[i]);
      if (!actual.insert(word).second) {
        olog << "Duplicate String branch solution" << std::endl;
        return false;
      }
    }
    if (actual != expected) {
      olog << "Wrong String branch solution set: expected " << expected.size()
           << ", got " << actual.size() << std::endl;
      return false;
    }
    return true;
  }

  /// Prune clones, then replay each original or reconstructed alternative
  static bool
  replay_matches(ReplaySpace& root, const Choice& choice,
                 const NSBlocks& restriction, int lower, int upper,
                 const Words& first, const Words& second) {
    for (int archived = 0; archived < 2; ++archived)
      for (unsigned int alternative = 0; alternative < 2; ++alternative) {
        std::unique_ptr<ReplaySpace> s(
          static_cast<ReplaySpace*>(root.clone()));
        NSBlocks domain(restriction);
        rel(*s, s->x[0], STRT_DOM, domain, lower, upper);
        if (s->status() == SS_FAILED)
          return false;
        if (archived) {
          Archive archive;
          choice.archive(archive);
          std::unique_ptr<const Choice> restored(s->choice(archive));
          s->commit(*restored, alternative);
        } else {
          s->commit(choice, alternative);
        }
        if (!solutions_match(*s, alternative == 0 ? first : second)) {
          olog << "Archived: " << archived
               << ", alternative: " << alternative << std::endl;
          return false;
        }
      }
    return true;
  }

  /// Replay the reviewed length endpoint before and after assignment
  class LengthReplay : public Base {
  public:
    LengthReplay(void) : Base("String::Branch::LengthReplay", TestTags::check()) {}

    virtual bool run(void) {
      NSBlocks domain;
      domain.emplace_back(NSIntSet('a'), 1, 3);
      for (int selector = 0; selector < 7; ++selector) {
        ReplaySpace root(domain, 1, 3, selector);
        if (root.status() != SS_BRANCH)
          return false;
        std::unique_ptr<const Choice> choice(root.choice());
        NSBlocks narrowed;
        narrowed.emplace_back(NSIntSet('a'), 2, 3);
        if (!replay_matches(root, *choice, narrowed, 2, 3, {},
                            {{'a', 'a'}, {'a', 'a', 'a'}}))
          return false;
        for (int length = 1; length <= 3; ++length) {
          Word word(length, 'a');
          NSBlocks assigned(word);
          Words first = length == 1 ? Words{word} : Words{};
          Words second = length == 1 ? Words{} : Words{word};
          if (!replay_matches(root, *choice, assigned, length, length,
                              first, second))
            return false;
        }
      }
      return true;
    }
  };

  /// Replay a saved scalar position after block splitting and merging
  class SymbolReplay : public Base {
  public:
    SymbolReplay(void) : Base("String::Branch::SymbolReplay", TestTags::check()) {}

    virtual bool run(void) {
      enum Fixture { BYTES, UNICODE, PREFIX, MANDATORY };
      for (int selector = 0; selector < 7; ++selector)
        for (Fixture fixture : {BYTES, UNICODE, PREFIX, MANDATORY}) {
          const int low = fixture == UNICODE ? 0x100 : 'a';
          const int high = fixture == UNICODE ? 0x1f600 : 'b';
          const bool prefix = fixture == PREFIX;
          const bool mandatory = fixture == MANDATORY;
          NSIntSet alphabet(low);
          alphabet.add(high);
          NSBlocks domain;
          if (prefix)
            domain.emplace_back(NSIntSet(low), 1, 1);
          domain.emplace_back(alphabet, 2, 2);
          const int length = prefix ? 3 : 2;
          ReplaySpace root(domain, length, length, selector,
                           mandatory ? high : -1);
          if (root.status() != SS_BRANCH)
            return false;
          std::unique_ptr<const Choice> choice(root.choice());
          const bool prefer_mandatory = selectors[selector].vals.select() ==
            StringValBranch::Select::STRING_VAL_LLLM;
          const int split = mandatory && prefer_mandatory ? high : low;
          for (int leading : {low, high}) {
            NSBlocks narrowed;
            Word head;
            if (prefix) {
              narrowed.emplace_back(NSIntSet(low), 1, 1);
              head.push_back(low);
            }
            narrowed.emplace_back(NSIntSet(leading), 1, 1);
            narrowed.emplace_back(alphabet, 1, 1);
            head.push_back(leading);
            Words expected;
            for (int trailing : {low, high}) {
              Word word(head);
              word.push_back(trailing);
              expected.insert(word);
              NSBlocks assigned(word);
              Words first = leading == split ? Words{word} : Words{};
              Words second = leading == split ? Words{} : Words{word};
              if (!replay_matches(root, *choice, assigned, length, length,
                                  first, second))
                return false;
            }
            if (!replay_matches(root, *choice, narrowed, length, length,
                                leading == split ? expected : Words{},
                                leading == split ? Words{} : expected))
              return false;
          }
        }
      return true;
    }
  };

  /// Exercise the fixed fallback without changing the public selector surface
  class FixedBrancher : public Gecode::String::Branch::None_LLLL {
  public:
    FixedBrancher(Home home, ViewArray<Gecode::String::StringView>& views)
      : None_LLLL(home, views) {}

    Choice* fixed_choice(void) {
      return position_decision(0, &x[0].domain(),
                               Gecode::String::Branch::Value::MIN);
    }
  };

  class FixedReplay : public Base {
  public:
    FixedReplay(void) : Base("String::Branch::FixedReplay", TestTags::check()) {}

    virtual bool run(void) {
      const Word word{0x100, 0x1f600};
      ReplaySpace root(NSBlocks(word), 2, 2, -1);
      ViewArray<Gecode::String::StringView> views(root, StringVarArgs(root.x));
      FixedBrancher* brancher = new (root) FixedBrancher(root, views);
      if (root.status() != SS_SOLVED)
        return false;
      std::unique_ptr<const Choice> choice(brancher->fixed_choice());
      return replay_matches(root, *choice, NSBlocks(word), 2, 2, {word}, {});
    }
  };

  /// Check exact alternatives and DFS recomputation on tiny structured domains
  class Recompute : public Base {
  public:
    Recompute(void) : Base("String::Branch::Recompute", TestTags::check()) {}

    virtual bool run(void) {
      const unsigned int distances[][2] = {{1, 1}, {2, 1}, {8, 2}};
      enum Fixture { ORDINARY, OPTIONAL_ONE, OPTIONAL_TWO };
      for (int selector = 0; selector < 7; ++selector)
        for (Fixture fixture : {ORDINARY, OPTIONAL_ONE, OPTIONAL_TWO}) {
          NSBlocks domain;
          Words expected;
          int lower, upper;
          if (fixture == ORDINARY) {
            domain.emplace_back(NSIntSet('a', 'b'), 1, 3);
            lower = 1;
            upper = 3;
            for (int length = 1; length <= 3; ++length)
              for (int bits = 0; bits < (1 << length); ++bits) {
                Word word;
                for (int i = 0; i < length; ++i)
                  word.push_back(bits & (1 << i) ? 'b' : 'a');
                expected.insert(word);
              }
          } else {
            domain.emplace_back(NSIntSet('a', 'b'), 0, 1);
            domain.emplace_back(NSIntSet('b', 'c'), 0, 1);
            lower = upper = fixture == OPTIONAL_ONE ? 1 : 2;
            expected = fixture == OPTIONAL_ONE ? Words{{'a'}, {'b'}, {'c'}} :
              Words{{'a', 'b'}, {'a', 'c'}, {'b', 'b'}, {'b', 'c'}};
          }
          ReplaySpace root(domain, lower, upper, selector);
          if (root.status() != SS_BRANCH)
            return false;
          if (fixture != ORDINARY) {
            std::unique_ptr<const Choice> choice(root.choice());
            Words first, second;
            for (const Word& word : expected)
              (word[0] == 'a' ? first : second).insert(word);
            if (!replay_matches(root, *choice, domain, lower, upper,
                                first, second))
              return false;
          }
          for (const auto& distance : distances)
            if (!solutions_match(root, expected, distance[0], distance[1]))
              return false;
        }
      return true;
    }
  };

  LengthReplay length_replay;
  SymbolReplay symbol_replay;
  FixedReplay fixed_replay;
  Recompute recompute;

}}}

// STATISTICS: test-string
