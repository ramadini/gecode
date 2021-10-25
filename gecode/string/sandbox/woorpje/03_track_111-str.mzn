var string: G;
var string: F;
var string: E;
var string: D;
constraint
  ((E) ++ (D) ++ ("a") ++ (E) ++ (D) ++ ("b") ++ (F)) =
    (("a") ++ (F) ++ (G) ++ (G) ++ ("baa"));
solve satisfy;
