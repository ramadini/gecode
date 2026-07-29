#pragma once

// Public-variable shape. This is a blueprint until compiled with generated
// ListVarImpBase support in Gecode 6.4.

#include <gecode/list/domain.hpp>
#include <gecode/kernel.hh>

#include "list-var-imp.hpp"

#include <utility>
#include <vector>

namespace Gecode {

namespace List { class ListView; }

class ListVar : public VarImpVar<List::ListVarImp> {
  friend class List::ListView;
  using VarImpVar<List::ListVarImp>::x;

 public:
  ListVar() = default;
  ListVar(const ListVar&) = default;

  explicit ListVar(Space& home, List::Domain domain) {
    if (domain.failed())
      throw Exception("ListVar", "empty list domain");
    x = new (home) List::ListVarImp(home, std::move(domain));
  }

  bool assigned() const noexcept { return x->assigned(); }
  unsigned int min_length() const noexcept { return x->min_length(); }
  unsigned int max_length() const noexcept { return x->max_length(); }
  const List::Domain& domain() const noexcept { return x->domain(); }
  std::vector<int> val() const { return x->domain().value(); }

  ListVar& operator=(const ListVar&) = default;
};

}  // namespace Gecode
