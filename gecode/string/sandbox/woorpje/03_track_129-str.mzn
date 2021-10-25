var string: I;
var string: K;
var string: H;
var string: G;
var string: F;
var string: J;
constraint
  ((F) ++ ("a") ++ (F) ++ ("b") ++ (G) ++ (I) ++ ("b") ++ (K)) =
    (("a") ++ (G) ++ (G) ++ (H) ++ (H) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
