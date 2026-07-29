# Initial G-Strings audit

This is a design audit, not a line-by-line defect report. Paths refer to the
G-Strings fork at `github.com/ramadini/gecode`.

## Main structural issues

### `StringVarImp` does too much

The old variable implementation stores `DashedString` and also implements
cross-variable operations such as equality and concatenation. This couples:

- domain representation;
- propagation algorithms;
- mutation of several variable implementations;
- notification of several subscription lists.

In Dashed, `ListVarImp` owns and restricts only its own domain. Propagators own
relationships among views.

### The view is not thin

The old `StringView` exposes mutable domain internals and forwards
multi-variable algorithms. A standard Gecode view should expose queries and
unary update operations, while propagators coordinate multiple views.

The new `ListView::update` must only perform the standard cloned-pointer update.
The domain is cloned by `ListVarImp::copy`; it must not be copied a second time
from the view.

### Ownership is difficult to verify

The prototype mixes Space allocation, manually linked range data, raw pointers,
and ordinary C++ heap ownership. That makes it hard to prove that every path has
matching allocation/disposal behavior, particularly across failure and space
cloning.

The new kernel uses RAII and immutable shared payloads. The Gecode variable type
is marked disposable so shared pointers and vectors are destructed when the
space is destroyed.

### Character assumptions leak into the abstraction

String symbols and alphabets are tied to character-oriented types in several
interfaces. The replacement uses `int` and `ValueSet` throughout. Text encoding
belongs in an optional modelling layer, not the solver core.

### Domain algorithms are hard to test independently

Old propagation logic is embedded in Gecode types. The new kernel is standalone,
allowing exhaustive tests of all short lists over small alphabets and sanitizer
runs without rebuilding the whole solver.

## Porting rule

Do not mechanically rename old classes. For each old operation:

1. state its language semantics;
2. write a pure `Domain` restriction;
3. property-test it by enumeration;
4. map its `Change` result to a Gecode modification event;
5. add a propagator-level test for subscriptions, cloning, failure, and
   subsumption.
