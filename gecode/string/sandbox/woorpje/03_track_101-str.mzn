var string: G;
var string: F;
var string: D;
constraint
  ((D) ++ (F) ++ ("a") ++ (D) ++ (F) ++ ("b") ++ (G)) =
    (("aa") ++ (D) ++ (F) ++ (F) ++ ("baa"));
solve satisfy;
