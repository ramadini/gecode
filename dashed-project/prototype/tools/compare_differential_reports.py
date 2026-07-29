#!/usr/bin/env python3
"""Compare deterministic tab-separated differential reports."""

from __future__ import annotations

import argparse
import csv
import subprocess
import tempfile
from pathlib import Path

FIELDS = ("case", "status", "left", "right")


def load(path: Path) -> dict[str, tuple[str, str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != FIELDS:
            raise SystemExit(
                f"{path}: expected columns {FIELDS}, got {reader.fieldnames}"
            )
        rows: dict[str, tuple[str, str, str]] = {}
        for row in reader:
            identifier = row["case"]
            if identifier in rows:
                raise SystemExit(f"{path}: duplicate case {identifier}")
            rows[identifier] = (row["status"], row["left"], row["right"])
        return rows


def compare(expected_path: Path, actual_path: Path) -> None:
    expected = load(expected_path)
    actual = load(actual_path)
    missing = sorted(expected.keys() - actual.keys())
    extra = sorted(actual.keys() - expected.keys())
    mismatches = sorted(
        identifier
        for identifier in expected.keys() & actual.keys()
        if expected[identifier] != actual[identifier]
    )
    if missing or extra or mismatches:
        lines = ["differential report mismatch"]
        lines.extend(f"missing: {identifier}" for identifier in missing)
        lines.extend(f"extra: {identifier}" for identifier in extra)
        for identifier in mismatches:
            lines.append(f"mismatch: {identifier}")
            lines.append(f"  expected: {expected[identifier]}")
            lines.append(f"  actual:   {actual[identifier]}")
        raise SystemExit("\n".join(lines))
    print(f"differential reports match: {len(expected)} cases")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--expected", required=True, type=Path)
    parser.add_argument("--actual", type=Path)
    parser.add_argument("--runner", type=Path)
    arguments = parser.parse_args()

    if (arguments.actual is None) == (arguments.runner is None):
        parser.error("provide exactly one of --actual or --runner")

    if arguments.actual is not None:
        compare(arguments.expected, arguments.actual)
        return 0

    with tempfile.TemporaryDirectory(prefix="gstrings-differential-") as directory:
        report = Path(directory) / "actual.tsv"
        subprocess.run(
            [str(arguments.runner), "--report", str(report)],
            check=True,
        )
        compare(arguments.expected, report)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
