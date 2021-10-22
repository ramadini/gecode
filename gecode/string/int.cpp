#include <gecode/string/int.hh>

namespace Gecode {

  void
  length(Home home, StringVar x, IntVar n) {
    using namespace String;
    GECODE_POST;
    GECODE_ES_FAIL(String::Int::Length<StringView>::post(home, x, n));
  }
  
  void
  pow(Home home, StringVar x, IntVar n, StringVar y) {
    using namespace String;
    GECODE_POST;
    GECODE_ES_FAIL((String::Int::Pow<StringView,StringView>::post(home,x,n,y)));
  }

}
