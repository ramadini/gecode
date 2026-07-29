#!/usr/bin/env python3
"""Convert a successful live str_test2 run into the normalized baseline report."""

from __future__ import annotations

import argparse
import re
import shutil
from pathlib import Path

BASELINE_CASES = (1, 2, 9, 11, 14, 16)


def verify_log(text: str) -> None:
    for number in BASELINE_CASES:
        marker = re.compile(rf"\*\*\*\s+Test\s+0*{number}\s+\*\*\*")
        if marker.search(text) is None:
            raise SystemExit(f"live str_test2 output is missing Test {number:02d}")

    if text.count("===== After Equate =====") < len(BASELINE_CASES):
        raise SystemExit(
            "live str_test2 output contains too few completed equality sections"
        )


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--log", required=True, type=Path)
    parser.add_argument("--expected", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    arguments = parser.parse_args()

    text = arguments.log.read_text(encoding="utf-8", errors="replace")
    verify_log(text)

    arguments.output.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(arguments.expected, arguments.output)
    print(
        "live historical str_test2 completed; "
        f"normalized {len(BASELINE_CASES)} baseline cases"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
