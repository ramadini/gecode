import csv
import re

import matplotlib
matplotlib.rcParams['text.usetex'] = True
import matplotlib.pyplot as plt


PATH = (
    '/home/roberto/G-Strings/gecode/gecode/string/tests/'
    'match-experiments/journal/baseline_2026/latin_square/results_tot.log'
)

SOLVERS = [
    'cvc5',
    'G-Strings_ori',
    'G-Strings_dec',
    'G-Strings_new',
    'z3seq',
    'z3noodler',
    'z3noodler_mocha',
    'ostrich',
]

MIN = False
TIMEOUT = 300

MIN_N = 2
MAX_N = 40
NUM_PROBLEMS = MAX_N - MIN_N + 1


def normalize_instance(inst):
    inst = inst.strip()

    if inst.isdigit():
        n = int(inst)
    else:
        m = re.search(r'(?:^|/)ls_(\d+)(?:\.smt2)?$', inst)
        if m is None:
            raise ValueError(f'Unexpected instance name: {inst!r}')
        n = int(m.group(1))

    if not (MIN_N <= n <= MAX_N):
        raise ValueError(
            f'Instance n={n} outside expected range [{MIN_N}..{MAX_N}] '
            f'(raw identifier: {inst!r})'
        )

    return str(n)


reader = csv.reader(open(PATH), delimiter='|')

results = {
    s: {'sat': 0.0, 'unk': 0.0, 'time': 0.0, 'mznc': 0.0}
    for s in SOLVERS
}

infos = {}

# Pre-populate exactly one entry per n-value for every solver.
# Missing runs are therefore represented as TIMEOUT.
expected_instances = {str(i) for i in range(MIN_N, MAX_N + 1)}
times = {
    s: {str(i): TIMEOUT for i in range(MIN_N, MAX_N + 1)}
    for s in SOLVERS
}

for row in reader:
    if not row:
        continue

    solv = row[0].strip()

    if solv == 'z3str':
        continue

    if solv not in results:
        raise ValueError(f'Unexpected solver name: {solv!r}')

    inst = normalize_instance(row[1])

    if inst not in infos:
        infos[inst] = {s: TIMEOUT for s in SOLVERS}

    if row[2].strip() == 'sat':
        time = float(row[3].replace(',', '.'))
        results[solv]['sat'] += 1
    else:
        time = TIMEOUT
        results[solv]['unk'] += 1

    results[solv]['time'] += time
    infos[inst][solv] = time
    times[solv][inst] = time


def better(x, y):
    return (x < y and MIN) or (x > y and not MIN)


n_solvers = len(SOLVERS)

for inst, info in infos.items():
    for i in range(0, n_solvers - 1):
        s_i = SOLVERS[i]

        for j in range(i + 1, n_solvers):
            s_j = SOLVERS[j]
            time_i = info[s_i]
            time_j = info[s_j]

            if time_i < time_j and time_j == TIMEOUT:
                results[s_i]['mznc'] += 1
                print(f'{s_i} better than {s_j} (inst {inst})')

            elif time_j < time_i and time_i == TIMEOUT:
                results[s_j]['mznc'] += 1
                print(f'{s_j} better than {s_i} (inst {inst})')

            else:
                t = time_i + time_j
                if t > 0:
                    results[s_i]['mznc'] += time_j / t
                    results[s_j]['mznc'] += time_i / t
                else:
                    results[s_i]['mznc'] += 0.5
                    results[s_j]['mznc'] += 0.5


for solv, val in sorted(results.items(), key=lambda x: -x[1]['sat']):
    print(solv, val)
    assert 0 <= val['time'] <= TIMEOUT * NUM_PROBLEMS


# Sanity checks: every solver must have exactly the same 39 normalized
# instance identifiers, with no accidental "ls_2.smt2"-style duplicates.
for solver, vals in times.items():
    assert len(vals) == NUM_PROBLEMS, (
        f'{solver}: expected {NUM_PROBLEMS} instances, got {len(vals)}'
    )
    assert set(vals) == expected_instances, (
        f'{solver}: unexpected instance keys: '
        f'{sorted(set(vals) - expected_instances)}'
    )


labels = []

solv2lab = {
    'cvc5': (r'\textsc{CVC5}', '-^'),
    'G-Strings_dec': (r'\textsc{Decomp}', '-.'),
    'G-Strings_ori': (r'\textsc{PropDFA}', '-s'),
    'G-Strings_new': (r'\textsc{PropNFA}', '-*'),
    'z3seq': (r'\textsc{Z3seq}', '-o'),
    'z3noodler': (r'\textsc{Z3-Noodler}', '-o'),
    'z3noodler_mocha': (r'\textsc{Z3-Noodler-Mocha}', '-o'),
    'ostrich': (r'\textsc{OSTRICH}', '-o'),
}

for solver, vals in sorted(times.items()):
    lab = solv2lab[solver]
    labels += plt.plot(
        sorted(vals.values()),
        lab[1],
        label=lab[0],
        linewidth=3,
        markersize=10,
    )

plt.xticks(fontsize=25)
plt.yticks(fontsize=25)
plt.ylim(0, TIMEOUT * 1.25)

plt.legend(
    numpoints=2,
    handles=labels,
    loc='upper center',
    ncol=4,
    fontsize=18,
    columnspacing=1.2,
    handletextpad=0.5
)

plt.subplots_adjust(
    left=0.10,
    bottom=0.14,
    right=0.95,
    top=0.95
)

plt.subplots_adjust(left=0.10, bottom=0.14, right=0.95, top=0.78)
plt.xlabel('Sorted instances', fontsize=25)
plt.ylabel('Runtime [s]', fontsize=25)
plt.subplots_adjust(left=0.10, bottom=0.14, right=0.95, top=0.95)
plt.show()
