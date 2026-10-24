/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *  http://www.gecode.org
 */

#include "test/string.hh"

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
      : Test("Replace::" + name(kind0), 2, "ab", 3, false, TestTags::check()),
        kind(kind0) {}

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

  std::string operation_name(Gecode::StringOpType operation) {
    switch (operation) {
    case Gecode::STRT_REP: return "First";
    case Gecode::STRT_REPLST: return "Last";
    case Gecode::STRT_REPALL: return "All";
    default: GECODE_NEVER;
    }
    return "Unknown";
  }

  class EqualResult : public Test {
  private:
    Gecode::StringOpType operation;
    bool shared;
  public:
    EqualResult(Gecode::StringOpType operation0, bool shared0)
      : Test("Replace::" + operation_name(operation0) + "::EqualResult::" +
             (shared0 ? "Alias" : "Distinct"), 2, "ab", 1,
             false, TestTags::check()),
        operation(operation0), shared(shared0) {}

    virtual bool solution(const Assignment& assignment) const {
      return (assignment[0] == assignment[1]) || (assignment[0] == "b");
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::StringVar source(home, "a");
      Gecode::StringVar result = shared ? source : Gecode::StringVar(home, "a");
      Gecode::rel(home, source, x[0], x[1], operation, result);
    }
  };

  class QueryResult : public Test {
  private:
    Gecode::StringOpType operation;
  public:
    QueryResult(Gecode::StringOpType operation0)
      : Test("Replace::" + operation_name(operation0) + "::QueryResult",
             1, "ab", 2, false, TestTags::check()), operation(operation0) {}

    virtual bool solution(const Assignment& assignment) const {
      return (operation != Gecode::STRT_REPALL) && (assignment[0] == "a");
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::StringVar source(home, "aa");
      Gecode::StringVar replacement(home, "");
      Gecode::rel(home, source, x[0], replacement, operation, x[0]);
    }
  };

  class EmptySource : public Test {
  private:
    Gecode::StringOpType operation;
  public:
    EmptySource(Gecode::StringOpType operation0)
      : Test("Replace::" + operation_name(operation0) + "::EmptySource",
             2, "ab", 1, false, TestTags::check()), operation(operation0) {}

    virtual bool solution(const Assignment& assignment) const {
      return assignment[0] == assignment[1];
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::StringVar source(home, "");
      Gecode::StringVar query(home, "");
      Gecode::rel(home, source, query, x[0], operation, x[1]);
    }
  };

  class AllProgressSpace : public Gecode::Space {
  public:
    Gecode::StringVar source;
    Gecode::StringVar query;
    Gecode::StringVar replacement;
    Gecode::StringVar result;

    AllProgressSpace(Gecode::String::StringSymbol symbol,
                     bool assigned_source, bool assigned_replacement)
      : source(*this, Gecode::String::NSIntSet(symbol),
               assigned_source ? 2 : 1, 2),
        query(*this, ""),
        replacement(*this, Gecode::String::NSIntSet('b'),
                    1, assigned_replacement ? 1 : 2) {
      Gecode::String::NSIntSet alphabet(symbol);
      alphabet.add('b');
      result = Gecode::StringVar(*this, alphabet, 3, 8);
      Gecode::replace_all(*this, source, query, replacement, result);
      Gecode::StringVarArgs operands;
      operands << source << query << replacement << result;
      Gecode::branch(*this, operands,
                     Gecode::STRING_VAR_NONE(), Gecode::STRING_VAL_LLLL());
    }

    AllProgressSpace(void)
      : source(*this, "ab"),
        query(*this, Gecode::String::NSIntSet('a', 'b'), 1, 1),
        replacement(*this, "x"), result(*this, "xb") {
      Gecode::replace_all(*this, source, query, replacement, result);
      Gecode::StringVarArgs operands;
      operands << source << query << replacement << result;
      Gecode::branch(*this, operands,
                     Gecode::STRING_VAR_NONE(), Gecode::STRING_VAL_LLLL());
    }

    AllProgressSpace(AllProgressSpace& s) : Gecode::Space(s) {
      source.update(*this, s.source);
      query.update(*this, s.query);
      replacement.update(*this, s.replacement);
      result.update(*this, s.result);
    }

    virtual Gecode::Space* copy(void) {
      return new AllProgressSpace(*this);
    }
  };

  class EmptyQuery : public Base {
  private:
    Gecode::String::StringSymbol symbol;
  public:
    EmptyQuery(bool scalar)
      : Base(scalar ? "String::Replace::All::EmptyQuery::Scalars"
                    : "String::Replace::All::EmptyQuery::Bytes", TestTags::check()),
        symbol(scalar ? 0x100 : 'a') {}

    virtual bool run(void) {
      using Gecode::String::StringVal;
      const StringVal expected[2][2] = {
        {StringVal::from_symbols({'b', symbol, 'b'}),
         StringVal::from_symbols({'b', 'b', symbol, 'b', 'b'})},
        {StringVal::from_symbols({'b', symbol, 'b', symbol, 'b'}),
         StringVal::from_symbols({'b', 'b', symbol, 'b', 'b',
                                  symbol, 'b', 'b'})}
      };
      for (int assigned_source = 0; assigned_source <= 1; ++assigned_source)
        for (int assigned_replacement = 0;
             assigned_replacement <= 1; ++assigned_replacement) {
          AllProgressSpace root(symbol, assigned_source, assigned_replacement);
          Gecode::Search::Options options;
          options.threads = 1;
          Gecode::DFS<AllProgressSpace> search(&root, options);
          std::set<StringVal> actual;
          while (AllProgressSpace* solution = search.next()) {
            const int source_length = solution->source.min_length();
            const int replacement_length = solution->replacement.min_length();
            const bool assigned = solution->source.assigned() &&
              solution->replacement.assigned() && solution->result.assigned();
            bool valid = assigned &&
              (source_length >= 1) && (source_length <= 2) &&
              (replacement_length >= 1) && (replacement_length <= 2);
            if (valid) {
              const StringVal result = solution->result.val_symbols();
              valid = (result ==
                expected[source_length - 1][replacement_length - 1]) &&
                actual.insert(result).second;
            }
            delete solution;
            if (!valid)
              return false;
          }
          std::set<StringVal> expected_results;
          for (int source_length = assigned_source ? 2 : 1;
               source_length <= 2; ++source_length)
            for (int replacement_length = 1;
                 replacement_length <= (assigned_replacement ? 1 : 2);
                 ++replacement_length)
              expected_results.insert(
                expected[source_length - 1][replacement_length - 1]);
          if (actual != expected_results)
            return false;
        }
      return true;
    }
  };

  class VariableQuery : public Base {
  public:
    VariableQuery(void)
      : Base("String::Replace::All::VariableQuery", TestTags::check()) {}

    virtual bool run(void) {
      using Gecode::String::StringVal;
      AllProgressSpace root;
      if ((root.status() == Gecode::SS_FAILED) ||
          !root.query.may_chars().in('a'))
        return false;
      Gecode::Search::Options options;
      options.threads = 1;
      Gecode::DFS<AllProgressSpace> search(&root, options);
      std::set<StringVal> queries;
      while (AllProgressSpace* solution = search.next()) {
        const bool valid = solution->query.assigned() &&
          queries.insert(solution->query.val_symbols()).second;
        delete solution;
        if (!valid)
          return false;
      }
      return queries == std::set<StringVal>({StringVal::from_symbols({'a'})});
    }
  };

  class OverlapSpace : public Gecode::Space {
  public:
    Gecode::StringVar source;
    Gecode::StringVar result;

    OverlapSpace(Gecode::String::StringSymbol symbol, bool last,
                 bool assigned_source)
      : source(*this, Gecode::String::NSIntSet(symbol),
               assigned_source ? 3 : 2, 3) {
      Gecode::String::NSIntSet alphabet(symbol);
      alphabet.add('b');
      result = Gecode::StringVar(*this, alphabet, 1, 2);
      Gecode::StringVar query(*this,
        Gecode::String::StringVal::from_symbols({symbol, symbol}));
      Gecode::StringVar replacement(*this, "b");
      if (last)
        Gecode::replace_last(*this, source, query, replacement, result);
      else
        Gecode::replace(*this, source, query, replacement, result);
      Gecode::StringVarArgs operands;
      operands << source << result;
      Gecode::branch(*this, operands,
                     Gecode::STRING_VAR_NONE(), Gecode::STRING_VAL_LLLL());
    }

    OverlapSpace(OverlapSpace& s) : Gecode::Space(s) {
      source.update(*this, s.source);
      result.update(*this, s.result);
    }

    virtual Gecode::Space* copy(void) {
      return new OverlapSpace(*this);
    }
  };

  class Overlap : public Base {
  private:
    bool last;

    static Gecode::String::StringVal
    replace(const Gecode::String::StringVal& source, bool last) {
      using Gecode::String::StringSymbol;
      using Gecode::String::StringVal;
      // The source repeats one symbol and the query contains two copies.
      std::vector<StringSymbol> result(source.begin(), source.end());
      const StringVal::size_type position = last ? source.size() - 2 : 0;
      result.erase(result.begin() + position, result.begin() + position + 2);
      result.insert(result.begin() + position, 'b');
      return StringVal::from_symbols(std::move(result));
    }

    bool check_solutions(Gecode::String::StringSymbol symbol,
                         bool assigned_source) const {
      using Gecode::String::StringSymbol;
      using Gecode::String::StringVal;
      typedef std::pair<StringVal, StringVal> Assignment;
      OverlapSpace root(symbol, last, assigned_source);
      if ((root.status() == Gecode::SS_FAILED) ||
          (root.source.assigned() != assigned_source))
        return false;
      Gecode::Search::Options options;
      options.threads = 1;
      Gecode::DFS<OverlapSpace> engine(&root, options);
      std::set<Assignment> actual;
      while (OverlapSpace* solution = engine.next()) {
        const bool valid = solution->source.assigned() &&
          solution->result.assigned() && actual.insert(Assignment(
            solution->source.val_symbols(),
            solution->result.val_symbols())).second;
        delete solution;
        if (!valid)
          return false;
      }
      std::set<Assignment> expected;
      for (int length = assigned_source ? 3 : 2; length <= 3; ++length) {
        const StringVal source = StringVal::from_symbols(
          std::vector<StringSymbol>(length, symbol));
        expected.insert(Assignment(source, replace(source, last)));
      }
      return actual == expected;
    }

    bool check_assignment(Gecode::String::StringSymbol symbol,
                          bool assigned_source, bool correct_result) const {
      using Gecode::String::StringVal;
      OverlapSpace root(symbol, last, assigned_source);
      if ((root.status() == Gecode::SS_FAILED) ||
          (root.source.assigned() != assigned_source))
        return false;
      const StringVal source = StringVal::from_symbols({symbol, symbol, symbol});
      const StringVal result = replace(source, correct_result ? last : !last);
      Gecode::rel(root, root.source, Gecode::STRT_EQ,
                  Gecode::StringVar(root, source));
      Gecode::rel(root, root.result, Gecode::STRT_EQ,
                  Gecode::StringVar(root, result));
      const bool failed = root.status() == Gecode::SS_FAILED;
      return correct_result
        ? !failed && root.result.assigned() &&
          (root.result.val_symbols() == result)
        : failed;
    }

  public:
    Overlap(bool last0)
      : Base(last0 ? "String::Replace::Last::Overlap"
                   : "String::Replace::First::Overlap", TestTags::check()), last(last0) {}

    virtual bool run(void) {
      const Gecode::String::StringSymbol symbols[] = {'a', 0x100};
      for (unsigned int i = 0; i < sizeof(symbols) / sizeof(symbols[0]); ++i)
        for (int assigned_source = 0; assigned_source <= 1; ++assigned_source)
          if (!check_solutions(symbols[i], assigned_source) ||
              !check_assignment(symbols[i], assigned_source, true) ||
              !check_assignment(symbols[i], assigned_source, false))
            return false;
      return true;
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
    ScalarReplacement(void)
      : Base("String::Replace::ScalarReplacement", TestTags::check()) {}

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
  EqualResult first_equal_alias(Gecode::STRT_REP, true);
  EqualResult first_equal_distinct(Gecode::STRT_REP, false);
  EqualResult last_equal_alias(Gecode::STRT_REPLST, true);
  EqualResult last_equal_distinct(Gecode::STRT_REPLST, false);
  EqualResult all_equal_alias(Gecode::STRT_REPALL, true);
  EqualResult all_equal_distinct(Gecode::STRT_REPALL, false);
  QueryResult first_query_result(Gecode::STRT_REP);
  QueryResult last_query_result(Gecode::STRT_REPLST);
  QueryResult all_query_result(Gecode::STRT_REPALL);
  EmptySource first_empty_source(Gecode::STRT_REP);
  EmptySource last_empty_source(Gecode::STRT_REPLST);
  EmptyQuery empty_query_bytes(false);
  EmptyQuery empty_query_scalars(true);
  VariableQuery variable_query;
  Overlap first_overlap(false);
  Overlap last_overlap(true);

}}}

// STATISTICS: test-string
