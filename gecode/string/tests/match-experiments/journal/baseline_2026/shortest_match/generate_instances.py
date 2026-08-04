from random import randint, choice

DZN_PATH = '/home/roberto/G-Strings/gecode/gecode/string/tests/match-experiments/journal/shortest_match/dzn/sm_'
SMT_PATH = '/home/roberto/G-Strings/gecode/gecode/string/tests/match-experiments/journal/shortest_match/smt/sm_'

def randchar(idx = sorted(set.union(set(range(48,58)),
                                    set(range(65,91)),
                                    set(range(97,123))))):
  return chr(idx[randint(0, len(idx)-1)])

def output_dzn(L, N, W):
  # Output to dzn:
  with open(DZN_PATH + str(L) + '_' + str(N) + '.dzn', 'w') as f:
    f.write("L = " + str(L) + ";\nN = " + str(N) + ";\n")
    f.write("PATTERNS = [\n")
    for w in W:
      f.write('\t"' + w + '",\n')
    f.write("]")
  
def parse_match(tokens):
  re = '(re.++ re.all'
  for token in tokens:
    if '|' in token:
      if '*' in token:
        re += ' (re.*'
        star = True
      else:
        star = False
      re += ' (re.union'
      for t in token.split('|'):
        re += f' (str.to_re "{t}")'
      re += ')'
      if star:
        re += ')'
    else:
      re += f' (str.to_re "{token}") '
  return re + ' re.all)'
  
def output_smt(L, N, W):
  # Output to smtlib:
  with open(SMT_PATH + str(L) + '_' + str(N) + '.smt2', 'w') as f:
    f.write('(declare-fun L () Int)\n(declare-fun W () String)\n')
    f.write(f'(assert (= (str.len W) L))\n')
    f.write(f'(assert (<= L {L}))\n')
    for w in W:
      tokens = w.replace('(',' ').replace(')*','* ').replace(')', ' ').split()
      f.write(f'(assert (str.in_re W {parse_match(tokens)}))\n')
    f.write('(check-sat)\n')

for L in [32, 64, 128, 256, 512]:
  for N in [5, 10, 15, 20, 25]:
    if L < 512 and N < 25:
      continue
    w = ''.join([randchar() for _ in range(0, L)])
    indexes = [0, 0]
    while (len(indexes) > len(set(indexes))):
      indexes = sorted([randint(1, L - 1) for _ in range(0, N - 1)])
    strings = [w[0 : indexes[0]]]
    indexes = indexes + [L]
    strings += [w[indexes[i] : indexes[i + 1]] for i in range(0, N - 1)]
    assert len(strings) == N and sum(len(s) for s in strings) == L
    for i in range(0, N - 1):
      for j in range(i + 1, N):
        choice = randint(0, 1)
        if choice:
          # Force an overlap between string[i] and string[j].
          choice = randint(0, 2)
          li = len(strings[i])
          lj = len(strings[j])
          assert li > 0 and lj > 0
          if (choice == 0):
            if li < lj:
              if strings[i] not in strings[j]:
                # i within j
                n = randint(0, lj - li)
                strings[j] = strings[j][:n] + strings[i] + strings[j][n + li:]
            elif strings[j] not in strings[i]:
              # j within i
              n = randint(0, li - lj)
              strings[i] = strings[i][:n] + strings[j] + strings[i][n + lj:]
          elif (choice == 1):
            # i before j
            n = randint(1, min(li, lj))
            strings[j] = strings[i][-n:] + strings[j][n:]
          else:
            # j before i
            n = randint(1, min(li, lj))
            strings[i] = strings[j][-n:] + strings[i][n:]
    assert len(strings) == N and sum(len(s) for s in strings) == L
    print(L,N)
    patterns = []
    for w in strings:
      print ('Before: ' + w)
      n = randint(1, max(1,len(w) // 5)) * 2
      indexes = [0, 0]
      while len(indexes) != len(set(indexes)):
        indexes = sorted([randint(0, len(w)) for _ in range(n)])
      for i in range(0, len(indexes)-1, 2):
        j = i + 1
        idx_i = indexes[i]
        idx_j = indexes[j]
        assert idx_i != idx_j
        w1 = '(' + w[idx_i : idx_j]
        assert (len(w1) > 0)
        n = 1
        for _ in range(randint(1, 5)):
          ww = ''.join(randchar() for _ in range(idx_j-idx_i))
          w1 += '|' + ww
          n += 1 + len(ww)
        if randint(0, 1):
          w1 += ')*'
          n += 2
        else:
          w1 += ')'
          n += 1
        w = w[:idx_i] + w1 + w[idx_j:]
        for k in range(j + 1, len(indexes)):
          indexes[k] += n
      print ('After: ' + w)
      patterns += [w]
    assert len(patterns) == N
    
    # Output.
    output_dzn(L, N, patterns)
    output_smt(L, N, patterns)

