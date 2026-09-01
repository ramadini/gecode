#include <gecode/string.hh>
#include <gecode/string/match.hh>
#include <gecode/string/extensional/parse-reg.hpp>

namespace Gecode {

  void
  match(Home home, StringVar x, string r, IntVar i) {
    GECODE_POST;
    GECODE_ES_FAIL(String::post_match(
      home, x, r, i, String::MATCH_AUTO));
  }

}
