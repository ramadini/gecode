MIN_N = 2
MAX_N = 26
ALPHABET = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"]
#SORTED = ''.join(ALPHABET)
SMT_PATH = '/home/roberto/papers/regular_match/experiments/latin_square/smt/ls_'

def generateN(N, f):  
  for rc in ['row', 'col']:
    for k in range(N):
      f.write(f'(declare-fun {rc}_{k} () String)\n')
  f.write('\n')
  for i in range(N):    
    for j in range(N):
      for rc in ['r', 'c']:
        f.write(f'(declare-fun {rc}_{i}_{j} () Int)\n')
  f.write('\n')
  for k in range(2, N):
    for rc in ['row', 'col']:
      f.write(f'(assert (str.in_re {rc}_{k} (re.* (re.range "A" "{ALPHABET[N-1]}"))))\n')
  f.write('\n')
  for k in range(2, N):
    for rc in ['row', 'col']:
      f.write(f'(assert (= (str.len {rc}_{k}) {N}))\n')
  f.write('\n')
  for i in range(N):
    for j in range(N):
      f.write(f'(assert (>= r_{i}_{j} 0))\n')
  f.write('\n')
  for i in range(N):
    for j in range(N):
      f.write(f'(assert (>= c_{i}_{j} 0))\n')
  f.write('\n')
  for i in range(N):
    for j in range(N):
      f.write(f'(assert (= r_{i}_{j} (str.indexof row_{i} "{ALPHABET[j]}" 0)))\n')
  f.write('\n')
  for i in range(N):
    for j in range(N):
      f.write(f'(assert (= c_{i}_{j} (str.indexof col_{i} "{ALPHABET[j]}" 0)))\n')
  f.write('\n')
  for c in range(N):
    for i in range(N):
      for j in range(N):      
        f.write(f'(assert (= (= r_{i}_{c} {j}) (= c_{j}_{c} {i})))\n')
  f.write('\n(check-sat)\n')

for i in range(MIN_N, MAX_N+1):
  with open(SMT_PATH + str(i) + '.smt2', 'w') as f:
    generateN(i, f)

