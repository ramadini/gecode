# Native List memory profiling

The native memory profiler complements ASan/LSan. Sanitizers prove that owned
allocations are released; this profiler records process-level resident memory
and checks that repeated native workloads reach a steady state.

## Short development profile

The default smoke profile reuses `build-list-native` and `build-native-core`:

```sh
./dashed-project/scripts/run-list-memory-profile.sh
```

It runs three isolated processes:

- construction, propagation, and destruction of native List spaces;
- cloning and destruction of spaces carrying a shared 16,384-element literal;
- recomputation-heavy DFS with exact List branching and complete solution
  destruction.

Two unmeasured batches absorb one-time Gecode and allocator initialization.
After every measured batch the Linux/glibc heap is trimmed when available, and
`VmRSS` is sampled from `/proc/self/status`. The report also records
`ru_maxrss`, operations, and DFS solutions.

## Acceptance profile

For a longer milestone measurement, run:

```sh
LIST_MEMORY_PROFILE_PROFILE=acceptance \
  ./dashed-project/scripts/run-list-memory-profile.sh
```

The runner does not rebuild sanitizer trees. It compiles one small optimized
profiler against the cached normal native libraries. If the normal cache is
missing, it can bootstrap it once through `gecode/list/tests/run-tests`; set
`LIST_MEMORY_PROFILE_ALLOW_BOOTSTRAP=0` to fail immediately instead.

Reports are written under `dashed-project/runs/list-memory/<timestamp>/` and
contain one TSV row per scenario. The steady-growth gate compares the first and
last thirds of measured RSS samples. A scenario fails only when growth exceeds
the larger of 32 MiB and 25 percent of its warmed baseline; acceptance uses 20
percent. These deliberately tolerant process-level limits detect sustained
growth without treating allocator noise as a List leak. Override them with
`LIST_MEMORY_PROFILE_MAX_GROWTH_KB` and
`LIST_MEMORY_PROFILE_MAX_GROWTH_PERCENT` when collecting diagnostics.

Peak RSS is a baseline, not a cross-machine invariant. Compare reports only on
the same compiler, build type, architecture, and representative workload.
