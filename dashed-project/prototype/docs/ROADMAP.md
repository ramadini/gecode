# Roadmap

## Milestone 1 — domain kernel (included)

- generic integer symbols;
- persistent fixed-run representation;
- canonical normalization;
- first six conservative propagators;
- tests and sanitizer configuration;
- Gecode integration specification.

## Milestone 2 — compile native `ListVar`

- native module naming is list-based (`gecode/list.hh`, `gecode/list/`, `Gecode::List`); the dashed engine is isolated behind a backend seam (included);
- integrate `.vis` generation in a Gecode 6.4 source tree;
- finish `ListVarImp`, `ListVar`, and `ListView` against generated signatures;
- Gecode-style `ListVarArgs`/`ListVarArray` traits, construction, and cloning (included);
- add native equality and length propagators;
- test space cloning, disposal, subscriptions, failure, and subsumption under
  ASan/UBSan.

## Milestone 3 — restore propagation strength

- port the original dashed equality sweeps as pure segmented-cursor algorithms;
- port concat sweeps without materializing long literals;
- exact assigned-side disequality pruning for one symbolic position and one
  endpoint singleton repetition count (included); extend further only when the
  result remains representable without an internal domain union;
- compare every ported operation with exhaustive small-language semantics;
- fixture-backed differential reports for six preserved `str_test2` equality
  sweeps are included; exact native DFS solution sets now cover equality,
  disequality, concatenation, length, and reified equality, with native search
  statistics recorded and sanity-checked; live historical-runner and direct
  G-Strings search-stat comparisons remain pending.

## Milestone 4 — modelling and search

- branch on length through Gecode integer branchers (included);
- exact backend partitions for unambiguous block counts and one distinguishing symbol position (included);
- native `branch_exact` brancher with cloned views, archived choices, deterministic printing, and explicit rejection of unsupported domains (included);
- semantic branch choices based on total length or an absolute list position, with exact descendant-literal classification and negation pruning (included);
- wrap semantic choices in native Gecode no-good literals (included);
- deterministic variable printing (included) and archive support;
- MiniZinc/FlatZinc-facing predicates if desired.

## Milestone 5 — broader constraint set

Port additional G-Strings constraints in families, only after the common domain
cursor API is stable.

## Performance work

Measure before adding representation complexity. Candidate optimizations:

- inline storage for one or two segments;
- small inline exact literals;
- immutable literal rope for repeated cross-buffer concatenation;
- hash/fingerprint for quick assigned equality and prefix checks;
- cached segment prefix-length ranges;
- specialized cursors that compare repeat blocks with literal spans without
  materialization;
- Gecode `Region` scratch allocation inside propagation.

### Differential equality expansion

- [x] Add `str_test2` equality cases 03-08 to the preserved-source corpus.
- [x] Prevent region reconstruction from producing an unsound equality
      failure when the value-only sweep remains feasible.
- [x] Classify exact matches, matching failure, a corrected unsound legacy
      expectation, and a sound-but-weaker List projection.
- [x] Classify the large/unbounded equality cases 10, 12, 13, and 15.
- [ ] Continue with fixed-literal boundary cases 17-20 and assignment case 22.
- [ ] Classify the lower-level sweep stress cases 21 and 23 separately.
