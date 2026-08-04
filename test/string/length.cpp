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

#include <sstream>

namespace Test { namespace String { namespace Length {

  /// Test string length against an integer interval
  class Range : public Test {
  private:
    int min;
    int max;

    static std::string name(int min, int max) {
      std::ostringstream os;
      if (min == max)
        os << "Exact::" << min;
      else
        os << "Range::" << min << "To" << max;
      return os.str();
    }

  public:
    Range(int min0, int max0)
      : Test("Length::" + name(min0, max0), 1, "ab", 3),
        min(min0), max(max0) {}

    virtual bool solution(const Assignment& a) const {
      const int length = static_cast<int>(a[0].size());
      return (length >= min) && (length <= max);
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::IntVar length(home, min, max);
      Gecode::length(home, x[0], length);
    }
  };

  Range exact_empty(0, 0);
  Range exact_two(2, 2);
  Range one_to_two(1, 2);
  Range outside_domain(4, 4);

}}}

// STATISTICS: test-string
