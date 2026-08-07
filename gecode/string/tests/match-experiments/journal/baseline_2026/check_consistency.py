#!/usr/bin/env python3

import math
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent

FAMILIES = {
    "latin_square": {
        "path": ROOT / "latin_square" / "results_tot.log",
        "objective": False,
    },
    "shortest_match": {
        "path": ROOT / "shortest_match" / "results_tot.log",
        "objective": True,
    },
    "dna_match": {
        "path": ROOT / "dna_match" / "results_tot.log",
        "objective": True,
    },
}

SOLVED = {"sat", "opt"}


def instance_id(family: str, raw: str) -> str:
    name = Path(raw.strip()).name
    for suffix in (".smt2", ".dzn"):
        if name.endswith(suffix):
            name = name[:-len(suffix)]
            break
    if family == "latin_square" and name.isdigit():
        return f"ls_{name}"
    return name


def parse_objective(raw: str):
    try:
        value = float(raw)
    except ValueError:
        return None
    return None if math.isnan(value) else value


def read_rows(path: Path, family: str, objective: bool):
    rows = []
    malformed = 0

    if not path.exists():
        print(f"ERROR missing file: {path}")
        return rows, 1

    for line_no, line in enumerate(path.read_text().splitlines(), 1):
        if not line.strip():
            continue

        fields = line.split("|")
        minimum = 5 if objective else 4
        if len(fields) < minimum:
            print(f"ERROR malformed row {path}:{line_no}: {line}")
            malformed += 1
            continue

        solver, raw_instance, status = fields[:3]
        value = parse_objective(fields[3]) if objective else None

        rows.append(
            {
                "instance": instance_id(family, raw_instance),
                "solver": solver,
                "status": status,
                "objective": value,
                "line": line_no,
            }
        )

    return rows, malformed


def check_repetitions(label: str, rows, compare_objective: bool) -> int:
    grouped = defaultdict(list)
    for row in rows:
        grouped[(row["solver"], row["instance"])].append(row)

    errors = 0
    for (solver, instance), repetitions in sorted(grouped.items()):
        if len(repetitions) < 2:
            continue

        statuses = {row["status"] for row in repetitions}
        if len(statuses) > 1:
            print(
                f"ERROR {label}/{instance}: repetition status disagreement "
                f"for {solver}: {sorted(statuses)}"
            )
            errors += 1

        if compare_objective:
            objectives = {
                row["objective"]
                for row in repetitions
                if row["status"] == "opt" and row["objective"] is not None
            }
            if len(objectives) > 1:
                print(
                    f"ERROR {label}/{instance}: repetition objective disagreement "
                    f"for {solver}: {sorted(objectives)}"
                )
                errors += 1

    return errors


def check_across_solvers(label: str, rows, compare_objective: bool) -> int:
    grouped = defaultdict(list)
    for row in rows:
        grouped[row["instance"]].append(row)

    errors = 0
    for instance, results in sorted(grouped.items()):
        solved = [row for row in results if row["status"] in SOLVED]
        unsat = [row for row in results if row["status"] == "uns"]

        if solved and unsat:
            details = [
                (row["solver"], row["status"], row["objective"])
                for row in results
            ]
            print(
                f"ERROR {label}/{instance}: SAT/UNSAT disagreement: {details}"
            )
            errors += 1

        if compare_objective:
            optimal = [
                (row["solver"], row["objective"])
                for row in results
                if row["status"] == "opt" and row["objective"] is not None
            ]
            distinct = {value for _, value in optimal}
            if len(distinct) > 1:
                print(
                    f"ERROR {label}/{instance}: objective disagreement: {optimal}"
                )
                errors += 1

    return errors


def main() -> int:
    errors = 0

    for family, config in FAMILIES.items():
        rows, malformed = read_rows(
            config["path"],
            family,
            config["objective"],
        )
        errors += malformed
        errors += check_repetitions(
            family,
            rows,
            config["objective"],
        )
        errors += check_across_solvers(
            family,
            rows,
            config["objective"],
        )

    if errors:
        print(f"consistency check: FAILED ({errors} issue(s))")
        return 1

    print("consistency check: OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
