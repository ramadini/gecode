#include <gecode/string.hh>

namespace Gecode {

  StringVarArgs::StringVarArgs(Space& home, int n, const String::NSBlocks& v)
  : VarArgArray<StringVar>(n) {
    String::NSBlocks domain(v);
    for (int i = size(); i--; )
      a[i] = StringVar(home, domain, 0, String::DashedString::_MAX_STR_LENGTH);
  }

  StringVarArray::StringVarArray(Space& home, int n)
  : VarArray<StringVar>(home, n) {
    for (int i = size(); i--; )
      x[i] = StringVar(home);
  }

  StringVarArray::StringVarArray(Space& home, int n, String::NSBlocks& v)
  : VarArray<StringVar>(home, n) {
    for (int i = size(); i--; )
      x[i] = StringVar(home, v, 0, String::DashedString::_MAX_STR_LENGTH);
  }

}

// STATISTICS: string-var
