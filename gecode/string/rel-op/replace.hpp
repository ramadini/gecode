namespace Gecode { namespace String {

  forceinline
  Replace::Replace(Home home, ViewArray<StringView>& x, bool a, bool l)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, x), all(a), last(l) {}

  forceinline
  Replace::Replace(Space& home, Replace& p)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, p), all(p.all), last(p.last)
  {}

}}

// STATISTICS: string-prop
