# Dashed - Gecode with dashed string variables

![Gecode](images/gecode-logo-100.png "Gecode")

Gecode is an open source C++ toolkit for developing
constraint-based systems and applications. Gecode provides a
constraint solver with state-of-the-art performance while being
modular and extensible.

[master](https://github.com/Gecode/gecode/tree/master): 
[![Build Status master](https://api.travis-ci.org/Gecode/gecode.svg?branch=master)](https://travis-ci.org/Gecode/gecode)

[develop](https://github.com/Gecode/gecode/tree/develop): 
[![Build Status develop](https://api.travis-ci.org/Gecode/gecode.svg?branch=develop)](https://travis-ci.org/Gecode/gecode)


This repository contains the __Dashed__ solver, an extension of Gecode enabling 
string variables and constraints based on the _dashed string_ representation.
We refer to [1] for more details.
Dashed Solver is a re-ingeneering of former _G-String_ solver.

## Installation

To install Dashed:

1. move into ```gecode``` folder

2. ```./configure``` with ```--enable-stringvars``` option enabled.

3. ```make```

4. ```sudo make install```

Note that C++ 11 is needed. If you want to perform some sanity checks on string 
variables run the script ```run-tests``` in ```gecode/string/tests``` folder.

## MiniZinc support

Dashed can solve problems encoded in [MiniZinc](http://www.minizinc.org/) 
language. The support for string variables in MiniZinc is still in progress and 
not part of the official release (for more information about MiniZinc with 
strings, please see [2]).

You can solve a MiniZinc model with strings ```M.mzn``` (together with optional 
data ```D.dzn```) in one step:

    $ mzn-dashed M.mzn [D.dzn]       (1)
   
or in two steps:
   
    $ mzn2fzn-dashed M.mzn [D.dzn]   (2)
    $ fzn-dashed M.fzn

In particular, (2) converts ```M.mzn``` into a FlatZinc instance ```M.fzn``` 
(this is the _F^{str}_ conversion described in [2]). If you wish to decompose 
```M.mzn``` into a FlatZinc instance _not_ containing string variables, use:

    $ mzn2fzn-str --stdlib-dir gecode/flatzinc M.mzn [D.dzn]
  
and then solve the corresponding FlatZinc instance, where string variables and 
constraints are translated into corresponding integer variables and constraints.
Note that this translation (i.e., the _F^{int}_ conversion of [2]) can take a 
long time and, although supporting all the constraints of [2], does not handle 
all the string constraints supported by Dashed. This translation is defined 
in ```gecode/flatzinc/std/nostrings.mzn``` library.

## Getting All the Info You Need...

You can find lots of information on
[Gecode's webpages](https://gecode.github.io),
including how to download, compile, install, and use it. 

In particular,
Gecode comes with
[extensive tutorial and reference documentation](https://gecode.github.io/documentation.html).

## Download Gecode

Gecode packages (source, Apple MacOS, Microsoft Windows) can be downloaded from
[GitHub](https://github.com/Gecode/gecode/releases)
or
[Gecode's webpages](https://gecode.github.io/download.html).

## Contributing to Gecode

We happily accept smaller contributions and fixes, please provide them as pull requests against the develop branch. For larger contributions, please get in touch.

## Gecode License

Gecode is licensed under the
[MIT license](https://github.com/Gecode/gecode/blob/master/LICENSE).

## References

1. R. Amadini, G. Gange, P.J. Stuckey, G. Tack. Dashed strings for string 
   constraint Solving. In Artificial Intelligence, Volume 289:103368 (2020)

2. R. Amadini, P. Flener, J. Pearson, J.D. Scott, P.J. Stuckey, G. Tack.
   MiniZinc with Strings. In LOPSTR 2016, Edinburgh, Scotland, UK.
   
   
