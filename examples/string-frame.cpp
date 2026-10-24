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
 * \brief Synthesize self-length-prefixed frames
 *
 * A frame is decimal(total length) + ":" + payload. The payload has
 * length 2..8, belongs to (ab|ba)(ab|ba)*, contains aba, and excludes bb.
 * The prefix has width 1..2; nat2str supplies canonical decimal digits.
 * Thus an eight-symbol payload needs the two-digit prefix 11, rather
 * than 10: the prefix itself contributes to the total length.
 * Run string-frame -solutions 0 for all eleven frames.
 *
 * \ingroup Example
 */
class StringFrame : public Script {
protected:
  StringVar payload;
  StringVar frame;

public:
  StringFrame(const Options& opt)
    : Script(opt), payload(*this, NSIntSet('a', 'b'), 2, 8),
      frame(*this, 4, 11, StringAlphabet::ascii()) {
    StringVar prefix(*this, NSIntSet('0', '9'), 1, 2);
    IntVar total_length(*this, 4, 11);
    length(*this, frame, total_length);
    nat2str(*this, total_length, prefix);

    extensional(*this, payload, "(ab|ba)(ab|ba)*");
    contains(*this, payload, StringVar(*this, "aba"));
    contains(*this, payload, StringVar(*this, "bb"), BoolVar(*this, 0, 0));

    StringVarArgs parts(3);
    parts[0] = prefix;
    parts[1] = StringVar(*this, ":");
    parts[2] = payload;
    gconcat(*this, parts, frame);

    StringVarArgs candidates(1);
    candidates[0] = payload;
    branch(*this, candidates, STRING_VAR_BLOCKMIN(), STRING_VAL_LLLM());
  }

  StringFrame(StringFrame& s) : Script(s) {
    payload.update(*this, s.payload);
    frame.update(*this, s.frame);
  }

  virtual Space*
  copy(void) {
    return new StringFrame(*this);
  }

  virtual void
  print(std::ostream& os) const {
    os << "\tframe = \"" << frame.val() << "\"" << std::endl;
  }
};

int
main(int argc, char* argv[]) {
  Options opt("Self-length-prefixed frame");
  opt.solutions(1);
  opt.parse(argc, argv);
  Script::run<StringFrame, DFS, Options>(opt);
  return 0;
}

// STATISTICS: example-any
