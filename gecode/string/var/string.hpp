#include <gecode/string.hh>

namespace Gecode {

  forceinline
  StringVar::StringVar(void) {}

  forceinline
  StringVar::StringVar(const StringVar& y)
    : VarImpVar<String::StringVarImp>(y.varimp()) {}

  forceinline
  StringVar::StringVar(const String::StringView& y)
    : VarImpVar<String::StringVarImp>(y.varimp()) {}
    
  StringVar::StringVar(Space& home)
    : VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home)) {}
    
  StringVar::StringVar(Space& home, const String::Block& b)
    : VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home, b)) {}
    
  StringVar::StringVar(Space& home, const String::DashedString& d)
    : VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home, d)) {}
  
  forceinline int
  StringVar::min_length() const {
    return x->min_length();
  }

  forceinline int
  StringVar::max_length() const {
    return x->max_length();
  }
  
  forceinline std::vector<int>
  StringVar::val() const {
    return x->val();
  }
  
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringVar& v) {
    os << "Var. " << v.varimp() << " [ " << v.varimp()->dom() << " ]";
    return os;
  }   

}
