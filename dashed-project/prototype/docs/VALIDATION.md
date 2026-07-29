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
is not already available. The current gate covers six `str_test2.cpp`
root-domain fixtures; it is not yet a substitute for live side-by-side search.
See `DIFFERENTIAL_TESTING.md` for the report protocol and remaining work.
