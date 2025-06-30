%Problem: Least Matching Motifs.
%Input:
%  A set of N sequences S each having fixed length L (defined by a pattern S)
%  A set of K motifs M = {M_1​,...,M_K​}
%Objective:
%  Find a string matching the least motifs from M.

include "globals.mzn";

% Parameters.
int: L;
int: K;
string: S;
array[int] of string: M;
bool: newM;

var string: target;
array[1..K] of var 0..L: matches;
constraint matches = [
  if newM then str_match_new(target, M[i]) else str_match(target, M[i]) endif
  | i in 1..K
];

constraint str_len(target) = L;
constraint str_reg(target, S);

var int: obj; constraint obj = K - count(matches, 0);
solve minimize obj;

%import random
%
%%def generate_degenerate_sequence(L, degenerate_fraction=0.05):
%    bases = ['A', 'C', 'G', 'T']
%    num_degenerate = int(L * degenerate_fraction)
%    
%    % Choose random positions for degenerate bases
%    degenerate_positions = set(random.sample(range(L), num_degenerate))
%    
%    sequence = []
%    for i in range(L):
%        if i in degenerate_positions:
%            % Randomly choose 2 to 4 unique bases for the degenerate position
%            k = random.randint(2, 4)
%            alleles = random.sample(bases, k)
%            token = f"({'|'.join(alleles)})"
%        else:
%            token = random.choice(bases)
%        sequence.append(token)
%    
%    return ''.join(sequence)
%
%% Example usage:
%degenerate_seq = generate_degenerate_sequence(512)
%print(degenerate_seq)









