namespace Gecode { namespace String {

  forceinline
  Concat::Concat(Home home, StringView y0, StringView y1, StringView y2)
  : MixTernaryPropagator<StringView, PC_STRING_DOM, StringView, PC_STRING_DOM,
      StringView, PC_STRING_DOM>(home, y0, y1, y2) {}

  forceinline
  Concat::Concat(Space& home, Concat& p)
  : MixTernaryPropagator<StringView, PC_STRING_DOM,
         StringView, PC_STRING_DOM, StringView,PC_STRING_DOM>(home, p) {}

}}

// STATISTICS: string-prop
