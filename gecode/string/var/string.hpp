namespace Gecode {

  /*
   * Constructors and access
   *
   */

  forceinline
  StringVar::StringVar(void) {}

  forceinline
  StringVar::StringVar(const StringVar& y)
    : VarImpVar<String::StringVarImp>(y.varimp()) {}

  forceinline
  StringVar::StringVar(const String::StringView& y)
    : VarImpVar<String::StringVarImp>(y.varimp()) {}


  forceinline bool
  StringVar::assigned() const {
    return x->domain().known();
  }

  forceinline string
  StringVar::val() const {
    return x->domain().val();
  }

  forceinline const String::DashedString&
  StringVar::domain() const {
    return x->domain();
  }

  forceinline int
  StringVar::min_length() const {
    return x->domain().min_length();
  }

  forceinline int
  StringVar::max_length() const {
    return x->domain().max_length();
  }

  forceinline String::NSIntSet
  StringVar::may_chars() const {
    return x->domain().may_chars();
  }

  forceinline String::NSIntSet
  StringVar::must_chars() const {
    return x->domain().must_chars();
  }

  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringVar& v) {
    os << "Var. " << v.varimp() << ": " << v.x->domain() << " ["
       << v.min_length() << ".." << v.max_length() << "]";
    return os;
  }
}
