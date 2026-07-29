# Dashed

`Dashed` is a clean-room refactoring kernel for G-Strings: finite lists of
**generic integers** represented by dashed blocks, intended to become a native
Gecode variable type named `ListVar`.

This repository is **milestone 1**. It contains a tested, Gecode-independent
C++17 domain/propagation kernel and a concrete Gecode 6.4 integration blueprint.
It does not yet claim to be a drop-in replacement for every G-Strings
constraint.

## What is implemented

- Generic integer alphabets (`ValueSet`), with no character-set assumptions.
- Persistent dashed-list domains.
- Exact fixed runs stored as shared immutable slices rather than one block per
  integer.
- Canonical normalization and length-bound maintenance.
- Conservative propagation kernels for:
  - equality;
  - disequality;
  - reified equality;
  - reified disequality;
  - concatenation;
  - length.
- Unit and small exhaustive property tests.
- AddressSanitizer/UndefinedBehaviorSanitizer build option.
- CMake install/export support (`find_package(Dashed CONFIG)`).
- Gecode integration material:
  - `list.vis` modification events and propagation conditions;
  - proposed `ListVarImp`, `ListVar`, and `ListView` responsibilities;
  - a propagator adapter pattern that keeps cross-variable logic outside the
    variable implementation.

## Memory representation

A domain is a normalized sequence of two segment kinds:

```text
RepeatSegment  = (finite integer set, minimum repetitions, maximum repetitions)
LiteralSegment = immutable exact sequence slice
```

A fixed list of 100,000 integers occupies one backing `std::vector<int>` and
one `LiteralSegment`. Prefixes and suffixes share the same backing storage:

```text
whole  : storage S, [0, 100000)
prefix : storage S, [0,  60000)
suffix : storage S, [60000, 100000)
```

Cloning a domain copies small descriptors and shared pointers, not the literal
payload. Adjacent slices from the same buffer are normalized back into one
segment. Consecutive old-style `(singleton,1,1)` blocks are also compacted in
one pass into one literal segment, so a few unknown blocks can surround a very
long fixed run without one segment per integer.

`ValueSet` also avoids heap allocation for the common cases empty, singleton,
and one interval. Multiple disjoint ranges use shared immutable storage.

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for invariants and trade-offs.

## Build the standalone kernel

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug \
  -DDASHED_ENABLE_SANITIZERS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Release build:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
ctest --test-dir build-release --output-on-failure
```

Run the fixed-sequence microbenchmark:

```bash
./build-release/dashed_fixed_sequence_bench
./build-release/dashed_old_style_compaction_bench
```

## Small example

```cpp
#include <dashed/dashed.hpp>

using dashed::Domain;
using dashed::ValueSet;

Domain x = Domain::top(ValueSet(-100, 100), 0, 20);
Domain y = Domain::fixed({7, 8, 9});

auto result = dashed::propagate_equal(x, y);
// x is now the fixed integer list [7, 8, 9].
```

## Gecode integration status

The integration directory is deliberately separated from the tested kernel.
It documents the native Gecode architecture and includes the `.vis`
specification required to generate `ListVarImpBase`.

The intended layering is:

```text
ListVar / ListView
        |
        v
ListVarImp  -- owns exactly one monotone Domain
        |
        v
standalone dashed domain operators
        ^
        |
propagators -- coordinate two or more views
```

In particular, equality and concatenation are **not** methods of `ListVarImp`.
The old prototype's variable implementation mixed storage, notification, and
multi-variable algorithms; Dashed separates them.

Read [integration/gecode/README.md](integration/gecode/README.md) before trying
the adapter. The adapter is not part of the default build because it requires a
Gecode source build with generated variable-implementation headers.

## Current propagation boundary

The first kernels are intentionally conservative. They preserve correctness
and already perform useful length reasoning, assigned-value transfer, exact
prefix/suffix propagation, and safe same-shape intersections. The full original
dashed sweep algorithms have not yet been ported. Their port belongs after the
new domain invariants and Gecode lifecycle have been validated.

See [docs/ROADMAP.md](docs/ROADMAP.md).

## Repository guide

```text
include/dashed/       standalone public API
src/                  domain and propagation implementation
tests/                unit and exhaustive small-domain tests
bench/                memory/representation microbenchmark
integration/gecode/   Gecode 6.4 integration blueprint
docs/                 design, audit, semantics, and roadmap
```

## License

MIT. See [LICENSE](LICENSE).
