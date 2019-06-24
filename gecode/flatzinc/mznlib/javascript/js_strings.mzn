% FIXME: Just a workaround atm.
function var string:
js_split(var string: x, string: sep) ::promise_total = 
let {
  var string of {","}: y;
  constraint str_len(y) = 2 * str_gcc(x, [sep])[1] + 1;
} in y;
%let {
%  array[int] of var string: A = str_split(x, sep, len);
%  var string: y = str_gconcat([
%    [A[i] ++ ",", ""][(A[i] = "\\x00") + 1] | i in index_set(A)
%  ]);
%} in str_sub(y, 1, str_len(y) - 1);

% Modelling x.slice(i, j).
function var string:
js_str_slice(var string: Sx, var string: Ni, var JSType: Tj, var string: Sj) 
::promise_total = 
let {
  var string: Nj = to_num(Tj, Sj);
  var 0..ub(str_len(Sx)): i = 
    if Ni = "NaN" \/ Ni = "-Infinity" then
      -1
    elseif is_neg(Ni) then
      max(0, str_len(Sx) - str2nat(negate(Ni)))
    else
      str2nat(Ni)
    endif;
  var 0..ub(str_len(Sx)): j = 
    if Nj = "Infinity" \/ Tj = Undef then
      str_len(Sx)
    elseif Ni = "Infinity" \/ Nj = "-Infinity" \/ Nj = "NaN" \/ Nj = "0" then
      0
    elseif is_neg(Nj) then
      str_len(Sx) - str2nat(negate(Nj))
    else
      str2nat(Nj)
    endif;
} in str_sub(Sx, i + 1, j);   
         
% Modelling x.substr(i, j).
function var string:
js_substr(var string: Sx, var string: Ni, var JSType: Tj, var string: Sj) 
::promise_total = 
let {
  var string: Nj = to_num(Tj, Sj);
  var 0..ub(str_len(Sx)): i = 
    if Ni = "NaN" \/ Ni = "-Infinity" then
      -1
    elseif is_neg(Ni) then
      max(0, str_len(Sx) - str2nat(negate(Ni)))
    else
      str2nat(Ni)
    endif;
  var 0..ub(str_len(Sx)): j = 
    if Nj = "Infinity" \/ Tj = Undef then
      str_len(Sx)
    elseif Ni = "Infinity" \/ Nj = "-Infinity" \/ Nj = "NaN" \/ Nj = "0" then
      0
    elseif is_neg(Nj) then
      str_len(Sx) - str2nat(negate(Nj))
    else
      str2nat(Nj)
    endif;
} in str_sub(Sx, i + 1, i + j);

% Modelling x.charCodeAt(i).
function var string:
js_char_code_at(var string: x, var string: i) ::promise_total =
let {
  var int: j = str2nat(i) + 1;
} in ["NaN", nat2str(str_code_at(x, j))][(0 < j /\ j <= str_len(x)) + 1]


