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

  /// Parse the short decimal strings enumerated by these tests.
  static int
  str_to_nat(const std::string& s) {
    if (s.empty())
      return -1;
    int value = 0;
    for (std::string::const_iterator c = s.begin(); c != s.end(); ++c) {
      if ((*c < '0') || (*c > '9'))
        return -1;
      value = 10 * value + (*c - '0');
    }
    return value;
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
    StrToNat(int lo0, int hi0, const std::string& alphabet0 = "09a+",
             int max_length0 = 2)
      : Test("Channel::" + name(lo0, hi0), 1, alphabet0, max_length0),
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
    int min_length;

    static std::string name(int lo0, int hi0) {
      std::ostringstream os;
      os << "NatToStr::" << lo0 << "To" << hi0;
      return os.str();
    }

  public:
    NatToStr(int lo0, int hi0, const std::string& alphabet0 = "09a+",
             int max_length0 = 2, int min_length0 = 0)
      : Test("Channel::" + name(lo0, hi0), 1, alphabet0, max_length0),
        lo(lo0), hi(hi0), min_length(min_length0) {}

    virtual bool solution(const Assignment& a) const {
      const std::string& s = a[0];
      if (static_cast<int>(s.size()) < min_length)
        return false;
      if (s.empty())
        return lo < 0;
      if ((hi < 0) || ((s.size() > 1) && (s[0] == '0')))
        return false;
      int value = 0;
      for (std::string::const_iterator c = s.begin(); c != s.end(); ++c) {
        if ((*c < '0') || (*c > '9'))
          return false;
        const int digit = *c - '0';
        if ((value > hi / 10) ||
            ((value == hi / 10) && (digit > hi % 10)))
          return false;
        value = 10 * value + digit;
      }
      return value >= lo;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      if (min_length > 0) {
        Gecode::String::NSBlocks domain(1, Gecode::String::NSBlock(
          Gecode::String::NSIntSet('0', '9'), min_length, max_length));
        Gecode::rel(home, x[0], Gecode::STRT_DOM, domain,
                    min_length, max_length);
      }
      Gecode::IntVar n(home, lo, hi);
      Gecode::nat2str(home, n, x[0]);
    }
  };

  StrToNat str_to_nat_only_negative(-1, -1);
  StrToNat str_to_nat_wide(-1, 99);
  StrToNat str_to_nat_narrow(0, 5);
  StrToNat str_to_nat_unreachable(11, 50);
  StrToNat str_to_nat_three_digits(0, 999, "12", 3);
  StrToNat str_to_nat_digit_boundary(9, 101, "019", 3);
  StrToNat str_to_nat_leading_zeros(1, 101, "019", 4);
  StrToNat str_to_nat_invalid(-1, 101, "019a", 3);
  StrToNat str_to_nat_equal_width(100, 219, "0129", 3);
  StrToNat str_to_nat_four_digits(10, 1001, "012", 4);

  NatToStr nat_to_str_only_negative(-3, -1);
  NatToStr nat_to_str_wide(-1, 99);
  NatToStr nat_to_str_narrow(0, 5);
  NatToStr nat_to_str_unreachable(11, 50);
  NatToStr nat_to_str_zero(-1, 0, "01", 1, 1);
  NatToStr nat_to_str_max_digits(0, Gecode::Int::Limits::max, "9", 10, 9);

}}}

// STATISTICS: test-string
