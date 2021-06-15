#include <gecode/string.hh>
#include <gecode/string/rel-op.hh>

namespace Gecode { namespace String {

  template<class View0, class View1, class View2>
  void
  rel_op_post(Home home, StringRelOpType r, View0 x0, View1 x1, View2 x2) {
    using namespace String::RelOp;
    GECODE_POST;
    switch (r) {
    case STRT_CAT:
      GECODE_ES_FAIL((Concat<View0,View1,View2>::post(home, x0, x1, x2)));
      break;
    default:
      throw UnknownRelation("String::rel_op_post");
    }
  }

}}

namespace Gecode {

  void
  rel(Home home, StringRelOpType r, StringVar x, StringVar y,  StringVar z) {
    using namespace String;
    rel_op_post<StringView,StringView,StringView>(home, r, x, y, z);
  }
  
  void
  concat(Home home, StringVar x, StringVar y, StringVar z) {
    using namespace String;
    GECODE_POST;
    GECODE_ES_FAIL((RelOp::Concat<StringView,StringView,StringView>
      ::post(home,x,y,z)));
  }
  
  void
  concat(Home home, StringVar x, StringVar y, std::vector<int> w) {
    using namespace String;
    GECODE_POST;
    ConstStringView vw(home, &w[0], w.size());
    GECODE_ES_FAIL((RelOp::Concat<StringView,StringView,ConstStringView>
      ::post(home,x,y,vw)));
  }
  
}
