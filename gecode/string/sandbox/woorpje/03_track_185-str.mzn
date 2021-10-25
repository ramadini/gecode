var string: H;
var string: F;
var string: G;
var string: J;
var string: D;
constraint
  ((F) ++ (D) ++ ("a") ++ (F) ++ (D) ++ ("b") ++ (H)) =
    (("a") ++ (D) ++ (G) ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
