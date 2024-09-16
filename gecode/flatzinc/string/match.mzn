% Include this file in the MiniZinc model to decompose the match constraint.

int: MAX_LEN = 1000;

function var int:
str_match_new(var string: x, string: regexp) :: promise_total = 
  str_match(x, regexp, MAX_LEN);

function var int:
str_match(var string: x, string: regexp) :: promise_total = 
  str_match(x, regexp, MAX_LEN);

function var int:
str_match(var string: x, string: regexp, int: maxlen) :: promise_total = 
let {
  var 0..str_ub(x): i;
  constraint if str_reg("", regexp) then
    i = 1
  else
    i <= str_len(x) /\ i <= maxlen /\
    if i = 0 then
      not(str_reg(x, ".*(" ++ regexp ++ ").*"))
    else
      not(str_reg(str_sub(x, 1, i-1), ".*(" ++ regexp ++ ").*")) /\
          str_reg(str_sub(x, i, str_len(x)), "(" ++ regexp ++ ").*") /\
      forall (j in 1..maxlen where j < i) (
        not(str_reg(str_sub(x, j, str_len(x)), "(" ++ regexp ++ ").*"))
      )
    endif
  endif
} in i;

enum match_mode = {
  LAZY,
  GREEDY
};

function var int:
str_lazy_match(var string: x, string: rho, var string: y) =
  str_match(x, rho, LAZY, MAX_LEN, y);
function var int:
str_lazy_match(var string: x, string: rho, int: maxl, var string: y) =
  str_match(x, rho, LAZY, maxl, y);

function var int:
str_greedy_match(var string: x, string: rho, var string: y) =
  str_match(x, rho, GREEDY, MAX_LEN, y);
function var int:
str_greedy_match(var string: x, string: rho, int: maxl, var string: y) =
  str_match(x, rho, GREEDY, maxl, y);

function var int:
str_match(var string: x, string: rho, match_mode: m, var string: y) =
  str_match(x, rho, m, MAX_LEN, y);

function var int:
str_match(var string: x, string: rho, match_mode: m, int: maxl, var string: y) =
let {
  var 0..maxl: p = str_match(x, rho, maxl);
  constraint p > 0 -> (
    str_reg(y, rho) /\ let { var lb(p)..maxl: q } in 
    p <= q /\ q <= maxl /\ y = str_sub(x, p, q) /\
    if m = LAZY then
      forall (k in p..q-1) ( not(str_reg(str_sub(x, p, k), rho)) )
    else
      forall (k in q+1..maxl) ( not(str_reg(str_sub(x, p, k), rho)) )
    endif
  );
} in p;



