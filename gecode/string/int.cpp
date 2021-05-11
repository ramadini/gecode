#include <gecode/string/int.hh>

namespace Gecode {

  forceinline void
  length(Home home, StringVar x, IntVar n) {
//    using namespace String;
    GECODE_POST;
    GECODE_ES_FAIL(String::Int::Length<String::StringView>::post(home, x, n));
  }

}
