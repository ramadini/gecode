#include <gecode/string.hh>
#include <gecode/string/rel-op.hh>

namespace Gecode { namespace String {

  template<class View0, class View1, class View2>
  void
  rel_op_post(Home home, StringOpType r, View0 x0, View1 x1, View2 x2) {
    using namespace String::RelOp;
    GECODE_POST;
    switch (r) {
    case STRT_CAT:
      concat(home, x0, x1, x2);
      break;
    default:
      throw UnknownRelation("String::rel_op_post");
    }
  }
  
  template<class View0, class View1>
  void
  rel_op_post(Home home, StringOpType r, View0 x0, View1 x1) {
    using namespace String::RelOp;
    GECODE_POST;
    switch (r) {
    case STRT_REV:
      reverse(home,x0,x1);
      break;
    default:
      throw UnknownRelation("String::rel_op_post");
    }
  }

}}

namespace Gecode {

  void
  rel(Home home, StringOpType r, StringVar x, StringVar y,  StringVar z) {
    using namespace String;
    rel_op_post<StringView,StringView,StringView>(home, r, x, y, z);
  }
  
  void
  rel(Home home, StringOpType r, StringVar x, StringVar y) {
    using namespace String;
    rel_op_post<StringView,StringView>(home, r, x, y);
  }
  
  void
  concat(Home home, StringVar x, StringVar y, StringVar z) {
    using namespace String;
    if (z.assigned()) {
      concat(home, x, y, z.val());
      return;
    }
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
  
  void
  reverse(Home home, StringVar x, StringVar y) {
    using namespace String;
    if (x.assigned()) {
      eq(home, y, x.val());
      return;
    }
    else if (y.assigned()) {
      eq(home, x, y.val());
      return;
    }
    GECODE_POST;
    GECODE_ES_FAIL((RelOp::Reverse<StringView,StringView>::post(home,x,y)));
  }
  
  void
  reverse(Home home, StringVar x, std::vector<int> w) {
    using namespace String;
    StringView vx(x);
    GECODE_POST;
    if (w.empty()) {
      ConstStringView vw;
      GECODE_ES_FAIL((Rel::Eq<StringView,ConstStringView>::post(home, vx, vw)));
    }
    else {
      std::reverse(std::begin(w), std::end(w));
      ConstStringView vw(home, &w[0], w.size());
      GECODE_ES_FAIL((Rel::Eq<StringView,ConstStringView>::post(home, vx, vw)));
    }
  }
  
  void
  replace(Home home, StringVar x, StringVar q, StringVar t, StringVar y) {
    using namespace String;
    GECODE_POST;
    ViewArray<StringView> v(home, 4);
    v[0] = x;
    v[1] = q;
    v[2] = t;
    v[3] = y;
    GECODE_ES_FAIL(RelOp::Replace<StringView>::post(home,v));
  }
  void
  replace_last(Home home, StringVar x, StringVar q, StringVar t, StringVar y) {
    using namespace String;
    GECODE_POST;
    ViewArray<StringView> v(home, 4);
    v[0] = x;
    v[1] = q;
    v[2] = t;
    v[3] = y;
    GECODE_ES_FAIL(RelOp::Replace<StringView>::post(home,v,false,true));
  }
  void
  replace_all(Home home, StringVar x, StringVar q, StringVar t, StringVar y) {
    using namespace String;
    GECODE_POST;
    ViewArray<StringView> v(home, 4);
    v[0] = x;
    v[1] = q;
    v[2] = t;
    v[3] = y;
    GECODE_ES_FAIL(RelOp::Replace<StringView>::post(home,v,true,false));
  }
  
}
