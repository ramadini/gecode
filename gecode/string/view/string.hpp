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
}}

namespace Gecode { namespace String {

  StringView::SweepFwdIterator::SweepFwdIterator(const StringView& x) 
  : pos(0,0), sv(x) {};
  
  forceinline bool
  StringView::SweepFwdIterator::operator ()(void) const {
    return pos.idx < sv.size();
  };
  
  forceinline void
  StringView::SweepFwdIterator::operator ++(void) {
    if (pos.idx >= sv.size())
      return;
    pos.idx++;
    pos.off = 0;
  };
  
  forceinline Position& 
  StringView::SweepFwdIterator::operator *(void) {
    return pos;
  }
  
  StringView::PushBwdIterator::PushBwdIterator(const StringView& x) 
  : sv(x) {
    int n = sv.size() - 1;
    pos.idx = n;
    pos.off = sv[n].ub();
  };
  
  forceinline bool
  StringView::PushBwdIterator::operator ()(void) const {
    return pos.idx >= 0;
  };
  
  forceinline void
  StringView::PushBwdIterator::operator --(void) {
    if (pos.idx <= 0)
      return;
    pos.idx--;
    pos.off = pos.idx >= 0 ? sv[pos.idx].ub() : 0;
  };
  
  forceinline Position& 
  StringView::PushBwdIterator::operator *(void) { 
    return pos;
  }
  
  StringView::StretchBwdIterator::StretchBwdIterator(const StringView& x) 
  : sv(x) {
    int n = sv.size() - 1;
    pos = Position(n, sv[n].lb());
  };
  
  forceinline bool
  StringView::StretchBwdIterator::operator ()(void) const {
    return pos.idx >= 0;
  };
  
  forceinline void
  StringView::StretchBwdIterator::operator --(void) {
    if (pos.idx <= 0)
      return;
    pos.idx--;
    pos.off = pos.idx >= 0 ? sv[pos.idx].lb() : 0;
  };
  
  forceinline Position& 
  StringView::StretchBwdIterator::operator *(void) {
    return pos;
  }
  
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
