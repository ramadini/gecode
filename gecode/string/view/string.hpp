namespace Gecode { namespace String {

  StringView::
  SweepIterator::SweepIterator(const StringView& x, const Position& p)
  : sv(x), pos(p) {
    if (!p.isNorm(x)) {
      if (p.off == sv[p.idx].ub()) {
        pos.idx++;
        pos.off = 0;
      }
      else
        throw OutOfLimits("StringView::SweepFwdIterator::SweepFwdIterator");
    }
    assert (isOK());
  };
  
  }
  
  forceinline const Position& 
  StringView::SweepIterator::operator *() {
    return pos;
  }

  forceinline int
  StringView::SweepIterator::lb() const {
    return sv[pos.idx].lb();
  }
  
  forceinline int
  StringView::SweepIterator::ub() const {
    return sv[pos.idx].ub();
  }
  
  forceinline bool
  StringView::SweepIterator::disj(const Block& b) const {
    return sv[pos.idx].baseDisjoint(b);
  }
  
  forceinline bool
  StringView::SweepIterator::isOK() const {
    return pos.isNorm(sv);
  }
  

}}

namespace Gecode { namespace String {

  StringView::
  SweepFwdIterator::SweepFwdIterator(const StringView& x) 
  : SweepIterator(x, Position(0,0)) {};
  
  forceinline void
  StringView::SweepFwdIterator::moveNext() {
    if (pos.idx >= sv.size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK());
  }
  
  forceinline bool
  StringView::SweepFwdIterator::hasNext(void) const {
    return pos.idx < sv.size();
  }
  
  forceinline int
  StringView::SweepFwdIterator::must_consume() const {
    return std::max(0, lb() - pos.off);
  }
  
  forceinline int
  StringView::SweepFwdIterator::may_consume() const {
    return ub() - pos.off;
  }
  
  forceinline void
  StringView::SweepFwdIterator::consume(int k) {
    pos.off += k;
    if (pos.off >= sv[pos.idx].ub()) {
      if (pos.off > sv[pos.idx].ub())
        throw OutOfLimits("StringView::SweepBwdIterator::consume");
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  forceinline void
  StringView::SweepFwdIterator::consumeMand(int k) {
    pos.off += k; 
    if (pos.off > sv[pos.idx].lb())
      throw OutOfLimits("StringView::SweepBwdIterator::consume");
    if (pos.off == sv[pos.idx].ub()) {
      pos.idx++;
      pos.off = 0;
    }     
    assert (isOK());
  }

}}

namespace Gecode { namespace String {
  
  StringView::SweepBwdIterator::SweepBwdIterator(const StringView& x) 
  : SweepIterator(x, Position(x.size(), 0)) {};
  
  forceinline void
  StringView::SweepBwdIterator::moveNext() {
    if (pos.idx == 0)
      return;
    if (pos.off > 0)
      pos.idx--;
    pos.off = 0;
    assert (isOK());
  };
  
  forceinline bool
  StringView::SweepBwdIterator::hasNext(void) const {
    return pos.idx > 0;
  };
  
  forceinline int
  StringView::SweepBwdIterator::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].ub() : pos.off;
  }
  
  forceinline int
  StringView::SweepBwdIterator::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].lb() 
                                         : min(pos.off, sv[pos.idx].lb());
  }
  
  forceinline void
  StringView::SweepBwdIterator::consume(int k) {
    pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("StringView::SweepBwdIterator::consume");
    assert (isOK());
  }
  
  forceinline void
  StringView::SweepBwdIterator::consumeMand(int k) {
    pos.off = sv[pos.idx - (pos.idx > 0 && pos.off == 0)].lb() - k;
    if (pos.off < 0)
      throw OutOfLimits("StringView::SweepBwdIterator::consume");
    assert (isOK());
  }

}}

namespace Gecode { namespace String {

  /*
   * Constructors and access
   *
   */

  forceinline
  StringView::StringView(void) {}
  forceinline
  StringView::StringView(const StringVar& y)
    : VarImpView<StringVar>(y.varimp()) {}
  forceinline
  StringView::StringView(StringVarImp* y)
    : VarImpView<StringVar>(y) {}

  forceinline int 
  StringView::size() const {
    return x->size();
  }
  
  forceinline Block&
  StringView::operator[](int i) {
    return x->operator[](i);
  }
  forceinline const Block&
  StringView::operator[](int i) const {
    return x->operator[](i);
  }
  
  forceinline int
  StringView::max_length() const {
    return x->max_length();
  }
  
  forceinline int
  StringView::min_length() const {
    return x->min_length();
  }
  
  forceinline bool
  StringView::assigned() const {
    return x->assigned();
  }
  
  forceinline StringView::SweepFwdIterator
  StringView::sweep_fwd_iterator(void) {
    return SweepFwdIterator(*this);
  }
  
  forceinline StringView::SweepBwdIterator
  StringView::push_bwd_iterator(void) {
    return SweepBwdIterator(*this);
  }
  
  forceinline StringView::StretchBwdIterator
  StringView::stretch_bwd_iterator(void) {
    return StretchBwdIterator(*this);
  }
  
//  TODO? ModEvent
//  StringView::equate(const DashedString& d) {
//    return x->equate(d);
//  }
     
}}
