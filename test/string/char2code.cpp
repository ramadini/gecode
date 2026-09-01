/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *  http://www.gecode.org
 */

#include "test/string.hh"

namespace Test { namespace String { namespace Char2Code {

  class Code : public Test {
  private:
    int code;

    static std::string name(int code0) {
      return (code0 == -1) ? "Empty" :
        std::string(1, static_cast<char>(code0));
    }

  public:
    Code(int code0)
      : Test("Char2Code::" + name(code0), 1, "ab", 1), code(code0) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string& value = assignment[0];
      return (code == -1) ? value.empty() :
        (value.size() == 1) &&
        (static_cast<unsigned char>(value[0]) == code);
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::IntVar n(home, code, code);
      Gecode::char2code(home, x[0], n);
    }
  };

  Code empty(-1);
  Code a('a');
  Code b('b');

}}}

// STATISTICS: test-string
