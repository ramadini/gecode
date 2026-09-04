#include <gecode/int/arithmetic.hh>

namespace Gecode { namespace String {

  forceinline ExecStatus
  Concat::post(Home home, StringView x0, StringView x1, StringView x2,
               bool channel) {
    if (x0.same(x2))
      rel(home, x1, STRT_EQ, StringVar(home, ""));
    else if (x1.same(x2))
      rel(home, x0, STRT_EQ, StringVar(home, ""));
    else if (x0.same(x1)) {
      StringVar y(
        home, x1.may_chars(), x1.min_length(), x1.max_length()
      );
      (void) new (home) Concat(home, x0, y, x2);
      rel(home, x1, STRT_EQ, y);
      if (channel) {
        IntVar lx0(home, x0.min_length(), x0.max_length());
        IntVar ly(home, y.min_length(), y.max_length());
        IntVar lx2(home, x2.min_length(), x2.max_length());
        length(home, StringVar(x0), lx0);
        length(home, y, ly);
        length(home, StringVar(x2), lx2);
        IntArgs ia;
        ia << 1 << 1 << -1;
        IntVarArgs iv;
        iv << lx0 << ly << lx2;
        linear(home, ia, iv, IRT_EQ, 0);
      }
    }
    else {
      (void) new (home) Concat(home, x0, x1, x2);
      if (channel) {
        IntVar lx0(home, x0.min_length(), x0.max_length());
        IntVar lx1(home, x1.min_length(), x1.max_length());
        IntVar lx2(home, x2.min_length(), x2.max_length());
        length(home, StringVar(x0), lx0);
        length(home, StringVar(x1), lx1);
        length(home, StringVar(x2), lx2);
        IntArgs ia;
        ia << 1 << 1 << -1;
        IntVarArgs iv;
        iv << lx0 << lx1 << lx2;
        linear(home, ia, iv, IRT_EQ, 0);
      }
    }
    return ES_OK;
  }

  forceinline Actor*
  Concat::copy(Space& home) {
    return new (home) Concat(home, *this);
  }

  forceinline
  Concat::Concat(Home home, StringView y0, StringView y1, StringView y2)
  : MixTernaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM,
      StringView, PC_STRING_DOM>(home, y0, y1, y2) {}

  forceinline
  Concat::Concat(Space& home, Concat& p)
  : MixTernaryPropagator<StringView, PC_STRING_DOM,
         StringView, PC_STRING_DOM, StringView,PC_STRING_DOM>(home, p) {}

  forceinline ExecStatus
  Concat::propagate(Space& home, const ModEventDelta&) {
//    std::cerr<<"\nConcat::propagate: "<<x2<<" = "<<x0<<" ++ "<<x1<<std::endl;
    while (true) {
      assert (x0.domain().is_normalized() && x1.domain().is_normalized() &&
              x2.domain().is_normalized());
      GECODE_ME_CHECK(x2.concat(home, x0, x1));
      if(home.failed())
        return ES_FAILED;
      //std::cerr<<"After concat: "<<x2<<" = "<<x0<<" ++ "<<x1<<std::endl;
      assert (x0.domain().is_normalized() && x1.domain().is_normalized() &&
              x2.domain().is_normalized());
      int assigned = x0.assigned() + x1.assigned() + x2.assigned();
      if (assigned == 3) {
        string s0, s1, s2;
        const bool b0 = x0.domain().try_val_bytes(s0);
        const bool b1 = x1.domain().try_val_bytes(s1);
        const bool b2 = x2.domain().try_val_bytes(s2);
        if (b0 && b1 && b2) {
          if (s2 != s0 + s1)
            return ES_FAILED;
        }
        else {
          const StringVal v0 = x0.domain().val_symbols();
          const StringVal v1 = x1.domain().val_symbols();
          const StringVal v2 = x2.domain().val_symbols();
          if (v2.size() != v0.size() + v1.size() ||
              !std::equal(v0.begin(), v0.end(), v2.begin()) ||
              !std::equal(v1.begin(), v1.end(),
                v2.begin() + static_cast<std::ptrdiff_t>(v0.size())))
            return ES_FAILED;
        }
        return home.ES_SUBSUMED(*this);
      }
      if (assigned != 2)
        return ES_FIX;
    }
  }

}}
