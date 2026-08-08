#!/usr/bin/env python3

import argparse
import ast
import csv
import math
import os
import re
from pathlib import Path

SOLVERS = [
    "cvc5",
    "G-Strings_ori",
    "G-Strings_new",
    "G-Strings_dec",
    'z3seq',
    'z3noodler',
    'z3noodler_mocha',
    'ostrich'
]
LABELS = {
    "cvc5": r"\textsc{CVC5}",
    "z3seq": r"\textsc{Z3seq}",
    "G-Strings_ori": r"\textsc{PropDFA}",
    "G-Strings_new": r"\textsc{PropNFA}",
    "G-Strings_dec": r"\textsc{Decomp}",
    'z3noodler': r'\textsc{Z3-Noodler}',
    'z3noodler_mocha': r'\textsc{Z3-Noodler-Mocha}',
    'ostrich': r'\textsc{OSTRICH}'
}


def parse_args():
    here = Path(__file__).resolve().parent
    parser = argparse.ArgumentParser(description="Summarise baseline_2026 shortest-match results.")
    parser.add_argument("--timeout", type=float, default=float(os.environ.get("TIMEOUT", 300)))
    parser.add_argument("--csv", type=Path, default=None, help="Write the summary as CSV.")
    parser.add_argument(
        "--gstrings-log", type=Path, default=here / "results_gstrings_sm.log"
    )
    parser.add_argument("--smt-log", type=Path, default=here / "results_smt_sm.log")
    parser.add_argument("--instances", type=Path, default=here / "dzn")
    return parser.parse_args()


def instance_id(raw):
    name = Path(raw.strip()).name
    match = re.fullmatch(r"(sm_\d+_\d+)\.(?:dzn|smt2)", name)
    if not match:
        raise ValueError(f"cannot extract shortest-match instance from {raw!r}")
    return match.group(1)


def number(raw):
    try:
        return float(raw)
    except ValueError:
        return math.nan


def values_dict(raw):
    try:
        value = ast.literal_eval(raw)
    except (SyntaxError, ValueError):
        return {}
    if not isinstance(value, dict):
        return {}
    result = {}
    for key, val in value.items():
        try:
            result[float(key)] = float(val)
        except (TypeError, ValueError):
            continue
    return result


def read_log(path, data):
    if not path.exists():
        return
    with path.open(newline="") as handle:
        for line_no, row in enumerate(csv.reader(handle, delimiter="|"), 1):
            if not row:
                continue
            if len(row) != 6:
                raise ValueError(f"{path}:{line_no}: expected 6 fields, found {len(row)}")
            solver, raw_instance, status, raw_obj, raw_values, raw_time = row
            if solver not in SOLVERS:
                continue
            try:
                inst = instance_id(raw_instance)
                wall = float(raw_time.replace(',','.'))
            except ValueError as exc:
                raise ValueError(f"{path}:{line_no}: {exc}") from exc
            data[(solver, inst)] = {
                "status": status,
                "objective": number(raw_obj),
                "values": values_dict(raw_values),
                "wall": wall,
            }


def expected_instances(directory, data):
    files = sorted(directory.glob("sm_*.dzn"), key=lambda p: tuple(map(int, re.findall(r"\d+", p.stem))))
    if files:
        return [p.stem for p in files]
    return sorted({inst for _, inst in data}, key=lambda s: tuple(map(int, re.findall(r"\d+", s))))


def better(left, right):
    return left < right


def score(record, lower, upper, timeout):
    if record["status"] == "opt":
        return 1.0
    objective = record["objective"]
    if math.isnan(objective):
        return 0.0
    if lower == upper:
        return 0.75
    value = (objective - lower) / (2.0 * (upper - lower))
    value = min(0.5, max(0.0, value))
    return 0.75 - value


def main():
    args = parse_args()
    raw = {}
    read_log(args.gstrings_log, raw)
    read_log(args.smt_log, raw)
    instances = expected_instances(args.instances, raw)
    if not instances:
        raise SystemExit("no shortest-match instances found")

    default = {
        "status": "missing",
        "objective": math.nan,
        "values": {},
        "wall": args.timeout,
    }
    records = {
        solver: {inst: raw.get((solver, inst), default.copy()) for inst in instances}
        for solver in SOLVERS
    }

    results = {
        solver: {
            "opt": 0.0,
            "sat": 0.0,
            "unk": 0.0,
            "ttf": 0.0,
            "time": 0.0,
            "score": 0.0,
            "borda": 0.0,
            "iborda": 0.0,
        }
        for solver in SOLVERS
    }

    bounds = {}
    for inst in instances:
        objectives = [
            records[solver][inst]["objective"]
            for solver in SOLVERS
            if not math.isnan(records[solver][inst]["objective"])
        ]
        bounds[inst] = (min(objectives), max(objectives)) if objectives else (math.nan, math.nan)

    for solver in SOLVERS:
        for inst in instances:
            rec = records[solver][inst]
            status = rec["status"]
            if status == "opt":
                results[solver]["opt"] += 1
                results[solver]["sat"] += 1
                proof_time = min(rec["wall"], args.timeout)
            elif status == "sat":
                results[solver]["sat"] += 1
                proof_time = args.timeout
            else:
                results[solver]["unk"] += 1
                proof_time = args.timeout
            results[solver]["time"] += proof_time
            results[solver]["ttf"] += (
                min(rec["values"].values()) if rec["values"] else args.timeout
            )
            lower, upper = bounds[inst]
            if not math.isnan(lower):
                results[solver]["score"] += score(rec, lower, upper, args.timeout)

    for inst in instances:
        for i, left in enumerate(SOLVERS[:-1]):
            for right in SOLVERS[i + 1 :]:
                a = records[left][inst]
                b = records[right][inst]
                ao = a["objective"]
                bo = b["objective"]
                if not math.isnan(ao) and not math.isnan(bo) and better(ao, bo):
                    results[left]["borda"] += 1.0
                    results[left]["iborda"] += 1.0
                elif not math.isnan(ao) and not math.isnan(bo) and better(bo, ao):
                    results[right]["borda"] += 1.0
                    results[right]["iborda"] += 1.0
                elif not math.isnan(ao) and not math.isnan(bo) and ao == bo:
                    results[left]["iborda"] += 0.5
                    results[right]["iborda"] += 0.5
                    at = min(a["wall"], args.timeout) if a["status"] == "opt" else args.timeout
                    bt = min(b["wall"], args.timeout) if b["status"] == "opt" else args.timeout
                    total = at + bt
                    if total > 0:
                        results[left]["borda"] += bt / total
                        results[right]["borda"] += at / total
                    else:
                        results[left]["borda"] += 0.5
                        results[right]["borda"] += 0.5

    count = len(instances)
    rows = []
    for solver in SOLVERS:
        item = results[solver]
        rows.append(
            {
                "solver": solver,
                "opt_percent": round(100.0 * item["opt"] / count, 2),
                "sat_percent": round(100.0 * item["sat"] / count, 2),
                "unk_percent": round(100.0 * item["unk"] / count, 2),
                "average_ttf": round(item["ttf"] / count, 2),
                "average_time": round(item["time"] / count, 2),
                "score": round(item["score"], 2),
                "borda": round(item["borda"], 2),
                "iborda": round(item["iborda"], 2),
            }
        )

    print("solver & opt & sat & unk & ttf & time & score & borda & iborda \\\\")
    for row in rows:
        print(
            f"{LABELS[row['solver']]} & {row['opt_percent']:.2f} & "
            f"{row['sat_percent']:.2f} & {row['unk_percent']:.2f} & "
            f"{row['average_ttf']:.2f} & {row['average_time']:.2f} & "
            f"{row['score']:.2f} & {row['borda']:.2f} & {row['iborda']:.2f} \\\\"
        )

    if args.csv:
        args.csv.parent.mkdir(parents=True, exist_ok=True)
        with args.csv.open("w", newline="") as handle:
            writer = csv.DictWriter(handle, fieldnames=list(rows[0].keys()))
            writer.writeheader()
            writer.writerows(rows)


if __name__ == "__main__":
    main()
