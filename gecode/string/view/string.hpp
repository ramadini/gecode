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
  : pos({0,0}), sv(x) {};
  
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
  
//  StringView::BwdPushIterator::BwdPushIterator(const StringView& x) 
//  : pos({sv.size(),0}), sv(x) {}; FIXME???
//  
//  forceinline bool
//  StringView::BwdPushIterator::operator ()(void) const {
//    return pos.i < 0 FIXME???
//  };
//  
//  forceinline void
//  StringView::BwdPushIterator::operator --(void) {
//    if (pos.idx >= 0) FIXME???
//      return;
//    pos.idx--;
//    pos.off = 0;
//  };
  
//  StringView::BwdStretchIterator::BwdStretchIterator(const StringView& x) 
//  : pos({sv.size(),0}), sv(x) {}; FIXME???
//  
//  forceinline bool
//  StringView::BwdStretchIterator::operator ()(void) const {
//    return pos.i < 0 FIXME???
//  };
//  
//  forceinline void
//  StringView::BwdStretchIterator::operator --(void) {
//    if (pos.idx >= 0) FIXME???
//      return;
//    pos.idx--;
//    pos.off = 0;
//  };
  
  forceinline int 
  StringView::size() const {
    return x->size();
  }
  
  forceinline Block&
  StringView::operator[](int i) {
    return x->[i];
  }
  forceinline const Block&
  StringView::operator[](int i) const {
    return x->[i];
  }
  
  ModEvent
  StringView::equate(const DashedString& d) {
    return x->equate(d);
  }
     
}}
