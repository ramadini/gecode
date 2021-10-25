var string: H;
var string: K;
var string: F;
var string: G;
var string: L;
var string: M;
constraint
  ((F) ++ (F) ++ (F) ++ (F) ++ ("a") ++ (F) ++ (F) ++ (F) ++ (F) ++ ("b") ++ (K) ++ (K) ++ ("b") ++ (L))
    =
    (("a") ++ (G) ++ ("a") ++ (F) ++ (H) ++ (H) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
