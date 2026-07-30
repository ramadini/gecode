#!/usr/bin/env python3
"""Resolve and summarize Callgrind self-costs deterministically."""

from __future__ import annotations

import argparse
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path
import re
import sys
from typing import DefaultDict, Dict, Iterable, List, Sequence, Tuple


REFERENCE_RE = re.compile(r"^\(([^)]+)\)(?:\s+(.*))?$")
UNRESOLVED_NUMERIC_RE = re.compile(r"^\(\d+\)$")


@dataclass(frozen=True)
class Symbol:
    object_path: str
    file_path: str
    function: str


@dataclass(frozen=True)
class RankedSymbol:
    scope: str
    object_path: str
    file_path: str
    function: str
    instances: int
    cost: int


def register_reference(value: str, table: Dict[str, str]) -> None:
    """Record a compressed Callgrind name definition, if present."""
    match = REFERENCE_RE.match(value.strip())
    if match is None:
        return
    key, name = match.groups()
    if name:
        table[key] = name


def resolve_reference(value: str, table: Dict[str, str]) -> str:
    """Resolve a literal or compressed Callgrind name after the first pass."""
    value = value.strip()
    match = REFERENCE_RE.match(value)
    if match is None:
        return value
    key, name = match.groups()
    if name:
        return name
    return table.get(key, f"({key})")


def scope_for(function: str, file_path: str, object_path: str) -> str:
    # Function names are more reliable than inline source locations.
    if "Gecode::List::" in function or "Gecode::ListVar" in function:
        return "list"
    if "dashed::" in function:
        return "dashed"

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


def collect_name_tables(
    lines: Sequence[str],
) -> Tuple[Dict[str, str], Dict[str, str], Dict[str, str]]:
    """Collect definitions from both caller and callee records.

    Callgrind may define an ID first in cfn/cfi/cob and use it later in
    fn/fl/ob, or define it after its first use. A complete first pass avoids
    leaking raw IDs such as ``(1738)`` into the report.
    """
    object_names: Dict[str, str] = {}
    file_names: Dict[str, str] = {}
    function_names: Dict[str, str] = {}

    for raw_line in lines:
        line = raw_line.strip()
        if line.startswith(("ob=", "cob=")):
            register_reference(line.split("=", 1)[1], object_names)
        elif line.startswith(("fl=", "fi=", "fe=", "cfi=")):
            register_reference(line.split("=", 1)[1], file_names)
        elif line.startswith(("fn=", "cfn=")):
            register_reference(line.split("=", 1)[1], function_names)

    return object_names, file_names, function_names


def parse_callgrind(path: Path) -> Tuple[str, DefaultDict[Symbol, int]]:
    lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
    object_names, file_names, function_names = collect_name_tables(lines)

    current_object = "???"
    current_file = "???"
    current_function = "???"
    position_count = 1
    event_name = "Ir"
    costs: DefaultDict[Symbol, int] = defaultdict(int)
    skip_call_cost = False

    for raw_line in lines:
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
            current_object = resolve_reference(line[3:], object_names)
            skip_call_cost = False
            continue
        if line.startswith(("fl=", "fi=", "fe=")):
            current_file = resolve_reference(line[3:], file_names)
            skip_call_cost = False
            continue
        if line.startswith("fn="):
            current_function = resolve_reference(line[3:], function_names)
            skip_call_cost = False
            continue
        if line.startswith(("cob=", "cfi=", "cfn=")):
            # Definitions were collected during the first pass. Callee context
            # must not replace the current caller symbol used for self-costs.
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

    unresolved = sorted({
        value
        for symbol in costs
        for value in (symbol.object_path, symbol.file_path, symbol.function)
        if UNRESOLVED_NUMERIC_RE.match(value)
    })
    if unresolved:
        joined = ", ".join(unresolved[:10])
        raise ValueError(f"unresolved compressed Callgrind IDs: {joined}")

    return event_name, costs


def aggregate_symbols(costs: Dict[Symbol, int]) -> List[RankedSymbol]:
    """Aggregate inline/source-location fragments by resolved function name."""
    grouped: DefaultDict[str, List[Tuple[Symbol, int]]] = defaultdict(list)
    for symbol, cost in costs.items():
        grouped[symbol.function].append((symbol, cost))

    aggregated: List[RankedSymbol] = []
    for function, entries in grouped.items():
        representative, _ = max(
            entries,
            key=lambda item: (
                item[1],
                item[0].file_path,
                item[0].object_path,
            ),
        )
        scope_costs: DefaultDict[str, int] = defaultdict(int)
        for symbol, cost in entries:
            scope_costs[
                scope_for(symbol.function, symbol.file_path, symbol.object_path)
            ] += cost
        scope = max(scope_costs.items(), key=lambda item: (item[1], item[0]))[0]
        aggregated.append(
            RankedSymbol(
                scope=scope,
                object_path=representative.object_path,
                file_path=representative.file_path,
                function=function,
                instances=len(entries),
                cost=sum(cost for _, cost in entries),
            )
        )
    return aggregated


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
            if scope_for(symbol.function, symbol.file_path, symbol.object_path)
            in {"list", "dashed", "gecode"}
        }
    else:
        selected = dict(costs)

    aggregated = aggregate_symbols(selected)
    total = sum(symbol.cost for symbol in aggregated)
    ordered = sorted(
        aggregated,
        key=lambda symbol: (-symbol.cost, symbol.function, symbol.file_path),
    )[:top]
    for rank, symbol in enumerate(ordered, start=1):
        percent = (100.0 * symbol.cost / total) if total else 0.0
        yield [
            scenario,
            view,
            str(rank),
            event_name,
            str(symbol.cost),
            f"{percent:.3f}",
            symbol.scope,
            str(symbol.instances),
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
    write_header = (
        not args.append
        or not args.output.exists()
        or args.output.stat().st_size == 0
    )
    with args.output.open(mode, encoding="utf-8", newline="") as handle:
        if write_header:
            handle.write(
                "scenario\tview\trank\tevent\tself_events\tself_percent"
                "\tscope\tinstances\tobject\tfile\tfunction\n"
            )
        for view in ("all", "project"):
            for row in ranked_rows(args.scenario, event_name, costs, view, args.top):
                handle.write("\t".join(row) + "\n")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except ValueError as error:
        raise SystemExit(str(error)) from error
