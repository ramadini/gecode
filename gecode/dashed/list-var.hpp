#pragma once

// Public-variable shape. This is a blueprint until compiled with generated
// ListVarImpBase support in Gecode 6.4.

#include <dashed/domain.hpp>
#include <gecode/kernel.hh>

#include "list-var-imp.hpp"

#include <utility>
#include <vector>

namespace Gecode {

namespace Dashed { class ListView; }

class ListVar : public VarImpVar<Dashed::ListVarImp> {
  friend class Dashed::ListView;
  using VarImpVar<Dashed::ListVarImp>::x;

 public:
  ListVar() = default;
  ListVar(const ListVar& y)
      : VarImpVar<Dashed::ListVarImp>(y.varimp()) {}

  explicit ListVar(Space& home, dashed::Domain domain) {
    if (domain.failed())
      throw Exception("ListVar", "empty list domain");
    x = new (home) Dashed::ListVarImp(home, std::move(domain));
  }

  bool assigned() const noexcept { return x->assigned(); }
  unsigned int min_length() const noexcept { return x->min_length(); }
  unsigned int max_length() const noexcept { return x->max_length(); }
  const dashed::Domain& domain() const noexcept { return x->domain(); }
  std::vector<int> val() const { return x->domain().value(); }

  ListVar& operator=(const ListVar&) = default;
};

}  // namespace Gecode
