/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *  http://www.gecode.org
 */

#include "test/string.hh"

namespace Test { namespace String { namespace Find {

  class FirstA : public Test {
  private:
    int index;

    static std::string name(int index0) {
      return (index0 == 0) ? "Absent" : "First";
    }

  public:
    FirstA(int index0)
      : Test("Find::" + name(index0), 1, "ab", 2), index(index0) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string::size_type position = assignment[0].find('a');
      return (position == std::string::npos) ? index == 0 :
        static_cast<int>(position) + 1 == index;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::find(home, Gecode::StringVar(home, "a"), x[0],
                   Gecode::IntVar(home, index, index));
    }
  };

  FirstA absent(0);
  FirstA first(1);

  class LastA : public Test {
  private:
    int index;

    static std::string name(int index0) {
      return (index0 == 0) ? "Absent" : "Last";
    }

  public:
    LastA(int index0)
      : Test("RFind::" + name(index0), 1, "ab", 2), index(index0) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string::size_type position = assignment[0].rfind('a');
      return (position == std::string::npos) ? index == 0 :
        static_cast<int>(position) + 1 == index;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::rfind(home, Gecode::StringVar(home, "a"), x[0],
                     Gecode::IntVar(home, index, index));
    }
  };

  LastA rabsent(0);
  LastA last(2);

  class LastAB : public Test {
  private:
    int index;

    static std::string name(int index0) {
      return (index0 == 0) ? "Absent" : "Last";
    }

  public:
    LastAB(int index0)
      : Test("RFind::AB::" + name(index0), 1, "ab", 3), index(index0) {}

    virtual bool solution(const Assignment& assignment) const {
      const std::string::size_type position = assignment[0].rfind("ab");
      return (position == std::string::npos) ? index == 0 :
        static_cast<int>(position) + 1 == index;
    }

    virtual void post(Gecode::Space& home, Gecode::StringVarArray& x) {
      Gecode::rfind(home, Gecode::StringVar(home, "ab"), x[0],
                     Gecode::IntVar(home, index, index));
    }
  };

  LastAB rabsent_ab(0);
  LastAB last_ab(2);

}}}

// STATISTICS: test-string
