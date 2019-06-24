#include <gecode/string.hh>
#include <gecode/string/concat.hh>
#include <gecode/string/rev.hh>
#include <gecode/string/ucase.hh>
#include <gecode/string/lcase.hh>

namespace Gecode { namespace String {

  void
  rel_op_post(
    Home home, StringView x0, StringView x1, StringOpType ot, StringView x2
  ) {
    GECODE_POST;
    switch (ot) {
    case STRT_CAT:
      GECODE_ES_FAIL((Concat::post(home, x0, x1, x2)));
      break;
    default:
      throw UnknownRelation("String::rel-op");
    }
  }

  template<class View0, class View1>
  void
  rel_op_post(Home home, View0 x0, StringOpType ot, View1 x1) {
    GECODE_POST;
    switch (ot) {
    case STRT_REV:
      GECODE_ES_FAIL((Rev::post(home, x0, x1)));
      break;
    case STRT_UCASE:
      GECODE_ES_FAIL((UpperCase::post(home, x0, x1)));
      break;
    case STRT_LCASE:
      GECODE_ES_FAIL((LowerCase::post(home, x0, x1)));
      break;
    default:
      throw UnknownRelation("String::rel-op");
    }
  }

}}

namespace Gecode {

  void
  rel(Home home, StringVar x, StringVar y, StringOpType o, StringVar z) {
    using namespace String;
    rel_op_post(home, x, y, o, z);
  }

  void
  rel(Home home, StringVar x, StringOpType o, StringVar y) {
    using namespace String;
    rel_op_post<StringView, StringView>(home, x, o, y);
  }

}