# Native List performance benchmarking

Phase 14 starts by recording a reproducible baseline before changing the
implementation. The benchmark is not a cross-machine leaderboard and does not
commit machine-specific timing results.

## Short development profile

The default smoke profile reuses `build-list-native` and `build-native-core`:

```sh
./dashed-project/scripts/run-list-performance-benchmark.sh
```

It measures three isolated native workloads:

- construction, propagation, and destruction of a List disequality,
  concatenation, and length space;
- cloning a solved space carrying a shared 16,384-element literal;
- complete recomputation-heavy DFS over the 40-solution exact-branching model.

Each scenario runs in its own process invocation, performs two unmeasured
warm-ups, and reports median, 95th percentile, minimum, and maximum nanoseconds
per operation. Operation and solution counts plus a checksum guard against
accidentally benchmarking removed work.

## Stable baseline and acceptance profiles

Collect a more stable baseline with:

```sh
LIST_PERF_PROFILE=baseline \
LIST_PERF_CPU=0 \
LIST_PERF_REPORT_DIR=dashed-project/runs/list-performance/baseline \
  ./dashed-project/scripts/run-list-performance-benchmark.sh
```

The optional CPU pin requires `taskset`. Keep the compiler, build type, machine,
CPU governor, and background load stable when comparing reports. The longer
milestone profile is explicit:

```sh
LIST_PERF_PROFILE=acceptance \
LIST_PERF_CPU=0 \
  ./dashed-project/scripts/run-list-performance-benchmark.sh
```

Compare a new run with a saved TSV from the same environment:

```sh
LIST_PERF_PROFILE=baseline \
LIST_PERF_CPU=0 \
LIST_PERF_BASELINE=path/to/native-list-performance.tsv \
LIST_PERF_MAX_REGRESSION_PERCENT=15 \
  ./dashed-project/scripts/run-list-performance-benchmark.sh
```

The comparison gate uses scenario medians and defaults to a 15 percent
regression tolerance. Timing noise is expected, so optimization decisions must
be based on repeated same-machine measurements and profiler attribution rather
than one run.

## Optimization policy

This baseline deliberately changes no List algorithm. Subsequent optimization
work must identify a measured hotspot, preserve the semantic/differential and
memory-safety gates, and report before/after results for the affected scenario.
Reports are written below `dashed-project/runs/list-performance/` and are
ignored by Git.

## Callgrind hotspot attribution

Before changing an algorithm, collect instruction-level self-cost attribution
from the existing optimized benchmark binary:

```sh
./dashed-project/scripts/run-list-callgrind.sh
```

The default smoke profile uses tiny workloads and reuses `build-list-native`;
it does not rebuild Gecode. It writes raw Callgrind files plus a deterministic
TSV ranking for propagation, shared-literal cloning, and complete DFS. The TSV
contains both an all-code view and a project view covering List, dashed, and
Gecode sources.

For a more stable attribution run:

```sh
LIST_CALLGRIND_PROFILE=attribution \
LIST_CALLGRIND_SCENARIOS="propagate clone dfs" \
  ./dashed-project/scripts/run-list-callgrind.sh
```

Use `LIST_CALLGRIND_PROFILE=deep` only after a hotspot is already suspected.
Callgrind counts instructions rather than elapsed time, so confirm any proposed
optimization with the same-machine timing baseline. The first optimization
must target a function that is prominent in the affected scenario's project
ranking and must preserve operation, solution, and checksum invariants.
