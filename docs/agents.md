# Working on the Gecode sources

Use MPG for modelling concepts and Doxygen comments for API contracts and
implementation invariants. Keep behaviour documentation with the declarations
or algorithms it describes. `docs/` holds repository maintenance guidance and
the [CMake build reference](cmake-build.md). Example data formats and attribution
stay beside their data.

## Finding the relevant code

Public interfaces are in `gecode/<module>.hh`. Implementations live under the
matching module directory: `.hpp` files generally contain templates or inline
code, and `.cpp` files contain compiled definitions. For integer constraints,
start with the posting function in `gecode/int.hh`, then follow its
implementation into the constraint's propagators.

Use neighbouring code for formatting, naming, memory management, and cloning
conventions. Keep changes focused on the requested behaviour; add abstractions
and tests when they address a concrete maintenance or correctness problem.

Constraint tests live in `test/int/`, `test/set/`, and `test/float/`.
`test/search.cpp` covers search engines and worker control;
`test/flatzinc/` covers FlatZinc. Prefer the existing constraint harness for
solution and propagation checks. The harness contracts are in `test/int.hh`,
`test/set.hh`, `test/float.hh`, and `test/test.hh`.

## Building and checking a change

Keep generated build outputs under `build/`. A typical local build is:

```sh
cmake -S . -B build/local -DCMAKE_BUILD_TYPE=Release
cmake --build build/local -j 4
cmake --build build/local --target check
```

Use `GECODE_ENABLE_EXPERIMENTAL=ON` when changing AStar or CABS, and check the
default configuration too if the change crosses their feature guard. Use the
[CMake reference](cmake-build.md) for dependency options and sanitizer builds.

Run the tests relevant to the change. `check` selects the integrity suite;
`check-standard` provides broader coverage; `check-sweep` is for deliberate
long runs. The built `bin/gecode-test` supports `-test` name patterns, `-tag`
selection, and `-list-with-tags`; consult `-help` for replay options. For a
behaviour fix, prefer a case that exposes the bug over a test of code structure.
Documentation-only changes need documentation checks rather than new tests.

When adding or removing sources, update both `Makefile.in` and
`cmake/GecodeSources.cmake`; examples also have `examples/CMakeLists.txt`.
When include dependencies change, use the configured Autoconf `make depend`
recipe to regenerate dependencies with `misc/makedepend.py`. Keep checked-in
`Makefile.dep` paths relative to the source root, and review generated changes
for unrelated churn.

## Generated files and release notes

`configure.ac` is the source for `configure`. The shared version and library
ABI metadata live in `gecode-version.m4`. Use the configured `make regenerate`
target for changes to generator inputs; the helper in
`.github/ci/generated-source-freshness.sh` checks the required tool versions.
Ordinary C++ edits do not require regenerating the parser or variable headers.

Describe user-visible changes in the unreleased section of `changelog.in`.
Mark preview entries with `Experimental: true`. The planned changelog release
and the build version are separate. Update version and ABI metadata when
preparing a release.

Run `uv run misc/tidy.py --check` for changelog validation. When changing
Doxygen comments or documentation inputs, build the `doc` target. It requires
Doxygen 1.17.0 or newer and `uv`. New C++ files included in documentation metrics
need a `// STATISTICS: module-category` line matching a neighbouring file.
Helper pages and HTML are generated in the build directory.
