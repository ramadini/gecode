#!/usr/bin/env bash

BUNDLE_NAME="replace-with-bundle-name"
EXPECTED_BRANCH="dashed-listvar"
COMMIT_MESSAGE="Replace with commit message"

# Exact paths are matched exactly.
# Rules ending in / match any path below that directory.
ALLOWED_PATHS=(
  "dashed-project/prototype/src/propagation.cpp"
  "dashed-project/prototype/tests/test_core.cpp"
  "gecode/list/tests/rel-test.cpp"
)

STAGE_PATHS=(
  "dashed-project/prototype/src/propagation.cpp"
  "dashed-project/prototype/tests/test_core.cpp"
  "gecode/list/tests/rel-test.cpp"
)

step_1() {
  echo "Apply checkpoint 1 here"
}

test_1() {
  cmake \
    --build dashed-project/prototype/build-normalization \
    --target dashed_core_tests \
    --parallel "$JOBS"

  ./dashed-project/prototype/build-normalization/dashed_core_tests
}

step_2() {
  echo "Apply checkpoint 2 here"
}

test_2() {
  cmake \
    --build dashed-project/prototype/build-normalization \
    --target dashed_core_tests \
    --parallel "$JOBS"

  ./dashed-project/prototype/build-normalization/dashed_core_tests
}

step_3() {
  echo "Apply checkpoint 3 here"
}

test_3() {
  JOBS="$JOBS" ./gecode/list/tests/run-tests
}

final_tests() {
  cmake \
    --build dashed-project/prototype/build-normalization \
    --parallel "$JOBS"

  ctest \
    --test-dir dashed-project/prototype/build-normalization \
    --output-on-failure \
    -j "$JOBS"

  JOBS="$JOBS" ./gecode/list/tests/run-tests
}
