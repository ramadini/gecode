#include <gecode/string/globals.hh>

namespace Gecode {

  void
  element(Home home, StringVarArgs x, IntVar i, StringVar y) {
    using namespace String;
    GECODE_POST;
    ViewArray<String::StringView> vx(home, x);
    GECODE_ES_FAIL(String::Globals::Element<StringView>::post(home, vx, i, y));
  }
  
}
