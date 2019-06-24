#include <gecode/string.hh>


namespace Gecode {

  StringVar::StringVar(Space& home) :
    VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home)) {}

  StringVar::StringVar(Space& home, const string& s) :
    VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home, s)) {}

  StringVar::StringVar(Space& home, int a, int b) :
    VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home, a, b))
    {}

  StringVar::StringVar(Space& home, String::NSBlocks& v, int mil, int mal) :
    VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home, v, mil, mal)) {}

  StringVar::StringVar(Space& home, const String::NSIntSet& s, int a, int b) :
    VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home, s, a, b)) {}

}
