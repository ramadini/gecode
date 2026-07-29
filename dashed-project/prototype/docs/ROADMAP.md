# Roadmap

## Milestone 1 — domain kernel (included)

- generic integer symbols;
- persistent fixed-run representation;
- canonical normalization;
- first six conservative propagators;
- tests and sanitizer configuration;
- Gecode integration specification.

## Milestone 2 — compile native `ListVar`

- integrate `.vis` generation in a Gecode 6.4 source tree;
- finish `ListVarImp`, `ListVar`, and `ListView` against generated signatures;
- add `ListVarArgs`/`ListVarArray` traits;
- add native equality and length propagators;
- test space cloning, disposal, subscriptions, failure, and subsumption under
  ASan/UBSan.

## Milestone 3 — restore propagation strength

- port the original dashed equality sweeps as pure segmented-cursor algorithms;
- port concat sweeps without materializing long literals;
- extend disequality pruning beyond exact single-position witnesses;
- compare every ported operation with exhaustive small-language semantics and
  differential tests against G-Strings.

## Milestone 4 — modelling and search

- branch on length, block cardinality, or a distinguishing symbol position;
- no-good literals where useful;
- printing and archive support;
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
