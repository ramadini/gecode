#include <gecode/string.hh>

namespace Gecode { namespace String {

  OutOfLimits::OutOfLimits(const char* l):
    Exception(l, "Number out of limits") {}

  EmptyDashedString::EmptyDashedString(const char* l):
    Exception(l, "Attempt to create a dashed string with no blocks.") {}

  UnknownRelation::UnknownRelation(const char* l):
    Exception(l, "Unknown relation.") {}

}}
