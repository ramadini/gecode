#include <gecode/string.hh>

namespace Gecode { namespace String {

  StringVarImp::StringVarImp(Space& home) :
  StringVarImpBase(home), ds(home, DSBlock::top(home)) {};

  StringVarImp::StringVarImp(Space& h, int a, int b):
  StringVarImpBase(h), ds(h) {
    ds._blocks.at(0).S.init(h, DSIntSet(h, 0, DashedString::_MAX_STR_ALPHA));
    ds._blocks.at(0).l = a;
    ds._blocks.at(0).u = b;
    ds._min_length = min(DashedString::_MAX_STR_LENGTH, a);
    ds._max_length = min(DashedString::_MAX_STR_LENGTH, b);
  };

  StringVarImp::StringVarImp(Space& home, string s) :
  StringVarImpBase(home), ds(home, s) {};

  StringVarImp::StringVarImp(Space& home, NSBlocks& v, int mil, int mal) :
  StringVarImpBase(home), ds(home, v, mil, mal) {};

  StringVarImp::StringVarImp(Space& home, const NSIntSet& s, int a, int b) :
  StringVarImpBase(home), ds(home) {
    ds._blocks.at(0).S.init(home, s);
    ds._blocks.at(0).l = a;
    ds._blocks.at(0).u = b;
    ds._min_length = min(DashedString::_MAX_STR_LENGTH, a);
    ds._max_length = min(DashedString::_MAX_STR_LENGTH, b);
  };

  StringVarImp::StringVarImp(Space& home, StringVarImp& y) :
  StringVarImpBase(home, y), ds(home, y.ds) {};

}}
