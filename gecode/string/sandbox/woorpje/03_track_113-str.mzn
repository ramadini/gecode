var string: I;
var string: H;
var string: G;
var string: E;
var string: F;
var string: J;
constraint
  ((E) ++ (E) ++ ("a") ++ (E) ++ (E) ++ ("b") ++ (G) ++ (I) ++ ("b") ++ (I))
    =
    (("a") ++ (E) ++ (F) ++ (H) ++ (H) ++ (H) ++ (H) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
