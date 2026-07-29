#!/usr/bin/env python3
"""Compare deterministic solution-set reports without depending on row order."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path

FIELDS = ("case", "solution")


def load(path: Path) -> set[tuple[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != FIELDS:
            raise SystemExit(
                f"{path}: expected columns {FIELDS}, got {reader.fieldnames}"
            )
        rows = [(row["case"], row["solution"]) for row in reader]
    unique = set(rows)
    if len(rows) != len(unique):
        raise SystemExit(f"{path}: duplicate solution rows")
    return unique


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--expected", required=True, type=Path)
    parser.add_argument("--actual", required=True, type=Path)
    arguments = parser.parse_args()

    expected = load(arguments.expected)
    actual = load(arguments.actual)
    missing = sorted(expected - actual)
    extra = sorted(actual - expected)
    if missing or extra:
        lines = ["solution-set differential mismatch"]
        lines.extend(f"missing: {case}\t{solution}" for case, solution in missing)
        lines.extend(f"extra: {case}\t{solution}" for case, solution in extra)
        raise SystemExit("\n".join(lines))

    print(f"solution-set differential reports match: {len(expected)} solutions")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
