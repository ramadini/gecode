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

  //TODO:
  
  forceinline Block&
  StringVarImp::operator[](int i) {
    return x->[i];
  }
  forceinline const Block&
  StringVarImp::operator[](int i) const {
    return x->[i];
  }
  
  ModEvent
  StringView::equate(const DashedString& d) {
    return x->equate(d);
  }
     
}}
