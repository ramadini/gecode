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

namespace Test { namespace String { namespace GCC {

  class Counts : public Test {
  private:
    int a;
    int b;

    static std::string name(int a0, int b0) {
      return "Counts::" + std::to_string(a0) + "A" +
        std::to_string(b0) + "B";
    }

  public:
    Counts(int a0, int b0)
      : Test("GCC::" + name(a0, b0), 1, "abc", 3), a(a0), b(b0) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string& value = assignment[0];
      return static_cast<int>(
        std::count(value.begin(), value.end(), 'a')) == a &&
        static_cast<int>(
          std::count(value.begin(), value.end(), 'b')) == b;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::IntArgs characters;
      characters << 'a' << 'b';
      Gecode::IntVarArgs counts;
      counts << Gecode::IntVar(home, a, a) << Gecode::IntVar(home, b, b);
      Gecode::gcc(home, x[0], characters, counts);
    }
  };

  Counts no_a_or_b(0, 0);
  Counts one_a(1, 0);

  class MustChars : public Test {
  public:
    MustChars(void) : Test("GCC::MustChars", 1, "abc", 3) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string& value = assignment[0];
      return value.find('a') != std::string::npos;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::String::NSIntSet characters('a');
      Gecode::must_chars(home, x[0], characters);
    }
  };

  MustChars must_chars;

}}}

// STATISTICS: test-string
