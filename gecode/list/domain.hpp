#ifndef __GECODE_LIST_DOMAIN_HPP__
#define __GECODE_LIST_DOMAIN_HPP__

#include <dashed/domain.hpp>

namespace Gecode { namespace List {

/**
 * Public list-domain vocabulary.
 *
 * The native Gecode module deliberately exposes list terminology. The current
 * implementation maps these types to the standalone dashed domain kernel; a
 * future backend can replace that mapping without renaming ListVar or the
 * gecode/list module.
 */
using Length = dashed::Length;
inline constexpr Length kUnboundedLength = dashed::kUnboundedLength;
using IntRange = dashed::IntRange;
using ValueSet = dashed::ValueSet;
using LiteralSlice = dashed::LiteralSlice;
using RepeatSegment = dashed::RepeatSegment;
using LiteralSegment = dashed::LiteralSegment;
using Segment = dashed::Segment;
using Domain = dashed::Domain;

}} // namespace Gecode::List

#endif
