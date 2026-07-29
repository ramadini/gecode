# Native Gecode adapter blueprint

This directory is a source-integration blueprint for Gecode 6.4. The standalone
kernel is tested; these adapter files are **not included in the default CMake
build** and must be compiled inside a Gecode source tree after variable-base
generation.

## Why source integration is required

A new native variable implementation needs generated modification-event,
propagation-condition, and base-class definitions. Gecode 6.4 accepts additional
`.vis` files through `GECODE_WITH_VIS`.

## Proposed source placement

```text
gecode/dashed.hh
gecode/dashed/var-imp/list.vis
gecode/dashed/var-imp.hpp
gecode/dashed/var.hpp
gecode/dashed/view.hpp
gecode/dashed/propagator.hpp
```

Only `list.vis` and design headers are included here so far. The final adapter
should be added to Gecode's source inventory and public package exports.

## Configure generation

```bash
cmake -S /path/to/gecode -B /path/to/build \
  -DGECODE_WITH_VIS=gecode/dashed/var-imp/list.vis \
  -DGECODE_REGENERATE_VARIMP=ON \
  -DGECODE_SANITIZER=address-undefined
```

Use the generated `ListVarImpBase` signatures as the authority when completing
`list-var-imp.hpp`. Do not bypass generation by copying event constants from an
older fork.

## Adapter contract

- `ListVarImp` owns one `dashed::Domain`.
- All updates are monotone and notify once.
- `ListView` is thin.
- Propagators own cross-variable logic.
- Long literal buffers are immutable and shared across cloned spaces.
- `Dispose: true` ensures C++ payload destructors run.
- The old G-Strings source remains a semantic reference, not an architectural
  template.
