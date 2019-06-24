#include <gecode/string.hh>
#include <gecode/string/pow.hh>

namespace Gecode {

  void
  pow(Home home, StringVar x, IntVar n, StringVar y) {
    GECODE_POST;
    GECODE_ES_FAIL(String::Pow::post(home, x, n, y));
  }

}
