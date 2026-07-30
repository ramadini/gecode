# Validation status

## Tested in this milestone

The standalone kernel was built with GCC 14.2 in C++17 mode using both:

```text
Debug + AddressSanitizer + UndefinedBehaviorSanitizer
Release
```

The following passed:

- core unit tests;
- exhaustive small-language property tests;
- explicit leak detection during the sanitizer test run;
- CMake installation;
- a separate downstream `find_package(Dashed CONFIG)` consumer build;
- structural validation of `list.vis` event combinations and scheduling names.

The property suite enumerates short lists over a small generic integer alphabet
and checks that propagation never removes a feasible tuple for:

- equality;
- disequality;
- reified equality;
- concatenation;
- length.

It also checks domain-language preservation under normalization. Dedicated
exactness oracles compare assigned-side disequality pruning against concrete
language subtraction for both one-position witnesses and endpoint singleton
repetition counts.

## Representation checks

For one million fixed integers:

```text
elements=1000000
segments(original)=1
segments(joined)=1
structural_bytes=80
referenced_dynamic_bytes=4000024
owned_dynamic_bytes=0
```

The `owned_dynamic_bytes=0` result occurs after the original, two slices, and
recombined domain share the same immutable payload; it does not mean the payload
uses no memory.

For 100,000 old-style unit singleton blocks between two unknown blocks:

```text
fixed_unit_blocks_input=100000
segments_after_normalization=3
structural_bytes=160
referenced_dynamic_bytes=524312
```

Timing values are intentionally not recorded as stable benchmark claims because
they depend heavily on machine, compiler, and allocator.

## Not yet validated

The native Gecode adapter has not been compiled in this environment because a
Gecode 6.4 source build and generated `ListVarImpBase` headers are not installed.
The `.vis` file and adapter code are therefore an integration blueprint, not a
claim of a completed native variable module.

The next validation gate is a Gecode source build with:

```text
GECODE_WITH_VIS=gecode/list/var-imp/list.vis
GECODE_REGENERATE_VARIMP=ON
GECODE_SANITIZER=address-undefined
```

That build must test space cloning and destruction repeatedly before more of the
old sweep propagators are ported.

## G-Strings differential baseline

Run the translated equality corpus and verify it against the preserved legacy
source:

```sh
./dashed-project/scripts/run-gstrings-differential.sh
```

The verifier first searches preserved filesystem copies and local Git history.
The explicit runner may fetch `origin/master` or `origin/main` when the source
is not already available. The gate covers six `str_test2.cpp` root-domain
fixtures and an independent exact solution oracle for five small native DFS
models. It also builds a revision-keyed historical G-Strings checkout, runs the
real `str_test2` assertions, and compares its normalized six-case report with
the List kernel. The native search runner rejects missing, extra, and duplicate
solutions and records nodes, failures, and maximum depth. Direct historical
search-stat comparison remains pending. See `DIFFERENTIAL_TESTING.md` for the
report protocols and remaining work.

### Classified legacy root differences

`dashed_gstrings_extended_tests` exercises legacy `str_test2` cases 03-08.
For finite cases it enumerates the concrete languages and verifies soundness
before assigning a differential classification. The deterministic classified
TSV report is checked by `dashed_gstrings_extended_report`.

### Saturated global-length regression

`str_test2::test13` is a normalization regression for capped/unbounded length
arithmetic. The domain `(a|b)^0..M` intersected with `(a|b|c)^0..M b` must
retain the one-element list `b`. Unit and native-clone tests verify that an
unbounded segment can still contribute zero values when another segment is
mandatory.

### Fixed-literal differential regression

`dashed_gstrings_fixed_tests` covers legacy equality cases 17-20 and 22. It
checks exact assignment, long fixed-prefix/fixed-suffix boundaries, canonical
merging of adjacent literals from unrelated storage, and convergence to the
assigned list `aab`. The explicit G-Strings differential script additionally
extracts and replays the exact historical fixed strings, including the large
`test19` payload.

### Lower-level sweep stress regression

`dashed_gstrings_sweep_stress_tests` covers preserved `str_test2` cases 21 and
23. Case 21 verifies that global count tightening is followed by another
canonicalization pass and that a 50-position sweep remains feasible without
mutating its inputs. Case 23 uses an independent exact pattern oracle to prove
language disjointness while confirming that low-level sweep feasibility is not
an equatability oracle and that unsupported mixed-domain projections remain
transactional.

## Reproducible standalone sanitizer gate

Run the standalone memory-safety matrix with:

```sh
JOBS=1 ./dashed-project/scripts/run-standalone-sanitizers.sh
```

The gate configures independent Debug builds for AddressSanitizer and
UndefinedBehaviorSanitizer, compiles the standalone kernel and tests with
warnings treated as errors, and executes every CTest entry serially.
AddressSanitizer enables leak detection and halts at the first error; UBSan
prints a stack trace and does not recover from undefined behavior.

`dashed_sanitizer_lifecycle_tests` adds explicit stress for:

- immutable literal payloads surviving destruction of their source domain;
- independent narrowing of thousands of persistent domain copies;
- cleanup after failed equality, concatenation, length, and reified propagation;
- repeated normalization of many-segment domains;
- repeated literal slicing, concatenation, and propagation.

This gate validates the standalone backend. Native Gecode space allocation,
propagator disposal, brancher/NGL lifecycle, and DFS cloning remain a separate
sanitizer milestone.

## Native List sanitizer gate

Run the native Gecode lifecycle matrix with:

```sh
JOBS=1 ./dashed-project/scripts/run-native-list-sanitizers.sh
```

The runner configures separate Debug AddressSanitizer and
UndefinedBehaviorSanitizer builds for both the standalone backend and the
minimal Gecode libraries required by `ListVar`. It then compiles and runs the
native relation regression suite and a dedicated lifecycle stress executable
with the same sanitizer instrumentation.

`list-sanitizer-lifecycle` covers:

- immutable literal payloads shared across cloned spaces after the source
  space is destroyed;
- repeated cleanup of spaces failed by equality, concatenation, length, and
  reified equality;
- propagator subsumption followed by cloning and independent destruction;
- complete DFS with aggressive recomputation;
- exact brancher and no-good-literal copying, pruning, and space destruction.

AddressSanitizer runs with leak detection enabled. UBSan halts at the first
reported undefined operation. The native gate uses dedicated build directories
and does not replace the normal incremental Gecode build.

The sanitizer runner performs a source-level ownership audit before
building. Gecode search engines snapshot the supplied source space but do not
own that original pointer, so every List test deletes its DFS source space
immediately after constructing the engine. This keeps test-harness leaks from
masking implementation-level leak results.

### Native UBSan vptr boundary

The native sanitizer gate keeps all UndefinedBehaviorSanitizer checks enabled
for the dashed backend, the List implementation, and model/test translation
units, including `vptr`. Only the private Gecode shared-library build uses
`-fno-sanitize=vptr`; every other UBSan check remains active there. This narrow
compatibility boundary is required because Gecode's kernel uses standalone
`ActorLink` sentinel nodes whose addresses are intentionally converted to
`Brancher*` for list-end comparisons; UBSan's dynamic-type check rejects that
upstream representation before any List variable is created.

The private UBSan Gecode build also uses default RTTI visibility. The runner
checks the required internal typeinfo exports before linking and executes a
two-sided compiler-policy probe: full List flags must detect an invalid
downcast, while the Gecode compatibility flags must allow it. Normal and
release builds are unchanged.

### Native sanitizer iteration speed

The native sanitizer runner preserves compatible CMake caches, including a
partially completed build. Sanitized Gecode translation units can consume
several GiB each, so the runner selects conservative mode-specific build
parallelism: UBSan uses one compiler job on machines below 32 GiB, while ASan
uses at most one or two jobs on common developer machines. Set
`LIST_NATIVE_SANITIZER_BUILD_JOBS` only when the machine has been observed to
support higher concurrency. If a parallel compiler is killed for memory
pressure, the runner automatically resumes the same Ninja build with one job.

For a short development check, run:

```sh
LIST_NATIVE_SANITIZER_PROFILE=smoke \
  ./dashed-project/scripts/run-native-list-sanitizers.sh
```

The smoke profile runs the ownership audit and ASan lifecycle binary. The
acceptance profile remains the default and runs separate ASan and UBSan builds
with both lifecycle and full relation-regression binaries. Subsequent runs
reuse successful and partially completed compiler outputs instead of rebuilding
all Gecode libraries.

## Native List memory profile

Run the short cached profile with:

```sh
./dashed-project/scripts/run-list-memory-profile.sh
```

Use `LIST_MEMORY_PROFILE_PROFILE=acceptance` for the longer milestone workload.
The generated TSV records warmed start/end RSS, steady-state growth, peak RSS,
operation counts, and solution counts for repeated space destruction, cloning,
and complete DFS. Details and interpretation limits are documented in
`MEMORY_PROFILING.md`.

## Native List Valgrind gate

Run the short cached leak and memory-error confirmation with:

```sh
./dashed-project/scripts/run-list-valgrind.sh
```

The default profile checks repeated native space destruction, shared-literal
space cloning, and complete recomputation-heavy DFS under Memcheck. Definite
and indirect leaks are errors. `LIST_VALGRIND_PROFILE=acceptance` increases the
workload, while `LIST_VALGRIND_TOOLS=massif` records allocator-level attribution
without changing the leak gate. The runner fingerprints passing reports to
avoid repeating unchanged checks.

## Native List performance baseline

Run the short cached benchmark with:

```sh
./dashed-project/scripts/run-list-performance-benchmark.sh
```

Use `LIST_PERF_PROFILE=baseline` for a stable same-machine baseline and
`LIST_PERF_PROFILE=acceptance` for the longer milestone workload. Reports record
median and p95 nanoseconds per operation plus invariant operation, solution, and
checksum values for propagation, shared-literal cloning, and complete exact
DFS. `LIST_PERF_BASELINE` enables an optional median-regression comparison;
interpret timings only on equivalent hardware and build configurations.

## Native List Callgrind attribution

Run the cached smoke attribution with:

```sh
./dashed-project/scripts/run-list-callgrind.sh
```

The installer validates a single tiny propagation profile; the normal command
profiles propagation, cloning, and DFS. Raw Callgrind data and
`callgrind-hotspots.tsv` are stored below
`dashed-project/runs/list-performance/`. A valid report must contain both `all`
and `project` rankings for every requested scenario. Treat instruction counts
as attribution evidence and use the timing benchmark for before/after speed
claims.

Callgrind report validation also rejects unresolved compressed numeric symbol
IDs. Parser-only changes can be checked against existing raw data with
`LIST_CALLGRIND_REUSE_RAW=1`; this path must not execute the benchmark or
require Valgrind.
