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
#include <sstream>
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

    ReplaySpace(StringVarBranch vars, int lower0, int upper0, int lower1,
                int upper1)
      : x(*this, 2) {
      NSBlocks domain;
      domain.emplace_back(NSIntSet('a', 'b'), 1, 5);
      x[0] = StringVar(*this, domain, lower0, upper0);
      x[1] = StringVar(*this, domain, lower1, upper1);
      branch(*this, StringVarArgs(x), vars, STRING_VAL_LLLL());
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
    Search::NodeStop stop(1000);
    options.stop = &stop;
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
    if (search.stopped()) {
      olog << "String branch search did not exhaust" << std::endl;
      return false;
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
                 const Words& first, const Words& second,
                 bool prune_after = false) {
    for (int archived = 0; archived < 2; ++archived)
      for (unsigned int alternative = 0; alternative < 2; ++alternative) {
        std::unique_ptr<ReplaySpace> s(
          static_cast<ReplaySpace*>(root.clone()));
        NSBlocks domain(restriction);
        if (!prune_after) {
          rel(*s, s->x[0], STRT_DOM, domain, lower, upper);
          if (s->status() == SS_FAILED)
            return false;
        }
        if (archived) {
          Archive archive;
          choice.archive(archive);
          std::unique_ptr<const Choice> restored(s->choice(archive));
          s->commit(*restored, alternative);
        } else {
          s->commit(choice, alternative);
        }
        if (prune_after) {
          if (s->status() == SS_FAILED)
            return false;
          s.reset(static_cast<ReplaySpace*>(s->clone()));
          rel(*s, s->x[0], STRT_DOM, domain, lower, upper);
        }
        if (!solutions_match(*s, alternative == 0 ? first : second)) {
          olog << "Archived: " << archived
               << ", alternative: " << alternative << std::endl;
          return false;
        }
      }
    return true;
  }

  /// Select by length endpoints and retain the first candidate on ties
  class LengthSelection : public Base {
  public:
    LengthSelection(void)
      : Base("String::Branch::LengthSelection") {}

    virtual bool
    run(void) {
      struct Fixture {
        StringVarBranch selector;
        int lower0, upper0, lower1, upper1;
        int selected;
      };
      const Fixture fixtures[] = {{STRING_VAR_LENMIN(), 5, 5, 1, 2, 1},
                                  {STRING_VAR_LENMAX(), 5, 5, 1, 2, 0},
                                  {STRING_VAR_LENMIN(), 1, 5, 1, 2, 0},
                                  {STRING_VAR_LENMAX(), 2, 5, 1, 5, 0}};
      for (const Fixture& fixture : fixtures) {
        ReplaySpace root(fixture.selector, fixture.lower0, fixture.upper0,
                         fixture.lower1, fixture.upper1);
        if (root.status() != SS_BRANCH)
          return false;
        std::unique_ptr<const Choice> choice(root.choice());
        std::ostringstream printed;
        root.print(*choice, 0, printed);
        const std::string selected =
            " -- Var. " + std::to_string(fixture.selected) + " --";
        if (printed.str().find(selected) == std::string::npos) {
          olog << "Wrong length selector choice: " << printed.str()
               << ", expected variable " << fixture.selected << std::endl;
          return false;
        }
      }
      return true;
    }
  };

  /// Replay the reviewed length endpoint before and after assignment
  class LengthReplay : public Base {
  public:
    LengthReplay(void) : Base("String::Branch::LengthReplay") {}

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
    SymbolReplay(void) : Base("String::Branch::SymbolReplay") {}

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
    FixedReplay(void) : Base("String::Branch::FixedReplay") {}

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
    Recompute(void) : Base("String::Branch::Recompute") {}

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

  /// Partition overlapping optional blocks without repeating concrete strings
  class Positional : public Base {
  public:
    Positional(void) : Base("String::Branch::Positional") {}

    virtual bool
    run(void) {
      NSBlocks domain;
      domain.emplace_back(NSIntSet('a', 'b'), 0, 1);
      domain.emplace_back(NSIntSet('a', 'c'), 0, 1);
      domain.emplace_back(NSIntSet('b', 'c'), 0, 1);
      const Words first{{'a', 'a'}, {'a', 'b'}, {'a', 'c'}};
      const Words second{
          {'b', 'a'}, {'b', 'b'}, {'b', 'c'}, {'c', 'b'}, {'c', 'c'}};
      Words expected(first);
      expected.insert(second.begin(), second.end());
      const unsigned int distances[][2] = {{1, 1}, {2, 1}, {8, 2}};
      for (int selector = 0; selector < 7; ++selector) {
        ReplaySpace root(domain, 2, 2, selector);
        if (root.status() != SS_BRANCH)
          return false;
        std::unique_ptr<const Choice> choice(root.choice());
        for (bool prune_after : {false, true}) {
          if (!replay_matches(root, *choice, domain, 2, 2, first, second,
                              prune_after))
            return false;
          NSBlocks narrowed(domain);
          narrowed[1].S.remove('c');
          if (!replay_matches(root, *choice, narrowed, 2, 2, first,
                              {{'b', 'a'}, {'b', 'b'}, {'b', 'c'}},
                              prune_after))
            return false;
          for (const Word& word : expected) {
            Words positive = word[0] == 'a' ? Words{word} : Words{};
            Words negative = word[0] == 'a' ? Words{} : Words{word};
            if (!replay_matches(root, *choice, NSBlocks(word), 2, 2, positive,
                                negative, prune_after))
              return false;
          }
        }
        for (const auto& distance : distances)
          if (!solutions_match(root, expected, distance[0], distance[1]))
            return false;
        NSBlocks prefixed(domain);
        prefixed.insert(prefixed.begin(),
                        Gecode::String::NSBlock(NSIntSet(0x1f600), 1, 1));
        Words prefixed_expected;
        for (Word word : expected) {
          word.insert(word.begin(), 0x1f600);
          prefixed_expected.insert(word);
        }
        ReplaySpace prefixed_root(prefixed, 3, 3, selector);
        for (const auto& distance : distances)
          if (!solutions_match(prefixed_root, prefixed_expected, distance[0],
                               distance[1]))
            return false;
      }
      return true;
    }
  };

  LengthSelection length_selection;
  LengthReplay length_replay;
  SymbolReplay symbol_replay;
  FixedReplay fixed_replay;
  Recompute recompute;
  Positional positional;

}}}

// STATISTICS: test-string
