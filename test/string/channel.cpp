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

namespace Test { namespace String { namespace Channel {

  /// Decode a digit string the way StrToNat does: -1 if \a s is empty or
  /// contains a non-digit, the (leading-zeros-tolerant) decimal value
  /// otherwise.
  static int
  str_to_nat(const std::string& s) {
    if (s.empty())
      return -1;
    for (std::string::const_iterator c = s.begin(); c != s.end(); ++c)
      if ((*c < '0') || (*c > '9'))
        return -1;
    return std::stoi(s);
  }

  /// Encode the way NatToStr does: "" for negative n, the canonical
  /// (no leading zeros) decimal representation otherwise.
  static std::string
  nat_to_str(int n) {
    return (n < 0) ? "" : std::to_string(n);
  }

  /// Test str2nat against a fixed target range for the integer variable
  class StrToNat : public Test {
  private:
    int lo;
    int hi;

    static std::string name(int lo0, int hi0) {
      std::ostringstream os;
      os << "StrToNat::" << lo0 << "To" << hi0;
      return os.str();
    }

  public:
    StrToNat(int lo0, int hi0)
      : Test("Channel::" + name(lo0, hi0), 1, "09a+", 2),
        lo(lo0), hi(hi0) {}

    virtual bool solution(const Assignment& a) const {
      const int value = str_to_nat(a[0]);
      return (value >= lo) && (value <= hi);
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::IntVar n(home, lo, hi);
      Gecode::str2nat(home, x[0], n);
    }
  };

  /// Test nat2str against a fixed source range for the integer variable
  class NatToStr : public Test {
  private:
    int lo;
    int hi;

    static std::string name(int lo0, int hi0) {
      std::ostringstream os;
      os << "NatToStr::" << lo0 << "To" << hi0;
      return os.str();
    }

  public:
    NatToStr(int lo0, int hi0)
      : Test("Channel::" + name(lo0, hi0), 1, "09a+", 2),
        lo(lo0), hi(hi0) {}

    virtual bool solution(const Assignment& a) const {
      for (int n = lo; n <= hi; ++n)
        if (nat_to_str(n) == a[0])
          return true;
      return false;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::IntVar n(home, lo, hi);
      Gecode::nat2str(home, n, x[0]);
    }
  };

  StrToNat str_to_nat_only_negative(-1, -1);
  StrToNat str_to_nat_wide(-1, 99);
  StrToNat str_to_nat_narrow(0, 5);
  StrToNat str_to_nat_unreachable(11, 50);

  NatToStr nat_to_str_only_negative(-3, -1);
  NatToStr nat_to_str_wide(-1, 99);
  NatToStr nat_to_str_narrow(0, 5);
  NatToStr nat_to_str_unreachable(11, 50);

}}}

// STATISTICS: test-string
