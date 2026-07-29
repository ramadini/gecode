#pragma once

// Public-variable shape. This is a blueprint until compiled with generated
// ListVarImpBase support in Gecode 6.4.

#include <dashed/domain.hpp>
#include <gecode/kernel.hh>

#include "list-var-imp.hpp"

#include <ostream>
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


class ListVarArgs;
class ListVarArray;

/** Array traits for passing ListVar arguments through generic Gecode APIs. */
template<>
class ArrayTraits<VarArgArray<ListVar>> {
 public:
  using StorageType = ListVarArgs;
  using ValueType = ListVar;
  using ArgsType = ListVarArgs;
};

/** Array traits for space-owned ListVar arrays. */
template<>
class ArrayTraits<VarArray<ListVar>> {
 public:
  using StorageType = ListVarArray;
  using ValueType = ListVar;
  using ArgsType = ListVarArgs;
};

/** Heap-backed argument array of ListVar handles. */
class ListVarArgs : public VarArgArray<ListVar> {
 public:
  ListVarArgs() = default;

  explicit ListVarArgs(int n)
      : VarArgArray<ListVar>(n) {}

  ListVarArgs(const ListVarArgs& other)
      : VarArgArray<ListVar>(other) {}

  ListVarArgs(const VarArray<ListVar>& other)
      : VarArgArray<ListVar>(other) {}

  /** Create n independent variables with copies of the same domain. */
  ListVarArgs(
      Space& home,
      int n,
      const dashed::Domain& domain)
      : VarArgArray<ListVar>(n) {
    if (domain.failed())
      throw Exception("ListVarArgs", "empty list domain");

    for (int i = 0; i < n; ++i)
      (*this)[i] = ListVar(home, domain);
  }

  ListVarArgs& operator=(const ListVarArgs&) = default;
};

/** Space-owned array of ListVar handles. */
class ListVarArray : public VarArray<ListVar> {
 public:
  ListVarArray() = default;

  ListVarArray(Space& home, int n)
      : VarArray<ListVar>(home, n) {}

  ListVarArray(
      Space& home,
      const ListVarArgs& arguments)
      : VarArray<ListVar>(home, arguments) {}

  /** Create n independent variables with copies of the same domain. */
  ListVarArray(
      Space& home,
      int n,
      const dashed::Domain& domain)
      : VarArray<ListVar>(home, n) {
    if (domain.failed())
      throw Exception("ListVarArray", "empty list domain");

    for (int i = 0; i < n; ++i)
      (*this)[i] = ListVar(home, domain);
  }

  ListVarArray& operator=(const ListVarArray&) = default;
};

/** Print the canonical dashed domain of a list variable. */
inline std::ostream& operator<<(
    std::ostream& out,
    const ListVar& variable) {
  return out << variable.domain();
}


/** Traits for the concrete ListVarArgs public type. */
template<>
class ArrayTraits<ListVarArgs> {
 public:
  using StorageType = ListVarArgs;
  using ValueType = ListVar;
  using ArgsType = ListVarArgs;
};

/** Traits for the concrete ListVarArray public type. */
template<>
class ArrayTraits<ListVarArray> {
 public:
  using StorageType = ListVarArray;
  using ValueType = ListVar;
  using ArgsType = ListVarArgs;
};

}  // namespace Gecode
