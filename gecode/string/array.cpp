#include <gecode/string.hh>

namespace Gecode {

  StringVarArray::StringVarArray(Space& home, int n)
    : VarArray<StringVar>(home,n) {
    for (int i = size(); i--; )
      x[i] = StringVar(home);
  }

}

// STATISTICS: set-other
