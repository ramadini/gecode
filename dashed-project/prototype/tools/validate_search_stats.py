#!/usr/bin/env python3
"""Validate native List search statistics and expected solution counts."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path

COUNT_FIELDS = ("case", "solutions")
STATS_FIELDS = ("case", "solutions", "nodes", "failures", "depth")


def load_counts(path: Path) -> dict[str, int]:
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != COUNT_FIELDS:
            raise SystemExit(
                f"{path}: expected columns {COUNT_FIELDS}, got {reader.fieldnames}"
            )
        result: dict[str, int] = {}
        for row in reader:
            identifier = row["case"]
            if identifier in result:
                raise SystemExit(f"{path}: duplicate case {identifier}")
            result[identifier] = int(row["solutions"])
        return result


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--expected", required=True, type=Path)
    parser.add_argument("--actual", required=True, type=Path)
    arguments = parser.parse_args()

    expected = load_counts(arguments.expected)
    actual: dict[str, tuple[int, int, int, int]] = {}
    with arguments.actual.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != STATS_FIELDS:
            raise SystemExit(
                f"{arguments.actual}: expected columns {STATS_FIELDS}, got {reader.fieldnames}"
            )
        for row in reader:
            identifier = row["case"]
            if identifier in actual:
                raise SystemExit(
                    f"{arguments.actual}: duplicate case {identifier}"
                )
            values = tuple(
                int(row[field])
                for field in ("solutions", "nodes", "failures", "depth")
            )
            if any(value < 0 for value in values):
                raise SystemExit(
                    f"{arguments.actual}: negative statistic for {identifier}"
                )
            actual[identifier] = values

    missing = sorted(expected.keys() - actual.keys())
    extra = sorted(actual.keys() - expected.keys())
    mismatched = sorted(
        identifier
        for identifier in expected.keys() & actual.keys()
        if expected[identifier] != actual[identifier][0]
    )
    if missing or extra or mismatched:
        lines = ["native List search statistics mismatch"]
        lines.extend(f"missing: {identifier}" for identifier in missing)
        lines.extend(f"extra: {identifier}" for identifier in extra)
        for identifier in mismatched:
            lines.append(
                f"solution count {identifier}: expected {expected[identifier]}, "
                f"actual {actual[identifier][0]}"
            )
        raise SystemExit("\n".join(lines))

    for identifier, (_, nodes, _, depth) in actual.items():
        if nodes == 0:
            raise SystemExit(f"{identifier}: search expanded no nodes")
        if depth == 0:
            raise SystemExit(f"{identifier}: search reached no branch depth")

    print(f"native List search statistics validated: {len(actual)} cases")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
