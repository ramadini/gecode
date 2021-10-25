var string: H;
var string: G;
var string: F;
var string: J;
constraint
  ((F) ++ (F) ++ ("a") ++ (F) ++ (F) ++ ("b") ++ (H)) =
    (("a") ++ (G) ++ (G) ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
