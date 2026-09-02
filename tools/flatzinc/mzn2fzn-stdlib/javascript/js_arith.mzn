% Pattern for denoting numerical strings.
string: HEX = "(0(x|X)([0-9]|[a-f]|[A-F])([0-9]|[a-f]|[A-F])*)";
string: EXP = "((e|E)(()|+|-)[0-9][0-9]*)";
string: DC1 = "[0-9][0-9]*\\.[0-9]*(" ++EXP++ "|())";
string: DC2 = "\\.[0-9][0-9]*(" ++EXP++ "|())";
string: DC3 = "[0-9][0-9]*(" ++EXP++ "|())";
string: DEC = "((()|+|-)(Infinity|(" ++DC1++ ")|(" ++DC2++ ")|(" ++DC3++ ")))";
string: NUM = "NaN|(" ++HEX++ ")|(" ++DEC++ ")";

% Different ways of expressing zero.
string: ZERO = "((()|+|-)00*)|((()|+|-)00*\\.0*)|(0(x|X)00*)";
string: MZERO = "(-00*)|(-00*\\.0*)|(-0(x|X)00*)";

% FIXME: Not dealing with floats, hexadecimals, exponentials atm
string: INT = "(0|(()|-)[1-9][0-9]*)";
string: FLOAT = "(()|-)(0|[1-9][0-9]*)\\.([0-9]*[1-9]|0)";
string: NORM_NUM = INT ++ "|(()|-)Infinity|NaN";

% Helpers.
predicate is_num(var string: x) = str_reg(x, NUM);
predicate is_norm(var string: x) = str_reg(x, NORM_NUM);
predicate is_int(var string: x) = str_reg(x, INT);
predicate is_neg(var string: x) = str_char_at(x, 1) = "-";
predicate is_nat(var string: x) = str_reg(x, "(0|[1-9][0-9]*)");
predicate is_inf(var string: x) = str_reg(x, "(()|-)Infinity");
predicate is_nint(var string: x) = not(is_int(x));
predicate same_sign(var string: x, var string: y) = is_neg(x) = is_neg(y);
predicate is_finite(var JSType: t, var string: x) = t = Num /\ is_int(x);
predicate is_nan(var JSType: t, var string: x) = t = Num /\ x = "NaN";

% Negates a normalised numeric string x.
function var string:
negate(var string: x) ::promise_total = 
let {
  var string: y = [[
    "-" ++ x, str_sub(x, 2, str_len(x))][is_neg(x) + 1], 
    x][str_reg(x, "0|NaN") + 1];
  constraint is_norm(y);
} in y;

% Channeling str -> int.
% FIXME: -1 value is returned for both "-1" and non-integer expressions!
function var int:
str2int(var string: x) ::promise_total = 
  if is_neg(x) then
    -str2nat(negate(x))
  else
    str2nat(x)
  endif;
  
% Channeling int -> str.
function var string:
int2str(var int: n) ::promise_total = 
let {
  var string: x = [nat2str(n), "-" ++ nat2str(-n)][(n < 0) + 1];
  constraint is_int(x);
} in x;

% Strict equality, x === y.
predicate
strict_eq(
  var JSType: Tx, var string: Sx, var 0..N_ADDRS: Ax,
  var JSType: Ty, var string: Sy, var 0..N_ADDRS: Ay,
) = Tx = Ty /\ Sx = Sy /\ ((is_obj(Tx) \/ is_obj(Ty)) -> Ax = Ay) /\
   (Tx = Num -> Sx != "NaN") /\ (Ty = Num -> Sy != "NaN");
    
% Abstract equality, x == y.
predicate
abstract_eq(
  var JSType: Tx, var string: Sx, var 0..N_ADDRS: Ax,
  var JSType: Ty, var string: Sy, var 0..N_ADDRS: Ay,
) =
  (Tx in {Null, Undef} <-> Ty in {Null, Undef}) /\
  if (Tx = Bool /\ Ty = Bool) \/ (Tx = Str /\ Ty = Str) then
    Sx = Sy
  elseif Tx = Num /\ Ty = Num then
    Sx != "NaN" /\ Sy != "NaN" /\ Sx = Sy    
  elseif is_obj(Tx) \/ is_obj(Ty) then
    Ax != Ay -> (Sx = "" /\ Sy = "")
  else
    Sx != "NaN" /\ Sy != "NaN" /\ to_num(Tx, Sx) = to_num(Ty, Sy)
  endif;

% Unary minus.
function var string:
u_minus(var JSType: Tx, var string: Sx) ::promise_total = negate(to_num(Tx,Sx));
  
% Unary plus.
function var string:
u_plus(var JSType: Tx, var string: Sx) ::promise_total = to_num(Tx, Sx);

% Binary plus, z = x + y.
function var JSType:
plus_TYPE(var JSType: Tx, var JSType: Ty) ::promise_total =
let {
  var {Num, Str}: Tz;
  constraint (Tz = Num) = 
    (Tx in {Null, Undef, Bool, Num} /\ Ty in {Null, Undef, Bool, Num});
} in Tz;
function var string:
plus_SVAL(var JSType: Tx, var string: Sx, var JSType: Ty, var string: Sy) 
::promise_total = let {
  var string: Sz = 
  [Sx ++ Sy, add(to_num(Tx, Sx), to_num(Ty, Sy))]
    [(Tx in {Null, Undef, Bool, Num} /\ Ty in {Null, Undef, Bool, Num}) + 1];
} in Sz;

%FIXME: Not considering floats, hexs, exps, number overflows atm.

% Addition, z = x + y.
function var string:
add(var string: x, var string: y) ::promise_total =
let {
  var string: z;
  constraint is_norm(x) /\ is_norm(y) /\ is_norm(z) /\ 
    ((is_int(x) /\ is_int(y)) <-> is_int(z)) /\ add(x, y, z);
} in z;
predicate
add(var string: x, var string: y, var string: z) = 
  (x = "0" -> z = y) /\ (y = "0" -> z = x) /\ (z = "0" -> y = negate(x)) /\
  (is_nint(x) -> (z = x \/ (z = "NaN" /\ x = negate(y)))) /\
  (is_nint(y) -> (z = y \/ (z = "NaN" /\ y = negate(x)))) /\
  ((is_int(x) /\ is_int(y) /\ is_int(z)) -> (
    x = int2str(str2int(z) - str2int(y)) /\
    y = int2str(str2int(z) - str2int(x)) /\
    z = int2str(str2int(x) + str2int(y))
  ));

% Subtraction, z = x - y;
function var string:
sub(var string: x, var string: y) ::promise_total =
let {
  var string: z;
  constraint is_norm(x) /\ is_norm(y) /\ is_norm(z) /\ 
    ((is_int(x) /\ is_int(y)) <-> is_int(z)) /\ sub(x, y, z);
} in z;
predicate
sub(var string: x, var string: y, var string: z) = 
  (x = "0" -> z = negate(y)) /\ (y = "0" -> z = x) /\ (z = "0" -> y = x) /\
  (is_nint(x) -> (z = x \/ (z = "NaN" /\ x = y))) /\
  (is_nint(y) -> (z = y \/ (z = "NaN" /\ y = x))) /\
  ((is_int(x) /\ is_int(y) /\ is_int(z)) -> (
    x = int2str(str2int(z) + str2int(y)) /\
    y = int2str(str2int(x) - str2int(z)) /\
    z = int2str(str2int(x) - str2int(y))
  ));
    
% Multiplication, z = x * y.
function var string:
mul(var string: x, var string: y) ::promise_total =
let {
  var string: z;
  constraint is_norm(x) /\ is_norm(y) /\ is_norm(z) /\ 
  ((is_int(x) /\ is_int(y)) <-> is_int(z)) /\ mul(x, y, z);
} in z;
predicate
mul(var string: x, var string: y, var string: z) = 
  (x = "1" -> z = y) /\ (y = "1" -> z = x) /\ 
  (x = "0" -> z = ["0", "NaN"][is_inf(y) + 1]) /\
  (y = "0" -> z = ["0", "NaN"][is_inf(x) + 1]) /\  
  (z = "0" -> (x = "0" \/ y = "0")) /\
  (is_nint(x) -> z = [[negate(x), x][same_sign(x, y) + 1], "NaN"]
                     [(y = "0" \/ y = "NaN") + 1]) /\
  (is_nint(y) -> z = [[negate(y), y][same_sign(x, y) + 1], "NaN"]
                     [(x = "0" \/ x = "NaN") + 1]) /\
  ((is_int(x) /\ is_int(y) /\ is_int(z)) -> z = int2str(str2int(x)*str2int(y)));

% Division, z = x / y.
function var string:
divs(var string: x, var string: y) ::promise_total =
let {
  var string: z;
  constraint is_norm(x) /\ is_norm(y) /\ is_norm(z) /\ divs(x, y, z);
} in z;
predicate
divs(var string: x, var string: y, var string: z) = 
  (x = "0" -> z = ["0", "NaN"][(y = "NaN" \/ y = "0") + 1]) /\ 
  (str_reg(y, "(()|-)0") -> z = [["-Infinity", "Infinity"][same_sign(x, y) + 1], 
                                  "NaN"][(x = "NaN") + 1]) /\
  (z = "0" -> (x = "0" \/ is_inf(y))) /\ (y = "1" -> z = x) /\
  (is_nint(x) -> z = [["-Infinity", "Infinity"][same_sign(x, y) + 1], "NaN"]
                     [(x = "NaN" \/ is_nint(y)) + 1]) /\
  ((is_nint(y) /\ y != "-0") -> z = ["0", "NaN"]
                                    [(y = "NaN" \/ is_nint(x)) + 1]) /\ 
  (is_nint(z) -> (is_nint(x) \/ is_nint(y) \/ str_reg(y, "(()|-)0"))) /\
  ((is_int(x) /\ is_int(y) /\ is_int(z)) -> (
    x = int2str(str2int(z) * str2int(y)) /\ 
    y = int2str(str2int(z) * str2int(x))
  ));
  %FIXME: With z = int2str(str2int(x) div str2int(y))), mzn2fzn gets stuck!

% Modulo, z = x % y.
function var string:
mods(var string: x, var string: y) ::promise_total =
let {
  var string: z;
  var int: p;
  var int: q;
  constraint ((x = "0" \/ y = "1" \/ x = y) -> z = "0") /\ (x = "1" -> z = "1")
    /\ ((is_nint(x) \/ is_num(y) \/ x = "0") -> z = "NaN") /\
  % FIXME: mzn2fzn gets stuck with this:
  %  z = ["NaN", int2str(str2int(x) mod str2int(y))][(is_int(x) /\ is_int(y)) + 1]
  if is_int(x) /\ is_int(y) then
    0 <= q /\ q < str2int(x) /\ 
    [str2int(x) = p*str2int(y) + q /\ z = int2str(q), z = x]
      [(str2int(x) < str2int(y)) + 1]
  else
    z = "NaN" /\ p = 0 /\ q = 0
  endif;
  constraint is_norm(x) /\ is_norm(y) /\ is_norm(z);
} in z;

% Absolute value, y = |x|.
function var string:
absv(var string: x) = 
let {
  var string: y = [x, str_sub(x, 2, str_len(x))][is_neg(x) + 1];
  constraint is_norm(y);
} in y;

% Square root, y = sqrt(x) - rewritten into x = y * y.
function var string:
sqrt(var string: x) ::promise_total = 
let {
  var string: y;
  constraint is_norm(y) /\ x = mul(y, y);
} in y;

% Stubs (not dealing with floats).
function var string: floor (var string: x) ::promise_total = x;
function var string: ceil  (var string: x) ::promise_total = x;
function var string: round (var string: x) ::promise_total = x;

% Less than, x < y.
predicate
lt(var JSType: Tx, var string: Sx, var JSType: Ty, var string: Sy) =
  not(le(Ty, Sy, Tx, Sx)) /\ le(Tx, Sx, Ty, Sy) /\ Sx != Sy /\
  if Tx >= Str /\ Ty >= Str then
    Sx < Sy
  else
    lt_num(to_num(Tx, Sx), to_num(Ty, Sy))
  endif;
  
% Less or equal than, x <= y.
predicate
le(var JSType: Tx, var string: Sx, var JSType: Ty, var string: Sy) =
  if Tx >= Str /\ Ty >= Str then
    Sx <= Sy
  else
    le_num(to_num(Tx, Sx), to_num(Ty, Sy))
  endif;

% Not less than, not(x < y).
predicate
not_lt(var JSType: Tx, var string: Sx, var JSType: Ty, var string: Sy) =
  not(lt(Tx, Sx, Ty, Sy)) /\
  if Tx >= Str /\ Ty >= Str then
    Sy <= Sx
  else
    not_lt_num(to_num(Tx, Sx), to_num(Ty, Sy))
  endif;
  
% Not less or equal than, not(x <= y).
predicate
not_le(var JSType: Tx, var string: Sx, var JSType: Ty, var string: Sy) =
  not_lt(Tx, Sx, Ty, Sy) /\ not(le(Tx, Sx, Ty, Sy)) /\
  if Tx >= Str /\ Ty >= Str then
    Sy < Sx
  else
    not_le_num(to_num(Tx, Sx), to_num(Ty, Sy))
  endif;  

% Less than between numbers.
predicate
lt_num(var string: x, var string: y) =
  x != "NaN"/\ y != "NaN" /\ x != y /\ y != "-Infinity" /\ x != "Infinity" /\
  (x = "-Infinity" \/ y = "Infinity" \/ 
  (is_int(x) /\ is_int(y) /\ str2int(x) < str2int(y)));

% Less or equal than between numbers, x <= y.
predicate
le_num(var string: x, var string: y) = 
  x != "NaN"/\ y != "NaN" /\ (x != y -> lt_num(x, y));

predicate
lt_num(string: x, var string: y) = 
  x != "NaN"/\ y != "NaN" /\ x != y /\ y != "-Infinity" /\ x != "Infinity" /\
  (x = "-Infinity" \/ y = "Infinity" \/ 
  (is_int(x) /\ is_int(y) /\ str2int(x) < str2int(y)));
  
% Not less than between numbers.
predicate not_lt_num(var string: x, var string: y) = 
  [[le_num(y, x), true][(x = "NaN") + 1], true][(y = "NaN") + 1];
  
% Not less or equal than between numbers.
predicate not_le_num(var string: x, var string: y) = 
  [[lt_num(y, x), true][(x = "NaN") + 1], true][(y = "NaN") + 1];
  
% Less than, where w is an integer s.t. w < -2^31 \/ w > 2^31 - 1.
predicate lt_int_no32(var string: x, string: w) = 
  x != "NaN"/\ x != w /\ x != "Infinity" /\ (is_neg(w) -> is_neg(x)) /\
  if is_nat(x) then
    is_nat(w) /\ str_len(x) <= str_len(w) /\ (str_len(x) = str_len(w) -> x < w)    
  else
    is_neg(w) -> (str_len(x) >= str_len(w) /\ (str_len(x) = str_len(w) -> x > w))
  endif;
  
% Less or equal than, where w is an integer s.t. w < -2^31 \/ w > 2^31 - 1.
predicate le_int_no32(var string: x, string: w) =
  x != "NaN" /\ (x != w -> lt_int_no32(x, w));
  
% Greater than, where w is an integer s.t. w < -2^31 \/ w > 2^31 - 1.
predicate gt_int_no32(var string: x, string: w) = 
  x != "NaN"/\ x != w /\ x != "-Infinity" /\ (is_nat(w) -> is_nat(x)) /\
  if is_neg(x) then
    is_neg(w) /\ str_len(x) <= str_len(w) /\ (str_len(x) = str_len(w) -> x < w)    
  else
    is_nat(w) -> (str_len(x) >= str_len(w) /\ (str_len(x) = str_len(w) -> x > w))
  endif;
  
% Greater or equal than, where w is an integer s.t. w < -2^31 \/ w > 2^31 - 1.
predicate ge_int_no32(var string: x, string: w) =
  x != "NaN" /\ (x != w -> gt_int_no32(x, w));  

% Helpers for negation.  
predicate not_lt_int_no32(var string: x, string: w) = 
  x != "NaN" -> not(lt_int_no32(x, w));
predicate not_le_int_no32(var string: x, string: w) = 
  x != "NaN" -> not(le_int_no32(x, w));
predicate not_gt_int_no32(var string: x, string: w) = 
  x != "NaN" -> not(gt_int_no32(x, w));
predicate not_ge_int_no32(var string: x, string: w) = 
  x != "NaN" -> not(ge_int_no32(x, w));



