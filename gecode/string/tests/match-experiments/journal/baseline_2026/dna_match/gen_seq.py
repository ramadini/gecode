import json
import random

MOTIFS = [
    "TATA(A|T)(A|G)(A|G)",                         # TBP (W=A/T, R=A/G)
    "GGCCAATCT",                                   # CAAT-box
    "GGGCGG",                                      # SP1
    "GGG(A|G)(A|C|G|T)(A|C|G|T)(C|T)(C|T)CC",      # NF-κB
    "TGA(A|C|G|T)TCA",                             # AP-1
    "TGACGTCA",                                    # CREB
    "GTTAAT(A|C|G|T)ATTAAC",                       # HNF1
    "TTT(C|G)(C|G)CGC",                            # E2F (S=C/G)
    "(A|G)(C|T)AAA(C|T)A",                         # FOX
    "(A|T)GATA(A|G)",                              # GATA
    "ATGCAAAT",                                    # OCT
    "CATTGT",                                      # SOX
    "CACGTG",                                      # MYC (E-box)
    "GGAAA",                                       # NFAT
    "TTC(A|C|G|T)(A|C|G|T)(A|C|G|T)GAA",           # STAT
    "(A|G)(A|G)(A|G)C(A|T)(A|T)G(C|T)(C|T)(C|T)",  # p53 half-site
    "AGGTCA(A|C|G|T)AGGTCA",                       # HNF4
    "AGGTCA(A|C|G|T)(A|C|G|T)(A|C|G|T)TGACCT",     # ESR1
    "AGAACA(A|C|G|T)(A|C|G|T)(A|C|G|T)TGTTCT",     # Androgen R
    "GGTTATGGAATTCCC",                             # ZNF143
    "CCGCG(A|C|G|T)GG(A|C|G|T)GGCAG",              # CTCF
    "(C|T)TA(A|T)(A|T)TA(A|G)",                    # MEF2
    "TG(C|T)GGT",                                  # RUNX
    "(A|G)CGTG",                                   # HIF
    "GGGAGGG"                                      # MAZ
]
for motif in MOTIFS:
  for base in motif:
    assert base in {'A', 'C', 'G', 'T', '|', '(', ')'}

def generate_degenerate_sequence(L, degenerate_fraction=0.1):
    bases = ['A', 'C', 'G', 'T']
    num_degenerate = int(L * degenerate_fraction)
    # Choose random positions for degenerate bases
    degenerate_positions = set(random.sample(range(L), num_degenerate))
    sequence = []
    for i in range(L):
        if i in degenerate_positions:
            # Randomly choose 2 to 4 unique bases for the degenerate position
            k = random.randint(2, 4)
            alleles = random.sample(bases, k)
            token = f"({'|'.join(alleles)})"
        else:
            token = random.choice(bases)
        sequence.append(token)    
    return ''.join(sequence)

P = 0.05
DZN_FOLDER = '/home/roberto/G-Strings/gecode/gecode/string/tests/match-experiments/journal/dna_match/dzn'
for L in [128, 256, 512, 1024, 2048, 4096]:
  for K in [5, 10, 15, 20, 25]:
    with open(f'{DZN_FOLDER}/L{L}_K{K}.dzn', 'w') as dzn:
      dzn.write(f'L = {L};\n')
      dzn.write(f'K = {K};\n')
      dzn.write(f'S = "{generate_degenerate_sequence(L, P)}";\n')
      dzn.write(f'M = {json.dumps(random.sample(MOTIFS, K))};\n')


