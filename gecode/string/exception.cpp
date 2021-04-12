#include <gecode/string.hh>

namespace Gecode { namespace String {

  OutOfLimits::OutOfLimits(const char* l)
    : Exception(l,"Number out of limits") {}

  VariableEmptyDomain::VariableEmptyDomain(const char* l)
    : Exception(l,"Attempt to create variable with empty domain") {}

  TooFewArguments::TooFewArguments(const char* l)
    : Exception(l,"Passed argument array has too few elements") {}

  ArgumentSizeMismatch::ArgumentSizeMismatch(const char* l)
    : Exception(l,"Sizes of argument arrays mismatch") {}

  IllegalOperation::IllegalOperation(const char* l)
    : Exception(l,"Illegal operation type") {}

  UnknownBranching::UnknownBranching(const char* l)
    : Exception(l,"Unknown branching type") {}

  UnknownRelation::UnknownRelation(const char* l)
    : Exception(l,"Unknown relation type") {}

  UnknownOperation::UnknownOperation(const char* l)
    : Exception(l,"Unknown operation type") {}

}}

// STATISTICS: string-other

