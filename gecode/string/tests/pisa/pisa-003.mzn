var string: s;
var string: var0;
var string: ret;
constraint
  [(ret) = (s),(ret) = ("x")][1 + ((str_contains(s, "<")) \/ (str_contains(s, ">")))];
constraint (var0) = (("<scr") ++ ("ipt"));
constraint str_contains(s, var0);
constraint not ((ret) = ("x"));
solve satisfy;
