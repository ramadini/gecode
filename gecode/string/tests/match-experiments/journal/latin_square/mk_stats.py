import csv

import matplotlib
matplotlib.rcParams['text.usetex'] = True
import matplotlib.pyplot as plt

PATH = '/home/roberto/G-Strings/gecode/gecode/string/tests/match-experiments/journal/latin_square/results_tot.log'
SOLVERS = ['cvc5', 'G-Strings_ori', 'G-Strings_dec', 'G-Strings_new', 'z3seq']

TIMEOUT = 300.0
NUM_PROBLEMS = 39

reader = csv.reader(open(PATH), delimiter = '|')
results = dict((s, {'sat': 0.0, 'unk': 0.0, 'time': 0.0}) for s in SOLVERS)
infos = {}
times = dict(
  (s, dict((str(i), TIMEOUT) for i in range(2, NUM_PROBLEMS + 1))) for s in SOLVERS
)



for row in reader:
#  print(row)
  solv = row[0]
  if solv == 'z3str':
    continue
  inst = row[1] 
  if inst not in infos.keys():
    infos[inst] = {}
    for s in SOLVERS:
      infos[inst][s] = 0.0
  if row[2] == 'sat':
    time = float(row[3])
    results[solv]['sat'] += 1
  else:
    time = TIMEOUT
    results[solv]['unk'] += 1
  results[solv]['time'] += time
  infos[inst][solv] = time
#  print (solv, inst, time)
  times[solv][inst] = time

print (results)
print (times)

labels = []
solv2lab = {
  'cvc5': (r'\textsc{CVC5}', '-.'), 
  'G-Strings_dec': (r'\textsc{Decomp}', '-s'),
  'G-Strings_ori': (r'\textsc{PropDFA}', '-o'),
  'G-Strings_new': (r'\textsc{PropDFA}', '-^'),
  'z3seq': (r'\textsc{Z3seq}', '-*'),
}
for solver, vals in sorted(times.items()):
  lab = solv2lab[solver]
  labels += plt.plot(list(vals.values()), lab[1], label = lab[0], linewidth=3)
plt.xticks(
  range(NUM_PROBLEMS-1), range(2,NUM_PROBLEMS+1), rotation=45, fontsize=35
)
plt.yticks(fontsize=40)
plt.legend(numpoints=3, handles=labels, loc='best', fontsize=32)
plt.xlabel('n', fontsize=50)
plt.ylabel('runtime [s]', fontsize=50)
plt.subplots_adjust(left=0.10, bottom=0.14, right=0.95, top=0.95)
plt.show()
