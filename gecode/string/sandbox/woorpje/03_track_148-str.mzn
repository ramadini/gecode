var string: H;
var string: G;
var string: E;
var string: F;
constraint
  ((G) ++ (F) ++ ("a") ++ (G) ++ (F) ++ ("b") ++ (G)) =
    (("a") ++ (E) ++ (E) ++ (H) ++ (H) ++ ("baa"));
solve satisfy;
