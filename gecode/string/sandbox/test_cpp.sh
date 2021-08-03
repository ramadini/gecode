#!/bin/bash

if
  [[ "${@: -1}" == "--coverage" ]] 
then
  COV='--coverage'
else
  COV=
fi

echo 'Compiling'
time g++ $COV -O0 -ggdb -pipe -Wall -Wno-switch -I/usr/local/include/gecode -std=c++11 -c -o $1.o $1
if
  [ $? -ne 0 ]
then
  echo 'Failed!'
  exit 1
fi

echo 'Linking'
time g++ $COV -o $1.out $1.o -std=c++11 -L/usr/local/lib \
-lgecodestring -lgecodeint -lgecodeset -lgecodedriver -lgecodesearch -lgecodeminimodel -lgecodekernel -lgecodesupport -pthread
if
  [ $? -ne 0 ]
then
  echo 'Failed!'
  exit 1
fi

echo 'Executing'
if
  [[ "${@: -1}" == "--valgrind" ]] 
then
  valgrind --gen-suppressions=yes --leak-check=full ./$1.out $2 $3
else
  ./$1.out $2 $3
fi
if
  [ $? -ne 0 ]
then
  echo 'Failed!'
  exit 1
fi

#lcov --capture --directory project-dir --output-file coverage.info
#genhtml coverage.info --output-directory out

rm $1.out $1.o


