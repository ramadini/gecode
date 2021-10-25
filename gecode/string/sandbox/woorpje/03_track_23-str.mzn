var string: H;
var string: G;
var string: E;
var string: F;
constraint
  ((F) ++ (F) ++ ("a") ++ (F) ++ ("a") ++ (F) ++ (F) ++ ("a") ++ (F) ++ ("b") ++ (F) ++ (G) ++ ("ba"))
    =
    (("a") ++ (E) ++ ("a") ++ (E) ++ ("a") ++ (G) ++ (G) ++ (G) ++ (G) ++ ("b") ++ (H) ++ (H) ++ ("baa"));
solve satisfy;
