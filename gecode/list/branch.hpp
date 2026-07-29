#ifndef __GECODE_LIST_BRANCH_HPP__
#define __GECODE_LIST_BRANCH_HPP__

#include <gecode/int.hh>
#include <gecode/kernel.hh>
#include <gecode/list/backend.hpp>
#include <gecode/list/length.hpp>
#include <gecode/list/list-view.hpp>

#include <cassert>
#include <cstddef>
#include <optional>
#include <ostream>
#include <utility>

namespace Gecode { namespace List {

/**
 * Native no-good literal for one semantic exact-domain branch alternative.
 *
 * The literal owns a ListView plus a representation-independent backend
 * decision. It therefore survives space cloning and domain normalization.
 */
class ExactDomainNGL final : public NGL {
 private:
  ListView variable_;
  Backend::BranchDecision decision_;
  unsigned int alternative_;

  PropCond condition() const noexcept {
    return decision_.kind == Backend::BranchKind::repeat_count
        ? PC_LIST_LEN
        : PC_LIST_ANY;
  }

 public:
  ExactDomainNGL(
      Space& home,
      ListView variable,
      Backend::BranchDecision decision,
      unsigned int alternative)
      : NGL(home),
        variable_(variable),
        decision_(std::move(decision)),
        alternative_(alternative) {
    assert(alternative_ < 2);
  }

  ExactDomainNGL(
      Space& home,
      ExactDomainNGL& other)
      : NGL(home, other),
        variable_(),
        decision_(other.decision_),
        alternative_(other.alternative_) {
    variable_.update(home, other.variable_);
  }

  void subscribe(
      Space& home,
      Propagator& propagator) override {
    variable_.subscribe(
        home,
        propagator,
        condition());
  }

  void cancel(
      Space& home,
      Propagator& propagator) override {
    variable_.cancel(
        home,
        propagator,
        condition());
  }

  void reschedule(
      Space& home,
      Propagator& propagator) override {
    variable_.reschedule(
        home,
        propagator,
        condition());
  }

  Status status(const Space&) const override {
    switch (Backend::branch_literal_status(
        variable_.domain(),
        decision_,
        alternative_)) {
      case Backend::BranchLiteralStatus::failed:
        return FAILED;
      case Backend::BranchLiteralStatus::subsumed:
        return SUBSUMED;
      case Backend::BranchLiteralStatus::undecided:
        return NONE;
    }

    return NONE;
  }

  ExecStatus prune(Space& home) override {
    Domain restricted =
        Backend::prune_branch_literal(
            variable_.domain(),
            decision_,
            alternative_);

    const ModEvent event =
        variable_.replace(
            home,
            std::move(restricted));

    return me_failed(event)
        ? ES_FAILED
        : ES_OK;
  }

  NGL* copy(Space& home) override {
    return new (home) ExactDomainNGL(
        home,
        *this);
  }

  std::size_t dispose(Space&) override {
    return sizeof(*this);
  }
};

/**
 * Binary brancher for the exact partition fragment supplied by the current
 * List backend.
 *
 * A choice stores only a variable position and a stable backend decision.
 * It never stores a pointer into a domain, so archived choices remain valid
 * during recomputation and across space clones.
 */
class ExactDomainBrancher final : public Brancher {
 private:
  ViewArray<ListView> variables_;
  mutable int start_;

 public:
  class Choice final : public Gecode::Choice {
   private:
    int position_;
    Backend::BranchDecision decision_;

   public:
    Choice(
        const ExactDomainBrancher& brancher,
        int position,
        Backend::BranchDecision decision)
        : Gecode::Choice(brancher, 2),
          position_(position),
          decision_(std::move(decision)) {}

    int position() const noexcept {
      return position_;
    }

    const Backend::BranchDecision& decision() const noexcept {
      return decision_;
    }

    void archive(Archive& archive) const override {
      Gecode::Choice::archive(archive);

      archive
          << position_
          << static_cast<unsigned int>(decision_.kind)
          << static_cast<unsigned int>(decision_.length_pivot)
          << static_cast<unsigned int>(decision_.position)
          << decision_.value_pivot;
    }
  };

  ExactDomainBrancher(
      Home home,
      ViewArray<ListView>& variables)
      : Brancher(home),
        variables_(variables),
        start_(0) {}

  ExactDomainBrancher(
      Space& home,
      ExactDomainBrancher& other)
      : Brancher(home, other),
        variables_(),
        start_(other.start_) {
    variables_.update(home, other.variables_);
  }

  bool status(const Space&) const override {
    for (int index = start_;
         index < variables_.size();
         ++index) {
      if (variables_[index].assigned())
        continue;

      if (Backend::choose_branch(
              variables_[index].domain())) {
        start_ = index;
        return true;
      }

      // Returning false here would tell Gecode that search is solved even
      // though this ListVar is still unassigned. Fail loudly instead.
      throw Exception(
          "List::branch_exact",
          "domain is outside the exact partition fragment");
    }

    return false;
  }

  Gecode::Choice* choice(Space&) override {
    const std::optional<Backend::BranchDecision> decision =
        Backend::choose_branch(
            variables_[start_].domain());

    assert(decision.has_value());

    return new Choice(
        *this,
        start_,
        *decision);
  }

  Choice* choice(
      const Space&,
      Archive& archive) override {
    int position = 0;
    unsigned int kind = 0;
    unsigned int length_pivot = 0;
    unsigned int logical_position = 0;
    int value_pivot = 0;

    archive
        >> position
        >> kind
        >> length_pivot
        >> logical_position
        >> value_pivot;

    if (kind >
        static_cast<unsigned int>(
            Backend::BranchKind::value_set)) {
      throw Exception(
          "List::branch_exact",
          "invalid archived branch kind");
    }

    Backend::BranchDecision decision;
    decision.kind =
        static_cast<Backend::BranchKind>(kind);
    decision.length_pivot =
        static_cast<Length>(length_pivot);
    decision.position =
        static_cast<Length>(logical_position);
    decision.value_pivot = value_pivot;

    return new Choice(
        *this,
        position,
        std::move(decision));
  }

  ExecStatus commit(
      Space& home,
      const Gecode::Choice& raw_choice,
      unsigned int alternative) override {
    const auto& selected =
        static_cast<const Choice&>(raw_choice);

    if (selected.position() < 0 ||
        selected.position() >= variables_.size()) {
      throw Exception(
          "List::branch_exact",
          "archived variable position is invalid");
    }

    Domain restricted =
        Backend::apply_branch(
            variables_[selected.position()].domain(),
            selected.decision(),
            alternative);

    const ModEvent event =
        variables_[selected.position()].replace(
            home,
            std::move(restricted));

    return me_failed(event)
        ? ES_FAILED
        : ES_OK;
  }

  NGL* ngl(
      Space& home,
      const Gecode::Choice& raw_choice,
      unsigned int alternative) const override {
    const auto& selected =
        static_cast<const Choice&>(raw_choice);

    if (selected.position() < 0 ||
        selected.position() >= variables_.size()) {
      throw Exception(
          "List::branch_exact",
          "archived variable position is invalid");
    }

    return new (home) ExactDomainNGL(
        home,
        variables_[selected.position()],
        selected.decision(),
        alternative);
  }

  void print(
      const Space&,
      const Gecode::Choice& raw_choice,
      unsigned int alternative,
      std::ostream& out) const override {
    const auto& selected =
        static_cast<const Choice&>(raw_choice);
    const Backend::BranchDecision& decision =
        selected.decision();

    out << "list[" << selected.position() << "] ";

    switch (decision.kind) {
      case Backend::BranchKind::repeat_count:
        out << "length "
            << (alternative == 0 ? "<= " : "> ")
            << decision.length_pivot;
        break;

      case Backend::BranchKind::value_set:
        out << "position[" << decision.position << "].value "
            << (alternative == 0 ? "<= " : "> ")
            << decision.value_pivot;
        break;
    }
  }

  Actor* copy(Space& home) override {
    return new (home) ExactDomainBrancher(
        home,
        *this);
  }

  std::size_t dispose(Space&) override {
    return sizeof(*this);
  }

  static void post(
      Home home,
      const ListVarArgs& variables) {
    if (variables.size() == 0)
      return;

    ViewArray<ListView> views(
        home,
        variables.size());

    for (int index = 0;
         index < variables.size();
         ++index) {
      views[index] =
          ListView(variables[index]);

      if (!views[index].assigned() &&
          !Backend::choose_branch(
              views[index].domain())) {
        throw Exception(
            "List::branch_exact",
            "domain is outside the exact partition fragment");
      }
    }

    (void) new (home) ExactDomainBrancher(
        home,
        views);
  }
};

namespace Branching {

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

} // namespace Branching

}} // namespace Gecode::List

namespace Gecode {

/**
 * Branch directly on one ListVar using exact backend language partitions.
 *
 * This is intentionally stricter than branch_length(): an unsupported
 * unassigned domain raises an exception rather than being reported as a
 * solved search leaf.
 */
inline void branch_exact(
    Home home,
    ListVar variable) {
  GECODE_POST;

  ListVarArgs variables(1);
  variables[0] = variable;
  List::ExactDomainBrancher::post(
      home,
      variables);
}

/** Branch directly on ListVars in argument order. */
inline void branch_exact(
    Home home,
    const ListVarArgs& variables) {
  GECODE_POST;
  List::ExactDomainBrancher::post(
      home,
      variables);
}

/** Branch on the length of one list variable. */
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
