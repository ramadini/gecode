% Include this file in the MiniZinc model to decompose the match constraint.

function var int:
str_match(var string: x, string: regexp) :: promise_total = 
let {
  var 0..str_ub(x): i;
  constraint if str_reg("", regexp) then
    i = 1
  else
    i <= str_len(x) /\
    if i = 0 then
      not(str_reg(x, ".*" ++ regexp ++ ".*"))
    else
      not(str_reg(str_sub(x, 1, i-1), ".*" ++ regexp ++ ".*")) /\
          str_reg(str_sub(x, i, str_len(x)),  regexp ++ ".*")
    endif
  endif
} in i;
