import csv

import matplotlib
matplotlib.rcParams['text.usetex'] = True
import matplotlib.pyplot as plt

PATH = '/home/roberto/G-Strings/gecode/gecode/string/tests/match-experiments/journal/latin_square/results_tot.log'
SOLVERS = ['cvc5', 'G-Strings_ori', 'G-Strings_dec', 'G-Strings_new', 'z3seq']
MIN = False
TIMEOUT = 300
NUM_PROBLEMS = 40 - 2 + 1

reader = csv.reader(open(PATH), delimiter = '|')
results = dict((s, {'sat': 0.0, 'unk': 0.0, 'time': 0.0, 'mznc': 0.0}) for s in SOLVERS)
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
      infos[inst][s] = TIMEOUT
  if row[2] == 'sat':
    time = int(row[3])
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

def better(x, y):
  return (x < y and MIN) or (x > y and not MIN)


n = len(SOLVERS)
scores = {}
for inst, info in infos.items():
  for i in range(0, n - 1):
    s_i = SOLVERS[i]
#    print(inst,info)
    for j in range(i + 1, n):
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
  s_i = SOLVERS[n - 1]

for solv,val in sorted(results.items(), key = lambda x : -x[1]['sat']):
  print (solv,val)
  assert 0 <= val['time'] <= TIMEOUT * NUM_PROBLEMS
  assert 0 <= val['mznc'] <= (n - 1) * NUM_PROBLEMS

labels = []
solv2lab = {
  'cvc5': (r'\textsc{CVC5}', '-^'), 
  'G-Strings_dec': (r'\textsc{Decomp}', '-.'),
  'G-Strings_ori': (r'\textsc{PropDFA}', '-s'),
  'G-Strings_new': (r'\textsc{PropNFA}', '-*'),
  'z3seq': (r'\textsc{Z3seq}', '-o'),
}
for solver, vals in sorted(times.items()):
  lab = solv2lab[solver]
  labels += plt.plot(list(sorted(vals.values())), lab[1], label = lab[0], linewidth=3, markersize=10)
plt.xticks(
#  range(NUM_PROBLEMS-1), range(2,NUM_PROBLEMS+1), rotation=45, 
  fontsize=25
)
plt.yticks(fontsize=25)
plt.legend(numpoints=3, handles=labels, loc='best', fontsize=25)
plt.xlabel('Sorted instances', fontsize=25)
plt.ylabel('Runtime [s]', fontsize=25)
plt.subplots_adjust(left=0.10, bottom=0.14, right=0.95, top=0.95)
plt.show()
