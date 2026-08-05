#!/usr/bin/env python3
"""Cross-check statuses, objectives, repetitions, and validator outcomes."""
from __future__ import annotations

import argparse
import csv
import json
from collections import defaultdict
from pathlib import Path
from typing import Any

SATLIKE = {"sat", "optimal", "completed"}


def norm_objective(value: str | None) -> str | None:
    if value is None or value == "":
        return None
    try:
        return str(int(value))
    except ValueError:
        try:
            return repr(float(value))
        except ValueError:
            return value.strip()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("run_dir", type=Path)
    args = parser.parse_args()
    summary = args.run_dir / "summary.csv"
    if not summary.exists():
        raise SystemExit(f"missing {summary}")

    with summary.open(newline="", encoding="utf-8") as stream:
        rows = list(csv.DictReader(stream))

    issues: list[dict[str, Any]] = []
    grouped: dict[tuple[str, str], list[dict[str, str]]] = defaultdict(list)
    repetitions: dict[tuple[str, str, str], list[dict[str, str]]] = defaultdict(list)
    for row in rows:
        grouped[(row["family"], row["instance"])].append(row)
        repetitions[(row["solver"], row["family"], row["instance"])].append(row)
        if row.get("validation") == "invalid":
            issues.append({"kind": "invalid-witness", "row": row})

    for key, values in repetitions.items():
        statuses = {row["status"] for row in values}
        objectives = {norm_objective(row.get("objective")) for row in values if row["status"] == "optimal"}
        objectives.discard(None)
        if len(statuses) > 1:
            issues.append({"kind": "repetition-status-mismatch", "key": key, "statuses": sorted(statuses)})
        if len(objectives) > 1:
            issues.append({"kind": "repetition-objective-mismatch", "key": key, "objectives": sorted(objectives)})

    for key, values in grouped.items():
        statuses = {row["status"] for row in values}
        if "unsat" in statuses and statuses.intersection(SATLIKE):
            issues.append({"kind": "sat-unsat-disagreement", "key": key, "rows": values})
        optimal = {
            norm_objective(row.get("objective"))
            for row in values
            if row["status"] == "optimal" and norm_objective(row.get("objective")) is not None
        }
        if len(optimal) > 1:
            issues.append({"kind": "optimal-objective-disagreement", "key": key, "objectives": sorted(optimal)})

    report = {
        "ok": not issues,
        "rows": len(rows),
        "instances": len(grouped),
        "issues": issues,
    }
    output = args.run_dir / "consistency-report.json"
    output.write_text(json.dumps(report, indent=2, sort_keys=True), encoding="utf-8")

    if issues:
        for issue in issues:
            print(f"ERROR {issue['kind']}: {issue.get('key', '')}")
        print(f"consistency check: FAILED ({len(issues)} issues); report: {output}")
        return 1
    print(f"consistency check: OK ({len(rows)} runs, {len(grouped)} instances); report: {output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
