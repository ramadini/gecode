#include <gecode/string.hh>
#include <gecode/string/char2code.hh>

namespace Gecode {

  void
  char2code(Home home, StringVar x, IntVar n) {
    GECODE_POST;
    GECODE_ME_FAIL(n.varimp()->gq(home, -1));
    GECODE_ME_FAIL(n.varimp()->lq(home, String::DashedString::_MAX_STR_ALPHA));
    GECODE_ME_FAIL(x.varimp()->lb(home, 0));
    GECODE_ME_FAIL(x.varimp()->ub(home, 1));
    GECODE_ES_FAIL(String::Char2Code::post(home, x, n));
  }

}
