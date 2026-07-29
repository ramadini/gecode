#!/usr/bin/env bash

set -Eeuo pipefail

usage() {
  cat <<'USAGE'
Usage:
  run-3-step-bundle.sh [--push] BUNDLE_FILE

Without --push:
  Runs all checkpoints and tests, then creates a local commit.

With --push:
  Pushes only after every checkpoint and final test passes.

The bundle file must define:

  BUNDLE_NAME
  EXPECTED_BRANCH
  COMMIT_MESSAGE
  ALLOWED_PATHS=(...)
  STAGE_PATHS=(...)

  step_1
  test_1
  step_2
  test_2
  step_3
  test_3
  final_tests
USAGE
}

PUSH_ENABLED=0

if [[ "${1:-}" == "--push" ]]; then
  PUSH_ENABLED=1
  shift
fi

if [[ "$#" -ne 1 ]]; then
  usage
  exit 2
fi

BUNDLE_INPUT="$1"

if [[ ! -f "$BUNDLE_INPUT" ]]; then
  echo "Bundle file not found: $BUNDLE_INPUT" >&2
  exit 2
fi

REPO_ROOT="$(
  git rev-parse --show-toplevel
)"

cd "$REPO_ROOT"

BUNDLE_PATH="$(
  realpath "$BUNDLE_INPUT"
)"

# shellcheck source=/dev/null
source "$BUNDLE_PATH"

: "${BUNDLE_NAME:?Bundle must define BUNDLE_NAME}"
: "${EXPECTED_BRANCH:?Bundle must define EXPECTED_BRANCH}"
: "${COMMIT_MESSAGE:?Bundle must define COMMIT_MESSAGE}"

if [[ "$BUNDLE_NAME" == "replace-with-bundle-name" ]]; then
  echo "The bundle template cannot be executed directly." >&2
  echo "Copy it to a new bundle file and implement its three steps." >&2
  exit 2
fi

if ! declare -p ALLOWED_PATHS >/dev/null 2>&1; then
  echo "Bundle must define ALLOWED_PATHS" >&2
  exit 2
fi

if ! declare -p STAGE_PATHS >/dev/null 2>&1; then
  echo "Bundle must define STAGE_PATHS" >&2
  exit 2
fi

for function_name in \
  step_1 test_1 \
  step_2 test_2 \
  step_3 test_3 \
  final_tests
do
  if ! declare -F "$function_name" >/dev/null; then
    echo "Bundle is missing function: $function_name" >&2
    exit 2
  fi
done

JOBS="${JOBS:-$(nproc)}"
export JOBS
export REPO_ROOT

timestamp="$(
  date -u +%Y%m%dT%H%M%SZ
)"

safe_name="$(
  printf '%s' "$BUNDLE_NAME" |
    tr -cs 'A-Za-z0-9._-' '-'
)"

LOG_DIR="$REPO_ROOT/dashed-project/runs/bundles/${timestamp}-${safe_name}"
mkdir -p "$LOG_DIR"

LOG_FILE="$LOG_DIR/run.log"

exec > >(
  tee -a "$LOG_FILE"
) 2>&1

echo "Bundle:          $BUNDLE_NAME"
echo "Repository:      $REPO_ROOT"
echo "Expected branch: $EXPECTED_BRANCH"
echo "Build jobs:      $JOBS"
echo "Push enabled:    $PUSH_ENABLED"
echo "Log:             $LOG_FILE"
echo

current_branch="$(
  git branch --show-current
)"

if [[ "$current_branch" != "$EXPECTED_BRANCH" ]]; then
  echo "Wrong branch." >&2
  echo "Current:  $current_branch" >&2
  echo "Expected: $EXPECTED_BRANCH" >&2
  exit 1
fi

if [[ -n "$(git status --porcelain)" ]]; then
  echo "Working tree must be clean before starting." >&2
  git status --short
  exit 1
fi

START_HEAD="$(
  git rev-parse HEAD
)"

echo "Starting commit: $START_HEAD"

if [[ "$PUSH_ENABLED" -eq 1 ]]; then
  echo
  echo "Checking remote starting point..."

  git fetch origin "$EXPECTED_BRANCH"

  remote_head="$(
    git rev-parse "origin/$EXPECTED_BRANCH"
  )"

  if [[ "$remote_head" != "$START_HEAD" ]]; then
    echo "Local branch is not aligned with origin." >&2
    echo "Local:  $START_HEAD" >&2
    echo "Remote: $remote_head" >&2
    exit 1
  fi
fi

save_failure_state() {
  local status="$1"
  local line="$2"
  local command="$3"

  set +e

  echo
  echo "=================================================="
  echo "BUNDLE FAILED"
  echo "=================================================="
  echo "Exit status: $status"
  echo "Line:        $line"
  echo "Command:     $command"
  echo
  echo "No commit or push was attempted after this failure."
  echo

  git status --short

  git diff > "$LOG_DIR/failure.patch"
  git diff --cached > "$LOG_DIR/failure-staged.patch"

  echo
  echo "Failure log:"
  echo "  $LOG_FILE"
  echo
  echo "Unstaged patch:"
  echo "  $LOG_DIR/failure.patch"
  echo
  echo "Staged patch:"
  echo "  $LOG_DIR/failure-staged.patch"
}

on_error() {
  local status="$?"
  local line="${BASH_LINENO[0]:-unknown}"
  local command="${BASH_COMMAND:-unknown}"

  trap - ERR
  save_failure_state "$status" "$line" "$command"
  exit "$status"
}

trap on_error ERR

run_checkpoint() {
  local number="$1"
  local step_function="step_${number}"
  local test_function="test_${number}"

  echo
  echo "=================================================="
  echo "CHECKPOINT $number: APPLY"
  echo "=================================================="

  "$step_function"

  echo
  echo "=================================================="
  echo "CHECKPOINT $number: FOCUSED TEST"
  echo "=================================================="

  "$test_function"

  echo
  echo "Checkpoint $number passed."
}

path_is_allowed() {
  local path="$1"
  local rule

  for rule in "${ALLOWED_PATHS[@]}"; do
    if [[ "$rule" == */ ]]; then
      if [[ "$path" == "$rule"* ]]; then
        return 0
      fi
    elif [[ "$path" == "$rule" ]]; then
      return 0
    fi
  done

  return 1
}

validate_changed_paths() {
  local -a changed_paths=()
  local path

  mapfile -t changed_paths < <(
    {
      git diff --name-only
      git diff --cached --name-only
      git ls-files --others --exclude-standard
    } |
      sed '/^$/d' |
      sort -u
  )

  if [[ "${#changed_paths[@]}" -eq 0 ]]; then
    echo "Bundle produced no source changes." >&2
    return 1
  fi

  for path in "${changed_paths[@]}"; do
    if ! path_is_allowed "$path"; then
      echo "Unexpected changed path: $path" >&2
      return 1
    fi
  done

  echo "Changed paths are within the bundle allowlist."
}

run_checkpoint 1
run_checkpoint 2
run_checkpoint 3

echo
echo "=================================================="
echo "FINAL REGRESSION MATRIX"
echo "=================================================="

final_tests

echo
echo "=================================================="
echo "SOURCE VALIDATION"
echo "=================================================="

git diff --check
validate_changed_paths

git add -- "${STAGE_PATHS[@]}"

if git diff --cached --quiet; then
  echo "Nothing was staged for commit." >&2
  exit 1
fi

if ! git diff --quiet; then
  echo "Unstaged tracked changes remain after staging." >&2
  git status --short
  exit 1
fi

mapfile -t untracked_paths < <(
  git ls-files --others --exclude-standard
)

if [[ "${#untracked_paths[@]}" -ne 0 ]]; then
  echo "Untracked source files remain after staging:" >&2
  printf '  %s\n' "${untracked_paths[@]}" >&2
  exit 1
fi

echo
echo "Staged changes:"
git diff --cached --stat

echo
echo "Creating local commit..."

git commit -m "$COMMIT_MESSAGE"

NEW_HEAD="$(
  git rev-parse HEAD
)"

echo "Created commit: $NEW_HEAD"

if [[ "$PUSH_ENABLED" -eq 1 ]]; then
  echo
  echo "=================================================="
  echo "REMOTE RACE CHECK"
  echo "=================================================="

  git fetch origin "$EXPECTED_BRANCH"

  remote_head="$(
    git rev-parse "origin/$EXPECTED_BRANCH"
  )"

  if [[ "$remote_head" != "$START_HEAD" ]]; then
    echo "Remote branch changed while the bundle was running." >&2
    echo "Original remote: $START_HEAD" >&2
    echo "Current remote:  $remote_head" >&2
    echo
    echo "The local commit is preserved, but nothing was pushed."
    exit 1
  fi

  echo
  echo "=================================================="
  echo "PUSH"
  echo "=================================================="

  git push origin "HEAD:$EXPECTED_BRANCH"

  echo "Push completed."
else
  echo
  echo "Push disabled. The commit exists locally only."
  echo "Review it, then push manually or rerun future bundles with --push."
fi

echo
echo "=================================================="
echo "BUNDLE PASSED"
echo "=================================================="

git status --short
git log -1 --oneline
