/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *  http://www.gecode.org
 */

#include "test/string.hh"

#include <sstream>

namespace Test { namespace String { namespace Substring {

  class Slice : public Test {
  private:
    int from;
    int to;

    static std::string name(int from0, int to0) {
      std::ostringstream os;
      os << "Slice::" << from0 << "To" << to0;
      return os.str();
    }

    static std::string slice(const std::string& source, int from, int to) {
      const int start = std::max(1, from);
      const int end = std::min(static_cast<int>(source.size()), to);
      return (end < start) ? "" : source.substr(start - 1, end - start + 1);
    }

  public:
    Slice(int from0, int to0)
      : Test("Substring::" + name(from0, to0), 2, "ab", 2),
        from(from0), to(to0) {}

    virtual bool solution(const Assignment& assignment) const {
      return assignment[1] == slice(assignment[0], from, to);
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::IntVar first(home, from, from);
      Gecode::IntVar last(home, to, to);
      Gecode::substr(home, x[0], first, last, x[1]);
    }
  };

  Slice first_two(1, 2);
  Slice after_end(3, 4);

}}}

// STATISTICS: test-string
