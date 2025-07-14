%Problem: Least Matching Motifs.
%Input:
%  A set of N sequences S each having fixed length L (defined by a pattern S)
%  A set of K motifs M = {M_1​,...,M_K​}
%Objective:
%  Find a string matching the least motifs from M, and if at least 2 match must
% occur, find the minimal span between max and min matching position.

include "globals.mzn";

% Parameters.
%int: L;
%int: K;
%string: S;
%array[int] of string: M;
%bool: newM;
bool: newM = true;
int: L = 17;
int: K = 3;
string: S = "CAC(T|G)TGCGTGGATAAAC";
array[int] of string: M = [ "CACGTG", "(A|G)CGTG", "(A|G)(C|T)AAA(A|C|T)"];

var string: target;
array[1..K] of var 0..L: matches;
constraint matches = [
  if newM then str_match_new(target, M[i]) else str_match(target, M[i]) endif
  | i in 1..K
];

constraint str_len(target) = L;
constraint str_reg(target, S);

var 0..L: obj; 

constraint obj =
  let {
    var 0..L: maxM = max(matches);
    % L+1 is a special value to avoid minM = 0 when 0 < count(matches, 0) < K.
    array[int] of var 0..L+1: m0 = [[m, L+1][(m = 0) + 1] | m in matches];
    var 0..L+1: minM = min(m0);
    % At least 2 matches (or at most K-2 non-matches).
    var bool: atLeast2 = (count(matches, 0) < K - 1);   
  } in atLeast2 * (maxM - minM);

solve minimize obj;



