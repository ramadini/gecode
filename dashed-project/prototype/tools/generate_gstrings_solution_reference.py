#!/usr/bin/env python3
"""Generate exact small solution sets for G-Strings-compatible list models."""

from __future__ import annotations

import argparse
import csv
import itertools
import sys
from pathlib import Path
from typing import Iterable, TextIO

FIELDS = ("case", "solution")
ALPHABET = (1, 2)


def lists_between(minimum: int, maximum: int) -> list[tuple[int, ...]]:
    values: list[tuple[int, ...]] = []
    for length in range(minimum, maximum + 1):
        values.extend(itertools.product(ALPHABET, repeat=length))
    return values


def encode_list(value: Iterable[int]) -> str:
    return "[" + ",".join(str(item) for item in value) + "]"


def encode(**fields: object) -> str:
    parts: list[str] = []
    for name, value in fields.items():
        if isinstance(value, tuple):
            rendered = encode_list(value)
        else:
            rendered = str(value)
        parts.append(f"{name}={rendered}")
    return ";".join(parts)


def rows() -> list[tuple[str, str]]:
    result: list[tuple[str, str]] = []

    optional_binary = lists_between(0, 2)
    for value in optional_binary:
        result.append(
            ("search.eq.optional_binary", encode(x=value, y=value))
        )

    singleton_binary = lists_between(1, 1)
    for left in singleton_binary:
        for right in singleton_binary:
            if left != right:
                result.append(
                    ("search.neq.binary_singleton", encode(x=left, y=right))
                )

    optional_singleton = lists_between(0, 1)
    for left in optional_singleton:
        for right in optional_singleton:
            result.append(
                (
                    "search.concat.optional_binary",
                    encode(x=left, y=right, z=left + right),
                )
            )

    for value in singleton_binary:
        result.append(
            ("search.length.binary_exact_one", encode(x=value, n=1))
        )

    for left in singleton_binary:
        for right in singleton_binary:
            result.append(
                (
                    "search.reified_eq.binary_singleton",
                    encode(x=left, y=right, b=int(left == right)),
                )
            )

    result.sort()
    return result


def write_report(output: TextIO) -> None:
    writer = csv.writer(output, delimiter="\t", lineterminator="\n")
    writer.writerow(FIELDS)
    writer.writerows(rows())


def load(path: Path) -> list[tuple[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != FIELDS:
            raise SystemExit(
                f"{path}: expected columns {FIELDS}, got {reader.fieldnames}"
            )
        loaded = [(row["case"], row["solution"]) for row in reader]
    if len(loaded) != len(set(loaded)):
        raise SystemExit(f"{path}: duplicate solution rows")
    return sorted(loaded)


def main() -> int:
    parser = argparse.ArgumentParser()
    group = parser.add_mutually_exclusive_group()
    group.add_argument("--report", type=Path)
    group.add_argument("--check", type=Path)
    arguments = parser.parse_args()

    if arguments.check is not None:
        expected = load(arguments.check)
        actual = rows()
        if expected != actual:
            missing = sorted(set(expected) - set(actual))
            extra = sorted(set(actual) - set(expected))
            lines = ["G-Strings search reference mismatch"]
            lines.extend(f"missing: {row}" for row in missing)
            lines.extend(f"extra: {row}" for row in extra)
            raise SystemExit("\n".join(lines))
        print(f"G-Strings search reference matches: {len(actual)} solutions")
        return 0

    if arguments.report is not None:
        with arguments.report.open("w", encoding="utf-8", newline="") as stream:
            write_report(stream)
        return 0

    write_report(sys.stdout)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
