% These redefinitions leave all the string variables and constraints as is.

%TODO: function var bool: 'in'(var string: x, set of string: D) = str_dom(x, D);
function var bool: 'in'(var string: x, array[int] of string: D) = str_dom(x, D);

function var int: str_len(var string: x);

predicate str_dom(var string: x, array[int] of string: D) = 
let {
  var 1..length(D): i;
} in array_string_element(i, D, x);

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
  str_find(if i <= 1 then x else str_sub(x, i, str_len(x)) endif, y, n);

predicate str_startswith(var string: x, var string: y) =
  str_len(x) >= str_len(y) /\ str_find(x, y) = 1;
   
predicate str_endswith(var string: x, var string: y) = 
  str_len(x) >= str_len(y) /\ str_rfind(x, y) = str_len(x) - str_len(y) + 1;
  
predicate str_prefixof(var string: x, var string: y) = str_startswith(y, x);

predicate str_suffixof(var string: x, var string: y) = str_endswith(y, x);

predicate str_contains(var string: x, var string: y) = str_find(x, y) > 0;

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
str_replace(var string: x, var string: x1, var string: y);

function var string:
str_replace_all(var string: x, var string: x1, var string: y);

function var string:
str_replace_last(var string: x, var string: x1, var string: y);

predicate
str_sub(var string: x, var string: y) = str_find(y, x) > 0;

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
str_char_at(var string: x, var int: i) :: promise_total = let {
  constraint i >= 1 /\ i <= str_len(x);
  var string: xp;
  var string: xs;
  var string: c;
  constraint str_len(c) = 1 /\ str_len(xp) = i-1 /\ x = xp ++ c ++ xs;
} in c;

function var string: nat2str(var int: n);
function var int: str2nat(var string: x);

annotation none;
annotation blockdim_min;
annotation lenblock_min;
annotation dimdeg_lenblock_min;

annotation llll;
annotation lllm;
annotation lslm;

annotation string_search(
  array[int] of var string: vars, ann: hvar, ann: hval, ann: comp
);
annotation string_default_search(array[int] of var string: vars);

