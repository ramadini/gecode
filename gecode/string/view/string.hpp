namespace Gecode { namespace String {

  StringView::
  SweepIterator::SweepIterator(const StringView& x, const Position& p)
  : sv(x), pos(p) {}
  
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

}}

namespace Gecode { namespace String {

  StringView::
  SweepFwdIterator::SweepFwdIterator(const StringView& x) 
  : SweepIterator(x, Position(0, 0)) {};
  
  StringView::
  SweepFwdIterator::SweepFwdIterator(const StringView& x, const Position& p)
  : SweepIterator(x, p) {
    if (p.idx < 0 || p.idx > x.size() || p.off < 0
    || (p.idx == x.size() && p.off != 0) || p.off > sv[p.idx].ub())
      throw OutOfLimits("StringView::SweepFwdIterator::SweepFwdIterator");
    if (p.off == sv[p.idx].ub()) {
      pos.idx++;
      pos.off = 0;  
    }
  };
  
  forceinline void
  StringView::SweepFwdIterator::next() {
    if (pos.idx >= sv.size())
      return;
    pos.idx++;
    pos.off = 0;
  }
  
  forceinline bool
  StringView::SweepFwdIterator::operator ()() const {
    return pos.idx < sv.size();
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
  }

  
  StringView::PushBwdIterator::PushBwdIterator(const StringView& x) 
  : SweepIterator(x, Position(x.size()-1,0)) {
    pos.off = sv[x.size()-1].ub();
  };
  
  StringView::
  PushBwdIterator::PushBwdIterator(const StringView& x, const Position& p)
  : SweepIterator(x, p) {
    if (p.idx < 0 || p.idx > x.size() || p.off < 0
    || (p.idx == x.size() && p.off != 0) || p.off > sv[p.idx].ub())
      throw OutOfLimits("StringView::PushBwdIterator::PushBwdIterator");
    if (p.off == 0) {
      pos.idx--;
      pos.idx >= 0 ? sv[pos.idx].ub() : 0;
    }
  };
  
  forceinline void
  StringView::PushBwdIterator::next(void) {
    if (pos.idx < 0)
      return;
    pos.idx--;
    pos.off = pos.idx >= 0 ? sv[pos.idx].ub() : 0;
  };
  
  forceinline bool
  StringView::PushBwdIterator::operator ()(void) const {
    return pos.idx >= 0;
  };
  
  forceinline void
  StringView::PushBwdIterator::consume(int k) {
    pos.off -= k;
    if (k <= 0) {
      if (k < 0)
        throw OutOfLimits("StringView::PushBwdIterator::consume");
      pos.idx--;
      pos.off = pos.idx >= 0 ? sv[pos.idx].ub() : 0;
    }
  }


  StringView::StretchBwdIterator::StretchBwdIterator(const StringView& x) 
  : SweepIterator(x, Position(x.size()-1,0)) {
    pos.off = sv[x.size()-1].lb();
  };
  
  StringView::
  StretchBwdIterator::StretchBwdIterator(const StringView& x, const Position& p)
  : SweepIterator(x, p) {
    if (p.idx < 0 || p.idx > x.size() || p.off < 0
    || (p.idx == x.size() && p.off != 0) || p.off > sv[p.idx].ub())
      throw OutOfLimits("StringView::StretchBwdIterator::StretchBwdIterator");
    if (p.off == 0) {
      pos.idx--;
      pos.idx >= 0 ? sv[pos.idx].lb() : 0;
    }
  };
  
  forceinline void
  StringView::StretchBwdIterator::next(void) {
    if (pos.idx < 0)
      return;
    pos.idx--;
    pos.off = pos.idx >= 0 ? sv[pos.idx].lb() : 0;
  };
  forceinline bool
  StringView::StretchBwdIterator::operator ()(void) const {
    return pos.idx >= 0;
  };

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
