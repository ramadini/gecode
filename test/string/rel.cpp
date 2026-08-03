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

namespace Test { namespace String { namespace Rel {

  /// Test a binary string relation, with optional aliasing
  class Binary : public Test {
  private:
    Gecode::StringRelType relation;
    bool shared;
  public:
    Binary(Gecode::StringRelType relation0, bool shared0)
      : Test("Rel::" + str(relation0) + (shared0 ? "::XX" : "::XY"),
             shared0 ? 1 : 2, "ac", 2, true),
        relation(relation0), shared(shared0) {}

    virtual bool solution(const Assignment& a) const {
      const std::string& x = a[0];
      const std::string& y = a[shared ? 0 : 1];
      switch (relation) {
      case Gecode::STRT_EQ:    return x == y;
      case Gecode::STRT_NQ:    return x != y;
      case Gecode::STRT_LEXLT: return x < y;
      case Gecode::STRT_LEXLQ: return x <= y;
      default:                 return false;
      }
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::rel(home, x[0], relation, x[shared ? 0 : 1]);
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x,
                      Gecode::Reify r) {
      Gecode::rel(home, x[0], relation, x[shared ? 0 : 1], r);
    }
  };

  Binary eq_xy(Gecode::STRT_EQ, false);
  Binary nq_xy(Gecode::STRT_NQ, false);
  Binary lt_xy(Gecode::STRT_LEXLT, false);
  Binary lq_xy(Gecode::STRT_LEXLQ, false);
  Binary eq_xx(Gecode::STRT_EQ, true);
  Binary nq_xx(Gecode::STRT_NQ, true);
  Binary lt_xx(Gecode::STRT_LEXLT, true);
  Binary lq_xx(Gecode::STRT_LEXLQ, true);

}}}

// STATISTICS: test-string
