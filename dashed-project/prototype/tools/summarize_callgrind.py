#!/usr/bin/env python3
"""Summarize Callgrind self-costs without depending on callgrind_annotate output."""

from __future__ import annotations

import argparse
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path
import sys
from typing import DefaultDict, Dict, Iterable, List, Tuple


@dataclass(frozen=True)
class Symbol:
    object_path: str
    file_path: str
    function: str


def parse_reference(value: str, table: Dict[str, str]) -> str:
    value = value.strip()
    if not value.startswith("("):
        return value
    close = value.find(")")
    if close < 0:
        return value
    key = value[1:close]
    remainder = value[close + 1 :].lstrip()
    if remainder:
        table[key] = remainder
        return remainder
    return table.get(key, f"({key})")


def scope_for(file_path: str, object_path: str) -> str:
    combined = f"{file_path}\n{object_path}"
    if "/gecode/list/" in combined:
        return "list"
    if "/dashed-project/prototype/" in combined:
        return "dashed"
    if "/gecode/" in combined:
        return "gecode"
    if combined.startswith("/usr/") or "\n/usr/" in combined:
        return "system"
    return "other"


def parse_callgrind(path: Path) -> Tuple[str, DefaultDict[Symbol, int]]:
    object_names: Dict[str, str] = {}
    file_names: Dict[str, str] = {}
    function_names: Dict[str, str] = {}
    current_object = "???"
    current_file = "???"
    current_function = "???"
    position_count = 1
    event_name = "Ir"
    costs: DefaultDict[Symbol, int] = defaultdict(int)
    skip_call_cost = False

    with path.open("r", encoding="utf-8", errors="replace") as handle:
        for raw_line in handle:
            line = raw_line.strip()
            if not line or line.startswith("#"):
                continue
            if line.startswith("events:"):
                events = line.split(":", 1)[1].split()
                if events:
                    event_name = events[0]
                continue
            if line.startswith("positions:"):
                positions = line.split(":", 1)[1].split()
                position_count = max(1, len(positions))
                continue
            if line.startswith("ob="):
                current_object = parse_reference(line[3:], object_names)
                skip_call_cost = False
                continue
            if line.startswith(("fl=", "fi=", "fe=")):
                current_file = parse_reference(line[3:], file_names)
                skip_call_cost = False
                continue
            if line.startswith("fn="):
                current_function = parse_reference(line[3:], function_names)
                skip_call_cost = False
                continue
            if line.startswith(("cob=", "cfi=", "cfn=")):
                continue
            if line.startswith("calls="):
                skip_call_cost = True
                continue
            if line.startswith(("summary:", "totals:", "version:", "creator:",
                                "pid:", "cmd:", "part:", "desc:", "thread:")):
                continue

            fields = line.split()
            if len(fields) <= position_count:
                continue
            try:
                event_cost = int(fields[position_count])
            except ValueError:
                continue
            if skip_call_cost:
                skip_call_cost = False
                continue
            costs[Symbol(current_object, current_file, current_function)] += event_cost

    return event_name, costs


def ranked_rows(
    scenario: str,
    event_name: str,
    costs: Dict[Symbol, int],
    view: str,
    top: int,
) -> Iterable[List[str]]:
    if view == "project":
        selected = {
            symbol: cost
            for symbol, cost in costs.items()
            if scope_for(symbol.file_path, symbol.object_path)
            in {"list", "dashed", "gecode"}
        }
    else:
        selected = dict(costs)

    total = sum(selected.values())
    ordered = sorted(
        selected.items(),
        key=lambda item: (-item[1], item[0].function, item[0].file_path),
    )[:top]
    for rank, (symbol, cost) in enumerate(ordered, start=1):
        percent = (100.0 * cost / total) if total else 0.0
        yield [
            scenario,
            view,
            str(rank),
            event_name,
            str(cost),
            f"{percent:.3f}",
            scope_for(symbol.file_path, symbol.object_path),
            symbol.object_path,
            symbol.file_path,
            symbol.function,
        ]


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("--scenario", required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--top", type=int, default=25)
    parser.add_argument("--append", action="store_true")
    args = parser.parse_args()

    if args.top <= 0:
        parser.error("--top must be positive")
    if not args.input.is_file():
        parser.error(f"Callgrind input does not exist: {args.input}")

    event_name, costs = parse_callgrind(args.input)
    if not costs:
        raise SystemExit(f"no self-cost records found in {args.input}")

    args.output.parent.mkdir(parents=True, exist_ok=True)
    mode = "a" if args.append else "w"
    write_header = not args.append or not args.output.exists() or args.output.stat().st_size == 0
    with args.output.open(mode, encoding="utf-8", newline="") as handle:
        if write_header:
            handle.write(
                "scenario\tview\trank\tevent\tself_events\tself_percent"
                "\tscope\tobject\tfile\tfunction\n"
            )
        for view in ("all", "project"):
            for row in ranked_rows(args.scenario, event_name, costs, view, args.top):
                handle.write("\t".join(row) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
