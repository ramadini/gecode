import string

MIN_N = 21
MAX_N = 40

# 26 uppercase + 14 lowercase = 40 distinct one-character symbols.
ALPHABET = list(string.ascii_uppercase + string.ascii_lowercase[:14])

SMT_PATH = 'smt/ls_'


def alphabet_re(N):
    """
    Regex denoting exactly the N symbols used by the instance.
    Keep the old compact encoding for N <= 26.
    """
    if N <= 26:
        return f'(re.range "A" "{ALPHABET[N-1]}")'

    # A-Z plus a, ..., the required lowercase character.
    last_lower = ALPHABET[N-1]
    return (
        f'(re.union '
        f'(re.range "A" "Z") '
        f'(re.range "a" "{last_lower}"))'
    )


def generateN(N, f):
    assert 2 <= N <= len(ALPHABET)

    f.write('(set-logic QF_SLIA)\n\n')

    # Row and column strings.
    for rc in ['row', 'col']:
        for k in range(N):
            f.write(f'(declare-fun {rc}_{k} () String)\n')
    f.write('\n')

    # First-match positions.
    for i in range(N):
        for j in range(N):
            for rc in ['r', 'c']:
                f.write(f'(declare-fun {rc}_{i}_{j} () Int)\n')
    f.write('\n')

    # Every row/column contains only symbols from the N-character alphabet.
    are = alphabet_re(N)
    for k in range(N):
        for rc in ['row', 'col']:
            f.write(
                f'(assert (str.in_re {rc}_{k} (re.* {are})))\n'
            )
    f.write('\n')

    # IMPORTANT: every row and every column has length N.
    for k in range(N):
        for rc in ['row', 'col']:
            f.write(f'(assert (= (str.len {rc}_{k}) {N}))\n')
    f.write('\n')

    # Every symbol must occur in every row.
    for i in range(N):
        for j in range(N):
            f.write(f'(assert (>= r_{i}_{j} 0))\n')
    f.write('\n')

    # Every symbol must occur in every column.
    for i in range(N):
        for j in range(N):
            f.write(f'(assert (>= c_{i}_{j} 0))\n')
    f.write('\n')

    # First occurrence of symbol j in row i.
    for i in range(N):
        for j in range(N):
            f.write(
                f'(assert (= r_{i}_{j} '
                f'(str.indexof row_{i} "{ALPHABET[j]}" 0)))\n'
            )
    f.write('\n')

    # First occurrence of symbol j in column i.
    for i in range(N):
        for j in range(N):
            f.write(
                f'(assert (= c_{i}_{j} '
                f'(str.indexof col_{i} "{ALPHABET[j]}" 0)))\n'
            )
    f.write('\n')

    # Row-column channeling.
    for symbol in range(N):
        for i in range(N):
            for j in range(N):
                f.write(
                    f'(assert (= '
                    f'(= r_{i}_{symbol} {j}) '
                    f'(= c_{j}_{symbol} {i})))\n'
                )

    f.write('\n(check-sat)\n')


for N in range(MIN_N, MAX_N + 1):
    with open(SMT_PATH + str(N) + '.smt2', 'w') as f:
        generateN(N, f)
