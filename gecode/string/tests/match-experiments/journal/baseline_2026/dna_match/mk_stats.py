#!/usr/bin/env python3

import argparse
import ast
import csv
import math
import os
import re
from pathlib import Path

GSTRING_SOLVERS = ["G-Strings_ori", "G-Strings_new", "G-Strings_dec"]
SMT_SOLVERS = ["cvc5", "z3seq"]
LABELS = {
    "cvc5": r"\textsc{CVC5}",
    "z3seq": r"\textsc{Z3seq}",
    "G-Strings_ori": r"\textsc{PropDFA}",
    "G-Strings_new": r"\textsc{PropNFA}",
    "G-Strings_dec": r"\textsc{Decomp}",
}


def parse_args():
    here = Path(__file__).resolve().parent
    parser = argparse.ArgumentParser(description="Summarise baseline_2026 DNA results.")
    parser.add_argument("--timeout", type=float, default=float(os.environ.get("TIMEOUT", 300)))
    parser.add_argument("--csv", type=Path, default=None, help="Write G-Strings summary as CSV.")
    parser.add_argument("--smt-csv", type=Path, default=None, help="Write aggregate SMT summary as CSV.")
    parser.add_argument("--plot", type=Path, default=None, help="Write the G-Strings runtime plot.")
    parser.add_argument(
        "--gstrings-log", type=Path, default=here / "results_gstrings_dna.log"
    )
    parser.add_argument("--smt-log", type=Path, default=here / "results_smt_dna.log")
    parser.add_argument("--instances", type=Path, default=here / "dzn")
    return parser.parse_args()


def instance_id(raw):
    name = Path(raw.strip()).name
    match = re.fullmatch(r"(L\d+_K\d+)\.dzn", name)
    if not match:
        raise ValueError(f"cannot extract DNA instance from {raw!r}")
    return match.group(1)


def instance_key(inst):
    match = re.fullmatch(r"L(\d+)_K(\d+)", inst)
    return tuple(map(int, match.groups()))


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


def read_gstrings(path):
    data = {}
    if not path.exists():
        return data
    with path.open(newline="") as handle:
        for line_no, row in enumerate(csv.reader(handle, delimiter="|"), 1):
            if not row:
                continue
            if len(row) != 6:
                raise ValueError(f"{path}:{line_no}: expected 6 fields, found {len(row)}")
            solver, raw_instance, status, raw_obj, raw_values, raw_time = row
            if solver not in GSTRING_SOLVERS:
                continue
            try:
                inst = instance_id(raw_instance)
                wall = float(raw_time)
            except ValueError as exc:
                raise ValueError(f"{path}:{line_no}: {exc}") from exc
            data[(solver, inst)] = {
                "status": status,
                "objective": number(raw_obj),
                "values": values_dict(raw_values),
                "wall": wall,
            }
    return data


def read_smt(path, timeout):
    rows = []
    if not path.exists():
        return rows
    with path.open(newline="") as handle:
        for line_no, row in enumerate(csv.reader(handle, delimiter="|"), 1):
            if not row:
                continue
            if len(row) != 6:
                raise ValueError(f"{path}:{line_no}: expected 6 fields, found {len(row)}")
            solver, instance, status, raw_obj, _values, raw_time = row
            if solver not in SMT_SOLVERS:
                continue
            try:
                wall = float(raw_time)
            except ValueError as exc:
                raise ValueError(f"{path}:{line_no}: invalid runtime {raw_time!r}") from exc
            rows.append(
                {
                    "solver": solver,
                    "instance": instance,
                    "status": status,
                    "objective": raw_obj,
                    "time": round(min(wall, timeout), 2),
                }
            )
    return rows


def score(record, lower, upper):
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


def write_csv(path, rows):
    if not rows:
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="") as handle:
        writer = csv.DictWriter(handle, fieldnames=list(rows[0].keys()))
        writer.writeheader()
        writer.writerows(rows)


def plot_runtime(path, times, instances, timeout):
    import matplotlib.pyplot as plt

    path.parent.mkdir(parents=True, exist_ok=True)
    x = range(len(instances))
    for solver in GSTRING_SOLVERS:
        plt.plot(x, [times[solver][inst] for inst in instances], marker="o", label=LABELS[solver])
    plt.xticks(list(x), [inst.replace("L", "(").replace("_K", ",") + ")" for inst in instances], rotation=45)
    plt.axhline(timeout, linestyle="--", linewidth=1)
    plt.xlabel("(L,K)")
    plt.ylabel("Runtime [s]")
    plt.legend()
    plt.tight_layout()
    plt.savefig(path)
    plt.close()


def main():
    args = parse_args()
    raw = read_gstrings(args.gstrings_log)
    files = sorted(args.instances.glob("L*_K*.dzn"), key=lambda p: instance_key(p.stem))
    instances = [p.stem for p in files]
    if not instances:
        instances = sorted({inst for _, inst in raw}, key=instance_key)
    if not instances:
        raise SystemExit("no DNA instances found")

    default = {
        "status": "missing",
        "objective": math.nan,
        "values": {},
        "wall": args.timeout,
    }
    records = {
        solver: {inst: raw.get((solver, inst), default.copy()) for inst in instances}
        for solver in GSTRING_SOLVERS
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
        for solver in GSTRING_SOLVERS
    }
    times = {solver: {} for solver in GSTRING_SOLVERS}

    bounds = {}
    for inst in instances:
        objectives = [
            records[solver][inst]["objective"]
            for solver in GSTRING_SOLVERS
            if not math.isnan(records[solver][inst]["objective"])
        ]
        bounds[inst] = (min(objectives), max(objectives)) if objectives else (math.nan, math.nan)

    for solver in GSTRING_SOLVERS:
        for inst in instances:
            rec = records[solver][inst]
            if rec["status"] == "opt":
                results[solver]["opt"] += 1
                results[solver]["sat"] += 1
                proof_time = min(rec["wall"], args.timeout)
            elif rec["status"] == "sat":
                results[solver]["sat"] += 1
                proof_time = args.timeout
            else:
                results[solver]["unk"] += 1
                proof_time = args.timeout
            times[solver][inst] = proof_time
            results[solver]["time"] += proof_time
            results[solver]["ttf"] += (
                min(rec["values"].values()) if rec["values"] else args.timeout
            )
            lower, upper = bounds[inst]
            if not math.isnan(lower):
                results[solver]["score"] += score(rec, lower, upper)

    for inst in instances:
        for i, left in enumerate(GSTRING_SOLVERS[:-1]):
            for right in GSTRING_SOLVERS[i + 1 :]:
                a = records[left][inst]
                b = records[right][inst]
                ao = a["objective"]
                bo = b["objective"]
                if not math.isnan(ao) and not math.isnan(bo) and ao < bo:
                    results[left]["borda"] += 1.0
                    results[left]["iborda"] += 1.0
                elif not math.isnan(ao) and not math.isnan(bo) and bo < ao:
                    results[right]["borda"] += 1.0
                    results[right]["iborda"] += 1.0
                elif not math.isnan(ao) and not math.isnan(bo) and ao == bo:
                    results[left]["iborda"] += 0.5
                    results[right]["iborda"] += 0.5
                    at = times[left][inst]
                    bt = times[right][inst]
                    total = at + bt
                    if total > 0:
                        results[left]["borda"] += bt / total
                        results[right]["borda"] += at / total
                    else:
                        results[left]["borda"] += 0.5
                        results[right]["borda"] += 0.5

    count = len(instances)
    rows = []
    for solver in GSTRING_SOLVERS:
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

    print("G-Strings per-instance results")
    print("solver & opt & sat & unk & ttf & time & score & borda & iborda \\\\")
    for row in rows:
        print(
            f"{LABELS[row['solver']]} & {row['opt_percent']:.2f} & "
            f"{row['sat_percent']:.2f} & {row['unk_percent']:.2f} & "
            f"{row['average_ttf']:.2f} & {row['average_time']:.2f} & "
            f"{row['score']:.2f} & {row['borda']:.2f} & {row['iborda']:.2f} \\\\"
        )

    smt_rows = read_smt(args.smt_log, args.timeout)
    if smt_rows:
        print("\nAggregate quantifier-free SMT encoding (not paired with the 30 DZN instances)")
        print("solver & status & objective & time \\\\")
        for row in smt_rows:
            print(
                f"{LABELS[row['solver']]} & {row['status']} & {row['objective']} & "
                f"{row['time']:.2f} \\\\"
            )

    if args.csv:
        write_csv(args.csv, rows)
    if args.smt_csv:
        write_csv(args.smt_csv, smt_rows)
    if args.plot:
        plot_runtime(args.plot, times, instances, args.timeout)


if __name__ == "__main__":
    main()
