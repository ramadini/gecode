#include <gecode/string.hh>
#include <gecode/string/eq.hh>
#include <gecode/string/nq.hh>
#include <gecode/string/dom.hh>
#include <gecode/string/inc.hh>
#include <gecode/string/lex.hh>
#include <gecode/string/re-eq.hh>
#include <gecode/string/re-lex.hh>

namespace Gecode { namespace String {

  template<class View0>
  void
  rel_post(Home home, View0 x0, StringRelType r) {
    GECODE_POST;
    switch (r) {
      case STRT_INCLT:
        GECODE_ES_FAIL((Inc::post(home, x0, true)));
        break;
      case STRT_INCLQ:
        GECODE_ES_FAIL((Inc::post(home, x0, false)));
        break;
      case STRT_DECLT: {
        StringVar xr(home);
        rel(home, x0, STRT_REV, xr);
        GECODE_ES_FAIL((Inc::post(home, xr, true)));
        break;
      }
      case STRT_DECLQ: {
        StringVar xr(home);
        rel(home, x0, STRT_REV, xr);
        GECODE_ES_FAIL((Inc::post(home, xr, false)));
        break;
      }
      default:
        throw UnknownRelation("String::rel");
    }
  }

  template<class View0, class View1>
  void
  rel_post(Home home, View0 x0, StringRelType r, View1 x1) {
    GECODE_POST;
    switch (r) {
      case STRT_EQ:
        GECODE_ES_FAIL((Eq::post(home, x0, x1)));
        break;
      case STRT_NQ:
        GECODE_ES_FAIL((Nq::post(home, x0, x1)));
        break;
      case STRT_LEXLT:
        GECODE_ES_FAIL((Lex::post(home, x0, x1, true)));
        break;
      case STRT_LEXLQ:
        GECODE_ES_FAIL((Lex::post(home, x0, x1, false)));
        break;
      default:
        throw UnknownRelation("String::rel");
    }
  }

  template<class View0, class View1, ReifyMode rm>
  void
  rel_re(Home home, View0 x, StringRelType r, View1 y, BoolVar b) {
    GECODE_POST;
    switch (r) {
      case STRT_EQ:
        GECODE_ES_FAIL((ReEq<Gecode::Int::BoolView, rm>::post(home, x, y, b)));
        break;
      case STRT_LEXLT:
        GECODE_ES_FAIL((ReLex<Gecode::Int::BoolView, rm>
          ::post(home, x, y, b, true)));
        break;
      case STRT_LEXLQ:
        GECODE_ES_FAIL((ReLex<Gecode::Int::BoolView, rm>
          ::post(home, x, y, b, false)));
        break;
      case STRT_NQ: {
        Gecode::Int::NegBoolView notb(b);
        switch (rm) {
          case RM_EQV:
            GECODE_ES_FAIL((ReEq<Gecode::Int::NegBoolView, RM_EQV>
              ::post(home, x, y, notb)));
            break;
          case RM_IMP:
            GECODE_ES_FAIL((ReEq<Gecode::Int::NegBoolView, RM_PMI>
              ::post(home, x, y, notb)));
            break;
          case RM_PMI:
            GECODE_ES_FAIL((ReEq<Gecode::Int::NegBoolView, RM_IMP>
              ::post(home, x, y, notb)));
            break;
          default:
            throw Gecode::Int::UnknownReifyMode("String::rel");
        }
        break;
      }
      default:
        throw UnknownRelation("String::rel");
    }
  }

}}

namespace Gecode {

  void
  rel(Home home, StringVar x, StringRelType r) {
    using namespace String;
    rel_post<StringView>(home, x, r);
  }

  void
  rel(Home home, StringVar x, StringRelType r, StringVar y) {
    using namespace String;
    rel_post<StringView, StringView>(home, x, r, y);
  }

  void
  rel(
    Home home, StringVar x, StringRelType r, String::NSBlocks& y, int a, int b
  ) {
    using namespace String;
    if (r == STRT_DOM) {
      GECODE_POST;
      GECODE_ES_FAIL((Dom::post(home, x, y)));
    }
    else
      rel_post<StringView, StringView>(home, x, r, StringVar(home, y, a, b));
  }

  void
  rel(Home home, StringVar x, StringRelType rt, StringVar y, Reify r) {
    using namespace String;
    switch (r.mode()) {
      case RM_EQV:
        rel_re<StringView, StringView, RM_EQV>(home, x, rt, y, r.var());
        break;
      case RM_IMP:
        rel_re<StringView, StringView, RM_IMP>(home, x, rt, y, r.var());
        break;
      case RM_PMI:
        rel_re<StringView, StringView, RM_PMI>(home, x, rt, y, r.var());
        break;
      default:
        throw Gecode::Int::UnknownReifyMode("String::rel");
    }
  }

}
