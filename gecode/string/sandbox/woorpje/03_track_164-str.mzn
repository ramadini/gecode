var string: H;
var string: K;
var string: F;
var string: E;
var string: G;
var string: J;
var string: N;
constraint
  ((H) ++ (H) ++ (F) ++ (H) ++ ("a") ++ (H) ++ (H) ++ (F) ++ (H) ++ ("b") ++ (K) ++ ("b") ++ (K))
    =
    (("a") ++ (F) ++ (G) ++ (E) ++ (E) ++ (H) ++ (J) ++ (H) ++ (J) ++ ("b") ++ (N) ++ (N) ++ ("baa"));
solve satisfy;
