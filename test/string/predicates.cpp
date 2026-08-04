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

namespace Test { namespace String { namespace Predicates {

  /// Test prefix, suffix, and containment predicates
  class Predicate : public Test {
  public:
    enum Kind {
      STARTS_WITH,
      ENDS_WITH,
      CONTAINS
    };

  private:
    Kind kind;
    bool shared;

    static std::string name(Kind kind) {
      switch (kind) {
      case STARTS_WITH: return "StartsWith";
      case ENDS_WITH:   return "EndsWith";
      case CONTAINS:    return "Contains";
      default:          GECODE_NEVER;
      }
      return "Unknown";
    }

    void post(Gecode::Space& home, Gecode::StringVar x,
              Gecode::StringVar y) const {
      switch (kind) {
      case STARTS_WITH: Gecode::startswith(home, x, y); break;
      case ENDS_WITH:   Gecode::endswith(home, x, y); break;
      case CONTAINS:    Gecode::contains(home, x, y); break;
      default:          GECODE_NEVER;
      }
    }

    void post(Gecode::Space& home, Gecode::StringVar x,
              Gecode::StringVar y, Gecode::BoolVar b) const {
      switch (kind) {
      case STARTS_WITH: Gecode::startswith(home, x, y, b); break;
      case ENDS_WITH:   Gecode::endswith(home, x, y, b); break;
      case CONTAINS:    Gecode::contains(home, x, y, b); break;
      default:          GECODE_NEVER;
      }
    }

  public:
    Predicate(Kind kind0, bool shared0)
      : Test("Predicates::" + name(kind0) +
             (shared0 ? "::XX" : "::XY"),
             shared0 ? 1 : 2, "ab", 2, true),
        kind(kind0), shared(shared0) {}

    virtual bool solution(const Assignment& a) const {
      const std::string& x = a[0];
      const std::string& y = a[shared ? 0 : 1];
      switch (kind) {
      case STARTS_WITH:
        return (x.size() >= y.size()) &&
          (x.compare(0, y.size(), y) == 0);
      case ENDS_WITH:
        return (x.size() >= y.size()) &&
          (x.compare(x.size() - y.size(), y.size(), y) == 0);
      case CONTAINS:
        return x.find(y) != std::string::npos;
      default:
        return false;
      }
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      post(home, x[0], x[shared ? 0 : 1]);
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x,
                      Gecode::Reify r) {
      Gecode::BoolVar b(home, 0, 1);
      post(home, x[0], x[shared ? 0 : 1], b);
      switch (r.mode()) {
      case Gecode::RM_EQV:
        Gecode::rel(home, b, Gecode::IRT_EQ, r.var());
        break;
      case Gecode::RM_IMP:
        Gecode::rel(home, r.var(), Gecode::IRT_LQ, b);
        break;
      case Gecode::RM_PMI:
        Gecode::rel(home, b, Gecode::IRT_LQ, r.var());
        break;
      default:
        GECODE_NEVER;
      }
    }
  };

  Predicate starts_with_xy(Predicate::STARTS_WITH, false);
  Predicate starts_with_xx(Predicate::STARTS_WITH, true);
  Predicate ends_with_xy(Predicate::ENDS_WITH, false);
  Predicate ends_with_xx(Predicate::ENDS_WITH, true);
  Predicate contains_xy(Predicate::CONTAINS, false);
  Predicate contains_xx(Predicate::CONTAINS, true);

}}}

// STATISTICS: test-string
