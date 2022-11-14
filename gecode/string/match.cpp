#include <gecode/string.hh>
#include <gecode/string/match.hh>
#include <gecode/string/ext/parse-reg.hpp>

namespace Gecode {

  void
  match(Home home, StringVar x, String::RegEx* r, IntVar i) {
    GECODE_POST;
    GECODE_ES_FAIL(String::Match::post(home, x, r, i));
  }

}
