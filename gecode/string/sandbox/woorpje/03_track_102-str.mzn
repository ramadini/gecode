var string: H;
var string: G;
var string: F;
var string: E;
var string: D;
constraint
  ((D) ++ (F) ++ ("a") ++ (D) ++ (F) ++ ("b") ++ (G)) =
    (("a") ++ (D) ++ (E) ++ (H) ++ (H) ++ ("baa"));
solve satisfy;
