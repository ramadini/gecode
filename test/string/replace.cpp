/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *  http://www.gecode.org
 */

#include "test/string.hh"

#include <algorithm>
#include <set>
#include <utility>
#include <vector>

namespace Test { namespace String { namespace Replace {

  class Occurrence : public Test {
  public:
    enum Kind {
      ALL
    };

  private:
    Kind kind;

    static std::string name(Kind kind0) {
      switch (kind0) {
      case ALL: return "All";
      default:    GECODE_NEVER;
      }
      return "Unknown";
    }

    static std::string replace(const std::string& source, Kind kind) {
      std::string result(source);
      for (std::string::size_type position = result.find('a');
           position != std::string::npos;
           position = result.find('a', position + 1))
        result[position] = 'b';
      return result;
    }

  public:
    Occurrence(Kind kind0)
      : Test("Replace::" + name(kind0), 2, "ab", 3), kind(kind0) {}

    virtual bool solution(const Assignment& assignment) const {
      return assignment[1] == replace(assignment[0], kind);
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::StringVar query(home, "a");
      Gecode::StringVar replacement(home, "b");
      switch (kind) {
      case ALL:
        Gecode::replace_all(home, x[0], query, replacement, x[1]);
        break;
      default:
        GECODE_NEVER;
      }
    }
  };

  /// Keep the replacement open when assigned source/query decomposition runs.
  class ScalarReplacementSpace : public Gecode::Space {
  public:
    Gecode::StringVar replacement;
    Gecode::StringVar result;

    ScalarReplacementSpace(const Gecode::String::StringVal& source,
                           const Gecode::String::StringVal& query,
                           Gecode::StringOpType operation, bool assigned)
      : replacement(*this, Gecode::String::NSIntSet(0x100, 0x101), 0, 2) {
      Gecode::String::NSIntSet alphabet = replacement.may_chars();
      for (Gecode::String::StringSymbol symbol : source)
        alphabet.add(symbol);
      const int length = static_cast<int>(source.size());
      Gecode::StringVar x = assigned ? Gecode::StringVar(*this, source)
        : Gecode::StringVar(*this, alphabet, 0, length);
      result = Gecode::StringVar(*this, alphabet, 0, length + 2 * (length + 1));
      Gecode::rel(*this, x, Gecode::StringVar(*this, query), replacement,
                  operation, result);
      if (!assigned)
        Gecode::rel(*this, x, Gecode::STRT_EQ,
                    Gecode::StringVar(*this, source));
      Gecode::branch(*this, Gecode::StringVarArgs() << replacement << result,
                     Gecode::STRING_VAR_NONE(), Gecode::STRING_VAL_LLLL());
    }

    ScalarReplacementSpace(ScalarReplacementSpace& s) : Gecode::Space(s) {
      replacement.update(*this, s.replacement);
      result.update(*this, s.result);
    }

    virtual Gecode::Space* copy(void) {
      return new ScalarReplacementSpace(*this);
    }
  };

  class ScalarReplacement : public Base {
  private:
    typedef std::vector<Gecode::String::StringSymbol> Word;
    typedef std::pair<Word, Word> Assignment;

    // Query is empty or the single byte 'a'; select positions in the source.
    static Word replace(const Word& source, const Word& replacement,
                        bool empty_query, Gecode::StringOpType operation) {
      std::vector<Word::size_type> positions;
      for (Word::size_type i = 0; i <= source.size(); ++i)
        if (empty_query || (i < source.size() && source[i] == 'a'))
          positions.push_back(i);
      if (operation != Gecode::STRT_REPALL && !positions.empty()) {
        const Word::size_type position = operation == Gecode::STRT_REPLST
          ? positions.back() : positions.front();
        positions.assign(1, position);
      }
      Word result;
      for (Word::size_type i = 0; i <= source.size(); ++i) {
        const bool selected = std::find(positions.begin(), positions.end(), i)
          != positions.end();
        if (selected)
          result.insert(result.end(), replacement.begin(), replacement.end());
        if (i < source.size() && (empty_query || !selected))
          result.push_back(source[i]);
      }
      return result;
    }

    static bool check(const Word& source, bool empty_query,
                      Gecode::StringOpType operation, bool assigned) {
      using Gecode::String::StringVal;
      const Word query = empty_query ? Word() : Word{'a'};
      ScalarReplacementSpace root(StringVal::from_symbols(source),
        StringVal::from_symbols(query), operation, assigned);
      if (root.status() == Gecode::SS_FAILED || root.replacement.assigned())
        return false;
      Gecode::Search::Options options;
      options.threads = 1;
      Gecode::DFS<ScalarReplacementSpace> search(&root, options);
      std::set<Assignment> actual;
      while (ScalarReplacementSpace* solution = search.next()) {
        const bool assigned = solution->replacement.assigned() &&
          solution->result.assigned();
        bool unique = false;
        if (assigned) {
          const StringVal replacement = solution->replacement.val_symbols();
          const StringVal result = solution->result.val_symbols();
          unique = actual.insert(Assignment(
            Word(replacement.begin(), replacement.end()),
            Word(result.begin(), result.end()))).second;
        }
        delete solution;
        if (!assigned || !unique)
          return false;
      }
      std::set<Assignment> expected;
      std::vector<Word> replacements(1);
      for (int first = 0x100; first <= 0x101; ++first) {
        replacements.push_back(Word{first});
        for (int second = 0x100; second <= 0x101; ++second)
          replacements.push_back(Word{first, second});
      }
      for (const Word& replacement : replacements)
        expected.insert(Assignment(replacement,
          replace(source, replacement, empty_query, operation)));
      return actual == expected;
    }

  public:
    ScalarReplacement(void) : Base("String::Replace::ScalarReplacement") {}

    virtual bool run(void) {
      for (int symbol : {static_cast<int>('b'), 0x102})
        for (bool empty_query : {false, true})
          for (Gecode::StringOpType operation :
               {Gecode::STRT_REP, Gecode::STRT_REPLST, Gecode::STRT_REPALL})
            for (bool assigned : {false, true})
              if (!check(Word{symbol, 'a', symbol, 'a', symbol},
                         empty_query, operation, assigned))
                return false;
      return true;
    }
  };

  ScalarReplacement scalar_replacement;

  Occurrence all(Occurrence::ALL);

}}}

// STATISTICS: test-string
