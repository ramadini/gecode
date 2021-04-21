namespace Gecode { namespace String {

  StringView::SweepIterator::SweepIterator(const StringView& x, Position p)
  : sv(x), pos(p) {}
  
  forceinline Position& 
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

  StringView::SweepFwdIterator::SweepFwdIterator(const StringView& x) 
  : SweepIterator(x, Position(0, 0)) {};
  
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

  
  StringView::PushBwdIterator::PushBwdIterator(const StringView& x) 
  : SweepIterator(x, Position(x.size()-1, x[x.size()-1].ub())) {};
  
  forceinline void
  StringView::PushBwdIterator::next(void) {
    if (pos.idx <= 0)
      return;
    pos.idx--;
    pos.off = pos.idx >= 0 ? sv[pos.idx].ub() : -1;
  };
  
  forceinline bool
  StringView::PushBwdIterator::operator ()(void) const {
    return pos.idx >= 0;
  };


  StringView::StretchBwdIterator::StretchBwdIterator(const StringView& x) 
  : SweepIterator(x, Position(x.size()-1, x[x.size()-1].lb())) {};
  
  forceinline void
  StringView::StretchBwdIterator::next(void) {
    if (pos.idx <= 0)
      return;
    pos.idx--;
    pos.off = pos.idx >= 0 ? sv[pos.idx].lb() : -1;
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
  
//  forceinline StringView::SweepFwdIterator
//  StringView::sweep_fwd_iterator(void) {
//    return SweepFwdIterator(*this);
//  }
//  forceinline StringView::PushBwdIterator
//  StringView::push_bwd_iterator(void) {
//    return PushBwdIterator(*this);
//  }
//  forceinline StringView::StretchBwdIterator
//  StringView::stretch_bwd_iterator(void) {
//    return StretchBwdIterator(*this);
//  }
  
//  TODO? ModEvent
//  StringView::equate(const DashedString& d) {
//    return x->equate(d);
//  }
     
}}
