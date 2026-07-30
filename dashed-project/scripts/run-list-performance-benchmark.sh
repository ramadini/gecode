#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
GECODE_ROOT="$(cd -- "$PROJECT_ROOT/.." && pwd)"
PROTOTYPE_ROOT="$PROJECT_ROOT/prototype"
TEST_DIR="$GECODE_ROOT/gecode/list/tests"
CORE_BUILD="${LIST_PERF_CORE_BUILD:-$PROTOTYPE_ROOT/build-native-core}"
GECODE_BUILD="${LIST_PERF_GECODE_BUILD:-$GECODE_ROOT/build-list-native}"
BENCHMARK_BINARY="$GECODE_BUILD/list-performance-benchmark"
BENCHMARK_SOURCE="$TEST_DIR/performance-benchmark.cpp"
PROFILE="${LIST_PERF_PROFILE:-smoke}"
CXX="${CXX:-g++}"
USE_CACHE="${LIST_PERF_USE_CACHE:-1}"
MAX_REGRESSION_PERCENT="${LIST_PERF_MAX_REGRESSION_PERCENT:-15}"

case "$PROFILE" in
  smoke)
    samples=3
    propagate_iterations=100
    clone_iterations=20
    dfs_iterations=1
    ;;
  baseline)
    samples=7
    propagate_iterations=1000
    clone_iterations=200
    dfs_iterations=5
    ;;
  acceptance)
    samples=11
    propagate_iterations=5000
    clone_iterations=1000
    dfs_iterations=20
    ;;
  *)
    echo "Unsupported List performance profile: $PROFILE" >&2
    exit 1
    ;;
esac

case "$MAX_REGRESSION_PERCENT" in
  ''|*[!0-9]* )
    echo "LIST_PERF_MAX_REGRESSION_PERCENT must be a non-negative integer." >&2
    exit 1
    ;;
esac

native_cache_ready() {
  [[ -e "$CORE_BUILD/libdashed_core.a" ]] &&
  [[ -e "$GECODE_BUILD/libgecodeint.so" ]] &&
  [[ -e "$GECODE_BUILD/libgecodesearch.so" ]] &&
  [[ -e "$GECODE_BUILD/libgecodekernel.so" ]] &&
  [[ -e "$GECODE_BUILD/libgecodesupport.so" ]]
}

if ! native_cache_ready; then
  cat >&2 <<EOF
The short performance runner does not rebuild Gecode.
Populate the normal native cache once with:

  JOBS="\$(nproc)" ./gecode/list/tests/run-tests

Then rerun this command.
EOF
  exit 1
fi

if ! command -v "$CXX" >/dev/null 2>&1; then
  echo "C++ compiler not found: $CXX" >&2
  exit 1
fi

needs_compile=0
if [[ ! -x "$BENCHMARK_BINARY" ]]; then
  needs_compile=1
else
  for dependency in \
    "$BENCHMARK_SOURCE" \
    "$GECODE_ROOT/gecode/list/list-var-imp.cpp" \
    "$CORE_BUILD/libdashed_core.a"
  do
    if [[ "$dependency" -nt "$BENCHMARK_BINARY" ]]; then
      needs_compile=1
      break
    fi
  done
fi

if (( needs_compile )); then
  echo "Compiling cached native List performance benchmark"
  "$CXX" \
    -std=c++17 \
    -O3 \
    -g \
    -DNDEBUG \
    -DGECODE_HAS_LIST_VARS=1 \
    -DGECODE_NO_AUTOLINK \
    -I"$GECODE_BUILD" \
    -I"$GECODE_ROOT" \
    -I"$PROTOTYPE_ROOT/include" \
    "$BENCHMARK_SOURCE" \
    "$GECODE_ROOT/gecode/list/list-var-imp.cpp" \
    "$CORE_BUILD/libdashed_core.a" \
    -L"$GECODE_BUILD" \
    -Wl,-rpath,"$GECODE_BUILD" \
    -lgecodeint \
    -lgecodesearch \
    -lgecodekernel \
    -lgecodesupport \
    -pthread \
    -o "$BENCHMARK_BINARY"
fi

if [[ -n "${LIST_PERF_REPORT_DIR:-}" ]]; then
  report_dir="$LIST_PERF_REPORT_DIR"
else
  timestamp="$(date -u +%Y%m%dT%H%M%SZ)"
  report_dir="$PROJECT_ROOT/runs/list-performance/$timestamp"
fi
mkdir -p "$report_dir"
report="$report_dir/native-list-performance.tsv"
metadata="$report_dir/metadata.txt"
stamp="$report_dir/.fingerprint"

fingerprint="$({
  printf 'profile=%s\nsamples=%s\npropagate=%s\nclone=%s\ndfs=%s\n' \
    "$PROFILE" "$samples" "$propagate_iterations" "$clone_iterations" \
    "$dfs_iterations"
  "$CXX" --version | head -n 1
  sha256sum \
    "$0" \
    "$BENCHMARK_SOURCE" \
    "$GECODE_ROOT/gecode/list/list-var-imp.cpp"
  stat -c '%n:%s:%Y' \
    "$CORE_BUILD/libdashed_core.a" \
    "$GECODE_BUILD/libgecodeint.so" \
    "$GECODE_BUILD/libgecodesearch.so" \
    "$GECODE_BUILD/libgecodekernel.so" \
    "$GECODE_BUILD/libgecodesupport.so"
} | sha256sum | awk '{print $1}')"

cached_result_valid() {
  [[ "$USE_CACHE" == "1" ]] || return 1
  [[ -f "$stamp" && -f "$report" ]] || return 1
  [[ "$(cat "$stamp")" == "$fingerprint" ]] || return 1
  python3 - "$report" <<'PY'
import csv
import sys

with open(sys.argv[1], newline='') as handle:
    rows = list(csv.DictReader(handle, delimiter='\t'))
if [row.get('scenario') for row in rows] != ['propagate', 'clone', 'dfs']:
    raise SystemExit(1)
if any(row.get('result') != 'PASS' for row in rows):
    raise SystemExit(1)
PY
}

run_benchmark() {
  local scenario="$1"
  local iterations="$2"
  local command=(
    "$BENCHMARK_BINARY"
    --scenario "$scenario"
    --profile "$PROFILE"
    --report "$report"
    --samples "$samples"
    --iterations "$iterations"
  )
  if [[ -n "${LIST_PERF_CPU:-}" ]]; then
    if ! command -v taskset >/dev/null 2>&1; then
      echo "taskset is required when LIST_PERF_CPU is set." >&2
      exit 127
    fi
    command=(taskset -c "$LIST_PERF_CPU" "${command[@]}")
  fi
  "${command[@]}"
}

if cached_result_valid; then
  echo "Reusing cached native List performance report: $report_dir"
else
  rm -f "$report" "$metadata" "$stamp"
  printf 'Profile:    %s\n' "$PROFILE"
  printf 'Benchmark:  %s\n' "$BENCHMARK_BINARY"
  printf 'Report:     %s\n' "$report"
  if [[ -n "${LIST_PERF_CPU:-}" ]]; then
    printf 'CPU pin:    %s\n' "$LIST_PERF_CPU"
  fi

  run_benchmark propagate "$propagate_iterations"
  run_benchmark clone "$clone_iterations"
  run_benchmark dfs "$dfs_iterations"

  python3 - "$report" <<'PY'
import csv
import sys

with open(sys.argv[1], newline='') as handle:
    rows = list(csv.DictReader(handle, delimiter='\t'))
expected = ['propagate', 'clone', 'dfs']
if [row['scenario'] for row in rows] != expected:
    raise SystemExit(f'expected scenarios {expected}, found {[row["scenario"] for row in rows]}')
if any(row['result'] != 'PASS' for row in rows):
    raise SystemExit('one or more native List performance scenarios failed')
if int(rows[2]['solutions']) != int(rows[2]['operations']) * 40:
    raise SystemExit('DFS solution invariant changed')
print(f"Native List performance benchmark passed: {len(rows)} scenarios")
PY

  {
    printf 'commit=%s\n' "$(git -C "$GECODE_ROOT" rev-parse HEAD)"
    printf 'profile=%s\n' "$PROFILE"
    printf 'compiler=%s\n' "$($CXX --version | head -n 1)"
    printf 'machine=%s\n' "$(uname -a)"
    printf 'cpu_pin=%s\n' "${LIST_PERF_CPU:-none}"
    printf 'fingerprint=%s\n' "$fingerprint"
  } > "$metadata"
  printf '%s\n' "$fingerprint" > "$stamp"
fi

if [[ -n "${LIST_PERF_BASELINE:-}" ]]; then
  python3 - "$LIST_PERF_BASELINE" "$report" "$MAX_REGRESSION_PERCENT" <<'PY'
import csv
from pathlib import Path
import sys

baseline_path = Path(sys.argv[1])
current_path = Path(sys.argv[2])
tolerance = int(sys.argv[3])
if not baseline_path.is_file():
    raise SystemExit(f'performance baseline does not exist: {baseline_path}')

def load(path):
    with path.open(newline='') as handle:
        return {row['scenario']: row for row in csv.DictReader(handle, delimiter='\t')}

baseline = load(baseline_path)
current = load(current_path)
expected = {'propagate', 'clone', 'dfs'}
if set(baseline) != expected or set(current) != expected:
    raise SystemExit('baseline and current reports must contain propagate, clone, and dfs')

failed = False
print('scenario\tbaseline_median_ns\tcurrent_median_ns\tchange_percent\tresult')
for scenario in ('propagate', 'clone', 'dfs'):
    old = int(baseline[scenario]['median_ns_per_op'])
    new = int(current[scenario]['median_ns_per_op'])
    change = ((new - old) / old * 100.0) if old else float('inf')
    passed = new <= old * (1.0 + tolerance / 100.0)
    print(f'{scenario}\t{old}\t{new}\t{change:.2f}\t{"PASS" if passed else "FAIL"}')
    failed = failed or not passed
if failed:
    raise SystemExit(f'performance regression exceeded {tolerance}%')
PY
fi

cat "$report"
