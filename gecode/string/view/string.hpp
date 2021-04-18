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

  StringView::FwdPosIterator::FwdPosIterator(const StringView& x) 
  : pos(0,0), sv(x) {};
  
  forceinline bool
  StringView::FwdPosIterator::operator ()(void) const {
    return pos.idx < sv.size();
  };
  
  forceinline void
  StringView::FwdPosIterator::operator ++(void) {
    if (pos.idx >= sv.size())
      return;
    pos.idx++;
    pos.off = 0;
  };
  
  forceinline Position& 
  StringView::FwdPosIterator::operator *(void) {
    return pos;
  }
  
  StringView::BwdPushIterator::BwdPushIterator(const StringView& x) 
  : sv(x) {
    int n = sv.size() - 1;
    pos.idx = n;
    pos.off = sv[n].ub();
  };
  
  forceinline bool
  StringView::BwdPushIterator::operator ()(void) const {
    return pos.idx >= 0;
  };
  
  forceinline void
  StringView::BwdPushIterator::operator --(void) {
    if (pos.idx <= 0)
      return;
    pos.idx--;
    pos.off = pos.idx >= 0 ? sv[pos.idx].ub() : 0;
  };
  
  forceinline Position& 
  StringView::BwdPushIterator::operator *(void) { 
    return pos;
  }
  
  StringView::BwdStretchIterator::BwdStretchIterator(const StringView& x) 
  : sv(x) {
    int n = sv.size() - 1;
    pos = Position(n, sv[n].lb());
  };
  
  forceinline bool
  StringView::BwdStretchIterator::operator ()(void) const {
    return pos.idx >= 0;
  };
  
  forceinline void
  StringView::BwdStretchIterator::operator --(void) {
    if (pos.idx <= 0)
      return;
    pos.idx--;
    pos.off = pos.idx >= 0 ? sv[pos.idx].lb() : 0;
  };
  
  forceinline Position& 
  StringView::BwdStretchIterator::operator *(void) {
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
  
//  TODO: ModEvent
//  StringView::equate(const DashedString& d) {
//    return x->equate(d);
//  }
     
}}
