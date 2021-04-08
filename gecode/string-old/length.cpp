#include <gecode/string.hh>
#include <gecode/string/length.hh>

namespace Gecode {

  void
  length(Home home, StringVar s, IntVar l) {
    GECODE_POST;
    GECODE_ES_FAIL(String::Length::post(home, s, l));
  }

}
