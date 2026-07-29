# Architecture

## Design goals

1. Preserve the dashed-string abstraction while making symbols arbitrary
   integers.
2. Keep the domain kernel independent of Gecode so it can be tested exhaustively.
3. Make a future `ListVar` follow Gecode's standard separation between variable
   implementation, public variable, view, and propagator.
4. Make cloning cheap and ownership explicit.
5. Never sacrifice soundness for an optimization.

## Domain language

A dashed list is a sequence of segments. A repeated segment

```text
(S, l, u)
```

denotes between `l` and `u` consecutive integers, each selected from the finite
set `S`. A literal segment denotes exactly one integer sequence.

The language of a domain is the concatenation of the segment languages,
intersected with its global length interval.

## Canonical invariants

After `Domain::normalize()`:

- failure is represented explicitly;
- `min_length <= max_length`;
- the global interval is intersected with the sums of segment bounds;
- empty exact literals are removed;
- impossible positive repeat blocks with an empty value set fail;
- adjacent repeat blocks with identical value sets are merged;
- adjacent literal slices that are contiguous in the same immutable backing
  buffer are merged without copying;
- repeat counts are tightened from the global length interval where safe;
- assignment means fixed total length and every segment has one exact value and
  exact count.

## Persistent exact runs

`LiteralSlice` stores:

```cpp
std::shared_ptr<const std::vector<int>> storage;
std::size_t offset;
std::size_t size;
```

This is intentionally not Gecode-space memory. It is immutable, reference
counted, and safely shared by cloned spaces. A `ListVarImp` therefore needs the
Gecode disposer path so the C++ object holding the shared pointer is destroyed
when the space dies.

Benefits:

- exact sequences are dense rather than one object per symbol;
- Gecode cloning is nearly constant-time for long literals;
- prefix/suffix propagation is zero-copy;
- concatenating adjacent slices of the same storage is zero-copy;
- external candidate checking can stream over a contiguous span.

Potential later optimization: a small inline literal buffer or immutable rope
for workloads that repeatedly concatenate literals from different backing
buffers. This is not needed to solve the motivating "few unknown blocks plus
long fixed runs" case.

## Generic finite value sets

`ValueSet` uses a tagged representation:

- empty: no payload;
- singleton: one integer inline;
- interval: two integers inline;
- multiple ranges: `shared_ptr<const vector<IntRange>>`.

The representation is independent of Unicode, bytes, ASCII, or any other
character model.

## Mutation model

A `Domain` is mutable only through monotone restriction operations. Shared
payloads are immutable. An update either:

- leaves the language unchanged;
- replaces the domain by a subset;
- marks failure.

The public change classification is:

```text
none, length, domain, both, assigned, failed
```

The eventual Gecode adapter maps these classes to generated modification events.

## Exact search partitions

Search decisions are first defined and exhaustively checked in the pure domain
kernel. A count decision partitions one repeat-count interval at its midpoint
only when all other counts are exact. A value decision first isolates one
occurrence of an exact-count repeat block, then partitions only that logical
position's sparse value set. Both operations preserve immutable payload sharing
and construct normalized child domains.

Several variable-width blocks can give one concrete list multiple internal
segment decompositions, so splitting one such count would not form disjoint
language alternatives. The kernel reports no exact decision for that case
rather than introducing unsound search. This keeps branch semantics independent
of Gecode lifecycle code.

Archived decisions identify semantics rather than representation nodes. A count
choice stores a total-length threshold; a value choice stores an absolute list
position and an integer pivot. The same decision can therefore classify or
prune descendant domains even when normalization has split, merged, or replaced
the original segments. Exhaustive tests compare literal status and negation
pruning with concrete languages at every recursively generated descendant.

## Separation of responsibilities

### Domain kernel

Purely represents one list domain and performs local monotone operations.
It knows nothing about Gecode subscriptions or other variables.

### Variable implementation

Owns one `Domain`, maps a domain update to a Gecode `ModEvent`, calls
`notify`, supports cloning, and releases C++ payloads through `dispose`.

It must not implement equality, concatenation, or other multi-variable
constraints.

### View

A thin handle forwarding queries and unary updates to `ListVarImp`, consistent
with `IntView` and `SetView`. `update(home, other)` updates only the variable
implementation pointer during cloning; it does not manually clone domain state.

### Propagator

Copies or references the current domains, runs a standalone propagation kernel,
and commits each resulting restriction through its corresponding view. It owns
the relationship among variables and decides subscriptions/subsumption.

## Why not `vector<IntVar>`?

A fixed array of integer variables forces a maximum length, creates a variable
per position, and loses the dashed variable-width abstraction. `ListVar` is one
native variable whose domain compactly represents many list lengths and long
runs.
