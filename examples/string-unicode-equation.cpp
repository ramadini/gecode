/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Mikael Zayenz Lagerkvist <lagerkvist@gecode.dev>
 *
 *  Copyright:
 *     Mikael Zayenz Lagerkvist, 2026
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
 * \brief Solve the Unicode word equation X alpha-smile = smile-alpha X
 *
 * The alphabet consists of U+03B1 (alpha) and U+1F642 (smile). Lengths
 * count Unicode scalar values, so the smile occupies one position, not
 * four UTF-8 bytes. For 1 <= |X| <= 7 there are four solutions.
 * Run string-unicode-equation -solutions 0 to enumerate them all.
 *
 * \ingroup Example
 */
class StringUnicodeEquation : public Script {
protected:
  StringVar word;

public:
  StringUnicodeEquation(const Options& opt)
    : Script(opt),
      word(*this, 1, 7, StringAlphabet::from_codepoints({0x03B1, 0x1F642})) {
    StringVar suffix(*this, StringVal::from_symbols({0x03B1, 0x1F642}));
    StringVar prefix(*this, StringVal::from_symbols({0x1F642, 0x03B1}));
    StringVar equation(*this, 3, 9,
                       StringAlphabet::from_codepoints({0x03B1, 0x1F642}));
    rel(*this, word, suffix, STRT_CAT, equation);
    rel(*this, prefix, word, STRT_CAT, equation);

    StringVarArgs candidates(1);
    candidates[0] = word;
    branch(*this, candidates, STRING_VAR_BLOCKMIN(), STRING_VAL_LLLM());
  }

  StringUnicodeEquation(StringUnicodeEquation& s) : Script(s) {
    word.update(*this, s.word);
  }

  virtual Space*
  copy(void) {
    return new StringUnicodeEquation(*this);
  }

  virtual void
  print(std::ostream& os) const {
    os << "\tword = \"" << word.val_symbols().to_utf8() << "\"" << std::endl;
  }
};

int
main(int argc, char* argv[]) {
  Options opt("Unicode word equation");
  opt.solutions(1);
  opt.parse(argc, argv);
  Script::run<StringUnicodeEquation, DFS, Options>(opt);
  return 0;
}

// STATISTICS: example-any
