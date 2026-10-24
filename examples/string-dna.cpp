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
 * \brief Find a shortest DNA string containing four motifs
 *
 * ATG, TGA, GAT, and TAC must all occur, with overlaps allowed. Their
 * concatenation gives the upper bound 12; every motif has length three.
 * Branch-and-bound minimizes the string length. Run string-dna without
 * a solution limit to finish the proof: the unique optimum is ATGATAC,
 * of length seven. The default -solutions 0 completes BAB search; setting
 * a positive limit can stop at an incumbent before optimality is proved.
 *
 * \ingroup Example
 */
class StringDNA : public IntMinimizeScript {
protected:
  StringVar sequence;
  IntVar sequence_length;

public:
  StringDNA(const Options& opt)
    : IntMinimizeScript(opt),
      sequence(*this, 3, 12,
               StringAlphabet::from_codepoints({'A', 'C', 'G', 'T'})),
      sequence_length(*this, 3, 12) {
    length(*this, sequence, sequence_length);
    const char* motifs[] = {"ATG", "TGA", "GAT", "TAC"};
    for (const char* motif : motifs)
      contains(*this, sequence, StringVar(*this, motif));

    branch(*this, sequence_length, INT_VAL_MIN());
    StringVarArgs candidates(1);
    candidates[0] = sequence;
    branch(*this, candidates, STRING_VAR_BLOCKMIN(), STRING_VAL_LLLM());
  }

  StringDNA(StringDNA& s) : IntMinimizeScript(s) {
    sequence.update(*this, s.sequence);
    sequence_length.update(*this, s.sequence_length);
  }

  virtual Space*
  copy(void) {
    return new StringDNA(*this);
  }

  virtual IntVar
  cost(void) const {
    return sequence_length;
  }

  virtual void
  print(std::ostream& os) const {
    os << "\tsequence = \"" << sequence.val()
       << "\", length = " << sequence_length.val() << std::endl;
  }
};

int
main(int argc, char* argv[]) {
  Options opt("Shortest DNA motif superstring");
  opt.solutions(0);
  opt.parse(argc, argv);
  IntMinimizeScript::run<StringDNA, BAB, Options>(opt);
  return 0;
}

// STATISTICS: example-any
