#include <gecode/string.hh>
#include <gecode/string/element.hh>

namespace Gecode {

  void
  element(Home home, StringVarArgs a, IntVar i, StringVar x) {
    GECODE_POST;
    ViewArray<String::StringView> sa(home, a);
    GECODE_ES_FAIL((String::Element::post(home, sa, i, x)));
  }

}

