#!/usr/bin/env bash

set -euo pipefail

REPO_ROOT="$(git rev-parse --show-toplevel)"
PROTOTYPE="$REPO_ROOT/dashed-project/prototype"
BUILD="$PROTOTYPE/build-normalization"
JOBS="${JOBS:-$(nproc)}"

cmake \
  -S "$PROTOTYPE" \
  -B "$BUILD" \
  -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DDASHED_BUILD_TESTS=ON \
  -DDASHED_BUILD_BENCHMARKS=OFF

cmake \
  --build "$BUILD" \
  --target dashed_gstrings_differential_tests dashed_gstrings_extended_tests dashed_gstrings_large_tests dashed_gstrings_fixed_tests \
  --parallel "$JOBS"

python3 \
  "$PROTOTYPE/tools/verify_gstrings_fixtures.py" \
  --manifest "$PROTOTYPE/tests/gstrings/equality-fixtures.json" \
  --fetch-origin \
  "$@"

python3 \
  "$PROTOTYPE/tools/compare_differential_reports.py" \
  --expected "$PROTOTYPE/tests/gstrings/expected-equality.tsv" \
  --runner "$BUILD/dashed_gstrings_differential_tests"

python3 \
  "$PROTOTYPE/tools/compare_extended_differential_reports.py" \
  --expected "$PROTOTYPE/tests/gstrings/expected-equality-extended.tsv" \
  --runner "$BUILD/dashed_gstrings_extended_tests"

python3 \
  "$PROTOTYPE/tools/compare_extended_differential_reports.py" \
  --expected "$PROTOTYPE/tests/gstrings/expected-equality-large.tsv" \
  --runner "$BUILD/dashed_gstrings_large_tests"

python3 \
  "$PROTOTYPE/tools/compare_extended_differential_reports.py" \
  --expected "$PROTOTYPE/tests/gstrings/expected-equality-fixed.tsv" \
  --runner "$BUILD/dashed_gstrings_fixed_tests"

fixed_fixture_directory="$(mktemp -d)"
fixed_fixture_file="$fixed_fixture_directory/fixed-literals.tsv"
cleanup_fixed_fixtures() {
  rm -rf "$fixed_fixture_directory"
}
trap cleanup_fixed_fixtures EXIT

python3 \
  "$PROTOTYPE/tools/extract_gstrings_fixed_literals.py" \
  --output "$fixed_fixture_file" \
  --fetch-origin \
  "$@"

"$BUILD/dashed_gstrings_fixed_tests" \
  --source-fixtures "$fixed_fixture_file"

python3 \
  "$PROTOTYPE/tools/generate_gstrings_solution_reference.py" \
  --check "$PROTOTYPE/tests/gstrings/expected-search-solutions.tsv"

if [[ -n "${GSTRINGS_DIFFERENTIAL_RUNNER:-}" ]]; then
  legacy_report="$(mktemp)"
  list_report="$(mktemp)"
  trap 'rm -rf "$fixed_fixture_directory"; rm -f "$legacy_report" "$list_report"' EXIT

  "$GSTRINGS_DIFFERENTIAL_RUNNER" --report "$legacy_report"
  "$BUILD/dashed_gstrings_differential_tests" --report "$list_report"
  python3 \
    "$PROTOTYPE/tools/compare_differential_reports.py" \
    --expected "$legacy_report" \
    --actual "$list_report"
  echo "Live G-Strings runner matches the List kernel."
else
  echo "Fixture-backed differential baseline passed."
  echo "Set GSTRINGS_DIFFERENTIAL_RUNNER for live side-by-side execution."
fi
