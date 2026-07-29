#!/usr/bin/env bash

export DASHED_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
export GECODE_SRC="$DASHED_ROOT/gecode"
export DASHED_CORE="$DASHED_ROOT/prototype"
export DASHED_TESTS_OLD="$DASHED_ROOT/tests-old"
export DASHED_RUNS="$DASHED_ROOT/runs"
export DASHED_SCRIPTS="$DASHED_ROOT/scripts"
