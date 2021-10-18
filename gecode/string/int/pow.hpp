#include <gecode/int.hh>
#include <gecode/string/rel.hh>

namespace Gecode { namespace String { namespace Int {

  template<class View0,class View1>
  forceinline
  Pow<View0,View1>::Pow(Home home, View0 x, Gecode::Int::IntView n, View1 y)
  : MixTernaryPropagator<View0, PC_STRING_CARD, Gecode::Int::IntView,
  Gecode::Int::PC_INT_BND, View1, PC_STRING_CARD> (home, x, n, y) {}

  template<class View0,class View1>
  forceinline ExecStatus
  Pow<View0,View1>::post(Home home, View0 x, Gecode::Int::IntView n, View1 y) {
    if (x.same(y)) {
      GECODE_ME_CHECK(n.gq(home, 0));
      GECODE_ME_CHECK(n.lq(home, 1));
    }
    else
      (void) new (home) Pow(home, x, n, y);
    return ES_OK;
  }

  template<class View0,class View1>
  forceinline
  Pow<View0,View1>::Pow(Space& home, Pow& p)
  : MixTernaryPropagator<View0, PC_STRING_CARD, Gecode::Int::IntView,
  Gecode::Int::PC_INT_BND, View1, PC_STRING_CARD> (home, p) {}

  template<class View0,class View1>
  forceinline Actor*
  Pow<View0,View1>::copy(Space& home) {
    return new (home) Pow(home,*this);
  }

  template<class View0, class View1>
  forceinline ExecStatus
  Pow<View0,View1>::refine_card(Space& home) {
    //TODO:
    return ES_OK;
  }

  template<class View0,class View1>
  forceinline ExecStatus
  Pow<View0,View1>::propagate(Space& home, const ModEventDelta&) {
    std::cerr <<"\n"<< this << "::Pow::propagate "<<x1<<" =  "<<x0<<"\n";
    GECODE_ME_CHECK(x1.gq(home, x2.min_length() > 0));
    int a;
    do {
      //FIXME: Use a privete method of this class instead of:
      int l = x1.min(), u = x1.max();
      if (l == u && u == 1)
        GECODE_REWRITE(*this, (Gecode::String::Rel::Eq<View0,View1>
          ::post(home(*this), x0, x2)));
      // n == 0 \/ epsilon^n.
      if (u == 0 || x0.isNull()) {
        if (x2.isNull())
          return home.ES_SUBSUMED(*this);
        if (x2.min_length() > 0)
          return ES_FAILED;
        x2.nullify(home);
        return home.ES_SUBSUMED(*this);
      }
      // x^n = epsilon
      if (x2.isNull()) {
        if (l > 0) {
          if (x0.min_length() > 0)
            return ES_FAILED;
          x0.nullify(home);
          return home.ES_SUBSUMED(*this);
        }
        else if (x0.min_length() > 0) {
          x1.lq(home, 0);
          return home.ES_SUBSUMED(*this);  
        }
        return ES_FIX;
      }
      int n0 = x0.size(), m = n0 == 1 ? n0 : n0*l;
      Block xn[m+1];
      int uu = std::min(long(MAX_STRING_LENGTH), n0 * long(u - l));
      //FIXME: Implement may_chars -> xn[m].update(Block(x0.may_chars(), 0, uu));
      DashedString d0(home, xn, m+1);
      //FIXME: Implement DashedView(const DashedString& x) and change dom propagator too.
      //       GECODE_ME_CHECK(x0.equate(home, d0)); 
      GECODE_ME_CHECK(refine_card(home));
      a = x0.assigned() + x1.assigned() + x2.assigned();
      switch (a) {
      case 3:
        return home.ES_SUBSUMED(*this);
      case 2:
        if ((x1.assigned() && x1.val() == 0) || 
            (x0.max_length() == 0 && x2.max_length() == 0))
          return ES_FIX;
        break;
      }
    } while (a == 2);
    std::cerr << "After pow: " << x2 << " = " << x0 << " ** " << x1 << '\n';
    return ES_FIX;
  }

}}}
