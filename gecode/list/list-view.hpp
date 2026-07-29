#pragma once

#include "list-var.hpp"

namespace Gecode { namespace List {

class ListView : public VarImpView<ListVar> {
 protected:
  using VarImpView<ListVar>::x;

 public:
  ListView() = default;
  explicit ListView(const ListVar& y)
      : VarImpView<ListVar>(y.varimp()) {}
  explicit ListView(ListVarImp* y) : VarImpView<ListVar>(y) {}

  const Domain& domain() const noexcept { return x->domain(); }
  bool assigned() const noexcept { return x->assigned(); }
  unsigned int min_length() const noexcept { return x->min_length(); }
  unsigned int max_length() const noexcept { return x->max_length(); }
  std::vector<int> val() const { return x->domain().value(); }

  ModEvent replace(Space& home, Domain restricted) {
    return x->replace(home, std::move(restricted));
  }

  ModEvent tighten_length(Space& home, unsigned int lower,
                          unsigned int upper) {
    return x->tighten_length(home, lower, upper);
  }

  static ModEventDelta med(ModEvent me) { return ListVarImp::med(me); }

  void update(Space& home, ListView& other) {
    // No manual domain copying here. ListVarImp::copy owns that responsibility.
    VarImpView<ListVar>::update(home, other);
  }
};

}}  // namespace Gecode::List
