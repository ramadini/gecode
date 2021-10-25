var string: G;
var string: F;
var string: E;
var string: J;
var string: D;
constraint
  ((D) ++ ("a") ++ (D) ++ ("b") ++ (G)) =
    (("a") ++ (E) ++ (F) ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
