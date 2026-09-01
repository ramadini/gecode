/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Roberto Amadini <roberto.amadini@unibo.it>
 *
 *  Copyright:
 *     Roberto Amadini, 2026
 *
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
 *
 */

#include <gecode/driver.hh>
#include <gecode/string.hh>

using namespace Gecode;
using namespace Gecode::String;

/**
 * \brief %Example: Palindromes over a finite alphabet
 *
 * Find palindromes of length five over the alphabet \f$\{a,b,c\}\f$.
 *
 * \ingroup Example
 */
class StringPalindrome : public Script {
protected:
  StringVarArray words;
  IntVar word_length;

public:
  StringPalindrome(const Options& opt)
    : Script(opt), words(*this, 1),
      word_length(*this, 5, 5) {
    NSIntSet alphabet('a', 'c');
    words[0] = StringVar(*this, alphabet, 5, 5);

    length(*this, words[0], word_length);
    rel(*this, words[0], STRT_REV, words[0]);

    StringVarArgs candidates(words);
    branch(*this, candidates, STRING_VAR_BLOCKMIN(), STRING_VAL_LLLM());
  }

  StringPalindrome(StringPalindrome& s)
    : Script(s), word_length(s.word_length) {
    words.update(*this, s.words);
    word_length.update(*this, s.word_length);
  }

  virtual Space*
  copy(void) {
    return new StringPalindrome(*this);
  }

  virtual void
  print(std::ostream& os) const {
    os << "\tword = \"" << words[0].val() << "\"" << std::endl;
  }
};

/** \brief Main-function
 *  \relates StringPalindrome
 */
int
main(int argc, char* argv[]) {
  Options opt("String palindrome");
  opt.solutions(1);
  opt.parse(argc, argv);
  Script::run<StringPalindrome, DFS, Options>(opt);
  return 0;
}

// STATISTICS: example-any
