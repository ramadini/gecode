#!/usr/bin/env python3

import math
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent
SOLVED = {"sat", "opt"}


def instance_id(family: str, raw: str) -> str:
    name = Path(raw).name
    for suffix in (".smt2", ".dzn"):
        if name.endswith(suffix):
            name = name[: -len(suffix)]
    if family == "latin_square" and name.isdigit():
        return f"ls_{name}"
    return name


def read_rows(path: Path, family: str, objective: bool):
    rows = []
    if not path.exists():
        return rows
    for line_no, line in enumerate(path.read_text().splitlines(), 1):
        if not line.strip():
            continue
        fields = line.split("|")
        minimum = 5 if objective else 4
        if len(fields) < minimum:
            print(f"warning: malformed row {path}:{line_no}: {line}", file=sys.stderr)
            continue
        solver, raw_instance, status = fields[:3]
        value = None
        if objective:
            try:
                value = float(fields[3])
                if math.isnan(value):
                    value = None
            except ValueError:
                value = None
        rows.append((instance_id(family, raw_instance), solver, status, value))
    return rows


def check_group(label: str, rows, compare_objective: bool) -> int:
    grouped = defaultdict(list)
    for instance, solver, status, value in rows:
        grouped[instance].append((solver, status, value))

    errors = 0
    for instance, results in sorted(grouped.items()):
        solved = [(s, st, v) for s, st, v in results if st in SOLVED]
        unsat = [(s, st, v) for s, st, v in results if st == "uns"]
        if solved and unsat:
            print(f"ERROR {label}/{instance}: SAT/UNSAT disagreement: {results}")
            errors += 1

        if compare_objective:
            values = [
                (solver, value)
                for solver, status, value in results
                if status == "opt" and value is not None
            ]
            distinct = sorted({v for _, v in values})
            if len(distinct) > 1:
                print(f"ERROR {label}/{instance}: objective disagreement: {values}")
                errors += 1
    return errors


def main() -> int:
    errors = 0

    latin = []
    latin += read_rows(ROOT / "latin_square/results_gstrings_ls.log", "latin_square", False)
    latin += read_rows(ROOT / "latin_square/results_smt_ls.log", "latin_square", False)
    errors += check_group("latin_square", latin, False)

    shortest = []
    shortest += read_rows(ROOT / "shortest_match/results_gstrings_sm.log", "shortest_match", True)
    shortest += read_rows(ROOT / "shortest_match/results_smt_sm.log", "shortest_match", True)
    errors += check_group("shortest_match", shortest, True)

    dna_gstrings = read_rows(ROOT / "dna_match/results_gstrings_dna.log", "dna_match", True)
    errors += check_group("dna_match/gstrings", dna_gstrings, True)

    dna_smt = read_rows(ROOT / "dna_match/results_smt_dna.log", "dna_match", True)
    errors += check_group("dna_match/smt", dna_smt, True)

    if errors:
        print(f"consistency check: FAILED ({errors} disagreement(s))")
        return 1
    print("consistency check: OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
