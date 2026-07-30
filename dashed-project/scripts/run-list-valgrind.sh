#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
GECODE_ROOT="$(cd -- "$PROJECT_ROOT/.." && pwd)"
PROTOTYPE_ROOT="$PROJECT_ROOT/prototype"
MEMORY_RUNNER="$SCRIPT_DIR/run-list-memory-profile.sh"
CORE_BUILD="${LIST_MEMORY_CORE_BUILD:-$PROTOTYPE_ROOT/build-native-core}"
GECODE_BUILD="${LIST_MEMORY_GECODE_BUILD:-$GECODE_ROOT/build-list-native}"
PROFILE_BINARY="$GECODE_BUILD/list-memory-profile"
PROFILE_SOURCE="$GECODE_ROOT/gecode/list/tests/memory-profile.cpp"
PROFILE="${LIST_VALGRIND_PROFILE:-smoke}"
TOOLS="${LIST_VALGRIND_TOOLS:-memcheck}"
SCENARIOS="${LIST_VALGRIND_SCENARIOS:-spaces clones dfs}"
USE_CACHE="${LIST_VALGRIND_USE_CACHE:-1}"
VALGRIND_BIN="${VALGRIND:-valgrind}"

case "$PROFILE" in
  smoke)
    batches=3
    spaces_iterations=5
    clones_iterations=2
    dfs_iterations=1
    ;;
  acceptance)
    batches=5
    spaces_iterations=50
    clones_iterations=20
    dfs_iterations=4
    ;;
  *)
    echo "Unsupported Valgrind profile: $PROFILE" >&2
    exit 1
    ;;
esac

if ! command -v "$VALGRIND_BIN" >/dev/null 2>&1; then
  cat >&2 <<'EOF'
Valgrind was not found. On Ubuntu, install it with:

  sudo apt-get update
  sudo apt-get install valgrind
EOF
  exit 127
fi

for tool in $TOOLS; do
  case "$tool" in
    memcheck|massif) ;;
    *)
      echo "Unsupported Valgrind tool: $tool" >&2
      exit 1
      ;;
  esac
done
for scenario in $SCENARIOS; do
  case "$scenario" in
    spaces|clones|dfs) ;;
    *)
      echo "Unsupported List memory scenario: $scenario" >&2
      exit 1
      ;;
  esac
done
if [[ " $TOOLS " == *" massif "* ]] && ! command -v ms_print >/dev/null 2>&1; then
  echo "ms_print is required when LIST_VALGRIND_TOOLS includes massif." >&2
  exit 127
fi

LIST_MEMORY_PROFILE_ALLOW_BOOTSTRAP=0 \
LIST_MEMORY_PROFILE_COMPILE_ONLY=1 \
LIST_MEMORY_CORE_BUILD="$CORE_BUILD" \
LIST_MEMORY_GECODE_BUILD="$GECODE_BUILD" \
  "$MEMORY_RUNNER"

if [[ ! -x "$PROFILE_BINARY" ]]; then
  echo "Memory profiler was not produced: $PROFILE_BINARY" >&2
  exit 1
fi

if [[ -n "${LIST_VALGRIND_REPORT_DIR:-}" ]]; then
  report_dir="$LIST_VALGRIND_REPORT_DIR"
else
  timestamp="$(date -u +%Y%m%dT%H%M%SZ)"
  report_dir="$PROJECT_ROOT/runs/list-valgrind/$timestamp"
fi
mkdir -p "$report_dir"
summary="$report_dir/summary.tsv"
stamp="$report_dir/.fingerprint"

fingerprint="$({
  printf 'profile=%s\ntools=%s\nscenarios=%s\n' \
    "$PROFILE" "$TOOLS" "$SCENARIOS"
  "$VALGRIND_BIN" --version
  sha256sum \
    "$0" \
    "$MEMORY_RUNNER" \
    "$PROFILE_SOURCE" \
    "$GECODE_ROOT/gecode/list/list-var-imp.cpp"
  stat -c '%n:%s:%Y' \
    "$CORE_BUILD/libdashed_core.a" \
    "$GECODE_BUILD/libgecodeint.so" \
    "$GECODE_BUILD/libgecodesearch.so" \
    "$GECODE_BUILD/libgecodekernel.so" \
    "$GECODE_BUILD/libgecodesupport.so"
} | sha256sum | awk '{print $1}')"

expected_rows=0
for _tool in $TOOLS; do
  for _scenario in $SCENARIOS; do
    expected_rows=$((expected_rows + 1))
  done
done

cached_result_valid() {
  [[ "$USE_CACHE" == "1" ]] || return 1
  [[ -f "$stamp" && -f "$summary" ]] || return 1
  [[ "$(cat "$stamp")" == "$fingerprint" ]] || return 1
  python3 - "$summary" "$expected_rows" <<'PY'
import csv
import sys

path, expected = sys.argv[1], int(sys.argv[2])
with open(path, newline='') as handle:
    rows = list(csv.DictReader(handle, delimiter='\t'))
if len(rows) != expected:
    raise SystemExit(1)
if any(row.get('result') != 'PASS' for row in rows):
    raise SystemExit(1)
PY
}

if cached_result_valid; then
  echo "Reusing cached Valgrind result: $report_dir"
  cat "$summary"
  exit 0
fi

rm -f "$summary" "$stamp"
printf 'tool\tscenario\tprofile\tresult\terrors\tdefinitely_lost_bytes\tindirectly_lost_bytes\tpeak_heap_bytes\tlog\n' \
  > "$summary"

iterations_for() {
  case "$1" in
    spaces) printf '%s\n' "$spaces_iterations" ;;
    clones) printf '%s\n' "$clones_iterations" ;;
    dfs) printf '%s\n' "$dfs_iterations" ;;
  esac
}

profile_arguments() {
  local scenario="$1"
  local iterations="$2"
  printf '%s\0' \
    --scenario "$scenario" \
    --profile "valgrind-$PROFILE" \
    --batches "$batches" \
    --iterations "$iterations" \
    --max-growth-kb 1048576 \
    --max-growth-percent 1000
}

extract_lost_bytes() {
  local label="$1"
  local log="$2"
  local value
  value="$(sed -nE "s/.*${label}: ([0-9,]+) bytes.*/\\1/p" "$log" | tail -n 1 | tr -d ',')"
  if [[ -z "$value" ]]; then
    if grep -Fq 'All heap blocks were freed -- no leaks are possible' "$log"; then
      value=0
    else
      value=NA
    fi
  fi
  printf '%s\n' "$value"
}

run_memcheck() {
  local scenario="$1"
  local iterations="$2"
  local log="$report_dir/memcheck-$scenario.log"
  local output="$report_dir/memcheck-$scenario.out"
  local status
  local args=()
  while IFS= read -r -d '' argument; do
    args+=("$argument")
  done < <(profile_arguments "$scenario" "$iterations")

  echo "Valgrind memcheck: scenario=$scenario iterations=$iterations"
  set +e
  "$VALGRIND_BIN" \
    --tool=memcheck \
    --leak-check=full \
    --show-leak-kinds=definite,indirect \
    --errors-for-leak-kinds=definite,indirect \
    --error-exitcode=97 \
    --num-callers=32 \
    --log-file="$log" \
    "$PROFILE_BINARY" "${args[@]}" >"$output" 2>&1
  status=$?
  set -e

  if (( status != 0 )) || ! grep -Eq 'ERROR SUMMARY: 0 errors' "$log"; then
    echo "Valgrind memcheck failed for $scenario (status $status)." >&2
    cat "$output" >&2 || true
    cat "$log" >&2 || true
    return 1
  fi

  local definite indirect
  definite="$(extract_lost_bytes 'definitely lost' "$log")"
  indirect="$(extract_lost_bytes 'indirectly lost' "$log")"
  printf 'memcheck\t%s\t%s\tPASS\t0\t%s\t%s\tNA\t%s\n' \
    "$scenario" "$PROFILE" "$definite" "$indirect" "$log" >> "$summary"
}

run_massif() {
  local scenario="$1"
  local iterations="$2"
  local data="$report_dir/massif-$scenario.out"
  local output="$report_dir/massif-$scenario.program.out"
  local printed="$report_dir/massif-$scenario.txt"
  local args=()
  while IFS= read -r -d '' argument; do
    args+=("$argument")
  done < <(profile_arguments "$scenario" "$iterations")

  echo "Valgrind massif: scenario=$scenario iterations=$iterations"
  "$VALGRIND_BIN" \
    --tool=massif \
    --stacks=no \
    --time-unit=B \
    --massif-out-file="$data" \
    "$PROFILE_BINARY" "${args[@]}" >"$output" 2>&1
  ms_print "$data" > "$printed"

  local peak
  peak="$(python3 - "$data" <<'PY'
from pathlib import Path
import sys

peak = 0
current = {}
for line in Path(sys.argv[1]).read_text().splitlines():
    if line.startswith('snapshot='):
        if current:
            peak = max(peak, sum(current.values()))
        current = {}
    elif line.startswith(('mem_heap_B=', 'mem_heap_extra_B=', 'mem_stacks_B=')):
        key, value = line.split('=', 1)
        current[key] = int(value)
if current:
    peak = max(peak, sum(current.values()))
print(peak)
PY
)"
  printf 'massif\t%s\t%s\tPASS\t0\tNA\tNA\t%s\t%s\n' \
    "$scenario" "$PROFILE" "$peak" "$printed" >> "$summary"
}

for tool in $TOOLS; do
  for scenario in $SCENARIOS; do
    iterations="$(iterations_for "$scenario")"
    case "$tool" in
      memcheck) run_memcheck "$scenario" "$iterations" ;;
      massif) run_massif "$scenario" "$iterations" ;;
    esac
  done
done

python3 - "$summary" "$expected_rows" <<'PY'
import csv
import sys

path, expected = sys.argv[1], int(sys.argv[2])
with open(path, newline='') as handle:
    rows = list(csv.DictReader(handle, delimiter='\t'))
if len(rows) != expected:
    raise SystemExit(f'expected {expected} Valgrind rows, found {len(rows)}')
if any(row['result'] != 'PASS' for row in rows):
    raise SystemExit('one or more Valgrind scenarios failed')
print(f"Native List Valgrind gate passed: {len(rows)} checks")
PY

printf '%s\n' "$fingerprint" > "$stamp"
cat "$summary"
