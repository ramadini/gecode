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

namespace Test { namespace String {

  void
  Assignment::words_of_length(std::string& word, int length,
                              const std::string& alphabet) {
    if (length == 0) {
      words.push_back(word);
      return;
    }
    for (std::string::const_iterator c = alphabet.begin();
         c != alphabet.end(); ++c) {
      word.push_back(*c);
      words_of_length(word, length - 1, alphabet);
      word.erase(word.size() - 1);
    }
  }

  Assignment::Assignment(int n0, const std::string& alphabet,
                         int max_length)
    : n(n0), current(static_cast<unsigned int>(n0), 0), done(n0 == 0) {
    words.push_back("");
    std::string word;
    for (int length = 1; length <= max_length; ++length)
      words_of_length(word, length, alphabet);
  }

  void
  Assignment::operator++(void) {
    for (int i = n; i--; ) {
      unsigned int& position = current[static_cast<unsigned int>(i)];
      ++position;
      if (position < words.size())
        return;
      position = 0;
    }
    done = true;
  }

  Gecode::String::NSIntSet
  TestSpace::alphabet_set(void) const {
    Gecode::String::NSIntSet chars;
    for (std::string::const_iterator c = alphabet.begin();
         c != alphabet.end(); ++c)
      chars.add(static_cast<unsigned char>(*c));
    return chars;
  }

  TestSpace::TestSpace(int n, const std::string& alphabet0, int max_length0,
                       Test* test0)
    : alphabet(alphabet0), max_length(max_length0), x(*this, n),
      r(Gecode::BoolVar(*this, 0, 1), Gecode::RM_EQV), test(test0),
      reified(false) {
    Gecode::String::NSIntSet chars = alphabet_set();
    for (int i = x.size(); i--; ) {
      Gecode::String::NSBlocks domain(
        std::vector<Gecode::String::NSBlock>(
          1, Gecode::String::NSBlock(chars, 0, max_length)));
      Gecode::rel(*this, x[i], Gecode::STRT_DOM, domain, 0, max_length);
    }
  }

  TestSpace::TestSpace(int n, const std::string& alphabet0, int max_length0,
                       Test* test0, Gecode::ReifyMode rm)
    : alphabet(alphabet0), max_length(max_length0), x(*this, n),
      r(Gecode::BoolVar(*this, 0, 1), rm), test(test0), reified(true) {
    Gecode::String::NSIntSet chars = alphabet_set();
    for (int i = x.size(); i--; ) {
      Gecode::String::NSBlocks domain(
        std::vector<Gecode::String::NSBlock>(
          1, Gecode::String::NSBlock(chars, 0, max_length)));
      Gecode::rel(*this, x[i], Gecode::STRT_DOM, domain, 0, max_length);
    }
  }

  TestSpace::TestSpace(TestSpace& s)
    : Gecode::Space(s), alphabet(s.alphabet), max_length(s.max_length),
      test(s.test), reified(s.reified) {
    x.update(*this, s.x);
    Gecode::BoolVar b;
    Gecode::BoolVar sb(s.r.var());
    b.update(*this, sb);
    r.var(b);
    r.mode(s.r.mode());
  }

  Gecode::Space*
  TestSpace::copy(void) {
    return new TestSpace(*this);
  }

  void
  TestSpace::post(void) {
    if (reified)
      test->post(*this, x, r);
    else
      test->post(*this, x);
  }

  bool
  TestSpace::failed(void) {
    if (opt.log)
      olog << ind(3) << "Fixpoint: x[]=" << x
           << (reified ? " (reified)" : "") << std::endl;
    return status() == Gecode::SS_FAILED;
  }

  bool
  TestSpace::assigned(void) const {
    for (int i = x.size(); i--; )
      if (!x[i].assigned())
        return false;
    return true;
  }

  unsigned int
  TestSpace::propagators(void) {
    return Gecode::PropagatorGroup::all.size(*this);
  }

  int
  TestSpace::rndvar(void) const {
    assert(!assigned());
    int i = static_cast<int>(Base::rand(
      static_cast<unsigned int>(x.size())));
    while (x[i].assigned())
      i = (i + 1) % x.size();
    return i;
  }

  void
  TestSpace::rel(bool value) {
    assert(reified);
    Gecode::rel(*this, r.var(), Gecode::IRT_EQ, value ? 1 : 0);
  }

  void
  TestSpace::assign(const Assignment& a, bool skip) {
    int skipped = skip ? static_cast<int>(Base::rand(
      static_cast<unsigned int>(a.size()))) : -1;
    if (opt.log && (skipped >= 0))
      olog << ind(4) << "skip x[" << skipped << "]" << std::endl;
    for (int i = a.size(); i--; )
      if (i != skipped) {
        Gecode::StringVar value(*this, a[i]);
        Gecode::rel(*this, x[i], Gecode::STRT_EQ, value);
        if (Base::fixpoint() && failed())
          return;
      }
  }

  void
  TestSpace::restrict_lengths(const Assignment& a) {
    Gecode::String::NSIntSet chars = alphabet_set();
    for (int i = a.size(); i--; ) {
      const int length = static_cast<int>(a[i].size());
      Gecode::String::NSBlocks domain(
        std::vector<Gecode::String::NSBlock>(
          1, Gecode::String::NSBlock(chars, length, length)));
      Gecode::rel(*this, x[i], Gecode::STRT_DOM, domain, length, length);
      if (Base::fixpoint() && failed())
        return;
    }
  }

  bool
  TestSpace::prune(const Assignment& a, bool test_fixpoint) {
    if (failed() || assigned())
      return true;

    int i = rndvar();
    const std::string& target = a[i];
    switch (Base::rand(3)) {
    case 0: {
      if (opt.log)
        olog << ind(4) << "assign x[" << i << "] = \"" << target
             << "\"" << std::endl;
      Gecode::StringVar value(*this, target);
      Gecode::rel(*this, x[i], Gecode::STRT_EQ, value);
      break;
    }
    case 1: {
      if (opt.log)
        olog << ind(4) << "restrict length of x[" << i << "] to "
             << target.size() << std::endl;
      Gecode::String::NSIntSet chars = alphabet_set();
      Gecode::String::NSBlocks domain(
        std::vector<Gecode::String::NSBlock>(
          1, Gecode::String::NSBlock(
            chars, static_cast<int>(target.size()),
            static_cast<int>(target.size()))));
      Gecode::rel(*this, x[i], Gecode::STRT_DOM, domain,
                  static_cast<int>(target.size()),
                  static_cast<int>(target.size()));
      break;
    }
    default: {
      if (target.empty()) {
        if (opt.log)
          olog << ind(4) << "assign x[" << i << "] = \"\""
               << std::endl;
        Gecode::StringVar value(*this, target);
        Gecode::rel(*this, x[i], Gecode::STRT_EQ, value);
        break;
      }
      std::vector<Gecode::String::NSBlock> blocks;
      blocks.reserve(target.size());
      for (std::string::const_iterator t = target.begin();
           t != target.end(); ++t) {
        Gecode::String::NSIntSet chars(static_cast<unsigned char>(*t));
        for (std::string::const_iterator c = alphabet.begin();
             c != alphabet.end(); ++c)
          if ((*c != *t) && Base::rand(2))
            chars.add(static_cast<unsigned char>(*c));
        blocks.push_back(Gecode::String::NSBlock(chars, 1, 1));
      }
      Gecode::String::NSBlocks domain(std::move(blocks));
      if (opt.log)
        olog << ind(4) << "restrict x[" << i << "] to " << domain
             << std::endl;
      Gecode::rel(*this, x[i], Gecode::STRT_DOM, domain,
                  static_cast<int>(target.size()),
                  static_cast<int>(target.size()));
      break;
    }
    }

    if (!Base::fixpoint())
      return true;
    if (failed() || !test_fixpoint)
      return true;

    std::unique_ptr<TestSpace> copy(
      static_cast<TestSpace*>(clone()));
    copy->post();
    if (copy->failed())
      return false;
    for (int j = x.size(); j--; )
      if (x[j].domain() != copy->x[j].domain())
        return false;
    return !reified || (r.var().size() == copy->r.var().size());
  }

  Test::Test(const std::string& name, int arity0,
             const std::string& alphabet0, int max_length0, bool reified0)
    : Base("String::" + name), arity(arity0), alphabet(alphabet0),
      max_length(max_length0), reified(reified0) {}

  void
  Test::post(Gecode::Space&, Gecode::StringVarArray&, Gecode::Reify) {}

  std::string
  Test::str(Gecode::StringRelType relation) {
    switch (relation) {
    case Gecode::STRT_EQ:    return "Eq";
    case Gecode::STRT_NQ:    return "Nq";
    case Gecode::STRT_LEXLT: return "LexLt";
    case Gecode::STRT_LEXLQ: return "LexLq";
    default:                 return "Unknown";
    }
  }

  bool
  Test::run(void) {
    using namespace Gecode;
    Assignment assignment(arity, alphabet, max_length);
    std::set<std::vector<std::string> > expected;

    const auto failure = [&](const char* phase, const char* problem,
                             const Assignment& a) {
      if (opt.log)
        olog << "FAILURE" << std::endl
             << ind(1) << "Test:       " << phase << std::endl
             << ind(1) << "Problem:    " << problem << std::endl
             << ind(1) << "Assignment: " << a << std::endl;
      return false;
    };

    while (assignment()) {
      const bool sol = solution(assignment);
      if (sol) {
        std::vector<std::string> value;
        for (int i = 0; i < assignment.size(); ++i)
          value.push_back(assignment[i]);
        expected.insert(value);
      }

      {
        std::unique_ptr<TestSpace> s(
          new TestSpace(arity, alphabet, max_length, this));
        s->post();
        if (Base::rand(2) && !s->failed())
          s.reset(static_cast<TestSpace*>(s->clone()));
        s->assign(assignment);
        if (s->failed() == sol)
          return failure("Assignment after posting",
                         sol ? "Failed on solution" : "Accepted non-solution",
                         assignment);
        if (sol && (s->propagators() != 0))
          return failure("Assignment after posting", "No subsumption",
                         assignment);
      }

      {
        std::unique_ptr<TestSpace> s(
          new TestSpace(arity, alphabet, max_length, this));
        s->post();
        s->assign(assignment, true);
        (void) s->failed();
        s->assign(assignment);
        if (s->failed() == sol)
          return failure("Partial assignment after posting",
                         sol ? "Failed on solution" : "Accepted non-solution",
                         assignment);
      }

      {
        std::unique_ptr<TestSpace> s(
          new TestSpace(arity, alphabet, max_length, this));
        s->assign(assignment);
        s->post();
        if (s->failed() == sol)
          return failure("Assignment before posting",
                         sol ? "Failed on solution" : "Accepted non-solution",
                         assignment);
      }

      {
        std::unique_ptr<TestSpace> s(
          new TestSpace(arity, alphabet, max_length, this));
        s->post();
        s->restrict_lengths(assignment);
        s->assign(assignment);
        if (s->failed() == sol)
          return failure("Length pruning",
                         sol ? "Pruned solution" : "Accepted non-solution",
                         assignment);
      }

      {
        std::unique_ptr<TestSpace> s(
          new TestSpace(arity, alphabet, max_length, this));
        s->post();
        unsigned int steps = 1 + Base::rand(
          static_cast<unsigned int>(max_length + 2));
        while (steps-- && !s->failed() && !s->assigned())
          if (!s->prune(assignment))
            return failure("Random pruning", "No fixpoint", assignment);
        s->assign(assignment);
        if (s->failed() == sol)
          return failure("Random pruning",
                         sol ? "Pruned solution" : "Accepted non-solution",
                         assignment);
      }

      if (reified) {
        const ReifyMode modes[] = {RM_EQV, RM_IMP, RM_PMI};
        for (unsigned int m = 0; m < sizeof(modes) / sizeof(modes[0]); ++m) {
          const ReifyMode mode = modes[m];
          {
            std::unique_ptr<TestSpace> s(new TestSpace(
              arity, alphabet, max_length, this, mode));
            s->post();
            s->rel(sol);
            s->assign(assignment);
            if (s->failed())
              return failure("Reified assignment", "Failed", assignment);
            if (s->propagators() != 0)
              return failure("Reified assignment", "No subsumption",
                             assignment);
          }
          {
            std::unique_ptr<TestSpace> s(new TestSpace(
              arity, alphabet, max_length, this, mode));
            s->post();
            s->rel(!sol);
            s->assign(assignment);
            const bool contradiction =
              (mode == RM_EQV) || ((mode == RM_IMP) && !sol) ||
              ((mode == RM_PMI) && sol);
            if (s->failed() != contradiction)
              return failure("Reified opposite", contradiction ?
                             "Did not fail" : "Failed", assignment);
            if (!contradiction && (s->propagators() != 0))
              return failure("Reified opposite", "No subsumption",
                             assignment);
          }
          {
            std::unique_ptr<TestSpace> s(new TestSpace(
              arity, alphabet, max_length, this, mode));
            s->assign(assignment);
            s->post();
            if (s->failed())
              return failure("Reified result", "Failed", assignment);
            const bool forced = (mode == RM_EQV) ||
              ((mode == RM_IMP) && !sol) || ((mode == RM_PMI) && sol);
            if (forced && (!s->r.var().assigned() ||
                (s->r.var().val() != static_cast<int>(sol))))
              return failure("Reified result", "Wrong control value",
                             assignment);
            if (!forced && (s->propagators() != 0))
              return failure("Reified result", "No subsumption",
                             assignment);
          }
        }
      }

      ++assignment;
    }

    std::unique_ptr<TestSpace> root(
      new TestSpace(arity, alphabet, max_length, this));
    root->post();
    StringVarArgs vars(root->x);
    branch(*root, vars, STRING_VAR_NONE(), STRING_VAL_LLLL());
    Search::Options options;
    options.threads = 1;
    DFS<TestSpace> search(root.get(), options);
    root.reset();
    std::set<std::vector<std::string> > actual;
    while (TestSpace* solution = search.next()) {
      std::unique_ptr<TestSpace> s(solution);
      std::vector<std::string> value;
      for (int i = 0; i < s->x.size(); ++i) {
        if (!s->x[i].assigned())
          return false;
        value.push_back(s->x[i].val());
      }
      if (!actual.insert(value).second)
        return false;
    }
    if (actual != expected) {
      if (opt.log)
        olog << "FAILURE" << std::endl
             << ind(1) << "Test:       Search" << std::endl
             << ind(1) << "Problem:    Wrong solution set" << std::endl
             << ind(1) << "Expected:   " << expected.size() << std::endl
             << ind(1) << "Actual:     " << actual.size() << std::endl;
      return false;
    }

    return true;
  }

}}

std::ostream&
operator<<(std::ostream& os, const Test::String::Assignment& a) {
  os << "{";
  for (int i = 0; i < a.size(); ++i)
    os << '"' << a[i] << '"' << ((i + 1 == a.size()) ? "}" : ",");
  return os;
}

// STATISTICS: test-string
