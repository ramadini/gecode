% Include this file in the MiniZinc model to decompose the replace constraints.

% String replace: returns the replacement of first occurrence of q with q1 in x.
function var string:
str_replace(var string: x, var string: q, var string: q1) = 
let {
  var string: y;
  var string: p;
  var string: s;
  var 0..1: i;
  var 0..ub(str_len(x)): n;
  constraint
    x = p ++ str_pow(q, i) ++ s /\ y = p ++ str_pow(q1, i) ++ s
    /\ i = bool2int(n > 0) /\ n = str_find(q, x) /\ str_len(p) = max(0, n - 1)
    /\ str_find(q, p) = (str_len(q) = 0)
} in y;
    
function var string:
str_replace_last(var string: x, var string: x1, var string: y) ::promise_total = 
  str_rev(str_replace(str_rev(x), str_rev(x1), str_rev(y)));
