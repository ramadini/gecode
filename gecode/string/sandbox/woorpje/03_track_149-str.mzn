var string: H;
var string: I;
var string: G;
var string: F;
var string: J;
var string: N;
var string: L;
constraint
  ((G) ++ (G) ++ (F) ++ (G) ++ ("a") ++ (G) ++ (G) ++ (F) ++ (G) ++ ("b") ++ (L) ++ ("b") ++ (N))
    =
    (("a") ++ (H) ++ (F) ++ ("a") ++ (H) ++ (I) ++ (J) ++ (I) ++ (J) ++ ("baabaa"));
solve satisfy;
