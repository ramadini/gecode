import csv

import matplotlib
matplotlib.rcParams['text.usetex'] = True
import matplotlib.pyplot as plt

PATH = '/home/roberto/G-Strings/gecode/gecode/string/tests/match-experiments/journal/dna_match/results_tot.log'
SOLVERS = ['G-Strings_ori', 'G-Strings_new']
MIN = True

TIMEOUT = 300.0
NUM_PROBLEMS = 25

reader = csv.reader(open(PATH), delimiter = '|')
results = dict((s, {'opt': 0.0, 'sat': 0.0, 'unk': 0.0, 'time': 0.0, 
  'ttf': 0.0, 'mznc': 0.0, 'mznc-obj': 0.0, 'score': 0.0}) for s in SOLVERS)
infos = {}
times = dict(
  (s, dict(((l,k), TIMEOUT)
  for l in [256, 512, 1024, 2048, 4096] for k in [5, 10, 15, 20, 25]))
  for s in SOLVERS
)

for row in reader:
#  print(row)
  solv = row[0]
  inst = row[1][row[1].rfind('/')+1:row[1].find('.')]
  info = row[2] 
  oval = float(row[3])
  vals = eval(row[4])
  if vals:
    ttf = min(vals.values())
    results[solv]['ttf'] += ttf
  else:
    results[solv]['ttf'] += TIMEOUT
    time = TIMEOUT
  if info == 'opt':
    time = float(row[5])
    results[solv]['sat'] += 1
    results[solv]['opt'] += 1
  elif info == 'sat':
    time = TIMEOUT
    results[solv]['sat'] += 1
  else:
    time = TIMEOUT
    results[solv]['unk'] += 1
  results[solv]['time'] += time
  if time < TIMEOUT and ttf > time:
    assert (ttf == time + 1)
    results[solv]['time'] += 1
  if inst not in infos.keys():
    infos[inst] = {'min': 1000, 'max': 0}
    for s in SOLVERS:
      infos[inst][s] = None
  infos[inst][solv] = (time, oval)
  i1 = inst.rfind('/L')
  i2 = inst. find('_K')
  lk = (int(inst[i1 + 2 : i2]), int(inst[i2 + 2:]))
  times[solv][lk] = time
  if oval < infos[inst]['min']:
    infos[inst]['min'] = oval
  if oval > infos[inst]['max']:
    infos[inst]['max'] = oval
#  print(inst,solv,info,oval,vals,infos[inst])

for val in results.values():
  val['opt']  = val['opt'] * 100 / NUM_PROBLEMS
  val['sat']  = val['sat'] * 100 / NUM_PROBLEMS
  val['unk']  = val['unk'] * 100 / NUM_PROBLEMS
  val['ttf']  /= NUM_PROBLEMS
  val['time'] /= NUM_PROBLEMS

def better(x, y):
  return (x < y and MIN) or (x > y and not MIN)

def get_score(info, l, u):
  if info[0] < TIMEOUT:
    return 1
  elif info[1] != info[1]: # isNaN
    return 0
  elif l == u:
    return 0.75
  x = (info[1] - l) / (2 * (u - l))
  assert 0 <= x <= 0.5
  if MIN:
    return 0.75 - x
  else:
    return 0.25 + x

n = len(SOLVERS)
scores = {}
for inst, info in infos.items():
  for i in range(0, n - 1):
    s_i = SOLVERS[i]
#    print(inst,info)
    sc =  get_score(info[s_i], info['min'], info['max'])
    results[s_i]['score'] += sc
    i1 = inst.rfind('/L')
    i2 = inst. find('_K')
    l = int(inst[i1 + 2 : i2])
    m = int(inst[i2 + 2:])
    if (l, m) not in scores.keys():
      scores[(l, m)] = {}
    scores[(l, m)][s_i] = sc
    for j in range(i + 1, n):
      s_j = SOLVERS[j]
      time_i = info[s_i][0]
      time_j = info[s_j][0]
      oval_i = info[s_i][1]
      oval_j = info[s_j][1]
      if better(oval_i, oval_j):
        results[s_i]['mznc'] += 1
        results[s_i]['mznc-obj'] += 1
      elif better(oval_j, oval_i):
        results[s_j]['mznc'] += 1
        results[s_j]['mznc-obj'] += 1
      elif oval_i == oval_j: # For nan vs nan.
        results[s_i]['mznc-obj'] += 0.5
        results[s_j]['mznc-obj'] += 0.5
        t = time_i + time_j
        if t > 0:
          results[s_i]['mznc'] += time_j / t
          results[s_j]['mznc'] += time_i / t
        else:
          results[s_i]['mznc'] += 0.5
          results[s_j]['mznc'] += 0.5
  s_i = SOLVERS[n - 1]
  sc = get_score(info[s_i], info['min'], info['max'])
  results[s_i]['score'] += sc
  scores[(l, m)][s_i] = sc

for val in results.values():
  assert 0 <= val['ttf'] <= TIMEOUT
  assert 0 <= val['time'] <= TIMEOUT
  assert val['ttf'] <= val['time']
  assert 0 <= val['score'] <= NUM_PROBLEMS
  assert 0 <= val['mznc'] <= (n - 1) * NUM_PROBLEMS
  assert 0 <= val['mznc-obj'] <= (n - 1) * NUM_PROBLEMS
  assert val['sat'] >= val['opt']

print ('solver & opt & sat & unk & ttf & time & score & borda & iborda \\\\')
for s, it in results.items():
  for key, val in it.items():
    it[key] = round(val, 2)
  print(s,'&',it['opt'],'&',it['sat'],'&',it['unk'],'&',it['ttf'],'&',
        it['time'],'&',it['score'],'&',it['mznc'],'&',it['mznc-obj'],'\\\\')

labels = []
solv2lab = {
  'G-Strings_ori': (r'\textsc{PropDFA}', '-s'),
  'G-Strings_new': (r'\textsc{PropNFA}', '-*')
}
xvals = None
for solver, vals in times.items():
  lab = solv2lab[solver]
  if not xvals:
    xvals = list(vals.keys())
  labels += plt.plot(list(vals.values()), lab[1], label = lab[0], linewidth=3, markersize=10)
plt.xticks(range(NUM_PROBLEMS), xvals, rotation=45, fontsize=25)
plt.yticks(fontsize=25)
plt.legend(numpoints=3, handles=labels, loc='best', fontsize=25)
plt.xlabel('(L,K)', fontsize=25)
plt.ylabel('Runtime [s]', fontsize=25)
plt.subplots_adjust(bottom=0.14, right=0.95, top=0.95)
plt.show()
