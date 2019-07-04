# G-Strings - Gecode with String Variables

![Gecode](images/gecode-logo-100.png "Gecode")

Gecode is an open source C++ toolkit for developing
constraint-based systems and applications. Gecode provides a
constraint solver with state-of-the-art performance while being
modular and extensible.

[master](https://github.com/Gecode/gecode/tree/master): 
[![Build Status master](https://api.travis-ci.org/Gecode/gecode.svg?branch=master)](https://travis-ci.org/Gecode/gecode)

[develop](https://github.com/Gecode/gecode/tree/develop): 
[![Build Status develop](https://api.travis-ci.org/Gecode/gecode.svg?branch=develop)](https://travis-ci.org/Gecode/gecode)


This repository contains __G-Strings__ solver, an extension of Gecode enabling 
string variables and constraints based on the _dashed string_ representation.
We refer to [1,2,3,4,5,6] for more details.

## Installation

To install G-Strings:

1. move into ```gecode``` folder

2. ```./configure``` with ```--enable-stringvars``` option enabled.

3. ```make```

4. ```sudo make install```

Note that C++ 11 is needed. If you want to perform some sanity checks on string 
variables run the script ```run-tests``` in ```gecode/string/tests``` folder.

## MiniZinc support

G-Strings can solve problems encoded in [MiniZinc](http://www.minizinc.org/) 
language. The support for string variables in MiniZinc is still in progress and 
not part of the official release (for more information about MiniZinc with 
strings, please see [1]).

You can solve a MiniZinc model with strings ```M.mzn``` (together with optional 
data ```D.dzn```) in one step:

    $ mzn-gstrings M.mzn [D.dzn]       (1)
   
or in two steps:
   
    $ mzn2fzn-gstrings M.mzn [D.dzn]   (2)
    $ fzn-gstrings M.fzn              

In particular, (2) converts ```M.mzn``` into a FlatZinc instance ```M.fzn``` 
(this is the _F^{str}_ conversion described in [1]). If you wish to decompose 
```M.mzn``` into a FlatZinc instance _not_ containing string variables, use:

    $ mzn2fzn-str --stdlib-dir gecode/flatzinc M.mzn [D.dzn]
  
and then solve the corresponding FlatZinc instance, where string variables and 
constraints are translated into corresponding integer variables and constraints.
Note that this translation (i.e., the _F^{int}_ conversion of [1]) can take a 
long time and, although supporting all the constraints of [1], does not handle 
all the string constraints supported by G-Strings. This translation is defined 
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

1. R. Amadini, P. Flener, J. Pearson, J.D. Scott, P.J. Stuckey, G. Tack.
   MiniZinc with Strings. In LOPSTR 2016, Edinburgh, Scotland, UK.

2. R. Amadini, G. Gange, P.J. Stuckey, G. Tack. A Novel Approach to String 
   Constraint Solving. In CP 2017, Melbourne, Victoria, Australia.

3. R. Amadini, G. Gange, P.J. Stuckey. Sweep-based Propagation for String 
   Constraint Solving. In AAAI 2018, New Orleans, Lousiana, USA.

4. R. Amadini, G. Gange, P.J. Stuckey. Propagating lex, find and replace with
   dashed strings. In CPAIOR 2018, Delft, The Netherlands.

5. R. Amadini, G. Gange, P.J. Stuckey. Propagating regular membership with 
   dashed strings. In CP 2018, Lille, France.
   
6. R. Amadini, M. Andrlon, G. Gange, P. Schachte, H. Søndergaard, P. J. Stuckey. 
   Constraint Programming for Dynamic Symbolic Execution of JavaScript. In 
   CPAIOR 2019, Thessaloniki, Greece.   
   
   
