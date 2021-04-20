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

}
