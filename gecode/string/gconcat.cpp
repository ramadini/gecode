#include <gecode/string.hh>
#include <gecode/string/gconcat.hh>

namespace Gecode {

  void
  gconcat(Home home, StringVarArgs x, StringVar y) {
    ViewArray<String::StringView> v(home, x);
    switch (v.size()) {
    case 0:
      home.fail();
      return;
    case 1:
      rel(home, v[0], STRT_EQ, y);
      return;
    case 2:
      rel(home, v[0], v[1], STRT_CAT, y);
      return;
    default:
      GECODE_ES_FAIL((String::GConcat::post(home, v, y)));
    }
  }

}
