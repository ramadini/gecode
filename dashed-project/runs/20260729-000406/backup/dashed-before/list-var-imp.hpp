#pragma once

// Blueprint: compile this only inside a Gecode 6.4 source tree after list.vis
// has generated Gecode::Dashed::ListVarImpBase and event constants.

#include <dashed/domain.hpp>
#include <gecode/kernel.hh>

#include <cassert>
#include <memory>
#include <utility>

namespace Gecode { namespace Dashed {

class ListDelta : public Delta {
 public:
  explicit ListDelta(dashed::Change change = dashed::Change::none) noexcept
      : change_(change) {}

  dashed::Change change() const noexcept { return change_; }

 private:
  dashed::Change change_;
};

class ListVarImp : public ListVarImpBase {
 public:
  ListVarImp(Space& home, dashed::Domain domain)
      : ListVarImpBase(home), domain_(std::move(domain)) {
    assert(!domain_.failed());
  }

  ListVarImp(Space& home, ListVarImp& other)
      : ListVarImpBase(home, other), domain_(other.domain_) {}

  const dashed::Domain& domain() const noexcept { return domain_; }
  bool assigned() const noexcept { return domain_.assigned(); }
  unsigned int min_length() const noexcept { return domain_.min_length(); }
  unsigned int max_length() const noexcept { return domain_.max_length(); }

  // Standard dependency interface expected by VarImpView.
  void subscribe(Space& home, Propagator& p, PropCond pc,
                 bool schedule = true) {
    ListVarImpBase::subscribe(
        home, p, pc, assigned(), schedule);
  }

  void reschedule(Space& home, Propagator& p, PropCond pc) {
    ListVarImpBase::reschedule(
        home, p, pc, assigned());
  }

  void subscribe(Space& home, Advisor& a, bool fail = false) {
    ListVarImpBase::subscribe(
        home, a, assigned(), fail);
  }

  ModEvent replace(Space& home, dashed::Domain restricted) {
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

    dashed::Change change;
    if (!old_assigned && domain_.assigned()) {
      change = dashed::Change::assigned;
    } else {
      const bool length_changed = old_min != domain_.min_length() ||
                                  old_max != domain_.max_length();
      change = length_changed ? dashed::Change::both
                              : dashed::Change::domain;
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
  void dispose(Space&) noexcept {
    // Gecode still reads next_d() after this call, so do not destroy
    // the complete ListVarImp object. Destroy only the external payload.
    std::destroy_at(std::addressof(domain_));
  }

 private:
  dashed::Domain domain_;

  ListVarImp* perform_copy(Space& home) {
    return new (home) ListVarImp(home, *this);
  }

  ModEvent notify_change(Space& home, dashed::Change change) {
    ModEvent me = ME_LIST_NONE;
    switch (change) {
      case dashed::Change::none:     return ME_LIST_NONE;
      case dashed::Change::length:   me = ME_LIST_LEN; break;
      case dashed::Change::domain:   me = ME_LIST_DOM; break;
      case dashed::Change::both:     me = ME_LIST_BOTH; break;
      case dashed::Change::assigned: me = ME_LIST_VAL; break;
      case dashed::Change::failed:   return fail(home);
    }
    ListDelta delta(change);
    return notify(home, me, delta);
  }
};

}}  // namespace Gecode::Dashed
