# MiniZinc String Recovery

This directory recovers and documents the historical MiniZinc string-variable
extension used by `tools/flatzinc/mzn2fzn-str`.

The archived `mzn2fzn-str` compiler is the behavioral oracle. The initial
recovery milestone is restricted to pinning and building official libminizinc
2.0.97, preserving symbol evidence, and documenting the semantic delta. It
does not port variable strings to a current MiniZinc release.

Run the scripts in order:

```sh
tools/minizinc-string/scripts/fetch-upstream
tools/minizinc-string/scripts/build-upstream
tools/minizinc-string/scripts/extract-symbols
tools/minizinc-string/scripts/compare-symbols
```

Downloaded sources and build products are kept under `.work/` and are ignored.
See `archaeology/findings.md` for verified evidence and open questions.
