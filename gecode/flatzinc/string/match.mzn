% Include this file in the MiniZinc model to decompose the match constraint.

int: MAX_LEN = 1000;

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
