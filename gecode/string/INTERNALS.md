# Dashed internals

This document describes the implementation contracts shared by the string
variable, dashed-domain, and propagator layers. Public constraint semantics are
defined by `gecode/string.hh`; the structures below are implementation details.

## Dashed domains

A `DashedString` is an ordered sequence of `DSBlock` values. Each block contains
a character set and lower and upper occurrence bounds. The domain also stores
aggregate minimum and maximum string lengths. Normalized domains contain no
empty or mergeable adjacent blocks, and the aggregate bounds are consistent
with the block bounds.

`DSIntSet`, `DSBlock`, and `DSBlocks` are owned by a Gecode `Space`. Their clone
constructors allocate and copy into the destination space. `NSIntSet`,
`NSBlock`, and `NSBlocks` have ordinary C++ lifetimes and are used for temporary
domains and refinements. Code must not retain an `NS*` object in space-owned
state.

Dashed-domain operations report failure with their Boolean return value and
record successful mutation in `DashedString::_changed`. They do not schedule
propagators directly. The variable implementation owns that responsibility.

## Mutation transactions

Every multi-domain operation in `StringVarImp` creates a `DomainState` for each
domain before mutation. A state snapshots aggregate length bounds and whether
the variable was unassigned, and resets the domain's changed marker. After the
operation succeeds, the state emits at most one aggregate event:

* `ME_STRING_VAL` when the domain becomes assigned;
* `ME_STRING_LEN` when either aggregate length bound changes;
* `ME_STRING_DOM` for any other domain refinement;
* `ME_STRING_NONE` when the operation is a no-op.

Operations touching several variables combine these events with `me_combine`.
`StringVarImp::refine` applies an already-computed normalized `NSBlocks` domain
and explicitly records the replacement as a change. In-place refinements use
`begin_refinement` and `commit_refinement`; the commit similarly records the
change before classifying it. Propagators must not update `DashedString` and
issue notifications separately.
This analyze-mutate-notify order is part of the variable implementation
contract: adding a new dashed-domain operation must not notify from inside the
domain layer or leave a stale changed marker for the next transaction.

`StringView::domain()` exposes only a constant reference. Mutable domain access
is private to GCC, Find, and the decomposed regular-star poster and requires the
matching `DomainState` token from `begin_refinement`; all three writers commit
that token through `commit_refinement`. Other production callers either inspect
the constant domain or commit an analyzed ordinary-lifetime domain through
`StringVarImp::refine`. The regular-star path analyzes an `NSBlocks` copy before
mutating, so early failure cannot leave a partial update and actors subscribed
before posting see the resulting aggregate event.

## Sweep propagation

`sweep_x` analyzes whether blocks from one sequence can match another sequence.
It records proposed refinements in a `uvec` and does not apply them while
matching. This separation prevents an early refinement from changing the
meaning of later positions in the same pass.

Symmetric constraints use `sweep_both`. It runs `sweep_x` in one direction,
commits that pass, clears and reuses the refinement vector, then analyzes and
commits the reverse direction. Equality, reverse, binary concatenation, and
general concatenation share this orchestration. The direction and commit order
must remain stable because the second pass intentionally observes the first
pass's refinements.

## Propagator lifecycle

Propagators first handle assigned-variable rewrites and subsumption, then run
domain propagation. Concat, reverse, and power use explicit local loops only
for deterministic assignment-triggered fixpoints. Regular and reified regular
propagation use explicit loops around their complete forward/backward analysis
and commit stages. Numeric string channels similarly loop only when propagation
assigns one endpoint. Element performs its final assigned-index rewrite
directly, while GConcat and GCC share a terminal assigned-state helper between
entry and post-refinement paths.

Regular propagation records each backward pass as per-source-block refinements.
`merge_refined_blocks` drops null blocks and merges adjacent equal character
sets in source order; `commit_refined_blocks` then updates the dashed domain and
emits one aggregate variable event. Non-reified forward and reverse passes and
reified propagation share this commit path.

Automaton transition helpers append directly to caller-owned frontier sets.
Mandatory regular and match passes use this form to accumulate all source-state
transitions without constructing one temporary set per state. Optional
traversals visit matching destinations directly and use their distance arrays
for deduplication, so filtered transition APIs never allocate an intermediate
frontier.

`trimDFA` and `compDFA` own construction of their block-filtered reverse tables.
Forward-reverse and backward propagation consume those tables without knowing
their label representation. The tables remain pass-local: this avoids
duplicating immutable automaton storage and removes character checks from their
inner traversal loops.

Replacement runs each operation-specific transformation as one complete pass.
An explicit outer loop repeats that pass only for the assignment combinations
that previously caused an immediate recursive restart, preserving the posting
and subsumption order within each pass.

## Sharing and cloning

Immutable automata are held by typed shared handles. Propagator clones share
those handles and register disposal when required; temporary frontier storage
is rebuilt or moved between generations rather than shared between spaces.

GCC similarly shares its immutable character/index mapping and complete cover
set between clones. The cover contains every constrained character even when
its occurrence variable is assigned; occurrence counts are never character
identifiers and must not be used to remove entries from that set.

`StringVarImp` clones its `DashedString` into the destination `Space` exactly
once; `StringView::update` only installs that copied variable implementation
and does not copy the domain again. `StringBrancher` similarly owns cloning its
complete `ViewArray`; concrete branchers copy only their additional scalar
state. Branch choices archive only the selected variable position and scalar
split data, never pointers into a space or dashed domain.
