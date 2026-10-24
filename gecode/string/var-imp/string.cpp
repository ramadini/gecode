#include <gecode/string.hh>

namespace Gecode { namespace String {

  StringVarImp::StringVarImp(Space& home) :
    StringVarImpBase(home), ds(home, DSBlock::top(home)) {}

  StringVarImp::StringVarImp(Space& h, int a, int b) :
    StringVarImp(h, NSIntSet::top(), a, b) {}

  StringVarImp::StringVarImp(Space& home, string s) :
    StringVarImpBase(home), ds(home, s) {}

  StringVarImp::StringVarImp(Space& home, const StringVal& value) :
    StringVarImpBase(home), ds(home, value) {}

  StringVarImp::StringVarImp(Space& home, NSBlocks& v, int mil, int mal) :
    StringVarImpBase(home), ds(home, v, mil, mal) {}

  StringVarImp::StringVarImp(Space& home, const NSIntSet& s, int a, int b) :
    StringVarImpBase(home), ds(home) {
    if (a < 0 || b < 0 || a > DashedString::_MAX_STR_LENGTH ||
        b > DashedString::_MAX_STR_LENGTH)
      throw OutOfLimitsDS("StringVarImp::StringVarImp");
    if (a > b || (s.empty() && a > 0))
      throw EmptyDS("StringVarImp::StringVarImp");
    if (b == 0 || s.empty())
      return;
    ds._blocks.at(0).S.init(home, s);
    ds._blocks.at(0).l = ds._min_length = a;
    ds._blocks.at(0).u = ds._max_length = b;
  }

  StringVarImp::StringVarImp(Space& home, StringVarImp& y) :
    StringVarImpBase(home, y), ds(home, y.ds) {}

}}

// STATISTICS: string-var
