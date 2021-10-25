var string: I;
var string: G;
var string: E;
var string: D;
constraint
  ((D) ++ (D) ++ ("a") ++ (D) ++ (D) ++ ("b") ++ (G)) =
    (("a") ++ (D) ++ (E) ++ (I) ++ (I) ++ ("baa"));
solve satisfy;
