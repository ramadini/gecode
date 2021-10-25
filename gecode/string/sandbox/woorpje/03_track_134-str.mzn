var string: I;
var string: H;
var string: K;
var string: F;
var string: G;
var string: E;
var string: J;
constraint
  ((F) ++ (F) ++ (F) ++ (F) ++ ("a") ++ (F) ++ (F) ++ (F) ++ (F) ++ ("b") ++ (I) ++ ("ab") ++ (J))
    =
    (("a") ++ (E) ++ (G) ++ (H) ++ (G) ++ (H) ++ (G) ++ ("b") ++ (K) ++ (K) ++ ("baa"));
solve satisfy;
