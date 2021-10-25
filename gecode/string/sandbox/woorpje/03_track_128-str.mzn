var string: G;
var string: F;
var string: E;
var string: D;
constraint
  ((F) ++ ("a") ++ (F) ++ ("b") ++ (F)) =
    (("a") ++ (D) ++ (E) ++ (G) ++ (G) ++ ("baa"));
solve satisfy;
