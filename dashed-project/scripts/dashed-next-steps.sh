#!/usr/bin/env bash
# Dashed overnight continuation script.
#
# Scope:
#   1. Back up the current native Dashed integration.
#   2. Rebuild the low-memory Gecode/Dashed prerequisites with one job.
#   3. Add Gecode-style propagators for:
#        - equality and disequality
#        - equivalence-reified equality and disequality
#        - concatenation
#   4. Add a public aggregate header.
#   5. Compile and run a combined native runtime test.
#   6. Run the standalone Dashed tests.
#   7. Run an ASan/UBSan version of the combined native test.
#   8. Save logs, a patch, and a source archive.
#
# This script deliberately does NOT:
#   - git reset, checkout, clean, commit, or push;
#   - modify code outside gecode/dashed and the module header gecode/dashed.hh;
#   - enable parallel builds;
#   - implement half-reification (RM_IMP/RM_PMI);
#   - support aliased concat arguments yet.

set -Eeuo pipefail
IFS=$'\n\t'
umask 022

# Hard wall-clock cap. Override, for example, with DASHED_MAX_DURATION=4h.
if [[ "${DASHED_UNDER_TIMEOUT:-0}" != "1" ]] && command -v timeout >/dev/null 2>&1; then
  exec timeout --signal=TERM --kill-after=60s \
    "${DASHED_MAX_DURATION:-8h}" \
    env DASHED_UNDER_TIMEOUT=1 bash "$0" "$@"
fi

GECODE_SRC="${GECODE_SRC:-$HOME/src/gecode-main}"
DASHED_SRC="${DASHED_SRC:-$HOME/dashed-milestone1/dashed-refactor}"
GECODE_BUILD="${GECODE_BUILD:-$GECODE_SRC/build-listvar-native}"
CORE_BUILD="${CORE_BUILD:-$DASHED_SRC/build-native-core}"
ASAN_CORE_BUILD="${ASAN_CORE_BUILD:-$DASHED_SRC/build-nightly-asan-core}"
RUN_ROOT="${DASHED_RUN_ROOT:-$HOME/dashed-overnight-runs}"
STAMP="$(date +%Y%m%d-%H%M%S)"
RUN_DIR="$RUN_ROOT/$STAMP"
LOG="$RUN_DIR/overnight.log"
SRC_DIR="$RUN_DIR/generated-tests"
BIN_DIR="$RUN_DIR/bin"
BACKUP_DIR="$RUN_DIR/backup"

mkdir -p "$RUN_DIR" "$SRC_DIR" "$BIN_DIR" "$BACKUP_DIR"
exec > >(tee -a "$LOG") 2>&1

stage="initialization"
start_epoch="$(date +%s)"

on_error() {
  local code=$?
  echo
  echo "============================================================"
  echo "FAILED"
  echo "stage: $stage"
  echo "exit code: $code"
  echo "line: ${BASH_LINENO[0]:-unknown}"
  echo "log: $LOG"
  echo "============================================================"

  if [[ -d "$GECODE_SRC/.git" ]]; then
    git -C "$GECODE_SRC" status --short > "$RUN_DIR/git-status-after-failure.txt" || true
    git -C "$GECODE_SRC" diff -- gecode/dashed gecode/dashed.hh > "$RUN_DIR/dashed-after-failure.patch" || true
  fi
  if [[ -d "$GECODE_SRC/gecode/dashed" ]]; then
    if [[ -f "$GECODE_SRC/gecode/dashed.hh" ]]; then
      tar -C "$GECODE_SRC" -czf "$RUN_DIR/dashed-after-failure.tar.gz" gecode/dashed gecode/dashed.hh || true
    else
      tar -C "$GECODE_SRC" -czf "$RUN_DIR/dashed-after-failure.tar.gz" gecode/dashed || true
    fi
  fi
  exit "$code"
}
trap on_error ERR

say_stage() {
  stage="$1"
  echo
  echo "============================================================"
  echo "STAGE: $stage"
  echo "time: $(date --iso-8601=seconds)"
  echo "============================================================"
}

require_file() {
  if [[ ! -f "$1" ]]; then
    echo "Required file not found: $1" >&2
    exit 2
  fi
}

require_dir() {
  if [[ ! -d "$1" ]]; then
    echo "Required directory not found: $1" >&2
    exit 2
  fi
}

run_one_job_build() {
  CMAKE_BUILD_PARALLEL_LEVEL=1 cmake --build "$1" --parallel 1 "${@:2}"
}

say_stage "preflight"
require_dir "$GECODE_SRC"
require_dir "$DASHED_SRC"
require_file "$GECODE_SRC/CMakeLists.txt"
require_file "$DASHED_SRC/CMakeLists.txt"
require_file "$GECODE_SRC/gecode/dashed/var-imp/list.vis"
require_file "$GECODE_SRC/gecode/dashed/list-var-imp.hpp"
require_file "$GECODE_SRC/gecode/dashed/list-var-imp.cpp"
require_file "$GECODE_SRC/gecode/dashed/list-var.hpp"
require_file "$GECODE_SRC/gecode/dashed/list-view.hpp"
require_file "$GECODE_SRC/gecode/dashed/length.hpp"
require_file "$DASHED_SRC/integration/gecode/gecode/dashed/propagator-adapter.hpp"

for tool in cmake g++ ninja git tar; do
  command -v "$tool" >/dev/null || {
    echo "Required command not found: $tool" >&2
    exit 2
  }
done

echo "GECODE_SRC=$GECODE_SRC"
echo "DASHED_SRC=$DASHED_SRC"
echo "GECODE_BUILD=$GECODE_BUILD"
echo "CORE_BUILD=$CORE_BUILD"
echo "ASAN_CORE_BUILD=$ASAN_CORE_BUILD"
echo "RUN_DIR=$RUN_DIR"
echo
echo "Tool versions:"
cmake --version | head -n 1
g++ --version | head -n 1
ninja --version

echo
echo "Memory and disk before run:"
free -h || true
df -h "$HOME" || true

git -C "$GECODE_SRC" rev-parse HEAD > "$RUN_DIR/gecode-commit.txt"
git -C "$GECODE_SRC" status --short > "$RUN_DIR/git-status-before.txt"
git -C "$GECODE_SRC" diff > "$RUN_DIR/gecode-working-tree-before.patch"

say_stage "backup current Dashed integration"
cp -a "$GECODE_SRC/gecode/dashed" "$BACKUP_DIR/dashed-before"
if [[ -f "$GECODE_SRC/gecode/dashed.hh" ]]; then
  cp -a "$GECODE_SRC/gecode/dashed.hh" "$BACKUP_DIR/dashed.hh-before"
  tar -C "$GECODE_SRC" -czf "$BACKUP_DIR/dashed-before.tar.gz" gecode/dashed gecode/dashed.hh
else
  printf 'absent before run\n' > "$BACKUP_DIR/dashed.hh-was-absent.txt"
  tar -C "$GECODE_SRC" -czf "$BACKUP_DIR/dashed-before.tar.gz" gecode/dashed
fi

echo "Backup saved under: $BACKUP_DIR"

say_stage "verify required ListVar infrastructure patches"
# These checks ensure the previously validated checkpoint is present.
grep -q 'std::destroy_at' "$GECODE_SRC/gecode/dashed/list-var-imp.hpp" || {
  echo "list-var-imp.hpp does not contain the validated payload-only dispose()." >&2
  exit 3
}
grep -q 'Standard dependency interface expected by VarImpView' \
  "$GECODE_SRC/gecode/dashed/list-var-imp.hpp" || {
  echo "list-var-imp.hpp does not contain the validated subscribe/reschedule wrappers." >&2
  exit 3
}
grep -q 'VarImpView<ListVar>(y.varimp())' "$GECODE_SRC/gecode/dashed/list-view.hpp" || {
  echo "list-view.hpp does not contain the validated ListVar-to-view constructor." >&2
  exit 3
}

say_stage "reconfigure and rebuild minimal native Gecode libraries"
cmake -S "$GECODE_SRC" -B "$GECODE_BUILD" -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-DGECODE_HAS_DASHED_VARS=1" \
  -DGECODE_WITH_VIS=gecode/dashed/var-imp/list.vis \
  -DGECODE_ENABLE_QT=OFF \
  -DGECODE_ENABLE_GIST=OFF \
  -DGECODE_ENABLE_MPFR=OFF \
  -DGECODE_ENABLE_EXAMPLES=OFF \
  -DBUILD_TESTING=OFF \
  -DGECODE_ENABLE_SEARCH=OFF \
  -DGECODE_ENABLE_INT_VARS=ON \
  -DGECODE_ENABLE_SET_VARS=OFF \
  -DGECODE_ENABLE_FLOAT_VARS=OFF \
  -DGECODE_ENABLE_MINIMODEL=OFF \
  -DGECODE_ENABLE_DRIVER=OFF \
  -DGECODE_ENABLE_FLATZINC=OFF
run_one_job_build "$GECODE_BUILD"

require_file "$GECODE_BUILD/libgecodekernel.so"
require_file "$GECODE_BUILD/libgecodesupport.so"
require_file "$GECODE_BUILD/libgecodeint.so"

say_stage "rebuild standalone Dashed core"
cmake -S "$DASHED_SRC" -B "$CORE_BUILD" -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DDASHED_BUILD_TESTS=OFF \
  -DDASHED_BUILD_BENCHMARKS=OFF
run_one_job_build "$CORE_BUILD" --target dashed_core
require_file "$CORE_BUILD/libdashed_core.a"

say_stage "install tested propagation adapter"
ADAPTER_SOURCE="$DASHED_SRC/integration/gecode/gecode/dashed/propagator-adapter.hpp"
ADAPTER_DEST="$GECODE_SRC/gecode/dashed/propagator-adapter.hpp"

# On LP64 Linux, std::int64_t is typically long, while Gecode exposes
# IntView bound overloads for int and long long. Passing int64_t directly is
# therefore ambiguous. Select the wide overload explicitly in both the
# reusable integration source and the installed Gecode header.
python3 - "$ADAPTER_SOURCE" <<'PY_ADAPTER'
from pathlib import Path
import sys

path = Path(sys.argv[1])
text = path.read_text()
text = text.replace(
    "view.gq(home, bounds.lower)",
    "view.gq(home, static_cast<long long>(bounds.lower))")
text = text.replace(
    "view.lq(home, bounds.upper)",
    "view.lq(home, static_cast<long long>(bounds.upper))")
path.write_text(text)
PY_ADAPTER

cp "$ADAPTER_SOURCE" "$ADAPTER_DEST"

say_stage "write equality, disequality, and equivalence-reification propagators"
cat > "$GECODE_SRC/gecode/dashed/rel.hpp" <<'EOF_REL'
#ifndef __GECODE_DASHED_REL_HPP__
#define __GECODE_DASHED_REL_HPP__

#include <gecode/dashed/propagator-adapter.hpp>
#include <gecode/int.hh>

namespace Gecode { namespace Dashed {

class Equal final : public BinaryPropagator<ListView, PC_LIST_ANY> {
  using Base = BinaryPropagator<ListView, PC_LIST_ANY>;

protected:
  Equal(Home home, ListView x0, ListView x1)
      : Base(home, x0, x1) {}

  Equal(Space& home, Equal& other)
      : Base(home, other) {}

public:
  static ExecStatus post(Home home, ListView x0, ListView x1) {
    if (x0.varimp() == x1.varimp())
      return ES_OK;
    (void) new (home) Equal(home, x0, x1);
    return ES_OK;
  }

  Actor* copy(Space& home) override {
    return new (home) Equal(home, *this);
  }

  ExecStatus propagate(Space& home,
                       const ModEventDelta&) override {
    return Adapter::equal(home, *this, x0, x1);
  }
};

class NotEqual final : public BinaryPropagator<ListView, PC_LIST_ANY> {
  using Base = BinaryPropagator<ListView, PC_LIST_ANY>;

protected:
  NotEqual(Home home, ListView x0, ListView x1)
      : Base(home, x0, x1) {}

  NotEqual(Space& home, NotEqual& other)
      : Base(home, other) {}

public:
  static ExecStatus post(Home home, ListView x0, ListView x1) {
    if (x0.varimp() == x1.varimp())
      return ES_FAILED;
    (void) new (home) NotEqual(home, x0, x1);
    return ES_OK;
  }

  Actor* copy(Space& home) override {
    return new (home) NotEqual(home, *this);
  }

  ExecStatus propagate(Space& home,
                       const ModEventDelta&) override {
    return Adapter::not_equal(home, *this, x0, x1);
  }
};

class ReEqual final
    : public Int::ReBinaryPropagator<ListView, PC_LIST_ANY,
                                     Int::BoolView> {
  using Base = Int::ReBinaryPropagator<ListView, PC_LIST_ANY,
                                        Int::BoolView>;

protected:
  ReEqual(Home home, ListView x0, ListView x1, Int::BoolView b)
      : Base(home, x0, x1, b) {}

  ReEqual(Space& home, ReEqual& other)
      : Base(home, other) {}

public:
  static ExecStatus post(Home home, ListView x0, ListView x1,
                         Int::BoolView b) {
    if (x0.varimp() == x1.varimp())
      return me_failed(b.one(home)) ? ES_FAILED : ES_OK;
    (void) new (home) ReEqual(home, x0, x1, b);
    return ES_OK;
  }

  Actor* copy(Space& home) override {
    return new (home) ReEqual(home, *this);
  }

  ExecStatus propagate(Space& home,
                       const ModEventDelta&) override {
    return Adapter::reified_equal(home, *this, x0, x1, b);
  }
};

class ReNotEqual final
    : public Int::ReBinaryPropagator<ListView, PC_LIST_ANY,
                                     Int::BoolView> {
  using Base = Int::ReBinaryPropagator<ListView, PC_LIST_ANY,
                                        Int::BoolView>;

protected:
  ReNotEqual(Home home, ListView x0, ListView x1, Int::BoolView b)
      : Base(home, x0, x1, b) {}

  ReNotEqual(Space& home, ReNotEqual& other)
      : Base(home, other) {}

public:
  static ExecStatus post(Home home, ListView x0, ListView x1,
                         Int::BoolView b) {
    if (x0.varimp() == x1.varimp())
      return me_failed(b.zero(home)) ? ES_FAILED : ES_OK;
    (void) new (home) ReNotEqual(home, x0, x1, b);
    return ES_OK;
  }

  Actor* copy(Space& home) override {
    return new (home) ReNotEqual(home, *this);
  }

  ExecStatus propagate(Space& home,
                       const ModEventDelta&) override {
    return Adapter::reified_not_equal(home, *this, x0, x1, b);
  }
};

}} // namespace Gecode::Dashed

namespace Gecode {

inline void rel(Home home, ListVar x, IntRelType relation, ListVar y,
                IntPropLevel = IPL_DEF) {
  GECODE_POST;

  switch (relation) {
  case IRT_EQ:
    GECODE_ES_FAIL(Dashed::Equal::post(
        home, Dashed::ListView(x), Dashed::ListView(y)));
    break;
  case IRT_NQ:
    GECODE_ES_FAIL(Dashed::NotEqual::post(
        home, Dashed::ListView(x), Dashed::ListView(y)));
    break;
  default:
    throw Int::UnknownRelation("Dashed::rel");
  }
}

inline void rel(Home home, ListVar x, IntRelType relation, ListVar y,
                Reify reification, IntPropLevel = IPL_DEF) {
  GECODE_POST;

  // The standalone kernel currently implements equivalence reification.
  // Half-reification is intentionally rejected rather than approximated.
  if (reification.mode() != RM_EQV)
    throw Int::UnknownReifyMode("Dashed::rel");

  Int::BoolView b(reification.var());
  switch (relation) {
  case IRT_EQ:
    GECODE_ES_FAIL(Dashed::ReEqual::post(
        home, Dashed::ListView(x), Dashed::ListView(y), b));
    break;
  case IRT_NQ:
    GECODE_ES_FAIL(Dashed::ReNotEqual::post(
        home, Dashed::ListView(x), Dashed::ListView(y), b));
    break;
  default:
    throw Int::UnknownRelation("Dashed::rel");
  }
}

} // namespace Gecode

#endif
EOF_REL

say_stage "write concatenation propagator"
cat > "$GECODE_SRC/gecode/dashed/concat.hpp" <<'EOF_CONCAT'
#ifndef __GECODE_DASHED_CONCAT_HPP__
#define __GECODE_DASHED_CONCAT_HPP__

#include <gecode/dashed/propagator-adapter.hpp>
#include <gecode/int.hh>

namespace Gecode { namespace Dashed {

/// Propagator for z = x ++ y.
class Concat final : public TernaryPropagator<ListView, PC_LIST_ANY> {
  using Base = TernaryPropagator<ListView, PC_LIST_ANY>;

protected:
  // Stored order is z, x, y to match the standalone kernel adapter.
  Concat(Home home, ListView z, ListView x, ListView y)
      : Base(home, z, x, y) {}

  Concat(Space& home, Concat& other)
      : Base(home, other) {}

public:
  static ExecStatus post(Home home, ListView z,
                         ListView x, ListView y) {
    (void) new (home) Concat(home, z, x, y);
    return ES_OK;
  }

  Actor* copy(Space& home) override {
    return new (home) Concat(home, *this);
  }

  ExecStatus propagate(Space& home,
                       const ModEventDelta&) override {
    return Adapter::concat(home, *this, x0, x1, x2);
  }
};

}} // namespace Gecode::Dashed

namespace Gecode {

/// Post z = x ++ y.
inline void concat(Home home, ListVar x, ListVar y, ListVar z) {
  GECODE_POST;

  // Alias-safe transactional commits require a dedicated implementation.
  // Reject aliases explicitly for this first native milestone.
  if (x.varimp() == y.varimp() ||
      x.varimp() == z.varimp() ||
      y.varimp() == z.varimp())
    throw Int::ArgumentSame("Dashed::concat");

  GECODE_ES_FAIL(Dashed::Concat::post(
      home, Dashed::ListView(z),
      Dashed::ListView(x), Dashed::ListView(y)));
}

} // namespace Gecode

#endif
EOF_CONCAT

say_stage "write aggregate public header"
cat > "$GECODE_SRC/gecode/dashed.hh" <<'EOF_PUBLIC'
#ifndef __GECODE_DASHED_HH__
#define __GECODE_DASHED_HH__

#include <gecode/dashed/list-var.hpp>
#include <gecode/dashed/list-view.hpp>
#include <gecode/dashed/rel.hpp>
#include <gecode/dashed/concat.hpp>
#include <gecode/dashed/length.hpp>

#endif
EOF_PUBLIC

say_stage "syntax-check new public headers"
cat > "$SRC_DIR/header-syntax.cpp" <<'EOF_SYNTAX'
#include <gecode/dashed.hh>

void dashed_header_syntax(Gecode::Home home,
                          Gecode::ListVar x,
                          Gecode::ListVar y,
                          Gecode::ListVar z,
                          Gecode::BoolVar b,
                          Gecode::IntVar n) {
  Gecode::rel(home, x, Gecode::IRT_EQ, y);
  Gecode::rel(home, x, Gecode::IRT_NQ, y, Gecode::eqv(b));
  Gecode::concat(home, x, y, z);
  Gecode::length(home, z, n);
}
EOF_SYNTAX

g++ \
  -std=c++17 \
  -DGECODE_HAS_DASHED_VARS=1 \
  -I"$GECODE_BUILD" \
  -I"$GECODE_SRC" \
  -I"$DASHED_SRC/include" \
  -Wall -Wextra -Wpedantic -Werror \
  -Wno-unknown-pragmas -Wno-shadow -Wno-conversion \
  -fsyntax-only \
  "$SRC_DIR/header-syntax.cpp"

say_stage "write combined native runtime tests"
cat > "$SRC_DIR/native-constraints-test.cpp" <<'EOF_TEST'
#include <gecode/dashed.hh>

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

namespace {

using dashed::Domain;
using dashed::ValueSet;

class BinarySpace final : public Gecode::Space {
public:
  Gecode::ListVar x;
  Gecode::ListVar y;

  BinarySpace(Domain dx, Domain dy, Gecode::IntRelType relation)
      : Gecode::Space(),
        x(*this, std::move(dx)),
        y(*this, std::move(dy)) {
    Gecode::rel(*this, x, relation, y);
  }

  BinarySpace(BinarySpace& other)
      : Gecode::Space(other), x(), y() {
    x.update(*this, other.x);
    y.update(*this, other.y);
  }

  Gecode::Space* copy() override {
    return new BinarySpace(*this);
  }
};

class SelfBinarySpace final : public Gecode::Space {
public:
  Gecode::ListVar x;

  explicit SelfBinarySpace(Gecode::IntRelType relation)
      : Gecode::Space(),
        x(*this, Domain::top(ValueSet(1, 3), 0, 3)) {
    Gecode::rel(*this, x, relation, x);
  }

  SelfBinarySpace(SelfBinarySpace& other)
      : Gecode::Space(other), x() {
    x.update(*this, other.x);
  }

  Gecode::Space* copy() override {
    return new SelfBinarySpace(*this);
  }
};

class ReifiedSpace final : public Gecode::Space {
public:
  Gecode::ListVar x;
  Gecode::ListVar y;
  Gecode::BoolVar b;

  ReifiedSpace(Domain dx, Domain dy, Gecode::IntRelType relation,
               int bmin = 0, int bmax = 1,
               Gecode::ReifyMode mode = Gecode::RM_EQV)
      : Gecode::Space(),
        x(*this, std::move(dx)),
        y(*this, std::move(dy)),
        b(*this, bmin, bmax) {
    Gecode::rel(*this, x, relation, y,
                Gecode::Reify(b, mode));
  }

  ReifiedSpace(ReifiedSpace& other)
      : Gecode::Space(other), x(), y(), b() {
    x.update(*this, other.x);
    y.update(*this, other.y);
    b.update(*this, other.b);
  }

  Gecode::Space* copy() override {
    return new ReifiedSpace(*this);
  }
};

class ConcatSpace final : public Gecode::Space {
public:
  Gecode::ListVar x;
  Gecode::ListVar y;
  Gecode::ListVar z;

  ConcatSpace(Domain dx, Domain dy, Domain dz)
      : Gecode::Space(),
        x(*this, std::move(dx)),
        y(*this, std::move(dy)),
        z(*this, std::move(dz)) {
    Gecode::concat(*this, x, y, z);
  }

  ConcatSpace(ConcatSpace& other)
      : Gecode::Space(other), x(), y(), z() {
    x.update(*this, other.x);
    y.update(*this, other.y);
    z.update(*this, other.z);
  }

  Gecode::Space* copy() override {
    return new ConcatSpace(*this);
  }
};

class AliasConcatSpace final : public Gecode::Space {
public:
  Gecode::ListVar x;
  Gecode::ListVar z;

  AliasConcatSpace()
      : Gecode::Space(),
        x(*this, Domain::top(ValueSet(1, 2), 0, 2)),
        z(*this, Domain::top(ValueSet(1, 2), 0, 4)) {
    Gecode::concat(*this, x, x, z);
  }

  AliasConcatSpace(AliasConcatSpace& other)
      : Gecode::Space(other), x(), z() {
    x.update(*this, other.x);
    z.update(*this, other.z);
  }

  Gecode::Space* copy() override {
    return new AliasConcatSpace(*this);
  }
};

class LengthSpace final : public Gecode::Space {
public:
  Gecode::ListVar x;
  Gecode::IntVar n;

  LengthSpace(Domain domain, int nmin, int nmax)
      : Gecode::Space(),
        x(*this, std::move(domain)),
        n(*this, nmin, nmax) {
    Gecode::length(*this, x, n);
  }

  LengthSpace(LengthSpace& other)
      : Gecode::Space(other), x(), n() {
    x.update(*this, other.x);
    n.update(*this, other.n);
  }

  Gecode::Space* copy() override {
    return new LengthSpace(*this);
  }
};

void test_equality_assignment() {
  auto* s = new BinarySpace(
      Domain::top(ValueSet(1, 3), 2, 2),
      Domain::fixed(std::vector<int>{2, 3}),
      Gecode::IRT_EQ);

  assert(s->status() != Gecode::SS_FAILED);
  assert(s->x.assigned());
  assert(s->x.val() == std::vector<int>({2, 3}));
  delete s;
}

void test_equality_same_shape_intersection() {
  auto* s = new BinarySpace(
      Domain::repeat(ValueSet(1, 3), 2, 2),
      Domain::repeat(ValueSet(2, 4), 2, 2),
      Gecode::IRT_EQ);

  assert(s->status() != Gecode::SS_FAILED);
  assert(s->x.domain().accepts(Domain::fixed(std::vector<int>{2, 3})));
  assert(s->y.domain().accepts(Domain::fixed(std::vector<int>{3, 2})));
  assert(!s->x.domain().accepts(Domain::fixed(std::vector<int>{1, 1})));
  assert(!s->y.domain().accepts(Domain::fixed(std::vector<int>{4, 4})));
  delete s;
}

void test_equality_failure_and_aliases() {
  auto* failed = new BinarySpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::fixed(std::vector<int>{2}),
      Gecode::IRT_EQ);
  assert(failed->status() == Gecode::SS_FAILED);
  delete failed;

  auto* equal_self = new SelfBinarySpace(Gecode::IRT_EQ);
  assert(equal_self->status() != Gecode::SS_FAILED);
  delete equal_self;

  auto* unequal_self = new SelfBinarySpace(Gecode::IRT_NQ);
  assert(unequal_self->status() == Gecode::SS_FAILED);
  delete unequal_self;
}

void test_equality_clone_with_active_propagator() {
  auto* root = new BinarySpace(
      Domain::top(ValueSet(1, 3), 1, 3),
      Domain::top(ValueSet(1, 3), 1, 3),
      Gecode::IRT_EQ);

  assert(root->status() != Gecode::SS_FAILED);
  assert(!root->x.assigned());
  assert(!root->y.assigned());

  auto* clone = static_cast<BinarySpace*>(root->clone());
  Gecode::Dashed::ListView clone_y(clone->y);
  Gecode::ModEvent me = clone_y.replace(
      *clone, Domain::fixed(std::vector<int>{3, 2}));
  if (Gecode::me_failed(me))
    clone->fail();

  assert(clone->status() != Gecode::SS_FAILED);
  assert(clone->x.assigned());
  assert(clone->x.val() == std::vector<int>({3, 2}));

  assert(!root->x.assigned());
  assert(!root->y.assigned());

  delete root;
  delete clone;
}

void test_disequality() {
  auto* disjoint_lengths = new BinarySpace(
      Domain::top(ValueSet(1, 2), 1, 1),
      Domain::top(ValueSet(1, 2), 2, 2),
      Gecode::IRT_NQ);
  assert(disjoint_lengths->status() != Gecode::SS_FAILED);
  delete disjoint_lengths;

  auto* different = new BinarySpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::fixed(std::vector<int>{2}),
      Gecode::IRT_NQ);
  assert(different->status() != Gecode::SS_FAILED);
  delete different;

  auto* same = new BinarySpace(
      Domain::fixed(std::vector<int>{1, 2}),
      Domain::fixed(std::vector<int>{1, 2}),
      Gecode::IRT_NQ);
  assert(same->status() == Gecode::SS_FAILED);
  delete same;
}

void test_reified_equality() {
  auto* equal = new ReifiedSpace(
      Domain::fixed(std::vector<int>{1, 2}),
      Domain::fixed(std::vector<int>{1, 2}),
      Gecode::IRT_EQ);
  assert(equal->status() != Gecode::SS_FAILED);
  assert(equal->b.assigned() && equal->b.val() == 1);
  delete equal;

  auto* different = new ReifiedSpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::fixed(std::vector<int>{2}),
      Gecode::IRT_EQ);
  assert(different->status() != Gecode::SS_FAILED);
  assert(different->b.assigned() && different->b.val() == 0);
  delete different;

  auto* forced_true = new ReifiedSpace(
      Domain::top(ValueSet(1, 3), 2, 2),
      Domain::fixed(std::vector<int>{2, 3}),
      Gecode::IRT_EQ, 1, 1);
  assert(forced_true->status() != Gecode::SS_FAILED);
  assert(forced_true->x.assigned());
  assert(forced_true->x.val() == std::vector<int>({2, 3}));
  delete forced_true;

  auto* contradictory_false = new ReifiedSpace(
      Domain::fixed(std::vector<int>{2}),
      Domain::fixed(std::vector<int>{2}),
      Gecode::IRT_EQ, 0, 0);
  assert(contradictory_false->status() == Gecode::SS_FAILED);
  delete contradictory_false;
}

void test_reified_disequality() {
  auto* equal = new ReifiedSpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::fixed(std::vector<int>{1}),
      Gecode::IRT_NQ);
  assert(equal->status() != Gecode::SS_FAILED);
  assert(equal->b.assigned() && equal->b.val() == 0);
  delete equal;

  auto* different = new ReifiedSpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::fixed(std::vector<int>{2}),
      Gecode::IRT_NQ);
  assert(different->status() != Gecode::SS_FAILED);
  assert(different->b.assigned() && different->b.val() == 1);
  delete different;
}

void test_half_reification_rejected() {
  bool caught = false;
  try {
    auto* unsupported = new ReifiedSpace(
        Domain::top(ValueSet(1, 2), 0, 2),
        Domain::top(ValueSet(1, 2), 0, 2),
        Gecode::IRT_EQ, 0, 1, Gecode::RM_IMP);
    delete unsupported;
  } catch (const Gecode::Int::UnknownReifyMode&) {
    caught = true;
  }
  assert(caught);
}

void test_concat_forward() {
  auto* s = new ConcatSpace(
      Domain::fixed(std::vector<int>{1, 2}),
      Domain::fixed(std::vector<int>{3}),
      Domain::top(ValueSet(1, 3), 0, 5));

  assert(s->status() != Gecode::SS_FAILED);
  assert(s->z.assigned());
  assert(s->z.val() == std::vector<int>({1, 2, 3}));
  delete s;
}

void test_concat_backward_suffix() {
  auto* s = new ConcatSpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::top(ValueSet(1, 4), 0, 4),
      Domain::fixed(std::vector<int>{1, 2, 3}));

  assert(s->status() != Gecode::SS_FAILED);
  assert(s->y.assigned());
  assert(s->y.val() == std::vector<int>({2, 3}));
  delete s;
}

void test_concat_lengths_and_failure() {
  auto* lengths = new ConcatSpace(
      Domain::top(ValueSet(1, 2), 1, 2),
      Domain::top(ValueSet(1, 2), 2, 3),
      Domain::top(ValueSet(1, 2), 0, 10));

  assert(lengths->status() != Gecode::SS_FAILED);
  assert(lengths->z.min_length() == 3);
  assert(lengths->z.max_length() == 5);
  delete lengths;

  auto* failed = new ConcatSpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::fixed(std::vector<int>{2}),
      Domain::fixed(std::vector<int>{1, 3}));
  assert(failed->status() == Gecode::SS_FAILED);
  delete failed;
}

void test_concat_alias_rejected() {
  bool caught = false;
  try {
    auto* unsupported = new AliasConcatSpace();
    delete unsupported;
  } catch (const Gecode::Int::ArgumentSame&) {
    caught = true;
  }
  assert(caught);
}

void test_length_still_works() {
  auto* s = new LengthSpace(
      Domain::top(ValueSet(1, 3), 1, 5), 2, 4);

  assert(s->status() != Gecode::SS_FAILED);
  assert(s->x.min_length() == 2);
  assert(s->x.max_length() == 4);
  assert(s->n.min() == 2);
  assert(s->n.max() == 4);
  delete s;
}

} // namespace

int main() {
  test_equality_assignment();
  test_equality_same_shape_intersection();
  test_equality_failure_and_aliases();
  test_equality_clone_with_active_propagator();
  test_disequality();
  test_reified_equality();
  test_reified_disequality();
  test_half_reification_rejected();
  test_concat_forward();
  test_concat_backward_suffix();
  test_concat_lengths_and_failure();
  test_concat_alias_rejected();
  test_length_still_works();

  std::cout << "Dashed native constraint tests passed\n";
  return 0;
}
EOF_TEST

COMMON_FLAGS=(
  -std=c++17
  -DGECODE_HAS_DASHED_VARS=1
  -DGECODE_NO_AUTOLINK
  -I"$GECODE_BUILD"
  -I"$GECODE_SRC"
  -I"$DASHED_SRC/include"
  -Wall -Wextra -Wpedantic -Werror
  -Wno-unknown-pragmas -Wno-shadow -Wno-conversion
)
COMMON_LINK=(
  "$GECODE_SRC/gecode/dashed/list-var-imp.cpp"
  "$CORE_BUILD/libdashed_core.a"
  -L"$GECODE_BUILD"
  -Wl,-rpath,"$GECODE_BUILD"
  -lgecodeint
  -lgecodekernel
  -lgecodesupport
  -pthread
)

say_stage "compile combined native runtime tests"
g++ "${COMMON_FLAGS[@]}" \
  -O0 -g \
  "$SRC_DIR/native-constraints-test.cpp" \
  "${COMMON_LINK[@]}" \
  -o "$BIN_DIR/native-constraints-test"

say_stage "run combined native runtime tests"
"$BIN_DIR/native-constraints-test"

say_stage "run standalone Dashed regression tests"
# Reuse the already configured test build when available; otherwise configure it.
if [[ ! -f "$DASHED_SRC/build/CMakeCache.txt" ]]; then
  cmake -S "$DASHED_SRC" -B "$DASHED_SRC/build" -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug \
    -DDASHED_ENABLE_SANITIZERS=ON
fi
run_one_job_build "$DASHED_SRC/build"
ASAN_OPTIONS=detect_leaks=1 \
UBSAN_OPTIONS=print_stacktrace=1 \
ctest --test-dir "$DASHED_SRC/build" --output-on-failure

say_stage "build sanitizer-instrumented standalone core"
cmake -S "$DASHED_SRC" -B "$ASAN_CORE_BUILD" -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DDASHED_ENABLE_SANITIZERS=ON \
  -DDASHED_BUILD_TESTS=OFF \
  -DDASHED_BUILD_BENCHMARKS=OFF
run_one_job_build "$ASAN_CORE_BUILD" --target dashed_core
require_file "$ASAN_CORE_BUILD/libdashed_core.a"

say_stage "compile and run ASan/UBSan native constraint test"
g++ "${COMMON_FLAGS[@]}" \
  -O1 -g \
  -fsanitize=address,undefined \
  -fno-omit-frame-pointer \
  "$SRC_DIR/native-constraints-test.cpp" \
  "$GECODE_SRC/gecode/dashed/list-var-imp.cpp" \
  "$ASAN_CORE_BUILD/libdashed_core.a" \
  -L"$GECODE_BUILD" \
  -Wl,-rpath,"$GECODE_BUILD" \
  -lgecodeint -lgecodekernel -lgecodesupport -pthread \
  -fsanitize=address,undefined \
  -o "$BIN_DIR/native-constraints-test-asan"

# LeakSanitizer can report process-global allocations from a non-instrumented
# third-party library. The standalone core tests above retain leak detection;
# this mixed-library run focuses on address and undefined-behavior errors.
ASAN_OPTIONS=detect_leaks=0:halt_on_error=1 \
UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1 \
"$BIN_DIR/native-constraints-test-asan"

say_stage "collect results"
git -C "$GECODE_SRC" status --short > "$RUN_DIR/git-status-after.txt"
git -C "$GECODE_SRC" diff -- gecode/dashed gecode/dashed.hh > "$RUN_DIR/dashed.patch"
tar -C "$GECODE_SRC" -czf "$RUN_DIR/dashed-after.tar.gz" gecode/dashed gecode/dashed.hh

cat > "$RUN_DIR/README.txt" <<EOF_SUMMARY
Dashed overnight run completed successfully.

Run directory:
  $RUN_DIR

Gecode source:
  $GECODE_SRC

Standalone Dashed source:
  $DASHED_SRC

Generated/updated integration files:
  gecode/dashed/propagator-adapter.hpp
  gecode/dashed/rel.hpp
  gecode/dashed/concat.hpp
  gecode/dashed.hh

Validated constraints:
  equality
  disequality
  equivalence-reified equality
  equivalence-reified disequality
  concatenation (non-aliased variables)
  length regression

Intentional boundaries:
  RM_IMP and RM_PMI are rejected.
  concat aliases are rejected.
  No git commit or push was performed.

Useful artifacts:
  overnight.log
  dashed.patch
  dashed-after.tar.gz
  backup/dashed-before.tar.gz
  git-status-before.txt
  git-status-after.txt
EOF_SUMMARY

elapsed=$(( $(date +%s) - start_epoch ))
echo
cat "$RUN_DIR/README.txt"
echo "Elapsed seconds: $elapsed"
echo "Log: $LOG"
echo
free -h || true
df -h "$HOME" || true

echo
printf 'SUCCESS: all unattended stages passed.\n'
