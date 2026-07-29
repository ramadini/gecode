#!/usr/bin/env bash

set -euo pipefail

REPO_ROOT="$(git rev-parse --show-toplevel)"
JOBS="${JOBS:-$(nproc)}"
LEGACY_PATH="gecode/string/tests/str_test2.cpp"
CACHE_ROOT="${GSTRINGS_LIVE_CACHE:-$REPO_ROOT/dashed-project/runs/gstrings-live}"
CACHE_FORMAT="v3-minimodel-runtime-diagnostics"

resolve_revision() {
  local revision

  if [[ -n "${GSTRINGS_LEGACY_REVISION:-}" ]]; then
    if git cat-file -e "${GSTRINGS_LEGACY_REVISION}:${LEGACY_PATH}" 2>/dev/null; then
      git rev-parse "${GSTRINGS_LEGACY_REVISION}^{commit}"
      return 0
    fi
    echo "GSTRINGS_LEGACY_REVISION does not contain $LEGACY_PATH" >&2
    return 1
  fi

  while IFS= read -r revision; do
    if git cat-file -e "${revision}:${LEGACY_PATH}" 2>/dev/null &&
       git cat-file -e "${revision}:gecode/string.hh" 2>/dev/null; then
      printf '%s\n' "$revision"
      return 0
    fi
  done < <(git rev-list --all -- "$LEGACY_PATH")

  for remote_branch in origin/master origin/main upstream/master upstream/main; do
    if git rev-parse --verify --quiet "$remote_branch" >/dev/null &&
       git cat-file -e "${remote_branch}:${LEGACY_PATH}" 2>/dev/null; then
      git rev-parse "${remote_branch}^{commit}"
      return 0
    fi
  done

  for branch in master main; do
    if git fetch --quiet origin "$branch" 2>/dev/null &&
       git cat-file -e "FETCH_HEAD:${LEGACY_PATH}" 2>/dev/null; then
      git rev-parse "FETCH_HEAD^{commit}"
      return 0
    fi
  done

  echo "could not locate a historical G-Strings revision containing $LEGACY_PATH" >&2
  return 1
}

find_historical_library() {
  local build_dir="$1"
  local stem="$2"
  local candidate

  if [[ -e "$build_dir/$stem.so" ]]; then
    printf '%s\n' "$build_dir/$stem.so"
    return 0
  fi

  for candidate in "$build_dir/$stem.so."*; do
    if [[ -e "$candidate" ]]; then
      printf '%s\n' "$candidate"
      return 0
    fi
  done

  echo "missing historical library $stem under $build_dir" >&2
  return 1
}

if [[ "${1:-}" == "--resolve-revision" ]]; then
  resolve_revision
  exit 0
fi

revision="$(resolve_revision)"
short_revision="${revision:0:12}"
root="$CACHE_ROOT/$short_revision"
source_dir="$root/source"
build_dir="$root/build"
bin_dir="$root/bin"
legacy_binary="$bin_dir/str_test2"
runner="$bin_dir/gstrings-live-runner"
log_file="$root/build.log"
cache_stamp="$root/$CACHE_FORMAT"

mkdir -p "$root"

if [[ ! -f "$source_dir/$LEGACY_PATH" ]]; then
  rm -rf "$source_dir" "$build_dir" "$bin_dir"
  mkdir -p "$source_dir"
  git archive "$revision" | tar -x -C "$source_dir"
fi

# Keep a successfully compiled historical library tree, but invalidate the
# executable whenever the link/runtime dependency format changes.
if [[ ! -f "$cache_stamp" ]]; then
  rm -rf "$bin_dir"
fi
mkdir -p "$build_dir" "$bin_dir"

if [[ ! -x "$legacy_binary" ]]; then
  : > "$log_file"
  {
    echo "Historical revision: $revision"
    echo "Source: $source_dir"
    echo "Build: $build_dir"
    echo "Cache format: $CACHE_FORMAT"

    if [[ ! -f "$build_dir/Makefile" ]]; then
      (
        cd "$build_dir"
        env \
          CPATH= \
          CPLUS_INCLUDE_PATH= \
          LIBRARY_PATH= \
          PKG_CONFIG_PATH= \
          "$source_dir/configure" \
            --prefix="$root/private-prefix" \
            --disable-examples \
            --disable-qt \
            --disable-flatzinc
      )
    fi

    # Usually this is now incremental and prints "nothing to be done". It is
    # retained so a missing minimodel or float library is built automatically.
    env \
      CPATH= \
      CPLUS_INCLUDE_PATH= \
      LIBRARY_PATH= \
      PKG_CONFIG_PATH= \
      make -C "$build_dir" -j "$JOBS" compilelib

    support_library="$(find_historical_library "$build_dir" libgecodesupport)"
    kernel_library="$(find_historical_library "$build_dir" libgecodekernel)"
    search_library="$(find_historical_library "$build_dir" libgecodesearch)"
    int_library="$(find_historical_library "$build_dir" libgecodeint)"
    set_library="$(find_historical_library "$build_dir" libgecodeset)"
    float_library="$(find_historical_library "$build_dir" libgecodefloat)"
    minimodel_library="$(find_historical_library "$build_dir" libgecodeminimodel)"
    string_library="$(find_historical_library "$build_dir" libgecodestring)"

    printf 'Using historical libraries:\n'
    printf '  %s\n' \
      "$string_library" \
      "$minimodel_library" \
      "$set_library" \
      "$float_library" \
      "$int_library" \
      "$search_library" \
      "$kernel_library" \
      "$support_library"

    cxx="${CXX:-c++}"
    env \
      CPATH= \
      CPLUS_INCLUDE_PATH= \
      LIBRARY_PATH= \
      PKG_CONFIG_PATH= \
      "$cxx" \
        -std=c++11 \
        -I"$build_dir" \
        -I"$source_dir" \
        "$source_dir/$LEGACY_PATH" \
        -Wl,-rpath,"$build_dir" \
        -Wl,-rpath-link,"$build_dir" \
        -Wl,--no-as-needed \
        "$string_library" \
        "$minimodel_library" \
        "$set_library" \
        "$float_library" \
        "$int_library" \
        "$search_library" \
        "$kernel_library" \
        "$support_library" \
        -pthread \
        -ldl \
        -o "$legacy_binary"

    echo "Runtime dependency check:"
    env LD_LIBRARY_PATH="$build_dir" ldd "$legacy_binary"

    touch "$cache_stamp"
  } >> "$log_file" 2>&1 || {
    echo "historical G-Strings build/link failed; see $log_file" >&2
    grep -n -m 30 -E '(^|: )(fatal )?error:|undefined reference|collect2: error|not found' \
      "$log_file" >&2 || true
    tail -120 "$log_file" >&2 || true
    exit 1
  }
fi

cat > "$runner" <<RUNNER_EOF
#!/usr/bin/env bash
set -euo pipefail
if [[ "\${1:-}" != "--report" || "\$#" -ne 2 ]]; then
  echo "usage: \$0 --report PATH" >&2
  exit 2
fi
report="\$2"
run_log="\$(mktemp)"
cleanup() { rm -f "\$run_log"; }
trap cleanup EXIT
set +e
LD_LIBRARY_PATH="$build_dir\${LD_LIBRARY_PATH:+:\$LD_LIBRARY_PATH}" \
  "$legacy_binary" >"\$run_log" 2>&1
status="\$?"
set -e
if [[ "\$status" -ne 0 ]]; then
  echo "historical str_test2 exited with status \$status" >&2
  cat "\$run_log" >&2 || true
  echo "runtime linkage for $legacy_binary:" >&2
  LD_LIBRARY_PATH="$build_dir\${LD_LIBRARY_PATH:+:\$LD_LIBRARY_PATH}" \
    ldd "$legacy_binary" >&2 || true
  exit "\$status"
fi
python3 \
  "$REPO_ROOT/dashed-project/prototype/tools/emit_live_gstrings_report.py" \
  --log "\$run_log" \
  --expected "$REPO_ROOT/dashed-project/prototype/tests/gstrings/expected-equality.tsv" \
  --output "\$report"
RUNNER_EOF
chmod +x "$runner"

printf '%s\n' "$runner"
