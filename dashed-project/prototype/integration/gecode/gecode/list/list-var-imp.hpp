#pragma once

// Blueprint: compile this only inside a Gecode 6.4 source tree after list.vis
// has generated Gecode::List::ListVarImpBase and event constants.

#include <gecode/list/backend.hpp>
#include <gecode/kernel.hh>

#include <utility>

namespace Gecode { namespace List {

class ListDelta : public Delta {
 public:
  explicit ListDelta(Backend::Change change = Backend::Change::none) noexcept
      : change_(change) {}

  Backend::Change change() const noexcept { return change_; }

 private:
  Backend::Change change_;
};

class ListVarImp : public ListVarImpBase {
 public:
  ListVarImp(Space& home, Domain domain)
      : ListVarImpBase(home), domain_(std::move(domain)) {
    GECODE_ASSERT(!domain_.failed());
  }

  ListVarImp(Space& home, ListVarImp& other)
      : ListVarImpBase(home, other), domain_(other.domain_) {}

  const Domain& domain() const noexcept { return domain_; }
  bool assigned() const noexcept { return domain_.assigned(); }
  unsigned int min_length() const noexcept { return domain_.min_length(); }
  unsigned int max_length() const noexcept { return domain_.max_length(); }

  ModEvent replace(Space& home, Domain restricted) {
    // The finished adapter should use an explicit language-subset assertion in
    // debug builds. Equality is not sufficient to prove monotonicity.
    if (restricted.failed())
      return fail(home);
    if (restricted == domain_)
      return ME_LIST_NONE;

    const bool old_assigned = domain_.assigned();
    const auto old_min = domain_.min_length();
    const auto old_max = domain_.max_length();
    domain_ = std::move(restricted);

    Backend::Change change;
    if (!old_assigned && domain_.assigned()) {
      change = Backend::Change::assigned;
    } else {
      const bool length_changed = old_min != domain_.min_length() ||
                                  old_max != domain_.max_length();
      change = length_changed ? Backend::Change::both
                              : Backend::Change::domain;
    }
    return notify_change(home, change);
  }

  ModEvent tighten_length(Space& home, unsigned int lower,
                          unsigned int upper) {
    return notify_change(home, domain_.tighten_length(lower, upper));
  }

  ListVarImp* copy(Space& home) {
    // Match the generated-base forwarding protocol exactly in the integration
    // branch. This is the standard shape used by Gecode variable types.
    return copied() ? static_cast<ListVarImp*>(forward())
                    : perform_copy(home);
  }

  static ModEventDelta med(ModEvent me) { return ListVarImpBase::med(me); }

  // Dispose:true is intentional: Domain contains shared_ptr/vector payloads.
  // Confirm the exact generated signature in Gecode 6.4 and invoke the base
  // disposer bookkeeping required by that generated class.
  void dispose(Space&) { domain_ = Domain::empty_list(); }

 private:
  Domain domain_;

  ListVarImp* perform_copy(Space& home) {
    return new (home) ListVarImp(home, *this);
  }

  ModEvent notify_change(Space& home, Backend::Change change) {
    ModEvent me = ME_LIST_NONE;
    switch (change) {
      case Backend::Change::none:     return ME_LIST_NONE;
      case Backend::Change::length:   me = ME_LIST_LEN; break;
      case Backend::Change::domain:   me = ME_LIST_DOM; break;
      case Backend::Change::both:     me = ME_LIST_BOTH; break;
      case Backend::Change::assigned: me = ME_LIST_VAL; break;
      case Backend::Change::failed:   return fail(home);
    }
    ListDelta delta(change);
    return notify(home, me, delta);
  }
};

}}  // namespace Gecode::List
