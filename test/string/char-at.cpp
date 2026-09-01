/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *  http://www.gecode.org
 */

#include "test/string.hh"

#include <sstream>

namespace Test { namespace String { namespace CharAt {

  class Position : public Test {
  private:
    int index;

    static std::string name(int index0) {
      std::ostringstream os;
      os << "Position::" << index0;
      return os.str();
    }

  public:
    Position(int index0)
      : Test("CharAt::" + name(index0), 2, "ab", 2), index(index0) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string& source = assignment[0];
      const std::string& result = assignment[1];
      return (index < 1 || index > static_cast<int>(source.size()))
        ? result.empty()
        : result == source.substr(index - 1, 1);
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::IntVar index_var(home, index, index);
      Gecode::substr(home, x[0], index_var, index_var, x[1]);
    }
  };

  Position before_start(0);
  Position first(1);
  Position second(2);

}}}

// STATISTICS: test-string
