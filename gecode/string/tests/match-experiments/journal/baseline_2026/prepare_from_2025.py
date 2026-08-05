#!/usr/bin/env python3
"""Create/check baseline_2026 as a deterministic correction of baseline_2025.

This script never creates random instances. It copies the submitted instances and
applies only the two encoding corrections requested during review:
  * explicit alphabet/length constraints for every Latin-square row and column;
  * quantifier-free first-match predicates for the fixed-length DNA motifs.

Run from any directory; paths are resolved relative to this file.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
import sys
from pathlib import Path
from typing import Iterable

HERE = Path(__file__).resolve().parent
SUBMITTED = HERE.parent / "baseline_2025"
REVISED = HERE

BANNED_REVISED_FILES = (
    REVISED / "dna_match" / "gen_seq.py",
    REVISED / "latin_square" / "generate_smt.py",
    REVISED / "shortest_match" / "generate_instances.py",
)

DNA_QF_BLOCK = r'''; ---------- Fixed-length motifs ----------
; All motifs below accept words of one fixed length. For a motif of length m,
; an occurrence selected at 0-based index i is the first occurrence iff:
;   1. x[i .. i+m-1] matches the motif; and
;   2. the prefix ending immediately before the selected occurrence's final
;      character contains no complete occurrence of the motif.
; Any occurrence starting before i would end inside that prefix.

(define-const rho1 RegLan (str.to_re "CACGTG"))
(define-const rho2 RegLan
  (re.++ (re.union (str.to_re "A") (str.to_re "G"))
         (str.to_re "CGTG")))
(define-const rho3 RegLan
  (re.++
    (re.union (str.to_re "A") (str.to_re "G"))
    (re.union (str.to_re "C") (str.to_re "T"))
    (str.to_re "AAA")
    (re.union (str.to_re "A") (str.to_re "C") (str.to_re "T"))))

; i = -1 denotes no occurrence; otherwise i is the 0-based first-match index.
(define-fun match_at_index1 ((s String) (i Int)) Bool
  (ite (= i (- 1))
    (not (str.in_re s (re.++ re.all rho1 re.all)))
    (and
      (>= i 0)
      (<= (+ i 6) (str.len s))
      (str.in_re (str.substr s i 6) rho1)
      (not
        (str.in_re
          (str.substr s 0 (+ i 5))
          (re.++ re.all rho1 re.all))))))

(define-fun match_at_index2 ((s String) (i Int)) Bool
  (ite (= i (- 1))
    (not (str.in_re s (re.++ re.all rho2 re.all)))
    (and
      (>= i 0)
      (<= (+ i 5) (str.len s))
      (str.in_re (str.substr s i 5) rho2)
      (not
        (str.in_re
          (str.substr s 0 (+ i 4))
          (re.++ re.all rho2 re.all))))))

(define-fun match_at_index3 ((s String) (i Int)) Bool
  (ite (= i (- 1))
    (not (str.in_re s (re.++ re.all rho3 re.all)))
    (and
      (>= i 0)
      (<= (+ i 6) (str.len s))
      (str.in_re (str.substr s i 6) rho3)
      (not
        (str.in_re
          (str.substr s 0 (+ i 5))
          (re.++ re.all rho3 re.all))))))

'''


def sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1 << 20), b""):
            h.update(block)
    return h.hexdigest()


def copy_exact_tree(src: Path, dst: Path, patterns: Iterable[str]) -> None:
    dst.mkdir(parents=True, exist_ok=True)
    for pattern in patterns:
        for source in sorted(src.glob(pattern)):
            if source.is_file():
                target = dst / source.relative_to(src)
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(source, target)


def ensure_header(text: str) -> str:
    lines = text.splitlines()
    lines = [line for line in lines if not line.strip().startswith("(set-logic ")]
    lines = [line for line in lines if line.strip() != "(set-option :produce-models true)"]
    return "(set-logic QF_SLIA)\n(set-option :produce-models true)\n\n" + "\n".join(lines).lstrip() + "\n"


def fix_latin_text(text: str, n: int) -> str:
    if not 2 <= n <= 26:
        raise ValueError(f"unsupported Latin-square order: {n}")

    text = ensure_header(text)
    lines = text.splitlines()

    # Remove all existing row/column language and length assertions. They are
    # reinserted canonically for every declared row and column, making the
    # transformation idempotent and avoiding duplicate constraints.
    kept: list[str] = []
    membership = re.compile(r'^\s*\(assert \(str\.in_re (?:row|col)_\d+\b')
    length = re.compile(r'^\s*\(assert \(= \(str\.len (?:row|col)_\d+\)\s+\d+\)\)\s*$')
    for line in lines:
        if membership.search(line) or length.search(line):
            continue
        kept.append(line)

    declared = {
        (kind, int(idx))
        for kind, idx in re.findall(
            r'\(declare-fun\s+(row|col)_(\d+)\s+\(\)\s+String\)', "\n".join(kept)
        )
    }
    expected = {(kind, idx) for kind in ("row", "col") for idx in range(n)}
    if declared != expected:
        missing = sorted(expected - declared)
        extra = sorted(declared - expected)
        raise ValueError(f"unexpected Latin declarations for n={n}: missing={missing}, extra={extra}")

    insertion = 0
    for pos, line in enumerate(kept):
        if re.match(r'^\s*\(declare-fun\s+(?:row|col)_\d+\s+\(\)\s+String\)', line):
            insertion = pos + 1

    alphabet_end = chr(ord("A") + n - 1)
    constraints = [""]
    for idx in range(n):
        for kind in ("row", "col"):
            constraints.append(
                f'(assert (str.in_re {kind}_{idx} (re.* (re.range "A" "{alphabet_end}"))))'
            )
            constraints.append(f"(assert (= (str.len {kind}_{idx}) {n}))")
    constraints.append("")

    fixed = kept[:insertion] + constraints + kept[insertion:]
    return "\n".join(fixed).rstrip() + "\n"


def fix_dna_text(text: str) -> str:
    text = re.sub(r'\(set-logic\s+ALL\)', '(set-logic QF_SLIA)', text, count=1)
    start_markers = (
        "; ---------- match_at_index function ----------",
        "; ---------- Fixed-length motifs ----------",
    )
    start = next((text.find(marker) for marker in start_markers if text.find(marker) >= 0), -1)
    end_marker = "; ---------- Motif match positions ----------"
    end = text.find(end_marker)
    if start < 0 or end < 0 or end <= start:
        raise ValueError("DNA SMT file does not contain the expected section markers")
    fixed = text[:start] + DNA_QF_BLOCK + text[end:]
    if "forall" in fixed:
        raise ValueError("quantifier remained in corrected DNA encoding")
    return fixed.rstrip() + "\n"


def apply() -> dict[str, object]:
    if not SUBMITTED.is_dir():
        raise FileNotFoundError(f"missing submitted baseline: {SUBMITTED}")

    # Copy only immutable instance data. Models and runners in baseline_2026 are
    # maintained separately; no random instance generator is invoked.
    copy_exact_tree(
        SUBMITTED / "shortest_match",
        REVISED / "shortest_match",
        ("dzn/*.dzn", "smt/*.smt2"),
    )
    copy_exact_tree(
        SUBMITTED / "dna_match",
        REVISED / "dna_match",
        ("dzn/*.dzn",),
    )

    latin_src = SUBMITTED / "latin_square" / "smt"
    latin_dst = REVISED / "latin_square" / "smt"
    latin_dst.mkdir(parents=True, exist_ok=True)
    latin_files: list[str] = []
    for source in sorted(latin_src.glob("ls_*.smt2")):
        match = re.fullmatch(r"ls_(\d+)\.smt2", source.name)
        if not match:
            continue
        n = int(match.group(1))
        target = latin_dst / source.name
        target.write_text(fix_latin_text(source.read_text(encoding="utf-8"), n), encoding="utf-8")
        latin_files.append(source.name)

    dna_source = SUBMITTED / "dna_match" / "dna.smt2"
    dna_target = REVISED / "dna_match" / "dna.smt2"
    dna_target.parent.mkdir(parents=True, exist_ok=True)
    dna_target.write_text(fix_dna_text(dna_source.read_text(encoding="utf-8")), encoding="utf-8")

    return {
        "latin_instances": latin_files,
        "dna_source_sha256": sha256(dna_source),
        "dna_revised_sha256": sha256(dna_target),
    }


def compare_exact_dirs(left: Path, right: Path) -> list[str]:
    problems: list[str] = []
    left_files = {p.relative_to(left) for p in left.rglob("*") if p.is_file()}
    right_files = {p.relative_to(right) for p in right.rglob("*") if p.is_file()}
    for rel in sorted(left_files | right_files):
        lp, rp = left / rel, right / rel
        if not lp.exists():
            problems.append(f"extra revised file: {rel}")
        elif not rp.exists():
            problems.append(f"missing revised file: {rel}")
        elif sha256(lp) != sha256(rp):
            problems.append(f"content differs: {rel}")
    return problems


def check() -> dict[str, object]:
    errors: list[str] = []
    warnings: list[str] = []

    for path in BANNED_REVISED_FILES:
        if path.exists():
            errors.append(f"legacy instance generator must be removed: {path.relative_to(REVISED)}")

    errors.extend(
        f"shortest_match must be byte-identical: {problem}"
        for problem in compare_exact_dirs(
            SUBMITTED / "shortest_match" / "dzn", REVISED / "shortest_match" / "dzn"
        )
    )
    errors.extend(
        f"shortest_match must be byte-identical: {problem}"
        for problem in compare_exact_dirs(
            SUBMITTED / "shortest_match" / "smt", REVISED / "shortest_match" / "smt"
        )
    )
    errors.extend(
        f"DNA data must be byte-identical: {problem}"
        for problem in compare_exact_dirs(
            SUBMITTED / "dna_match" / "dzn", REVISED / "dna_match" / "dzn"
        )
    )

    for path in sorted((REVISED / "latin_square" / "smt").glob("ls_*.smt2")):
        match = re.fullmatch(r"ls_(\d+)\.smt2", path.name)
        if not match:
            continue
        n = int(match.group(1))
        text = path.read_text(encoding="utf-8")
        for idx in range(n):
            for kind in ("row", "col"):
                if f"(assert (= (str.len {kind}_{idx}) {n}))" not in text:
                    errors.append(f"{path.name}: missing length for {kind}_{idx}")
                membership = (
                    f'(assert (str.in_re {kind}_{idx} '
                    f'(re.* (re.range "A" "{chr(ord("A") + n - 1)}"))))'
                )
                if membership not in text:
                    errors.append(f"{path.name}: missing alphabet membership for {kind}_{idx}")

    dna_path = REVISED / "dna_match" / "dna.smt2"
    if not dna_path.exists():
        errors.append("missing dna_match/dna.smt2")
    else:
        dna = dna_path.read_text(encoding="utf-8")
        if "forall" in dna:
            errors.append("dna_match/dna.smt2 still contains forall")
        if "(set-logic QF_SLIA)" not in dna:
            warnings.append("dna_match/dna.smt2 does not declare QF_SLIA")

    report = {
        "ok": not errors,
        "errors": errors,
        "warnings": warnings,
        "submitted": str(SUBMITTED),
        "revised": str(REVISED),
    }
    return report


def main() -> int:
    parser = argparse.ArgumentParser()
    action = parser.add_mutually_exclusive_group(required=True)
    action.add_argument("--apply", action="store_true", help="copy submitted instances and apply deterministic fixes")
    action.add_argument("--check", action="store_true", help="verify revised instances and identity requirements")
    parser.add_argument("--json", action="store_true", help="emit machine-readable JSON")
    args = parser.parse_args()

    try:
        report = apply() if args.apply else check()
    except Exception as exc:  # noqa: BLE001 - command-line diagnostic
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2

    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    else:
        if args.apply:
            print(f"prepared revised instances from {SUBMITTED}")
            print(json.dumps(report, indent=2, sort_keys=True))
        else:
            for warning in report["warnings"]:
                print(f"WARNING: {warning}")
            for error in report["errors"]:
                print(f"ERROR: {error}")
            print("instance check: OK" if report["ok"] else "instance check: FAILED")
    return 0 if args.apply or report.get("ok", False) else 1


if __name__ == "__main__":
    raise SystemExit(main())
