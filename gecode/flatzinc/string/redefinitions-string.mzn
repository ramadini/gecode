% These redefinitions leave all the string variables and constraints as is.

%TODO: function var bool: 'in'(var string: x, set of string: D) = str_dom(x, D);
function var bool: 'in'(var string: x, array[int] of string: D) = str_dom(x, D);

function var int: str_len(var string: x);

predicate str_dom(var string: x, array[int] of string: D) = 
let {
  var 1..length(D): i;
} in array_string_element(i, D, x);

predicate str_eq_c(var string: x, var string: y);
predicate str_eq_c_reif(var string: x, var string: y, var bool: b);
predicate 
str_eq(var string: x, var string: y) = 
let {
  constraint str_len(x) = str_len(y);
} in str_eq_c(x, y);

function var string: str_rev_c(var string: x) ::promise_total;
function var string:
str_rev(var string: x) ::promise_total = 
let {
  var string: y = str_rev_c(x);
  constraint str_len(x) = str_len(y);
} in y;

function var string: str_pow_c(var string: x, var int: n) ::promise_total;
function var string:
str_pow(var string: x, var int: n) ::promise_total = 
let {
  var string: y = str_pow_c(x, n);
  constraint str_len(y) = str_len(x) * n;
} in y;

function var string: str_concat_c(var string: x, var string: y) ::promise_total;
function var string:
str_concat(var string: x, var string: y) ::promise_total = 
let {
  var string: z = str_concat_c(x, y);
  constraint str_len(z) = str_len(x) + str_len(y);
} in z;

function var int: 
str_find(var string: x, var string: y) ::promise_total =
let {
  var int: n;
  constraint str_find(x, y, n);
} in n;
predicate str_find(var string: x, var string: y, var int: n);

function var int: 
str_rfind(var string: x, var string: y) ::promise_total =
let {
  var int: n;
  constraint str_rfind(x, y, n);
} in n;
predicate str_rfind(var string: x, var string: y, var int: n);

function var int:
str_find_offset(var string: x, var string: y, var int: i) ::promise_total =
let {
  var int: n;
  constraint str_find_offset(x, y, n, i);
} in n;
predicate 
str_find_offset(var string: x, var string: y, var int: n, var int: i) =
  if (i <= 1) then
    str_find(x, y, n)
  else
    str_find(x, str_sub(y, i, str_len(y)), n)
  endif;

function var string:
str_code2char(var int: n) ::promise_total = 
let {
  var string(1): x;
  constraint n = str_char2code(x);
} in x;

function var int:
str_code_at(var string: x, var int: i) = str_char2code(str_char_at(x, i)); 

predicate str_startswith(var string: x, var string: y) =
  str_len(x) >= str_len(y) /\ str_find(y, x) = 1;
   
predicate str_endswith(var string: x, var string: y) = 
  str_len(x) >= str_len(y) /\ str_rfind(y, x) = str_len(x) - str_len(y) + 1;
  
predicate str_prefixof(var string: x, var string: y) = str_startswith(y, x);

predicate str_suffixof(var string: x, var string: y) = str_endswith(y, x);

predicate str_contains(var string: x, var string: y) = str_find(y, x) > 0;

predicate str_gcc(
  var string: x, array[int] of string: a, array[int] of var int: c
);
function array[int] of var int:
str_gcc(var string: x, array[int] of string: a) ::promise_total = 
let {
  array[1..length(a)] of var int: c;
  constraint str_gcc(x, a, c);
} in c;

function var string:
str_gconcat(array[int] of var string: x) ::promise_total =
let {
  var string: y;
  constraint str_gconcat(x, y);
  constraint str_len(y) = sum(i in index_set(x))(str_len(x[i]));  
} in y;
predicate str_gconcat(array[int] of var string: x, var string: y);

predicate str_reg(var string: x, string: regexp);
predicate str_reg_reif(var string: x, string: regexp, var bool: b);
predicate str_dfa_reif(string: x, int: Q, set of string: S, array[int, int] of 
                       int: d, int: q0, set of int: F, var bool: b);

function var string:
element(var int: i, string: x) = str_sub(x, i, i);

function var string: 
element(var int: i, array [int] of string: a) = 
  array_string_element(i, a);
function var string: 
element(var int: i, array [int] of var string: a) = 
  array_var_string_element(i, a);

function var string:
array_string_element(var int: i, array [int] of string: a) ::promise_total = 
let {
  var string: x;
  constraint array_string_element(i + 1 - min(index_set(a)), a, x);
} in x;
predicate 
array_string_element(var int: i, array [int] of string: a, var string: x);

function var string:
array_var_string_element(var int: i, array [int] of var string: a) 
::promise_total = let {
  var string: x;
  constraint array_var_string_element(i + 1 - min(index_set(a)), a, x);
} in x;
predicate 
array_var_string_element(
  var int: i, array [int] of var string: a, var string: x
);

function int: str_lb(var string: x) = lb(str_len(x));
function int: str_ub(var string: x) = ub(str_len(x));

function var string:
str_replace(var string: x, var string: x1, var string: y) ::promise_total = 
let {
  var string: y1;
  constraint str_replace(x, x1, y, y1);
} in y1;
predicate 
str_replace(var string: x, var string: x1, var string: y, var string: y1) 
= let {
  var string: pref;
  var string: suff;
  var string: z;
  var string: z1;
  var 1..2: i;
  var int: n;
} in
  y = str_gconcat([pref, z, suff]) /\ y1 = str_gconcat([pref, z1, suff]) /\
  z = ["", x][i] /\ z1 = ["", x1][i] /\ i = (n > 0) + 1 /\ n = str_find(x, y) /\
  str_len(pref) = max(0, n - 1) /\ str_find(x, pref) = (x = "");

predicate
str_sub(var string: x, var string: y) = str_find(x, y) > 0;

function var string:
str_sub(var string: x, var int : i, var int: j) :: promise_total = 
let {
  var string: y;
  var string: y1;
  var string: y2;
  var 1..ub(i): n = max(1, i);
  var min(lb(str_len(x)), lb(j))..min(ub(str_len(x)), ub(j)): m = 
    min(str_len(x), j);
  constraint if i > j \/ i > str_len(x) \/ j < 1 \/ x = "" then
    y = ""
  else
    x = y1 ++ y ++ y2 /\
    str_len(y)  = min(str_len(x), max(0, m - n + 1)) /\
    str_len(y1) = min(str_len(x), n - 1) /\
    str_len(y2) = max(0, str_len(x) - m)
  endif;
} in y;

function var string:
str_char_at(var string: x, var int: i) = str_sub(x, i, i);

function var string: nat2str(var int: n);
function var int: str2nat(var string: x);

function array[int] of var string:
str_split(var string: x, int: len) = [x];

function array[int] of var string: 
str_split(var string: x, var string: sep, int: len) :: promise_total = 
let {
  constraint assert(len >= 0, "Invalid array length!");
  var int: lx = str_len(x);
  int: n = min(len, ub(lx));
  array[1..n] of var int: A;
  array[1..n] of var string: B;
  var string: xsep = x ++ sep;
  constraint if sep = "" then
    forall (i in 1..n) (B[i] = [str_char_at(x, i), "\\x00"][(i > lx) + 1])
  else
    A[1] = str_find(sep, xsep) /\ B[1] = str_sub(x, 1, A[1] - 1) /\
    forall (i in 2..n) (
      A[i] = A[i - 1] + str_find(sep, str_sub(xsep, A[i - 1] + 1, lx + 1)) /\
      B[i] = [str_sub(x, A[i-1] + 1, A[i] - 1), "\\x00"][(A[i-1] + 1 > lx) + 1]
    )
  endif;
} in B;

annotation string_default_search(array[int] of var string: x);
