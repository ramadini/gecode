#!/usr/bin/env python3

import argparse
import csv
import math
import os
import re
from pathlib import Path

SOLVERS = [
    "cvc5",
    "z3seq",
    "G-Strings_ori",
    "G-Strings_new",
    "G-Strings_dec",
]
LABELS = {
    "cvc5": r"\textsc{CVC5}",
    "z3seq": r"\textsc{Z3seq}",
    "G-Strings_ori": r"\textsc{PropDFA}",
    "G-Strings_new": r"\textsc{PropNFA}",
    "G-Strings_dec": r"\textsc{Decomp}",
}


def parse_args():
    here = Path(__file__).resolve().parent
    parser = argparse.ArgumentParser(description="Summarise baseline_2026 Latin-square results.")
    parser.add_argument("--timeout", type=float, default=float(os.environ.get("TIMEOUT", 300)))
    parser.add_argument("--n-min", type=int, default=2)
    parser.add_argument("--n-max", type=int, default=26)
    parser.add_argument("--csv", type=Path, default=None, help="Write the summary as CSV.")
    parser.add_argument("--plot", type=Path, default=None, help="Write a cactus plot (PDF/PNG).")
    parser.add_argument(
        "--gstrings-log", type=Path, default=here / "results_gstrings_ls.log"
    )
    parser.add_argument("--smt-log", type=Path, default=here / "results_smt_ls.log")
    return parser.parse_args()


def instance_id(raw):
    raw = raw.strip()
    if raw.isdigit():
        return int(raw)
    match = re.search(r"ls_(\d+)\.smt2$", raw)
    if not match:
        raise ValueError(f"cannot extract Latin-square size from {raw!r}")
    return int(match.group(1))


def read_log(path, data):
    if not path.exists():
        return
    with path.open(newline="") as handle:
        for line_no, row in enumerate(csv.reader(handle, delimiter="|"), 1):
            if not row:
                continue
            if len(row) != 4:
                raise ValueError(f"{path}:{line_no}: expected 4 fields, found {len(row)}")
            solver, raw_instance, status, raw_time = row
            if solver not in SOLVERS:
                continue
            try:
                n = instance_id(raw_instance)
                wall = float(raw_time)
            except ValueError as exc:
                raise ValueError(f"{path}:{line_no}: {exc}") from exc
            data[(solver, n)] = (status, wall)


def pairwise_score(times, instances, timeout):
    scores = {solver: 0.0 for solver in SOLVERS}
    for n in instances:
        for i, left in enumerate(SOLVERS[:-1]):
            for right in SOLVERS[i + 1 :]:
                left_time = times[left][n]
                right_time = times[right][n]
                if left_time < timeout and right_time >= timeout:
                    scores[left] += 1.0
                elif right_time < timeout and left_time >= timeout:
                    scores[right] += 1.0
                else:
                    total = left_time + right_time
                    if total > 0:
                        scores[left] += right_time / total
                        scores[right] += left_time / total
                    else:
                        scores[left] += 0.5
                        scores[right] += 0.5
    return scores


def write_csv(path, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="") as handle:
        writer = csv.DictWriter(handle, fieldnames=list(rows[0].keys()))
        writer.writeheader()
        writer.writerows(rows)


def plot_cactus(path, times, instances, timeout):
    import matplotlib.pyplot as plt

    path.parent.mkdir(parents=True, exist_ok=True)
    for solver in SOLVERS:
        values = sorted(times[solver][n] for n in instances)
        plt.plot(range(1, len(values) + 1), values, label=LABELS[solver])
    plt.axhline(timeout, linestyle="--", linewidth=1)
    plt.xlabel("Sorted instances")
    plt.ylabel("Runtime [s]")
    plt.legend()
    plt.tight_layout()
    plt.savefig(path)
    plt.close()


def main():
    args = parse_args()
    if args.n_min > args.n_max:
        raise SystemExit("--n-min must not exceed --n-max")

    instances = list(range(args.n_min, args.n_max + 1))
    raw = {}
    read_log(args.gstrings_log, raw)
    read_log(args.smt_log, raw)

    times = {solver: {} for solver in SOLVERS}
    rows = []
    for solver in SOLVERS:
        sat = 0
        unknown = 0
        total_time = 0.0
        for n in instances:
            status, wall = raw.get((solver, n), ("missing", args.timeout))
            solved = status == "sat"
            if solved:
                sat += 1
                effective = min(wall, args.timeout)
            else:
                unknown += 1
                effective = args.timeout
            times[solver][n] = effective
            total_time += effective
        rows.append(
            {
                "solver": solver,
                "sat": sat,
                "unknown": unknown,
                "sat_percent": round(100.0 * sat / len(instances), 2),
                "average_time": round(total_time / len(instances), 2),
            }
        )

    scores = pairwise_score(times, instances, args.timeout)
    for row in rows:
        row["borda"] = round(scores[row["solver"]], 2)

    print("solver & sat & unk & sat\\% & avg. time & borda \\\\")
    for row in rows:
        print(
            f"{LABELS[row['solver']]} & {row['sat']} & {row['unknown']} & "
            f"{row['sat_percent']:.2f} & {row['average_time']:.2f} & "
            f"{row['borda']:.2f} \\\\" 
        )

    if args.csv:
        write_csv(args.csv, rows)
    if args.plot:
        plot_cactus(args.plot, times, instances, args.timeout)


if __name__ == "__main__":
    main()
