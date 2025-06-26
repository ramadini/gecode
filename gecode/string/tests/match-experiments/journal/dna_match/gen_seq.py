import json
import random

MOTIFS = [
    "TATA(W|A)(A|G)R",               # TBP
    "GGCCAATCT",                     # CAAT-box
    "GGGCGG",                        # SP1
    "GGG(R|A|G)(N|A|T|C|G)(N|A|T|C|G)(Y|C|T)(Y|C|T)CC",  # NF-κB
    "TGA(N|A|T|C|G)TCA",             # AP-1
    "TGACGTCA",                      # CREB
    "GTTAAT(N|A|T|C|G)ATTAAC",       # HNF1
    "TTT(S|C|G)(S|C|G)CGC",          # E2F
    "(A|G)(C|T)AAA(A|C|T)",          # FOX
    "(A|T)GATA(A|G)",                # GATA
    "ATGCAAAT",                      # OCT
    "CATTGT",                        # SOX
    "CACGTG",                        # MYC (E-box)
    "GGAAA",                         # NFAT
    "TTC(N|A|T|C|G)(N|A|T|C|G)(N|A|T|C|G)GAA",  # STAT
    "(A|G)(A|G)(A|G)C(A|T)(A|T)G(C|T)(C|T)(C|T)",  # p53 half-site
    "AGGTCA(N|A|T|C|G)AGGTCA",       # HNF4
    "AGGTCA(N|A|T|C|G)(N|A|T|C|G)(N|A|T|C|G)TGACCT",    # ESR1
    "AGAACA(N|A|T|C|G)(N|A|T|C|G)(N|A|T|C|G)TGTTCT",    # Androgen R
    "GGTTATGGAATTCCC",               # ZNF143
    "CCGCG(N|A|T|C|G)GG(N|A|T|C|G)GGCAG",  # CTCF
    "(C|T)TA(A|T)(A|T)(A|T)(A|T)TA(A|G)",  # MEF2
    "TG(C|T)GGT",                    # RUNX
    "(A|G)CGTG",                     # HIF
    "GGGAGGG"                        # MAZ
]

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


