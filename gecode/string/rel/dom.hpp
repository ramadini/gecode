namespace Gecode { namespace String {

  forceinline
  Dom::Dom(Home home, StringView x, NSBlocks& d)
    : UnaryPropagator<StringView, PC_STRING_DOM> (home, x), dom(new NSBlocks(d)) {}

  forceinline
  Dom::Dom(Space& home, bool share, Dom& p)
    : UnaryPropagator<StringView, PC_STRING_DOM>(home, share, p), dom(p.dom) {}

  ExecStatus
  Dom::post(Home home, StringView x, NSBlocks& d) {
    d.normalize();
    (void) new (home) Dom(home, x, d);
    return ES_OK;
  }

  Actor*
  Dom::copy(Space& home, bool share) {
    return new (home) Dom(home, share, *this);
  }

  ExecStatus
  Dom::propagate(Space& home, const ModEventDelta&) {
    // std::cerr<<"\nDom::propagate "<<x0<<" :: "<<*dom<<std::endl;
    GECODE_ME_CHECK(x0.dom(home, *dom));
    // std::cerr << "After equate: " << x0 << '\n';
    if (x0.assigned() || dom->contains<DSBlock, DashedString>(*x0.pdomain()))
      return home.ES_SUBSUMED(*this);
    assert (x0.pdomain()->is_normalized());
    // std::cerr<<"propagated: "<<x0<<" :: "<<*dom<<std::endl;
    return ES_FIX;
  }

  forceinline
  Dom::~Dom() {
    delete dom;
  }

}}
