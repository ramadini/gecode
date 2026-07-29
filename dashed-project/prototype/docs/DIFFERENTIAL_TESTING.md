# Differential testing against G-Strings

The first differential corpus is derived from the preserved G-Strings
`gecode/string/tests/str_test2.cpp` equality sweeps. Characters are mapped
injectively to integers (`a = 1`, `b = 2`, and so on), while repetition bounds
and root-domain expectations are retained.

## Current baseline

The fixture corpus covers `str_test2::test01`, `test02`, `test09`, `test11`,
`test14`, and `test16`. These are the legacy equality cases already supported
by the segmented sweep kernel. The test executable emits a deterministic TSV
report containing satisfiability and both root domains. The committed expected
report is compared at the semantic row level.

`verify_gstrings_fixtures.py` searches explicit roots, `DASHED_TESTS_OLD`,
standard preserved-tree locations, and local Git refs/history. The explicit
runner may fetch `origin/master` or `origin/main` when necessary. It extracts
each function body and checks source anchors, preventing translated fixtures
from silently drifting away from the legacy tests they represent.

Run:

```sh
./dashed-project/scripts/run-gstrings-differential.sh
```

Pass `--root PATH` when the legacy test tree is elsewhere.

## Live runner seam

This checkpoint is fixture-backed: it verifies translated root-domain results
against preserved legacy source, but it does not yet compile the historical
G-Strings solver. A future adapter can implement the same `--report PATH` TSV
protocol and be supplied as:

```sh
GSTRINGS_DIFFERENTIAL_RUNNER=/path/to/gstrings-runner \
  ./dashed-project/scripts/run-gstrings-differential.sh
```

The script then compares live G-Strings output with the List kernel instead of
only the committed fixture report.

## Remaining differential coverage

- live compilation and execution of the historical G-Strings checkout;
- satisfiability and complete solution sets for small searched models;
- concatenation, disequality, reification, and length constraints;
- objective values for optimization models;
- root propagation, search-node, and failure-count comparisons;
- an explicit explanation for every stronger or weaker result.
