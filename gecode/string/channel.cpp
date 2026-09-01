#include <gecode/string.hh>
#include <gecode/string/channel.hh>

namespace Gecode {

  void
  str2nat(Home home, StringVar x, IntVar n) {
    GECODE_POST;
    GECODE_ES_FAIL((String::StrToNat::post(home, x, n)));
  }

  void
  nat2str(Home home, IntVar n, StringVar x) {
    GECODE_POST;
    GECODE_ES_FAIL((String::NatToStr::post(home, n, x)));
  }

}
