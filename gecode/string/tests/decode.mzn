%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Given a fixed strings W_in of n ASCII characters, and and fixed string W_out %
% of m >= n bits, determine if there exists a prefix-code mapping W_in to (a   %
% substring of) W_out.                                                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% True iff the code is fixed-length.
bool: FIXED_LENGTH = false;
% Input string.
string: W_in = "ciccia";
int: n = string_length(W_in);
% Output string;
string: W_out = "000110110100101100011011000110110100101100001";
int: m = string_length(W_out);
% Number of distinct characters of the input string W_in.
int: d = 3;
% Distinct characters of W_in, as they occur from left to right.
array[1..d] of string: C = ["c", "i", "a"];
% C[I[j]] = W_in[j] for j = 1, ..., n.
array[1..n] of int: I = [1, 2, 1, 1, 2, 3];
% Binary string x[i] encodes symbol C[i] for i = 1, ..., d.
array[1..d] of var string of {"0", "1"}: x;
% y[i] is the string resulting from W_in after encoding symbols x[1], ..., x[i].
array[0..d] of var string: y;
% Binary string z[i] encodes W_in[i] for i = 1, ..., n.
array[1..n] of var string: z = [x[I[j]] | j in 1..n];
% Index of the first occurrence of y[d] in W_out.
var 1..n: idx;

constraint y[0] = W_in;
constraint forall (i in 1..d) (
  str_len(x[i]) > 0 /\ 
  str_find(x[i], W_out) > 0 /\
  y[i] = str_replace_all(C[i], x[i], y[i - 1]) /\
  if i < d then
    str_find(x[i + 1], W_out) >= str_find(x[i], W_out) + str_len(x[i]) /\
    if FIXED_LENGTH then
      str_len(x[i]) = str_len(x[i + 1])
    else
      true
    endif
  else 
    true
  endif /\
  % Imposing the prefix property.
  forall (j in 1..d where i != j) (
    str_find(x[i], x[j]) != 1
  )
);
constraint y[d] = str_gconcat(z);
constraint idx = str_find(y[d], W_out);

solve satisfy;

