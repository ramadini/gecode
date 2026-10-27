/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *  http://www.gecode.org
 */

#include "test/string.hh"

#include <gecode/string/match.hh>

namespace Test { namespace String { namespace Match {

  class FirstAB : public Test {
  private:
    int index;

    static std::string name(int index0) {
      return (index0 == 0) ? "Absent" : "First";
    }

  public:
    FirstAB(int index0)
      : Test("Match::" + name(index0), 1, "ab", 3), index(index0) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string::size_type position = assignment[0].find("ab");
      return (position == std::string::npos) ? index == 0 :
        static_cast<int>(position) + 1 == index;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::match(home, x[0], "ab", Gecode::IntVar(home, index, index));
    }
  };

  /// Space for direct public Match posting checks
  class MatchSpace : public Gecode::Space {
  public:
    MatchSpace(void) {}
    MatchSpace(MatchSpace& s) : Gecode::Space(s) {}
    virtual Gecode::Space* copy(void) {
      return new MatchSpace(*this);
    }
  };

  /// Retain total lengths when Match rebuilds the prefix and suffix blocks
  class AggregateBounds : public Base {
  private:
    Gecode::String::MatchImplementation implementation;

  public:
    AggregateBounds(const std::string& name,
                    Gecode::String::MatchImplementation implementation0)
      : Base("String::Match::AggregateBounds::" + name),
        implementation(implementation0) {}

    virtual bool
    run(void) {
      using namespace Gecode;
      // The original length-two words are ab, ac, and bc; only ab matches at 2.
      for (const std::string& word : {"ab", "ac", "bc", "abc"}) {
        MatchSpace home;
        Gecode::String::NSBlocks blocks;
        for (char symbol : {'a', 'b', 'c'})
          blocks.emplace_back(Gecode::String::NSIntSet(symbol), 0, 1);
        StringVar x(home, blocks, 2, 2);
        IntVar index(home, 2, 2);
        if (implementation == Gecode::String::MATCH_AUTO)
          match(home, x, "b", index);
        else if (Gecode::String::post_match(home, x, "b", index,
                                            implementation) == ES_FAILED)
          home.fail();
        if (home.status() == SS_FAILED || x.min_length() != 2 ||
            x.max_length() != 2 || !x.domain().is_normalized())
          return false;
        rel(home, x, STRT_EQ, StringVar(home, word));
        const bool accepted = home.status() != SS_FAILED;
        if (accepted != (word == "ab") ||
            (accepted && (!x.assigned() || x.val() != word)))
          return false;
      }
      return true;
    }
  };

  AggregateBounds aggregate_dfa("DFA", Gecode::String::MATCH_DFA);
  AggregateBounds aggregate_nfa("NFA", Gecode::String::MATCH_NFA);
  AggregateBounds aggregate_auto("AUTO", Gecode::String::MATCH_AUTO);

  FirstAB absent(0);
  FirstAB first(1);

}}}

// STATISTICS: test-string
