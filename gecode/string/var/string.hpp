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

}
