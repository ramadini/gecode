#include <gecode/string.hh>
#include <gecode/string/rel.hh>


namespace Gecode { namespace String {

  template<class View0, class View1>
  void
  rel_post(Home home, View0 x0, StringRelType r, View1 x1) {
    using namespace String::Rel;
    GECODE_POST;
    switch (r) {
    case STRT_EQ:
      GECODE_ES_FAIL((Eq<View0,View1>::post(home,x0,x1)));
      break;
    case STRT_NQ:
      GECODE_ES_FAIL((Nq<View0,View1>::post(home,x0,x1)));
      break;
    default:
      throw UnknownRelation("String::rel");
    }
  }
  
  template<class View0, class View1, ReifyMode rm>
  void
  rel_re(Home home, View0 x, StringRelType r, View1 y, BoolVar b) {
    using namespace String::Rel;
//    using namespace String::RelOp;
    GECODE_POST;
    switch (r) {
    case STRT_EQ:
      GECODE_ES_FAIL((ReEq<View0,View1,Gecode::Int::BoolView,rm>
                      ::post(home, x,y,b)));
      break;
    case STRT_NQ:
      {
        Gecode::Int::NegBoolView notb(b);
        switch (rm) {
        case RM_EQV:
          GECODE_ES_FAIL((ReEq<View0,View1,Gecode::Int::NegBoolView,RM_EQV>
                         ::post(home,x,y,notb)));
          break;
        case RM_IMP:
          GECODE_ES_FAIL((ReEq<View0,View1,Gecode::Int::NegBoolView,RM_PMI>
                         ::post(home,x,y,notb)));
          break;
        case RM_PMI:
          GECODE_ES_FAIL((ReEq<View0,View1,Gecode::Int::NegBoolView,RM_IMP>
                         ::post(home,x,y,notb)));
          break;
        default: throw Gecode::Int::UnknownReifyMode("String::rel");
        }
      }
      break;
    default:
      throw UnknownRelation("String::rel");
    }
  }
  
}}

namespace Gecode {

  void
  rel(Home home, StringVar x, StringRelType r, StringVar y) {
    using namespace String;
    rel_post<StringView,StringView>(home,x,r,y);
  }

  void
  rel(Home home, StringVar x, StringRelType rt, StringVar y, Reify r) {
    using namespace String;
    switch (r.mode()) {
    case RM_EQV:
      rel_re<StringView,StringView,RM_EQV>(home,x,rt,y,r.var());
      break;
    case RM_IMP:
      rel_re<StringView,StringView,RM_IMP>(home,x,rt,y,r.var());
      break;
    case RM_PMI:
      rel_re<StringView,StringView,RM_PMI>(home,x,rt,y,r.var());
      break;
    default: throw Gecode::Int::UnknownReifyMode("String::rel");
    }
  }
 
  void
  eq(Home home, StringVar x, StringVar y) {
    using namespace String;
    GECODE_POST;
    GECODE_ES_FAIL((Rel::Eq<StringView,StringView>::post(home, x, y)));
  }
  //FIXME: Not sure about this:
  void
  eq(Home home, StringVar x, std::vector<int> w) {
    using namespace String;
    StringView vx(x);
    GECODE_POST;
    if (w.empty()) {
      ConstStringView vw;
      GECODE_ES_FAIL((Rel::Eq<StringView,ConstStringView>::post(home, vx, vw)));
    }
    else {
      ConstStringView vw(home, &w[0], w.size());
      GECODE_ES_FAIL((Rel::Eq<StringView,ConstStringView>::post(home, vx, vw)));
    }
  }
  void
  eq(Home home, std::vector<int> w, StringVar x) {
    eq(home, x, w);
  }
  
  
  void
  nq(Home home, StringVar x, StringVar y) {
    using namespace String;
    GECODE_POST;
    GECODE_ES_FAIL((Rel::Nq<StringView,StringView>::post(home, x, y)));
  }
  
  //FIXME: Not sure about this:
  void
  nq(Home home, StringVar x, std::vector<int> w) {
    using namespace String;
    StringView vx(x);
    GECODE_POST;
    if (w.empty()) {
      ConstStringView vw;
      GECODE_ES_FAIL((Rel::Nq<StringView,ConstStringView>::post(home, vx, vw)));
    }
    else {
      ConstStringView vw(home, &w[0], w.size());
      GECODE_ES_FAIL((Rel::Nq<StringView,ConstStringView>::post(home, vx, vw)));
    }
  }
  void
  nq(Home home, std::vector<int> w, StringVar x) {
    nq(home, x, w);
  }
  
  void
  dom(Home home, StringVar x, const String::Block& b) {
    using namespace String;
    StringVar y(home, b);
    GECODE_POST;
    GECODE_ES_FAIL((Rel::Dom<StringView,StringView>::post(home, x, y)));
  }
  void
  dom(Home home, StringVar x, const String::DashedString& d) {
    using namespace String;
    StringVar y(home, d);
    GECODE_POST;
    GECODE_ES_FAIL((Rel::Dom<StringView,StringView>::post(home, x, y)));
  }
  
}







