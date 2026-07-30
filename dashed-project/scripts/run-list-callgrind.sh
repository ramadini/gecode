#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
GECODE_ROOT="$(cd -- "$PROJECT_ROOT/.." && pwd)"
PROTOTYPE_ROOT="$PROJECT_ROOT/prototype"
PERF_RUNNER="$SCRIPT_DIR/run-list-performance-benchmark.sh"
SUMMARIZER="$PROTOTYPE_ROOT/tools/summarize_callgrind.py"
CORE_BUILD="${LIST_CALLGRIND_CORE_BUILD:-$PROTOTYPE_ROOT/build-native-core}"
GECODE_BUILD="${LIST_CALLGRIND_GECODE_BUILD:-$GECODE_ROOT/build-list-native}"
BENCHMARK_BINARY="${LIST_CALLGRIND_BENCHMARK_BINARY:-$GECODE_BUILD/list-performance-benchmark}"
PROFILE="${LIST_CALLGRIND_PROFILE:-smoke}"
SCENARIOS="${LIST_CALLGRIND_SCENARIOS:-propagate clone dfs}"
USE_CACHE="${LIST_CALLGRIND_USE_CACHE:-1}"
REUSE_RAW="${LIST_CALLGRIND_REUSE_RAW:-0}"
TOP="${LIST_CALLGRIND_TOP:-25}"
VALGRIND="${VALGRIND:-valgrind}"

case "$PROFILE" in
  smoke)
    samples=3
    propagate_iterations=10
    clone_iterations=2
    dfs_iterations=1
    ;;
  attribution)
    samples=3
    propagate_iterations=100
    clone_iterations=20
    dfs_iterations=2
    ;;
  deep)
    samples=5
    propagate_iterations=500
    clone_iterations=100
    dfs_iterations=5
    ;;
  *)
    echo "Unsupported Callgrind profile: $PROFILE" >&2
    exit 1
    ;;
esac

case "$TOP" in
  ''|*[!0-9]* )
    echo "LIST_CALLGRIND_TOP must be a positive integer." >&2
    exit 1
    ;;
esac
if (( TOP <= 0 )); then
  echo "LIST_CALLGRIND_TOP must be a positive integer." >&2
  exit 1
fi
case "$REUSE_RAW" in
  0|1) ;;
  *)
    echo "LIST_CALLGRIND_REUSE_RAW must be 0 or 1." >&2
    exit 1
    ;;
esac

if [[ ! -x "$SUMMARIZER" ]]; then
  echo "Missing Callgrind summarizer: $SUMMARIZER" >&2
  exit 1
fi

for scenario in $SCENARIOS; do
  case "$scenario" in
    propagate|clone|dfs) ;;
    *)
      echo "Unsupported Callgrind scenario: $scenario" >&2
      exit 1
      ;;
  esac
done

if [[ -n "${LIST_CALLGRIND_REPORT_DIR:-}" ]]; then
  report_dir="$LIST_CALLGRIND_REPORT_DIR"
else
  if [[ "$REUSE_RAW" == "1" ]]; then
    echo "LIST_CALLGRIND_REPORT_DIR is required with LIST_CALLGRIND_REUSE_RAW=1." >&2
    exit 1
  fi
  timestamp="$(date -u +%Y%m%dT%H%M%SZ)"
  report_dir="$PROJECT_ROOT/runs/list-performance/callgrind-$timestamp"
fi
summary="$report_dir/callgrind-hotspots.tsv"
metadata="$report_dir/metadata.txt"
stamp="$report_dir/.fingerprint"

if [[ "$REUSE_RAW" == "1" ]]; then
  if [[ ! -d "$report_dir" || ! -f "$metadata" ]]; then
    echo "Existing Callgrind report metadata is required: $metadata" >&2
    exit 1
  fi
  grep -Fxq "profile=$PROFILE" "$metadata" || {
    echo "Existing raw profile does not match LIST_CALLGRIND_PROFILE=$PROFILE" >&2
    exit 1
  }
  grep -Fxq "scenarios=$SCENARIOS" "$metadata" || {
    echo "Existing raw scenarios do not match: $SCENARIOS" >&2
    exit 1
  }
  for scenario in $SCENARIOS; do
    raw="$report_dir/callgrind.$scenario.out"
    if [[ ! -s "$raw" ]]; then
      echo "Missing existing Callgrind data: $raw" >&2
      exit 1
    fi
  done
else
  mkdir -p "$report_dir"
  if ! command -v "$VALGRIND" >/dev/null 2>&1; then
    cat >&2 <<'EOF'
Valgrind is required for Callgrind hotspot attribution.
On Ubuntu install it with:

  sudo apt-get update
  sudo apt-get install valgrind
EOF
    exit 127
  fi
  if [[ ! -x "$PERF_RUNNER" ]]; then
    echo "Missing native List performance runner: $PERF_RUNNER" >&2
    exit 1
  fi

  benchmark_stale=0
  if [[ ! -x "$BENCHMARK_BINARY" ]]; then
    benchmark_stale=1
  else
    for dependency in \
      "$GECODE_ROOT/gecode/list/tests/performance-benchmark.cpp" \
      "$GECODE_ROOT/gecode/list/list-var-imp.cpp" \
      "$CORE_BUILD/libdashed_core.a"
    do
      if [[ ! -e "$dependency" || "$dependency" -nt "$BENCHMARK_BINARY" ]]; then
        benchmark_stale=1
        break
      fi
    done
  fi

  if (( benchmark_stale )); then
    echo "Preparing the cached native List performance binary"
    LIST_PERF_PROFILE=smoke \
    LIST_PERF_REPORT_DIR="${TMPDIR:-/tmp}/list-callgrind-performance-preflight" \
      "$PERF_RUNNER"
  fi
  if [[ ! -x "$BENCHMARK_BINARY" ]]; then
    echo "Native List performance binary was not produced: $BENCHMARK_BINARY" >&2
    exit 1
  fi
fi

if [[ "$REUSE_RAW" == "1" ]]; then
  fingerprint="$({
    printf 'mode=reuse-raw\nprofile=%s\nscenarios=%s\ntop=%s\n' \
      "$PROFILE" "$SCENARIOS" "$TOP"
    sha256sum "$0" "$SUMMARIZER"
    for scenario in $SCENARIOS; do
      sha256sum "$report_dir/callgrind.$scenario.out"
    done
  } | sha256sum | awk '{print $1}')"
else
  fingerprint="$({
    printf 'mode=profile\nprofile=%s\nscenarios=%s\ntop=%s\n' \
      "$PROFILE" "$SCENARIOS" "$TOP"
    "$VALGRIND" --version
    sha256sum \
      "$0" \
      "$SUMMARIZER" \
      "$GECODE_ROOT/gecode/list/tests/performance-benchmark.cpp" \
      "$GECODE_ROOT/gecode/list/list-var-imp.cpp" \
      "$BENCHMARK_BINARY"
    stat -c '%n:%s:%Y' \
      "$CORE_BUILD/libdashed_core.a" \
      "$GECODE_BUILD/libgecodeint.so" \
      "$GECODE_BUILD/libgecodesearch.so" \
      "$GECODE_BUILD/libgecodekernel.so" \
      "$GECODE_BUILD/libgecodesupport.so"
  } | sha256sum | awk '{print $1}')"
fi

cache_valid() {
  [[ "$USE_CACHE" == "1" ]] || return 1
  [[ -f "$stamp" && -f "$summary" ]] || return 1
  [[ "$(cat "$stamp")" == "$fingerprint" ]] || return 1
  python3 - "$summary" $SCENARIOS <<'PY'
import csv
import re
import sys

path = sys.argv[1]
expected = sys.argv[2:]
with open(path, newline='') as handle:
    rows = list(csv.DictReader(handle, delimiter='\t'))
if not rows:
    raise SystemExit(1)
for scenario in expected:
    views = {row['view'] for row in rows if row['scenario'] == scenario}
    if views != {'all', 'project'}:
        raise SystemExit(1)
if any(re.fullmatch(r'\(\d+\)', row['function']) for row in rows):
    raise SystemExit(1)
PY
}

iterations_for() {
  case "$1" in
    propagate) printf '%s\n' "$propagate_iterations" ;;
    clone) printf '%s\n' "$clone_iterations" ;;
    dfs) printf '%s\n' "$dfs_iterations" ;;
  esac
}

if cache_valid; then
  echo "Reusing cached native List Callgrind report: $report_dir"
else
  rm -f "$summary" "$stamp"
  printf 'Profile:    %s\n' "$PROFILE"
  printf 'Scenarios:  %s\n' "$SCENARIOS"
  printf 'Report:     %s\n' "$report_dir"

  first=1
  for scenario in $SCENARIOS; do
    iterations="$(iterations_for "$scenario")"
    raw="$report_dir/callgrind.$scenario.out"
    stdout_log="$report_dir/$scenario.stdout.log"
    stderr_log="$report_dir/$scenario.callgrind.log"

    if [[ "$REUSE_RAW" == "1" ]]; then
      echo "Re-summarizing existing $scenario Callgrind data"
    else
      rm -f "$raw" "$stdout_log" "$stderr_log"
      echo "Profiling $scenario: samples=$samples iterations=$iterations"
      "$VALGRIND" \
        --tool=callgrind \
        --callgrind-out-file="$raw" \
        --error-exitcode=99 \
        --cache-sim=no \
        --branch-sim=no \
        --collect-jumps=no \
        "$BENCHMARK_BINARY" \
          --scenario "$scenario" \
          --profile "callgrind-$PROFILE" \
          --samples "$samples" \
          --iterations "$iterations" \
          >"$stdout_log" 2>"$stderr_log"
    fi

    if [[ ! -s "$raw" ]]; then
      echo "Callgrind did not produce data for $scenario." >&2
      [[ -f "$stderr_log" ]] && cat "$stderr_log" >&2
      exit 1
    fi

    append=()
    if (( ! first )); then
      append=(--append)
    fi
    python3 "$SUMMARIZER" \
      "$raw" \
      --scenario "$scenario" \
      --output "$summary" \
      --top "$TOP" \
      "${append[@]}"
    first=0
  done

  valgrind_version="not-required-for-raw-resummarization"
  if [[ "$REUSE_RAW" != "1" ]]; then
    valgrind_version="$($VALGRIND --version)"
  elif command -v "$VALGRIND" >/dev/null 2>&1; then
    valgrind_version="$($VALGRIND --version)"
  fi
  {
    printf 'commit=%s\n' "$(git -C "$GECODE_ROOT" rev-parse HEAD)"
    printf 'profile=%s\n' "$PROFILE"
    printf 'scenarios=%s\n' "$SCENARIOS"
    printf 'valgrind=%s\n' "$valgrind_version"
    printf 'machine=%s\n' "$(uname -a)"
    printf 'fingerprint=%s\n' "$fingerprint"
  } > "$metadata"
  printf '%s\n' "$fingerprint" > "$stamp"
fi

python3 - "$summary" $SCENARIOS <<'PY'
import csv
import re
import sys

path = sys.argv[1]
scenarios = sys.argv[2:]
with open(path, newline='') as handle:
    rows = list(csv.DictReader(handle, delimiter='\t'))

print("scenario\trank\tself_percent\tscope\tinstances\tfunction")
for scenario in scenarios:
    selected = [
        row for row in rows
        if row['scenario'] == scenario and row['view'] == 'project'
    ][:10]
    if not selected:
        raise SystemExit(f'no project hotspot rows for {scenario}')
    for row in selected:
        if re.fullmatch(r'\(\d+\)', row['function']):
            raise SystemExit(f"unresolved compressed symbol in {scenario}: {row['function']}")
        print(
            f"{scenario}\t{row['rank']}\t{row['self_percent']}\t"
            f"{row['scope']}\t{row['instances']}\t{row['function']}"
        )
PY

printf '\nFull Callgrind hotspot report: %s\n' "$summary"
