#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
GECODE_ROOT="$(cd -- "$PROJECT_ROOT/.." && pwd)"
PROTOTYPE_ROOT="$PROJECT_ROOT/prototype"
TEST_DIR="$GECODE_ROOT/gecode/list/tests"

JOBS="${JOBS:-1}"
CXX="${CXX:-g++}"
CMAKE_GENERATOR="${CMAKE_GENERATOR:-Ninja}"
SANITIZER_PROFILE="${LIST_NATIVE_SANITIZER_PROFILE:-full}"
BUILD_JOBS_OVERRIDE="${LIST_NATIVE_SANITIZER_BUILD_JOBS:-}"

case "$SANITIZER_PROFILE" in
  full)
    default_modes="address undefined"
    default_binaries="lifecycle regression"
    ;;
  smoke)
    default_modes="address"
    default_binaries="lifecycle"
    ;;
  *)
    echo "Unsupported native sanitizer profile: $SANITIZER_PROFILE" >&2
    exit 1
    ;;
esac

SANITIZER_MODES="${LIST_NATIVE_SANITIZER_MODES:-$default_modes}"
BINARIES="${LIST_NATIVE_SANITIZER_BINARIES:-$default_binaries}"

case "$JOBS" in
  ''|*[!0-9]*|0)
    echo "JOBS must be a positive integer: $JOBS" >&2
    exit 1
    ;;
esac
if [[ -n "$BUILD_JOBS_OVERRIDE" ]]; then
  case "$BUILD_JOBS_OVERRIDE" in
    *[!0-9]*|0)
      echo "LIST_NATIVE_SANITIZER_BUILD_JOBS must be a positive integer: $BUILD_JOBS_OVERRIDE" >&2
      exit 1
      ;;
  esac
fi

if ! command -v "$CXX" >/dev/null 2>&1; then
  echo "C++ compiler not found: $CXX" >&2
  exit 1
fi

compiler_path="$(readlink -f "$(command -v "$CXX")")"
compiler_id="$($CXX --version | head -n 1 | tr ' /' '__' | tr -cd '[:alnum:]_.-')"

min_jobs() {
  local requested="$1"
  local cap="$2"
  if (( requested < cap )); then
    printf '%s\n' "$requested"
  else
    printf '%s\n' "$cap"
  fi
}

build_jobs_for_mode() {
  local mode="$1"
  local memory_kb=0
  local cap

  if [[ -n "$BUILD_JOBS_OVERRIDE" ]]; then
    min_jobs "$JOBS" "$BUILD_JOBS_OVERRIDE"
    return
  fi

  if [[ -r /proc/meminfo ]]; then
    memory_kb="$(awk '/^MemTotal:/ {print $2; exit}' /proc/meminfo)"
  fi
  case "$memory_kb" in
    ''|*[!0-9]*) memory_kb=0 ;;
  esac

  # Sanitized Gecode translation units can require several GiB each. Keep the
  # default deliberately conservative, especially for UBSan's large integer
  # extensional sources. Users can override this explicitly after observing
  # their machine with LIST_NATIVE_SANITIZER_BUILD_JOBS.
  if [[ ",$mode," == *",undefined,"* ]]; then
    if (( memory_kb >= 33554432 )); then
      cap=2
    else
      cap=1
    fi
  else
    if (( memory_kb >= 25165824 )); then
      cap=4
    elif (( memory_kb >= 12582912 )); then
      cap=2
    else
      cap=1
    fi
  fi

  min_jobs "$JOBS" "$cap"
}

audit_dfs_source_ownership() {
  python3 - "$TEST_DIR" <<'PY_AUDIT'
from pathlib import Path
import re
import sys


def matching_paren(text: str, opening: int) -> int:
    depth = 0
    quote = None
    escaped = False
    index = opening
    while index < len(text):
        char = text[index]
        if quote is not None:
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == quote:
                quote = None
            index += 1
            continue
        if char in {'"', "'"}:
            quote = char
        elif char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
            if depth == 0:
                return index
        index += 1
    raise ValueError("unterminated DFS constructor")


def first_argument(arguments: str) -> str:
    depth = 0
    quote = None
    escaped = False
    for index, char in enumerate(arguments):
        if quote is not None:
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == quote:
                quote = None
            continue
        if char in {'"', "'"}:
            quote = char
        elif char in "([{":
            depth += 1
        elif char in ")]}":
            depth -= 1
        elif char == "," and depth == 0:
            return arguments[:index].strip()
    return arguments.strip()


header = re.compile(r"Gecode::DFS<[^;{}]+>\s+[A-Za-z_]\w*\s*\(")
problems = []
for path in sorted(Path(sys.argv[1]).glob("*.cpp")):
    text = path.read_text()
    for match in header.finditer(text):
        opening = match.end() - 1
        try:
            closing = matching_paren(text, opening)
        except ValueError as error:
            problems.append(
                f"{path}:{text.count(chr(10), 0, match.start()) + 1}: {error}"
            )
            continue

        argument = first_argument(text[opening + 1:closing])
        line = text.count("\n", 0, match.start()) + 1
        if re.match(r"new\b", argument):
            problems.append(f"{path}:{line}: DFS constructed directly from new")
            continue

        identifier = re.fullmatch(r"[A-Za-z_]\w*", argument)
        if identifier is None:
            continue

        semicolon = text.find(";", closing)
        if semicolon == -1:
            problems.append(f"{path}:{line}: unterminated DFS declaration")
            continue

        tail = text[semicolon + 1:semicolon + 256]
        expected = re.compile(rf"^\s*delete\s+{re.escape(argument)}\s*;")
        if expected.search(tail) is None:
            problems.append(
                f"{path}:{line}: DFS source pointer '{argument}' is not deleted "
                "immediately after snapshot construction"
            )

if problems:
    raise SystemExit("\n".join(problems))
print("Native List DFS source-space ownership audit passed")
PY_AUDIT
}

audit_dfs_source_ownership

if [[ "${LIST_NATIVE_SANITIZER_AUDIT_ONLY:-0}" == "1" ]]; then
  exit 0
fi

if [[ "${LIST_NATIVE_SANITIZER_PRINT_PLAN:-0}" == "1" ]]; then
  for planned_mode in $SANITIZER_MODES; do
    printf 'mode=%s build_jobs=%s binaries=%s profile=%s\n' \
      "$planned_mode" \
      "$(build_jobs_for_mode "$planned_mode")" \
      "$BINARIES" \
      "$SANITIZER_PROFILE"
  done
  exit 0
fi

remove_stale_cache() {
  local build_dir="$1"
  local expected_source="$2"

  if [[ ! -f "$build_dir/CMakeCache.txt" ]]; then
    return
  fi

  local cached_source
  local cached_generator
  cached_source="$(
    sed -n 's/^CMAKE_HOME_DIRECTORY:INTERNAL=//p' \
      "$build_dir/CMakeCache.txt" | head -n 1
  )"
  cached_generator="$(
    sed -n 's/^CMAKE_GENERATOR:INTERNAL=//p' \
      "$build_dir/CMakeCache.txt" | head -n 1
  )"

  # Build directories already include the compiler identity. Do not discard a
  # valid cache merely because CMake records the compiler with a different
  # cache type or symlink spelling. CMake will diagnose a truly incompatible
  # compiler during configure; source and generator mismatches are the cases
  # that require a clean cache here.
  if [[ -z "$cached_source" || \
        "$cached_source" != "$expected_source" || \
        ( -n "$cached_generator" && \
          "$cached_generator" != "$CMAKE_GENERATOR" ) ]]; then
    echo "Removing stale build cache: $build_dir"
    rm -rf "$build_dir"
  fi
}

build_cmake_with_oom_retry() {
  local build_dir="$1"
  local jobs="$2"
  shift 2
  local -a target_args=("$@")
  local build_log
  local status

  build_log="$(mktemp)"
  set +e
  cmake --build "$build_dir" \
    "${target_args[@]}" \
    --parallel "$jobs" 2>&1 | tee "$build_log"
  status=${PIPESTATUS[0]}
  set -e

  if (( status != 0 && jobs > 1 )) && \
      grep -E 'Killed signal terminated program cc1plus|out of memory|cannot allocate memory' \
        "$build_log" >/dev/null; then
    echo
    echo "Parallel sanitizer compilation exhausted memory; resuming with one job."
    rm -f "$build_log"
    cmake --build "$build_dir" \
      "${target_args[@]}" \
      --parallel 1
    return
  fi

  rm -f "$build_log"
  return "$status"
}

verify_selective_vptr_policy() {
  local runtime_flags="$1"
  local compatibility_flags="$2"
  local temporary
  local full_status

  temporary="$(mktemp -d)"
  cat > "$temporary/vptr-probe.cpp" <<'CPP'
struct Base {
  virtual ~Base() = default;
};
struct Left : Base {};
struct Right : Base {};

int main() {
  Left left;
  Base* base = &left;
  Right* wrong = static_cast<Right*>(base);
  return wrong == nullptr;
}
CPP

  "$CXX" -std=c++17 -O1 -g \
    $runtime_flags \
    "$temporary/vptr-probe.cpp" \
    -o "$temporary/full-vptr"

  set +e
  UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=1" \
    "$temporary/full-vptr" >"$temporary/full-vptr.log" 2>&1
  full_status=$?
  set -e

  if (( full_status == 0 )); then
    echo "Full native List UBSan flags did not detect the vptr probe." >&2
    cat "$temporary/full-vptr.log" >&2
    exit 1
  fi
  if ! grep -E 'runtime error:.*(downcast|dynamic type|vptr)' \
      "$temporary/full-vptr.log" >/dev/null; then
    echo "The full-vptr probe failed for an unexpected reason." >&2
    cat "$temporary/full-vptr.log" >&2
    exit 1
  fi

  "$CXX" -std=c++17 -O1 -g \
    $compatibility_flags \
    "$temporary/vptr-probe.cpp" \
    -o "$temporary/no-vptr"
  UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=1" \
    "$temporary/no-vptr" >"$temporary/no-vptr.log" 2>&1

  rm -rf "$temporary"
  echo "Selective UBSan vptr policy probe passed"
}

verify_ubsan_rtti_exports() {
  local gecode_build="$1"
  local symbol
  local library
  local found
  local -a required_symbols=(
    "typeinfo for Gecode::Rnd::IMP"
    "typeinfo for Gecode::TraceFilter::TFO"
  )

  if ! command -v nm >/dev/null 2>&1; then
    echo "nm is required for the native UBSan RTTI visibility check." >&2
    exit 1
  fi

  for symbol in "${required_symbols[@]}"; do
    found=0
    while IFS= read -r -d '' library; do
      if nm -D -C --defined-only "$library" 2>/dev/null | \
          grep -F -- "$symbol" >/dev/null; then
        found=1
        break
      fi
    done < <(
      find "$gecode_build" -maxdepth 1 -type f \
        -name 'libgecode*.so*' -print0
    )

    if (( ! found )); then
      echo "Native UBSan build did not export required RTTI: $symbol" >&2
      nm -D -C --undefined-only "$gecode_build"/libgecodeint.so* 2>/dev/null | \
        grep -E 'typeinfo for Gecode::(Rnd::IMP|TraceFilter::TFO)' >&2 || true
      exit 1
    fi
  done

  echo "Native UBSan RTTI visibility probe passed"
}

compile_native_test() {
  local mode="$1"
  local gecode_build="$2"
  local core_build="$3"
  local source="$4"
  local output="$5"

  local runtime_flags="-fsanitize=$mode -fno-omit-frame-pointer -fno-sanitize-recover=all"
  local test_object="${output}.test.o"
  local list_object="${output}.list.o"
  local -a common_flags=(
    -std=c++17
    -O1
    -g
    -DGECODE_HAS_LIST_VARS=1
    -DGECODE_NO_AUTOLINK
    -isystem "$gecode_build"
    -isystem "$GECODE_ROOT"
    -isystem "$PROTOTYPE_ROOT/include"
  )

  rm -f "$test_object" "$list_object" "$output"

  "$CXX" \
    "${common_flags[@]}" \
    $runtime_flags \
    -c "$source" \
    -o "$test_object"

  "$CXX" \
    "${common_flags[@]}" \
    $runtime_flags \
    -c "$GECODE_ROOT/gecode/list/list-var-imp.cpp" \
    -o "$list_object"

  "$CXX" \
    $runtime_flags \
    "$test_object" \
    "$list_object" \
    "$core_build/libdashed_core.a" \
    -L"$gecode_build" \
    -Wl,-rpath,"$gecode_build" \
    -lgecodeint \
    -lgecodesearch \
    -lgecodekernel \
    -lgecodesupport \
    -pthread \
    -o "$output"
}

run_binary() {
  local mode="$1"
  local binary="$2"

  if [[ "$mode" == *address* ]]; then
    ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1:halt_on_error=1:strict_string_checks=1:check_initialization_order=1}" \
    UBSAN_OPTIONS="${UBSAN_OPTIONS:-print_stacktrace=1:halt_on_error=1}" \
      "$binary"
  else
    UBSAN_OPTIONS="${UBSAN_OPTIONS:-print_stacktrace=1:halt_on_error=1}" \
      "$binary"
  fi
}

run_mode() {
  local mode="$1"

  case "$mode" in
    address|undefined|address,undefined)
      ;;
    *)
      echo "Unsupported sanitizer mode: $mode" >&2
      exit 1
      ;;
  esac

  local mode_id="${mode//,/-}"
  local core_build="$PROTOTYPE_ROOT/build-native-sanitizer-${compiler_id}-${mode_id}-core"
  local gecode_build="$GECODE_ROOT/build-list-sanitizer-${compiler_id}-${mode_id}"
  local runtime_sanitizer_flags="-fsanitize=$mode -fno-omit-frame-pointer -fno-sanitize-recover=all"
  local gecode_sanitizer_flags="$runtime_sanitizer_flags"
  local -a visibility_args=()

  if [[ ",$mode," == *",undefined,"* ]]; then
    gecode_sanitizer_flags+=" -fno-sanitize=vptr"
    visibility_args+=(
      -DHAVE_VISIBILITY_HIDDEN_FLAG=FALSE
      -DCMAKE_CXX_VISIBILITY_PRESET=default
      -DCMAKE_VISIBILITY_INLINES_HIDDEN=OFF
    )
  fi

  local compiler_flags="-DGECODE_HAS_LIST_VARS=1 -O1 -g $gecode_sanitizer_flags"
  local build_jobs
  build_jobs="$(build_jobs_for_mode "$mode")"

  echo
  echo "=================================================="
  echo "NATIVE LIST SANITIZER: $mode"
  echo "=================================================="
  echo "Compiler:     $CXX"
  echo "Core build:   $core_build"
  echo "Gecode build: $gecode_build"
  echo "Build jobs:   $build_jobs (requested: $JOBS)"
  echo "Binaries:     $BINARIES"

  if [[ ",$mode," == *",undefined,"* ]]; then
    verify_selective_vptr_policy \
      "$runtime_sanitizer_flags" \
      "$gecode_sanitizer_flags"
  fi

  remove_stale_cache "$core_build" "$PROTOTYPE_ROOT"
  remove_stale_cache "$gecode_build" "$GECODE_ROOT"

  cmake \
    -S "$PROTOTYPE_ROOT" \
    -B "$core_build" \
    -G "$CMAKE_GENERATOR" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_CXX_FLAGS="-O1 -g" \
    -DDASHED_BUILD_TESTS=OFF \
    -DDASHED_BUILD_BENCHMARKS=OFF \
    -DDASHED_ENABLE_SANITIZERS=ON \
    -DDASHED_SANITIZER_SET="$mode"

  build_cmake_with_oom_retry \
    "$core_build" \
    "$build_jobs" \
    --target dashed_core

  cmake \
    -S "$GECODE_ROOT" \
    -B "$gecode_build" \
    -G "$CMAKE_GENERATOR" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_CXX_FLAGS="$compiler_flags" \
    -DCMAKE_EXE_LINKER_FLAGS="$runtime_sanitizer_flags" \
    -DCMAKE_SHARED_LINKER_FLAGS="$runtime_sanitizer_flags" \
    -DCMAKE_MODULE_LINKER_FLAGS="$runtime_sanitizer_flags" \
    "${visibility_args[@]}" \
    -DGECODE_WITH_VIS=gecode/list/var-imp/list.vis \
    -DGECODE_ENABLE_QT=OFF \
    -DGECODE_ENABLE_GIST=OFF \
    -DGECODE_ENABLE_MPFR=OFF \
    -DGECODE_ENABLE_EXAMPLES=OFF \
    -DBUILD_TESTING=OFF \
    -DGECODE_ENABLE_SEARCH=ON \
    -DGECODE_ENABLE_INT_VARS=ON \
    -DGECODE_ENABLE_SET_VARS=OFF \
    -DGECODE_ENABLE_FLOAT_VARS=OFF \
    -DGECODE_ENABLE_MINIMODEL=OFF \
    -DGECODE_ENABLE_DRIVER=OFF \
    -DGECODE_ENABLE_FLATZINC=OFF

  build_cmake_with_oom_retry \
    "$gecode_build" \
    "$build_jobs"

  if [[ ",$mode," == *",undefined,"* ]]; then
    verify_ubsan_rtti_exports "$gecode_build"
  fi

  local requested
  for requested in $BINARIES; do
    case "$requested" in
      lifecycle)
        local lifecycle_binary="$gecode_build/list-native-sanitizer-lifecycle"
        compile_native_test \
          "$mode" \
          "$gecode_build" \
          "$core_build" \
          "$TEST_DIR/sanitizer-lifecycle.cpp" \
          "$lifecycle_binary"
        run_binary "$mode" "$lifecycle_binary"
        ;;
      regression)
        local regression_binary="$gecode_build/list-native-sanitizer-regression"
        compile_native_test \
          "$mode" \
          "$gecode_build" \
          "$core_build" \
          "$TEST_DIR/rel-test.cpp" \
          "$regression_binary"
        run_binary "$mode" "$regression_binary"
        ;;
      *)
        echo "Unknown native sanitizer binary: $requested" >&2
        exit 1
        ;;
    esac
  done
}

for mode in $SANITIZER_MODES; do
  run_mode "$mode"
done

printf '\nNative List sanitizer matrix passed: %s\n' "$SANITIZER_MODES"
