#include <gecode/string.hh>
#include <gecode/string/match.hh>
#include <gecode/string/ext/parse-reg.hpp>

namespace Gecode {

  void
  match(Home home, StringVar x, string r, IntVar i) {
    GECODE_POST;
    GECODE_ES_FAIL(String::Match::post(home, x, r, i));
  }
  
  void
  match_new(Home home, StringVar x, string r, IntVar i) {
    GECODE_POST;
    GECODE_ES_FAIL(String::MatchNew::post(home, x, r, i));
  }

}
