% Include this file in the MiniZinc model to decompose the replace constraints.

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
  y = pref ++ z ++ suff /\ y1 = pref ++ z1 ++ suff /\
  z = ["", x][i] /\ z1 = ["", x1][i] /\ i = (n > 0) + 1 /\ n = str_find(x, y) /\
  str_len(pref) = max(0, n - 1) /\ str_find(x, pref) = (x = "");
    
function var string:
str_replace_last(var string: x, var string: x1, var string: y) ::promise_total = 
  str_rev(str_replace(str_rev(x), str_rev(x1), str_rev(y)));
