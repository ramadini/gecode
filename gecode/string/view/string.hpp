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
        throw OutOfLimits("StringView::PushBwdIterator::consume");
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }

}}

namespace Gecode { namespace String {
  
  StringView::PushBwdIterator::PushBwdIterator(const StringView& x) 
  : SweepIterator(x, Position(x.size(),0)) {};
  
  forceinline void
  StringView::PushBwdIterator::moveNext() {
    if (pos.idx == 0)
      return;
    pos.idx--;
    pos.off = 0;
    assert (isOK());
  };
  
  forceinline bool
  StringView::PushBwdIterator::hasNext(void) const {
    return pos.idx > 0;
  };
  
  forceinline int
  StringView::PushBwdIterator::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].ub() : pos.off;
  }
  
  forceinline void
  StringView::PushBwdIterator::consume(int k) {
    if (pos.idx > 0 && pos.off == 0)
      pos.off = sv[pos.idx-1].ub() - k;
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("StringView::PushBwdIterator::consume");
    assert (isOK());
  }

}}

namespace Gecode { namespace String {

  StringView::StretchBwdIterator::StretchBwdIterator(const StringView& x) 
  : SweepIterator(x, Position(x.size(),0)) {
    const Block& b(sv[x.size()-1]);
    if (b.lb() < b.ub()) {
      pos.idx--;
      pos.off = b.lb();
    }
    assert (isOK());
  };
  
  forceinline void
  StringView::StretchBwdIterator::moveNext() {
    if (pos.idx == 0)
      return;
    const Block& b(sv[pos.idx]);
    if (b.lb() < b.ub())
      pos.idx--;
      pos.off = b.lb();
    }
    else
      pos.off = 0;
    assert (isOK());
  };
  
  forceinline bool
  StringView::StretchBwdIterator::hasNext(void) const {
    return pos.idx > 0 || pos.off > 0;
  };

  forceinline int
  StringView::StretchBwdIterator::must_consume() const {
    return std::min(pos.off, sv[pos.idx].lb());
  }

  forceinline void
  StringView::StretchBwdIterator::consume(int k) {
    pos.off = std::min(pos.off, sv[pos.idx].lb()) - k;
    if (pos.off <= 0) {
      if (pos.off < 0)
        throw OutOfLimits("StringView::StretchBwdIterator::consume");
      pos.idx--;
      pos.off = pos.idx >= 0 ? sv[pos.idx].lb() : 0;
    }
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
  
  forceinline StringView::PushBwdIterator
  StringView::push_bwd_iterator(void) {
    return PushBwdIterator(*this);
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
