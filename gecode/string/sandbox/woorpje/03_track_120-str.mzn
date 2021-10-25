var string: G;
var string: E;
var string: F;
var string: J;
constraint
  ((E) ++ (F) ++ ("a") ++ (E) ++ (F) ++ ("b") ++ (G)) =
    (("aa") ++ (E) ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
