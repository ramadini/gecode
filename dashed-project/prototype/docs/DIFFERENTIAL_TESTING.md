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

## Exact searched solution baseline

The second differential layer covers small finite models for equality,
disequality, concatenation, length, and equivalence-reified equality.  A
standalone Python generator enumerates the mathematical reference languages
without calling either solver.  The native `ListVar` runner solves the same
models through Gecode DFS and emits one canonical TSV row per solution.

The gate compares solution sets rather than output order and rejects duplicate
solutions.  It also records native search nodes, failures, and maximum depth.
At this stage those statistics are validated for shape and solution-count
consistency; they are not yet compared with historical G-Strings statistics.

The exact corpus currently contains 24 solutions across five cases over the
injective character map `{a -> 1, b -> 2}`:

- equality over binary lists of length zero through two;
- disequality over binary singleton lists;
- concatenation of two binary lists of length zero or one;
- length equal to one for a binary list;
- equivalence-reified equality over binary singleton lists.

The native gate is part of `gecode/list/tests/run-tests`.  The standalone
reference can be checked independently with:

```sh
python3 \
  dashed-project/prototype/tools/generate_gstrings_solution_reference.py \
  --check \
  dashed-project/prototype/tests/gstrings/expected-search-solutions.tsv
```

This layer detects missing, extra, or duplicate native solutions.  A later
live historical runner must still execute the same models under G-Strings so
that node and failure counts, and any propagation-strength differences, can be
classified directly.

## Extended legacy equality classification

The translated root-propagation corpus now also covers `str_test2::test03`
through `test08`. These cases are not treated as blind golden outputs: small
finite cases are checked against the concrete language intersection so that a
legacy expectation can be classified rather than automatically copied.

The current classifications are:

- `test03`, `test07`, and `test08`: matching root propagation;
- `test05`: matching unsatisfiability;
- `test04`: the List result is the exact language intersection, while the
  historical expected block update both removes valid lists and admits lists
  outside the original intersection;
- `test06`: both List operands remain sound, the right operand reaches the
  exact historical common domain, and the left operand is a documented
  over-approximation.

`test06` also guards a critical failure mode. Region reconstruction may find
crossing start/end windows even though the value-only sweep is feasible. The
region layer is therefore no longer used as the final infeasibility oracle;
the value projection must independently confirm failure.

## Large and unbounded equality cases

The classified root-propagation corpus now also covers `str_test2::test10`,
`test12`, `test13`, and `test15`.

- `test10` matches the historical normalized block updates. A scaled concrete
  oracle confirms that both resulting operands are sound but intentionally
  weaker than the exact intersection.
- `test12` reaches the exact common language `b* c*` with total length at most
  2000.
- `test13` reaches `(a|b)^0..M-1 b`, including the one-element list `b`.
  This case exposed a saturated-length subtraction bug in domain
  normalization: removing an unbounded segment from a capped total incorrectly
  erased the mandatory contribution of the remaining segment. Prefix/suffix
  sums now compute each omitted-segment contribution without information loss.
- `test15` canonicalizes the three adjacent unary blocks to `a^1..6`.

The original large bounds are retained in the deterministic report. Scaled
finite analogues provide exhaustive concrete-language checks where direct
enumeration of the historical bounds would be impractical.


## Fixed-literal boundaries and assignment

The root-propagation corpus now covers `str_test2::test17` through `test20`
and assignment case `test22`.

- `test17` and `test19` exercise a fixed prefix ending at the maximum legacy
  alphabet symbol; the remaining suffix must stay below that symbol.
- `test18` exercises the symmetric fixed suffix boundary.
- `test20` removes an optional middle block and exposes literal slices backed
  by unrelated immutable vectors. Normalization now copies the complete run
  once into one canonical literal, so structural equality agrees with the
  historical `x == y` assertion.
- `test22` converges from two unassigned segmented domains to the single list
  `aab`.

The deterministic report uses a compact assigned-value hash rather than
printing the very large literals. During the explicit differential run,
`extract_gstrings_fixed_literals.py` extracts the exact C++ string literals
from the preserved legacy source and the List kernel replays all four fixed
cases. The committed `test19` case is also an 8193-element source-equivalent
stress fixture so ordinary CTest runs do not depend on the historical tree.


## Lower-level sweep stress cases

The preserved corpus now also classifies `str_test2::test21` and `test23`.
These are lower-level cursor tests, not ordinary equality golden cases.

- `test21` creates 50 lower-bounded singleton blocks under a global maximum
  length of 50. Global count tightening therefore fixes every block to one
  occurrence. Normalization now repeats its canonicalization pass after count
  bounds change, yielding one exact literal while preserving the 50 logical
  sweep positions. `analyze_repeat_sweep` remains non-mutating and feasible.
- `test23` intentionally documents that the historical `check_sweep` helper
  can succeed for two languages that are not equatable. An independent exact
  fixed-position oracle proves the two concrete pattern languages disjoint,
  while the structural sweep analysis remains feasible. The projection layers
  return `unsupported`, not `infeasible`, because mixed literal/repeat domains
  are outside those directional projection contracts.

This distinction is important: sweep-position feasibility is an internal
cursor property and must never be used by itself as a proof that two complete
list languages intersect.
