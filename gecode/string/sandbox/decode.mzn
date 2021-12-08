%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Given a fixed string W_in of n ASCII characters, and a fixed string W_out of %
% m >= n bits, determine if there is a prefix-code (possibly having variable   %
% length) mapping W_in to (a substring of) W_out.                              %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% True iff the code is fixed-length.
bool: FIXED_LENGTH = true;
% Input string.
string: W_in = "cassaforte";
int: n = string_length(W_in);
% Output string;
string: W_out = "010011000110000100100000011000110110000101110011011100110110000101100110011011110111001001110100011001010010000001100101001001110010000001100100011010010110010101110100011100100110111100100000011000010110110000100000011100010111010101100001011001000111001001101111";
int: m = string_length(W_out);
% Number of distinct characters of the input string W_in.
int: d = 8;
% Distinct characters of W_in, as they occur from left to right.
array[1..d] of string: C = ["c", "a", "s", "f", "o", "r", "t", "e"];
% C[I[j]] = W_in[j] for j = 1, ..., n.
array[1..n] of int: I = [1, 2, 3, 3, 2, 4, 5, 6, 7, 8];
% Binary string x[i] encodes symbol C[i] for i = 1, ..., d.
array[1..d] of var string(16) of {"0", "1"}: x;
% y[i] is the string resulting from W_in after encoding symbols x[1], ..., x[i].
array[0..d] of var string: y;
% Binary string z[i] encodes W_in[i] for i = 1, ..., n.
array[1..n] of var string: z = [x[I[j]] | j in 1..n];
% Index of the first occurrence of y[d] in W_out.
var 1..m: idx;

constraint y[0] = W_in;
constraint forall (i in 1..d) (
  str_len(x[i]) > 0 /\ 
  str_find(W_out,x[i]) > 0 /\
  y[i] = str_replace_all(y[i - 1], C[i], x[i]) /\
  if i < d then
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
    str_find(x[j],x[i]) != 1
  )
);

array[1..n] of var string: cz;
constraint cz[1] = z[1] /\ forall (i in 2..n) (cz[i] = cz[i-1] ++ z[i]) /\
           cz[n] = y[d];
constraint idx = str_find(W_out,y[d]);

% FIXME:
constraint x[1] = "01100011";
constraint x[2] = "01100001"; 
constraint x[3] = "01110011"; 
constraint x[4] = "01100110"; 
constraint x[5] = "01101111"; 
constraint x[6] = "01110010"; 
constraint x[7] = "01110100"; 
%constraint x[8] = "01100101";
constraint str_find(W_out,y[d]) = 25;

solve satisfy;

