var string: s;
var string: ret;
constraint
  [(ret) = (s),(ret) = ("x")][1 + ((str_contains(s, "<")) \/ (str_contains(s, ">")))];
constraint str_contains(s, "sc");
constraint str_contains(s, "ript scr=");
constraint not ((ret) = ("x"));
solve satisfy;
