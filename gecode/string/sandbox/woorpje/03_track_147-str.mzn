var string: I;
var string: H;
var string: G;
var string: E;
var string: D;
constraint
  ((G) ++ (D) ++ ("a") ++ (G) ++ (D) ++ ("b") ++ (I)) =
    (("a") ++ (G) ++ (E) ++ (H) ++ (H) ++ ("baa"));
solve satisfy;
