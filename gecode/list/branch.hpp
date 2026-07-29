#ifndef __GECODE_LIST_BRANCH_HPP__
#define __GECODE_LIST_BRANCH_HPP__

#include <gecode/list/length.hpp>
#include <gecode/int.hh>

namespace Gecode { namespace List { namespace Branching {

inline int lower_bound(ListVar variable) {
  const long long lower =
      static_cast<long long>(variable.min_length());
  Int::Limits::check(lower, "List::branch_length");
  return static_cast<int>(lower);
}

inline int upper_bound(ListVar variable) noexcept {
  const Length maximum =
      static_cast<Length>(Int::Limits::max);
  return variable.max_length() > maximum
      ? Int::Limits::max
      : static_cast<int>(variable.max_length());
}

inline IntVar length_variable(
    Home home,
    ListVar variable) {
  IntVar result(
      home,
      lower_bound(variable),
      upper_bound(variable));
  length(home, variable, result);
  return result;
}

}}} // namespace Gecode::List::Branching

namespace Gecode {

/**
 * Branch on the length of one list variable.
 *
 * This deliberately reuses Gecode's integer value-selection policies. The
 * auxiliary IntVar is linked bidirectionally to the ListVar by length().
 */
inline void branch_length(
    Home home,
    ListVar variable,
    IntValBranch values = INT_VAL_MIN()) {
  GECODE_POST;

  IntVar length =
      List::Branching::length_variable(
          home,
          variable);

  branch(home, length, values);
}


/** Branch on the lengths of several list variables. */
inline void branch_length(
    Home home,
    const ListVarArgs& variables,
    IntVarBranch variable_selection,
    IntValBranch value_selection) {
  GECODE_POST;

  IntVarArgs lengths(variables.size());

  for (int index = 0;
       index < variables.size();
       ++index) {
    lengths[index] =
        List::Branching::length_variable(
            home,
            variables[index]);
  }

  branch(
      home,
      lengths,
      variable_selection,
      value_selection);
}

/** Branch on list lengths in argument order. */
inline void branch_length(
    Home home,
    const ListVarArgs& variables,
    IntValBranch value_selection = INT_VAL_MIN()) {
  branch_length(
      home,
      variables,
      INT_VAR_NONE(),
      value_selection);
}

} // namespace Gecode

#endif
