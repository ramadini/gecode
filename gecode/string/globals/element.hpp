#include <gecode/string/globals.hh>

namespace Gecode { namespace String { namespace Globals {

  template<class View>
  forceinline
  Element<View>::Element(Home home, ViewArray<View> v, Gecode::Int::IntView i)
  : MixNaryOnePropagator <View, PC_STRING_CARD, 
    Gecode::Int::IntView, Gecode::Int::PC_INT_BND> (home, v, i) {}

  template<class View>
  forceinline ExecStatus
  Element<View>::post(Home home, ViewArray<View> x, Gecode::Int::IntView i, 
                                                                    View y) {
    int n = x.size();
    ViewArray<StringView> v(home, n+1);
    v[0] = y;
    for (int j = 0; j < n; ++j)
      v[j+1] = x[j];
    GECODE_ME_CHECK(i.gq(home, 1));
    GECODE_ME_CHECK(i.lq(home,x.size()-1));
    (void) new (home) Element(home, x, i);
    return ES_OK;
  }

  template<class View>
  forceinline
  Element<View>::Element(Space& home, Element& p)
  : MixNaryOnePropagator <View, PC_STRING_CARD, 
    Gecode::Int::IntView, Gecode::Int::PC_INT_BND> (home, p) {}

  template<class View>
  forceinline Actor*
  Element<View>::copy(Space& home) {
    return new (home) Element(home,*this);
  }

  template<class View>
  forceinline ExecStatus
  Element<View>::propagate(Space& home, const ModEventDelta&) {
//    std::cerr << "Element::propagate [";for(int i=1;i<x.size();++i)std::cerr<<x[i]<<", ";std::cerr<< "]["<<y<<"] = "<<x[0]<< "\n";
    if (y.assigned()) {
      rel(home, x[0], STRT_EQ, x[y.val()]);
      return home.ES_SUBSUMED(*this);
    }
    Set::GLBndSet s;
    int l = MAX_STRING_LENGTH, u = 0;
    for (IntVarValues i(y); i(); ++i) {
      View& vi = x[i.val()];
      std::cerr << "x["<< i.val() << "] = " << vi << " vs " << x[0] << "\n";
      if (check_equate_x(x[0],vi)) {
        if (!x[0].assigned()) {
          for (int j = 0; j < vi.size(); ++j)
            vi[j].includeBaseIn(home, s);
          int li = vi.min_length(), ui = vi.max_length();
          if (li < l) l = li;
          if (ui > u) u = ui;
          std::cerr << Block(home,CharSet(home,s),l,u) << '\n';
        }
      }
      else
        y.nq(home, i.val());
    }
    if (y.assigned()) {
      rel(home, x[0], STRT_EQ, x[y.val()]);
//      std::cerr << "Element::propagated [";for(int i=1;i<x.size();++i)std::cerr<<x[i]<<", ";std::cerr<< "]["<<y<<"] = "<<x[0]<< "\n";
      return home.ES_SUBSUMED(*this);
    }
    Block b(home, CharSet(home, s), l, u);
    GECODE_ME_CHECK(x[0].equate(home, ConstDashedView(b, 1)));
//    std::cerr << "Element::propagated [";for(int i=1;i<x.size();++i)std::cerr<<x[i]<<", ";std::cerr<< "]["<<y<<"] = "<<x[0]<< "\n";
    return ES_FIX;
  }

}}}
