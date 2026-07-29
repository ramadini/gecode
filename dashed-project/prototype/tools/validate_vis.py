#!/usr/bin/env python3
"""Small structural validator for Dashed's Gecode .vis specification.

This is not a replacement for Gecode's genvarimp.py. It catches incomplete or
asymmetric modification-event matrices before the source tree is integrated.
"""
from __future__ import annotations

import re
import sys
from pathlib import Path


def fields(lines: list[str], section: str) -> list[dict[str, str]]:
    result: list[dict[str, str]] = []
    i = 0
    while i < len(lines):
        if lines[i].strip().lower() != f"[{section.lower()}]":
            i += 1
            continue
        i += 1
        item: dict[str, str] = {}
        while i < len(lines) and not lines[i].lstrip().startswith("["):
            line = lines[i].strip()
            if line and not line.startswith("#") and ":" in line:
                key, value = line.split(":", 1)
                item[key.strip().lower()] = value.strip()
            i += 1
        result.append(item)
    return result


def main() -> int:
    path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(
        "integration/gecode/gecode/dashed/var-imp/list.vis"
    )
    lines = path.read_text(encoding="utf-8").splitlines()
    events = fields(lines, "ModEvent")
    conditions = fields(lines, "PropCond")

    names: list[str] = []
    specials: dict[str, str] = {}
    combines: dict[tuple[str, str], str] = {}
    for event in events:
        raw = event.get("name", "")
        if not raw:
            raise SystemExit("ModEvent without Name")
        parts = [x.strip() for x in raw.split("=", 1)]
        name = parts[0]
        names.append(name)
        if len(parts) == 2:
            specials[parts[1]] = name
        for lhs, rhs in re.findall(r"(\w+)\s*=\s*(\w+)", event.get("combine", "")):
            combines[(name, lhs)] = rhs

    required_specials = {"FAILED", "NONE", "ASSIGNED", "SUBSCRIBE"}
    missing = required_specials.difference(specials)
    if missing:
        raise SystemExit(f"missing special events: {sorted(missing)}")

    normal = [n for n in names if n not in {specials["FAILED"], specials["NONE"]}]
    for left in normal:
        for right in normal:
            if left == right and (left, right) not in combines:
                # Gecode's files commonly state self-combination explicitly;
                # require it here to keep the matrix auditable.
                raise SystemExit(f"missing Combine entry {left}={right}")
            value = combines.get((left, right))
            reverse = combines.get((right, left))
            if value is None:
                raise SystemExit(f"missing Combine entry {left} with {right}")
            if reverse is not None and reverse != value:
                raise SystemExit(
                    f"asymmetric Combine entries for {left},{right}: {value} vs {reverse}"
                )
            if value not in names:
                raise SystemExit(f"unknown combined event {value}")

    condition_names = set()
    for condition in conditions:
        raw = condition.get("name", "")
        if not raw:
            raise SystemExit("PropCond without Name")
        condition_names.add(raw.split("=", 1)[0].strip())
        for event in re.findall(r"\w+", condition.get("scheduledby", "")):
            if event not in names:
                raise SystemExit(f"unknown ScheduledBy event {event}")

    if not {"NONE", "VAL", "LEN", "DOM", "ANY"}.issubset(condition_names):
        raise SystemExit("missing expected propagation condition")

    print(f"validated {path}: {len(events)} events, {len(conditions)} conditions")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
