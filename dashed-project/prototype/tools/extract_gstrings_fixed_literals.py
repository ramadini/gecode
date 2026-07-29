#!/usr/bin/env python3
"""Extract fixed literals from preserved G-Strings str_test2 cases 17-20."""

from __future__ import annotations

import argparse
import ast
import re
from pathlib import Path

import verify_gstrings_fixtures as verifier

CASES = ("test17", "test18", "test19", "test20")
PATTERN = re.compile(
    r'DashedString\s+x\s*\(\s*\*this\s*,\s*"((?:\\.|[^"\\])*)"\s*\);',
    re.DOTALL,
)


def source_text(arguments: argparse.Namespace) -> tuple[str, str]:
    path = verifier.discover_source(verifier.candidate_roots(arguments.root))
    if path is not None:
        return str(path), path.read_text(encoding="utf-8")

    found = verifier.discover_git_source(arguments.fetch_origin)
    if found is not None:
        return found

    message = (
        "could not locate preserved G-Strings str_test2.cpp; set "
        "DASHED_TESTS_OLD, pass --root, or use --fetch-origin"
    )
    if arguments.optional:
        print(f"SKIP: {message}")
        return "", ""
    raise SystemExit(message)


def decode_cpp_string(raw: str) -> str:
    value = ast.literal_eval('"' + raw + '"')
    if not isinstance(value, str):
        raise ValueError("legacy C++ literal did not decode to text")
    return value


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--root", action="append", type=Path, default=[])
    parser.add_argument("--fetch-origin", action="store_true")
    parser.add_argument("--optional", action="store_true")
    arguments = parser.parse_args()

    label, source = source_text(arguments)
    if not source:
        return 0

    rows: list[tuple[str, str]] = []
    for function in CASES:
        body = verifier.extract_function(source, function)
        match = PATTERN.search(body)
        if match is None:
            raise SystemExit(f"could not extract fixed literal from {function} in {label}")
        text = decode_cpp_string(match.group(1))
        rows.append((f"str_test2.{function}", text.encode("utf-8").hex()))

    arguments.output.parent.mkdir(parents=True, exist_ok=True)
    with arguments.output.open("w", encoding="utf-8", newline="\n") as stream:
        for identifier, encoded in rows:
            stream.write(f"{identifier}\t{encoded}\n")

    print(f"extracted {len(rows)} fixed literals from {label}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
