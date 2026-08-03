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

#ifndef __GECODE_TEST_STRING_HH__
#define __GECODE_TEST_STRING_HH__

#include "test/test.hh"

#include <gecode/string.hh>

#include <string>
#include <vector>

namespace Test {

  /// Testing bounded strings
  namespace String {

    /// Generate all bounded string assignments over a finite alphabet
    class Assignment {
    private:
      int n;
      std::vector<std::string> words;
      std::vector<unsigned int> current;
      bool done;

      void words_of_length(std::string& word, int length,
                           const std::string& alphabet);
    public:
      /// Initialize assignments for \a n0 variables
      Assignment(int n0, const std::string& alphabet, int max_length);
      /// Test whether all assignments have been iterated
      bool operator()(void) const;
      /// Move to the next assignment
      void operator++(void);
      /// Return value for variable \a i
      const std::string& operator[](int i) const;
      /// Return number of variables
      int size(void) const;
    };

    class Test;

    /// Space for executing bounded string tests
    class TestSpace : public Gecode::Space {
    private:
      std::string alphabet;
      int max_length;

      Gecode::String::NSIntSet alphabet_set(void) const;
      int rndvar(void) const;
    public:
      /// Variables to be tested
      Gecode::StringVarArray x;
      /// Reification information
      Gecode::Reify r;
      /// The test currently run
      Test* test;
      /// Whether the test is for a reified propagator
      bool reified;

      /// Create a non-reified test space
      TestSpace(int n, const std::string& alphabet, int max_length,
                Test* test);
      /// Create a reified test space
      TestSpace(int n, const std::string& alphabet, int max_length,
                Test* test, Gecode::ReifyMode rm);
      /// Constructor for cloning \a s
      TestSpace(TestSpace& s);
      /// Copy space during cloning
      virtual Gecode::Space* copy(void);

      /// Post the tested propagator
      void post(void);
      /// Compute a fixpoint and check for failure
      bool failed(void);
      /// Test whether all string variables are assigned
      bool assigned(void) const;
      /// Return the number of propagators
      unsigned int propagators(void);
      /// Assign the reification control variable
      void rel(bool value);
      /// Assign all variables, optionally skipping one
      void assign(const Assignment& a, bool skip=false);
      /// Restrict every variable to the length of its assigned value
      void restrict_lengths(const Assignment& a);
      /// Apply one random target-preserving domain refinement
      bool prune(const Assignment& a, bool test_fixpoint=true);
    };

    /// Base class for bounded string constraint tests
    class Test : public Base {
    protected:
      int arity;
      std::string alphabet;
      int max_length;
      bool reified;
    public:
      /// Create and register a bounded string test
      Test(const std::string& name, int arity,
           const std::string& alphabet, int max_length,
           bool reified=false);
      /// Check whether assignment \a a is a solution
      virtual bool solution(const Assignment& a) const = 0;
      /// Post the tested constraint
      virtual void post(Gecode::Space& home,
                        Gecode::StringVarArray& x) = 0;
      /// Post the reified tested constraint
      virtual void post(Gecode::Space& home, Gecode::StringVarArray& x,
                        Gecode::Reify r);
      /// Perform test
      virtual bool run(void);
      /// Map string relation to a test name
      static std::string str(Gecode::StringRelType relation);
    };

  }
}

/// Print assignment \a a
std::ostream&
operator<<(std::ostream& os, const Test::String::Assignment& a);

#include "test/string.hpp"

#endif

// STATISTICS: test-string
