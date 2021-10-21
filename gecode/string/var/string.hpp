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
    
  forceinline StringVar::StringVar(Space& home)
    : VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home)) {}
    
  forceinline StringVar::StringVar(Space& home, const String::Block& b)
    : VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home, b)) {}
    
  forceinline StringVar::StringVar(Space& home, const String::DashedString& d)
    : VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home, d)) {}
  
  forceinline StringVar::StringVar(Space& home, const std::vector<int>& v)
    : VarImpVar<String::StringVarImp>(new (home) 
      String::StringVarImp(home, Gecode::String::DashedString(home, v[0], v.size()))) {}
  
  forceinline StringVar::StringVar(Space& home, int l, int u)
    : VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home, l, u)) {}  
    
  forceinline StringVar::StringVar(Space& home, const IntSet& s, int l, int u)
    : VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home, s, l, u)) {}  
  
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
    os << "Var. " << v.varimp() << " [ " << *v.varimp() << " ]";
    return os;
  }   

}
