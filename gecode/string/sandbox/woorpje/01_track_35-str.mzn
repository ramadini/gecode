var string: C;
var string: G;
var string: E;
var string: A;
constraint
  (("a") ++ (E) ++ ("ca") ++ (A) ++ ("ccbccaabaccbbbcbbaabccbccbbabcbab")) =
    (("ab") ++ (C) ++ (G) ++ ("ab"));
solve satisfy;
