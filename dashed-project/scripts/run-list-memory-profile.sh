#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
GECODE_ROOT="$(cd -- "$PROJECT_ROOT/.." && pwd)"
PROTOTYPE_ROOT="$PROJECT_ROOT/prototype"
TEST_DIR="$GECODE_ROOT/gecode/list/tests"
CORE_BUILD="${LIST_MEMORY_CORE_BUILD:-$PROTOTYPE_ROOT/build-native-core}"
GECODE_BUILD="${LIST_MEMORY_GECODE_BUILD:-$GECODE_ROOT/build-list-native}"
PROFILE_BINARY="$GECODE_BUILD/list-memory-profile"
PROFILE_SOURCE="$TEST_DIR/memory-profile.cpp"
PROFILE="${LIST_MEMORY_PROFILE_PROFILE:-smoke}"
CXX="${CXX:-g++}"
JOBS="${JOBS:-1}"
ALLOW_BOOTSTRAP="${LIST_MEMORY_PROFILE_ALLOW_BOOTSTRAP:-1}"
MAX_GROWTH_KB="${LIST_MEMORY_PROFILE_MAX_GROWTH_KB:-32768}"
MAX_GROWTH_PERCENT="${LIST_MEMORY_PROFILE_MAX_GROWTH_PERCENT:-25}"

case "$PROFILE" in
  smoke)
    batches=5
    spaces_iterations=200
    clones_iterations=100
    dfs_iterations=4
    ;;
  acceptance)
    batches=10
    spaces_iterations=1000
    clones_iterations=500
    dfs_iterations=20
    MAX_GROWTH_PERCENT="${LIST_MEMORY_PROFILE_MAX_GROWTH_PERCENT:-20}"
    ;;
  *)
    echo "Unsupported List memory profile: $PROFILE" >&2
    exit 1
    ;;
esac

case "$JOBS" in
  ''|*[!0-9]*|0)
    echo "JOBS must be a positive integer: $JOBS" >&2
    exit 1
    ;;
esac
case "$MAX_GROWTH_KB" in
  ''|*[!0-9]*|0)
    echo "LIST_MEMORY_PROFILE_MAX_GROWTH_KB must be positive." >&2
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
  if [[ "$ALLOW_BOOTSTRAP" != "1" ]]; then
    echo "Normal native build cache is missing; bootstrap was disabled." >&2
    echo "Run: JOBS=\"$JOBS\" ./gecode/list/tests/run-tests" >&2
    exit 1
  fi
  echo "Normal native build cache is missing; building it once."
  JOBS="$JOBS" "$TEST_DIR/run-tests"
fi

if ! native_cache_ready; then
  echo "Native cache bootstrap completed without the required libraries." >&2
  exit 1
fi

if ! command -v "$CXX" >/dev/null 2>&1; then
  echo "C++ compiler not found: $CXX" >&2
  exit 1
fi

needs_compile=0
if [[ ! -x "$PROFILE_BINARY" ]]; then
  needs_compile=1
else
  for dependency in \
    "$PROFILE_SOURCE" \
    "$GECODE_ROOT/gecode/list/list-var-imp.cpp" \
    "$CORE_BUILD/libdashed_core.a"
  do
    if [[ "$dependency" -nt "$PROFILE_BINARY" ]]; then
      needs_compile=1
      break
    fi
  done
fi

if (( needs_compile )); then
  echo "Compiling cached native List memory profiler"
  "$CXX" \
    -std=c++17 \
    -O2 \
    -g \
    -DGECODE_HAS_LIST_VARS=1 \
    -DGECODE_NO_AUTOLINK \
    -I"$GECODE_BUILD" \
    -I"$GECODE_ROOT" \
    -I"$PROTOTYPE_ROOT/include" \
    "$PROFILE_SOURCE" \
    "$GECODE_ROOT/gecode/list/list-var-imp.cpp" \
    "$CORE_BUILD/libdashed_core.a" \
    -L"$GECODE_BUILD" \
    -Wl,-rpath,"$GECODE_BUILD" \
    -lgecodeint \
    -lgecodesearch \
    -lgecodekernel \
    -lgecodesupport \
    -pthread \
    -o "$PROFILE_BINARY"
fi

if [[ "${LIST_MEMORY_PROFILE_COMPILE_ONLY:-0}" == "1" ]]; then
  echo "Native List memory profiler is ready: $PROFILE_BINARY"
  exit 0
fi

if [[ -n "${LIST_MEMORY_PROFILE_REPORT:-}" ]]; then
  report="$LIST_MEMORY_PROFILE_REPORT"
  mkdir -p "$(dirname -- "$report")"
else
  timestamp="$(date -u +%Y%m%dT%H%M%SZ)"
  report_dir="$PROJECT_ROOT/runs/list-memory/$timestamp"
  mkdir -p "$report_dir"
  report="$report_dir/native-list-memory.tsv"
fi
rm -f "$report"

printf 'Profile:       %s\n' "$PROFILE"
printf 'Profiler:      %s\n' "$PROFILE_BINARY"
printf 'Report:        %s\n' "$report"
printf 'Growth limit:  %s KiB or %s%%\n' \
  "$MAX_GROWTH_KB" "$MAX_GROWTH_PERCENT"

run_scenario() {
  local scenario="$1"
  local iterations="$2"
  "$PROFILE_BINARY" \
    --scenario "$scenario" \
    --profile "$PROFILE" \
    --report "$report" \
    --batches "$batches" \
    --iterations "$iterations" \
    --max-growth-kb "$MAX_GROWTH_KB" \
    --max-growth-percent "$MAX_GROWTH_PERCENT"
}

run_scenario spaces "$spaces_iterations"
run_scenario clones "$clones_iterations"
run_scenario dfs "$dfs_iterations"

python3 - "$report" <<'PY'
from pathlib import Path
import csv
import sys

path = Path(sys.argv[1])
with path.open(newline="") as handle:
    rows = list(csv.DictReader(handle, delimiter="\t"))
if [row["scenario"] for row in rows] != ["spaces", "clones", "dfs"]:
    raise SystemExit("memory profile did not emit all three scenarios")
if any(row["result"] != "PASS" for row in rows):
    raise SystemExit("one or more memory scenarios exceeded the growth limit")
print(f"Native List memory profile passed: {len(rows)} scenarios")
PY

cat "$report"
