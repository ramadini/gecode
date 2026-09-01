/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *  http://www.gecode.org
 */

#include "test/string.hh"

namespace Test { namespace String { namespace Element {

  class ConstantCandidates : public Test {
  public:
    ConstantCandidates(void)
      : Test("Element::ConstantCandidates", 1, "ab", 1) {}

    virtual bool solution(const Assignment& assignment) const {
      return assignment[0].empty() || (assignment[0] == "a");
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::StringVarArgs candidates;
      candidates << Gecode::StringVar(home, "")
                 << Gecode::StringVar(home, "a");
      Gecode::IntVar index(home, 0, 3);
      Gecode::element(home, candidates, index, x[0]);
    }
  };

  ConstantCandidates constant_candidates;

}}}

// STATISTICS: test-string
