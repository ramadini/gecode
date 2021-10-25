var string: H;
var string: K;
var string: F;
var string: J;
var string: D;
constraint
  ((D) ++ ("a") ++ (D) ++ ("b") ++ (K)) =
    (("a") ++ (F) ++ (H) ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
