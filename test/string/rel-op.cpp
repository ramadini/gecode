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

#include <algorithm>

namespace Test { namespace String { namespace RelOp {

  /// Test binary concatenation, including aliases
  class Concat : public Test {
  private:
    int x0;
    int x1;
    int x2;
  public:
    Concat(const std::string& sharing, int arity,
           int x00, int x10, int x20)
      : Test("RelOp::Concat::" + sharing, arity, "ab", 2),
        x0(x00), x1(x10), x2(x20) {}

    virtual bool solution(const Assignment& a) const {
      return a[x0] + a[x1] == a[x2];
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::rel(home, x[x0], x[x1], Gecode::STRT_CAT, x[x2]);
    }
  };

  /// Test generalized concatenation, including aliases
  class GConcat : public Test {
  private:
    std::vector<int> operands;
    int result;
  public:
    GConcat(const std::string& sharing, int arity,
            const std::vector<int>& operands0, int result0)
      : Test("RelOp::GConcat::" + sharing, arity, "ab", 2),
        operands(operands0), result(result0) {}

    virtual bool solution(const Assignment& a) const {
      std::string value;
      for (std::vector<int>::const_iterator i = operands.begin();
           i != operands.end(); ++i)
        value += a[*i];
      return value == a[result];
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::StringVarArgs input(static_cast<int>(operands.size()));
      for (int i = input.size(); i--; )
        input[i] = x[operands[static_cast<unsigned int>(i)]];
      Gecode::gconcat(home, input, x[result]);
    }
  };

  /// Test string reversal, including aliases
  class Reverse : public Test {
  private:
    bool shared;
  public:
    Reverse(bool shared0)
      : Test(std::string("RelOp::Reverse::") + (shared0 ? "XX" : "XY"),
             shared0 ? 1 : 2, "ab", 2), shared(shared0) {}

    virtual bool solution(const Assignment& a) const {
      std::string value(a[0]);
      std::reverse(value.begin(), value.end());
      return value == a[shared ? 0 : 1];
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::rel(home, x[0], Gecode::STRT_REV, x[shared ? 0 : 1]);
    }
  };

  Concat concat_xyz("XYZ", 3, 0, 1, 2);
  Concat concat_xxy("XXY", 2, 0, 0, 1);
  Concat concat_xyx("XYX", 2, 0, 1, 0);
  Concat concat_xyy("XYY", 2, 0, 1, 1);
  Concat concat_xxx("XXX", 1, 0, 0, 0);

  GConcat gconcat_xyzw("XYZW", 4, std::vector<int>{0, 1, 2}, 3);
  GConcat gconcat_xxxy("XXXY", 2, std::vector<int>{0, 0, 0}, 1);
  GConcat gconcat_xyzx("XYZX", 3, std::vector<int>{0, 1, 2}, 0);
  GConcat gconcat_xxxx("XXXX", 1, std::vector<int>{0, 0, 0}, 0);

  Reverse reverse_xy(false);
  Reverse reverse_xx(true);

}}}

// STATISTICS: test-string
