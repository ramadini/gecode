#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
PROTOTYPE_ROOT="$PROJECT_ROOT/prototype"

JOBS="${JOBS:-1}"
CXX="${CXX:-g++}"
CMAKE_GENERATOR="${CMAKE_GENERATOR:-Ninja}"
SANITIZER_MODES="${DASHED_SANITIZER_MODES:-address undefined}"
CTEST_REGEX="${DASHED_SANITIZER_CTEST_REGEX:-}"

if ! command -v "$CXX" >/dev/null 2>&1; then
  echo "C++ compiler not found: $CXX" >&2
  exit 1
fi

compiler_id="$($CXX --version | head -n 1 | tr ' /' '__' | tr -cd '[:alnum:]_.-')"

run_mode() {
  local mode="$1"
  local build_dir="$PROTOTYPE_ROOT/build-sanitizer-${compiler_id}-${mode}"

  case "$mode" in
    address|undefined|address,undefined)
      ;;
    *)
      echo "Unsupported sanitizer mode: $mode" >&2
      exit 1
      ;;
  esac

  echo
  echo "=================================================="
  echo "STANDALONE SANITIZER: $mode"
  echo "=================================================="
  echo "Compiler:  $CXX"
  echo "Build dir: $build_dir"

  if [[ -f "$build_dir/CMakeCache.txt" ]]; then
    local cached_source
    local cached_compiler
    local expected_compiler

    cached_source="$(
      sed -n 's/^CMAKE_HOME_DIRECTORY:INTERNAL=//p' \
        "$build_dir/CMakeCache.txt"
    )"
    cached_compiler="$(
      sed -n 's/^CMAKE_CXX_COMPILER:FILEPATH=//p' \
        "$build_dir/CMakeCache.txt"
    )"
    expected_compiler="$(readlink -f "$(command -v "$CXX")")"

    if [[ "$cached_source" != "$PROTOTYPE_ROOT" || \
          "$(readlink -f "$cached_compiler" 2>/dev/null || true)" != \
            "$expected_compiler" ]]; then
      echo "Removing stale sanitizer build cache."
      rm -rf "$build_dir"
    fi
  fi

  cmake \
    -S "$PROTOTYPE_ROOT" \
    -B "$build_dir" \
    -G "$CMAKE_GENERATOR" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_CXX_FLAGS="-Werror" \
    -DDASHED_BUILD_TESTS=ON \
    -DDASHED_BUILD_BENCHMARKS=OFF \
    -DDASHED_ENABLE_SANITIZERS=ON \
    -DDASHED_SANITIZER_SET="$mode"

  cmake \
    --build "$build_dir" \
    --parallel "$JOBS"

  local ctest_args=(
    --test-dir "$build_dir"
    --output-on-failure
    -j 1
  )

  if [[ -n "$CTEST_REGEX" ]]; then
    ctest_args+=(--tests-regex "$CTEST_REGEX")
  fi

  if [[ "$mode" == *address* ]]; then
    ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1:halt_on_error=1:strict_string_checks=1}" \
    UBSAN_OPTIONS="${UBSAN_OPTIONS:-print_stacktrace=1:halt_on_error=1}" \
      ctest "${ctest_args[@]}"
  else
    UBSAN_OPTIONS="${UBSAN_OPTIONS:-print_stacktrace=1:halt_on_error=1}" \
      ctest "${ctest_args[@]}"
  fi
}

for mode in $SANITIZER_MODES; do
  run_mode "$mode"
done

printf '\nStandalone sanitizer matrix passed: %s\n' "$SANITIZER_MODES"
