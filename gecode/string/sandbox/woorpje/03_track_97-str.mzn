var string: G;
var string: D;
var string: H;
constraint
  ((D) ++ ("a") ++ (D) ++ ("b") ++ (G)) =
    (("a") ++ (G) ++ (H) ++ (H) ++ ("baa"));
solve satisfy;
