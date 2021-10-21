% Coerces (Tx, Sx) to a normalised string value.
function var string:
to_num(var JSType: Tx, var string: Sx) ::promise_total =
let {
  var string: x = [[[[[[
    "NaN", 
    "0"][(Tx = Array) + 1], %FIXME: Approximation for arrays.
    [["NaN", [norm(Sx), Sx][is_norm(Sx) + 1]][is_num(Sx) + 1], "0"]
      [str_reg(Sx, "( |\t|\n)*") + 1]][(Tx = Str) + 1], 
    Sx][(Tx = Num) + 1], 
    ["0", "1"][(Sx = "true") + 1]][(Tx = Bool) + 1], 
    "NaN"][(Tx = Undef) + 1], 
    "0"][(Tx = Null) + 1];
  constraint is_norm(x);
} in x;

% -0 is not a normalized number, but it is needed for handling division by zero.
function var string:
to_num0(var JSType: Tx, var string: Sx) ::promise_total = 
  [to_num(Tx, Sx), "-0"][str_reg(Sx, MZERO) + 1];

% Normalising a not-normalised string Sx.
function var string: %FIXME: Deal with hexadecimal, exponential, floats...
norm(var string: Sx) ::promise_total = 
let {
  % d is the most significant digit of Sx.
  var string(1): d; 
  constraint str_reg(d, "[1-9]");
  var 0..str_ub(Sx): i = str_find(d, Sx);
  var string: Nx = [[[
    "NaN",
    ["", "-"][is_neg(Sx) + 1] ++ str_sub(Sx, i, str_len(Sx))][(i > 1) + 1], 
    "0"][str_reg(Sx, ZERO) + 1], 
    "Infinity"][(Sx = "+Infinity") + 1];
  constraint is_norm(Nx);
} in Nx;
  
% Keeps n decimals in normalised numeric string x.
function var string:
to_fixed(var string: x, var 0..100: n) = 
  [x, x ++ "." ++ str_pow("0", n)][(n > 0 /\ is_int(x)) + 1];

% Coerces (Tx, Sx) to a Boolean variable.
function var bool:
to_bool_var(var JSType: Tx, var string: Sx) ::promise_total = 
  is_obj(Tx) \/ (Tx = Bool /\ Sx = "true") \/ 
  (Tx = Num /\ Sx != "NaN" /\ not(str_reg(Sx, ZERO))) \/ (Tx = Str /\ Sx != "");

% Coerces (Tx, Sx) to a Boolean string value.
var string:
to_bool(var JSType: Tx, var string: Sx) ::promise_total = 
  ["false", "true"][to_bool_var(Tx, Sx) + 1];


