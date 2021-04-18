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
    return Position(0,0) <= pos && pos < Position(sv.size(),0);
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
