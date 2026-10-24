# Dashed - String Variables for Gecode

![Gecode](images/gecode-logo-100.png "Gecode")

_Dashed_ is a bounded-length string variable solver integrated into Gecode 6.4 and
based on the _dashed string_ representation. We refer to [1] for more details about dashed strings.
It was previously known as _G-Strings_. This repository contains the maintained Dashed
implementation together with the Gecode source tree on which it is built, and the old 
G-Strings implementation.

MiniZinc string models require Dashed’s bundled mzn2fzn-str compiler, based on MiniZinc 2.0.97 
with string-variable support [2]. The official MiniZinc release does not currently support this extension. 
The bundled compiler currently supports Linux x86-64.

Gecode is an open source C++ toolkit for developing constraint-based systems
and applications. Its existing package, library, namespace, and header names
are preserved for source and binary compatibility.

[![CI](https://github.com/ramadini/gecode/actions/workflows/build.yml/badge.svg)](https://github.com/ramadini/gecode/actions/workflows/build.yml)

## Getting All the Info You Need...

The [Gecode webpages](https://gecode.github.io) document the underlying
constraint-programming toolkit.

In particular,
Gecode comes with
[extensive tutorial and reference documentation](https://gecode.github.io/documentation.html).
The build and package instructions below describe this Dashed-enabled tree.
The [Dashed engineering and performance report](docs/dashed-final-report.md)
summarizes the final refactoring, validation, and benchmark results w.r.t. G-Strings.

The tutorial, *Modeling and Programming with Gecode* (MPG), explains how to
model and solve problems. API and implementation contracts belong in the
Doxygen comments alongside the source.

## Building Gecode

See the [CMake build and package reference](docs/cmake-build.md) for build,
installation, and downstream package options. The repository also supports
Autoconf builds.

## Get Dashed

Clone this repository and use the `master` branch:

```bash
git clone https://github.com/ramadini/gecode.git
cd gecode
```

Official Gecode source releases do not include Dashed.

For source changes, see the [repository guide for agents](docs/agents.md).

## Contributing to Dashed

Submit fixes and improvements as pull requests against this repository's
`master` branch.

## License

Dashed and the Gecode source tree are distributed under the
[MIT license](LICENSE).

## References

1. R. Amadini, G. Gange, P.J. Stuckey. _Dashed strings for string constraint solving_.
   Artif. Intell. 289: 103368 (2020)
   
2. R. Amadini, P. Flener, J. Pearson, J.D. Scott, P.J. Stuckey, G. Tack.
   MiniZinc with Strings. In LOPSTR 2016, Edinburgh, Scotland, UK.
   
   
