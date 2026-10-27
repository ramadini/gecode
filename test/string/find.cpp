/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *  http://www.gecode.org
 */

#include "test/string.hh"

#include <algorithm>
#include <memory>
#include <set>

namespace Test { namespace String { namespace Find {

  class FirstA : public Test {
  private:
    int index;

    static std::string name(int index0) {
      return (index0 == 0) ? "Absent" : "First";
    }

  public:
    FirstA(int index0)
      : Test("Find::" + name(index0), 1, "ab", 2), index(index0) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string::size_type position = assignment[0].find('a');
      return (position == std::string::npos) ? index == 0 :
        static_cast<int>(position) + 1 == index;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::find(home, Gecode::StringVar(home, "a"), x[0],
                   Gecode::IntVar(home, index, index));
    }
  };

  FirstA absent(0);
  FirstA first(1);

  Gecode::String::StringVal
  symbol_word(const std::string& word, int first) {
    std::vector<Gecode::String::StringSymbol> symbols;
    for (char symbol : word)
      symbols.push_back(first + symbol - 'a');
    return Gecode::String::StringVal::from_symbols(symbols);
  }

  class ExtractionSpace : public Gecode::Space {
  public:
    Gecode::StringVar pattern;
    Gecode::StringVar source;

    ExtractionSpace(const Gecode::String::StringVal& source_value,
                    int length, int index, int first, bool assign_before_post)
      : pattern(*this, Gecode::String::NSIntSet(first, first + 1),
                length, length),
        source(assign_before_post ? Gecode::StringVar(*this, source_value) :
          Gecode::StringVar(*this,
            Gecode::String::NSIntSet(first, first + 1),
            static_cast<int>(source_value.size()),
            static_cast<int>(source_value.size()))) {
      Gecode::find(*this, pattern, source, Gecode::IntVar(*this, index, index));
      if (!assign_before_post) {
        // Exercise assignment after Find has reached its initial fixpoint.
        status();
        Gecode::rel(*this, source, Gecode::STRT_EQ,
                    Gecode::StringVar(*this, source_value));
      }
      Gecode::branch(*this, Gecode::StringVarArgs() << pattern,
                     Gecode::STRING_VAR_NONE(), Gecode::STRING_VAL_LLLL());
    }

    ExtractionSpace(ExtractionSpace& s) : Gecode::Space(s) {
      pattern.update(*this, s.pattern);
      source.update(*this, s.source);
    }

    virtual Gecode::Space* copy(void) {
      return new ExtractionSpace(*this);
    }
  };

  bool
  extraction_matches(const std::string& source_word, int length, int index,
                     int first, bool assign_before_post) {
    using Gecode::String::StringVal;
    const StringVal source = symbol_word(source_word, first);
    std::set<StringVal> expected;
    for (Assignment candidate(1, "ab", length); candidate(); ++candidate) {
      if (static_cast<int>(candidate[0].size()) != length)
        continue;
      const StringVal pattern = symbol_word(candidate[0], first);
      const auto position = std::search(source.begin(), source.end(),
                                        pattern.begin(), pattern.end());
      int first_position = 0;
      if (pattern.empty())
        first_position = 1;
      else if (position != source.end())
        first_position = static_cast<int>(position - source.begin()) + 1;
      if (first_position == index)
        expected.insert(pattern);
    }

    ExtractionSpace root(source, length, index, first, assign_before_post);
    Gecode::Search::Options options;
    options.threads = 1;
    Gecode::DFS<ExtractionSpace> search(&root, options);
    std::set<StringVal> actual;
    while (ExtractionSpace* solution = search.next()) {
      actual.insert(solution->pattern.val_symbols());
      delete solution;
    }
    if (actual == expected)
      return true;
    if (opt.log)
      olog << "Find extraction: source=" << source_word << " length=" << length
           << " index=" << index << " first=" << first
           << " assign_before_post=" << assign_before_post
           << " expected=" << expected.size() << " actual=" << actual.size()
           << std::endl;
    return false;
  }

  class Extraction : public Base {
  public:
    Extraction(void)
      : Base("String::Find::Extraction") {}

    virtual bool run(void) {
      for (int first : {static_cast<int>('a'), 0x100})
        for (bool assign_before_post : {false, true}) {
          // Index 2 must reject b: its first occurrence in bba is 1.
          for (int index : {0, 1, 2, 3, 4})
            if (!extraction_matches("bba", 1, index, first, assign_before_post))
              return false;
          if (!extraction_matches("bbb", 1, 0, first, assign_before_post))
            return false;
        }
      return true;
    }
  };

  Extraction extraction;

  class ExtractionOracle : public Base {
  public:
    ExtractionOracle(void)
      : Base("String::Find::ExtractionOracle") {}

    virtual bool run(void) {
      for (int first : {static_cast<int>('a'), 0x100})
        for (Assignment source(1, "ab", 3); source(); ++source)
          for (int length = 0; length <= 2; ++length)
            for (int index = 0; index <= static_cast<int>(source[0].size()) + 1;
                 ++index)
              for (bool assign_before_post : {false, true})
                if (!extraction_matches(source[0], length, index, first,
                                        assign_before_post))
                  return false;
      return true;
    }
  };

  ExtractionOracle extraction_oracle;

  class LastA : public Test {
  private:
    int index;

    static std::string name(int index0) {
      return (index0 == 0) ? "Absent" : "Last";
    }

  public:
    LastA(int index0)
      : Test("RFind::" + name(index0), 1, "ab", 2), index(index0) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string::size_type position = assignment[0].rfind('a');
      return (position == std::string::npos) ? index == 0 :
        static_cast<int>(position) + 1 == index;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::rfind(home, Gecode::StringVar(home, "a"), x[0],
                     Gecode::IntVar(home, index, index));
    }
  };

  LastA rabsent(0);
  LastA last(2);

  class LastAB : public Test {
  private:
    int index;

    static std::string name(int index0) {
      return (index0 == 0) ? "Absent" : "Last";
    }

  public:
    LastAB(int index0)
      : Test("RFind::AB::" + name(index0), 1, "ab", 3), index(index0) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string::size_type position = assignment[0].rfind("ab");
      return (position == std::string::npos) ? index == 0 :
        static_cast<int>(position) + 1 == index;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::rfind(home, Gecode::StringVar(home, "ab"), x[0],
                     Gecode::IntVar(home, index, index));
    }
  };

  LastAB rabsent_ab(0);
  LastAB last_ab(2);

  class ReverseSpace : public Gecode::Space {
  public:
    Gecode::StringVar pattern;
    Gecode::StringVar source;
    Gecode::IntVar index;

    ReverseSpace(const Gecode::String::StringVal& pattern_value,
                 const Gecode::String::StringVal& source_value, bool assigned,
                 int first)
      : pattern(assigned ? Gecode::StringVar(*this, pattern_value) :
          Gecode::StringVar(*this,
            Gecode::String::NSIntSet(first, first + 1), 0, 2)),
        source(assigned ? Gecode::StringVar(*this, source_value) :
          Gecode::StringVar(*this,
            Gecode::String::NSIntSet(first, first + 1), 0, 3)),
        index(*this, 0, 4) {
      Gecode::rfind(*this, pattern, source, index);
      if (!assigned) {
        Gecode::rel(*this, pattern, Gecode::STRT_EQ,
                    Gecode::StringVar(*this, pattern_value));
        Gecode::rel(*this, source, Gecode::STRT_EQ,
                    Gecode::StringVar(*this, source_value));
      }
      Gecode::branch(*this, index, Gecode::INT_VAL_MIN());
    }

    ReverseSpace(ReverseSpace& s) : Gecode::Space(s) {
      pattern.update(*this, s.pattern);
      source.update(*this, s.source);
      index.update(*this, s.index);
    }

    virtual Gecode::Space* copy(void) {
      return new ReverseSpace(*this);
    }
  };

  class ReverseBoundaries : public Base {
  private:
    static int last_position(const Gecode::String::StringVal& pattern,
                             const Gecode::String::StringVal& source) {
      int last = 0;
      for (int offset = 0; offset <= static_cast<int>(source.size()); ++offset)
        if (pattern.size() <= source.size() - offset &&
            std::equal(pattern.begin(), pattern.end(), source.begin() + offset))
          last = offset + 1;
      return last;
    }

    static bool matches(const Gecode::String::StringVal& pattern,
                        const Gecode::String::StringVal& source, bool assigned,
                        int first) {
      ReverseSpace root(pattern, source, assigned, first);
      Gecode::Search::Options options;
      options.threads = 1;
      Gecode::DFS<ReverseSpace> search(&root, options);
      std::unique_ptr<ReverseSpace> solution(search.next());
      if (!solution || !solution->index.assigned() ||
          solution->index.val() != last_position(pattern, source))
        return false;
      std::unique_ptr<ReverseSpace> extra(search.next());
      return !extra;
    }

  public:
    ReverseBoundaries(void) : Base("String::RFind::Boundaries") {}

    virtual bool run(void) {
      for (int first : {static_cast<int>('a'), 0x100})
        for (Assignment pattern(1, "ab", 2); pattern(); ++pattern)
          for (Assignment source(1, "ab", 3); source(); ++source) {
            std::vector<Gecode::String::StringSymbol> p, s;
            for (char symbol : pattern[0])
              p.push_back(first + symbol - 'a');
            for (char symbol : source[0])
              s.push_back(first + symbol - 'a');
            for (bool assigned : {false, true})
              if (!matches(Gecode::String::StringVal::from_symbols(p),
                           Gecode::String::StringVal::from_symbols(s),
                           assigned, first))
                return false;
          }
      return true;
    }
  };

  ReverseBoundaries reverse_boundaries;

}}}

// STATISTICS: test-string
