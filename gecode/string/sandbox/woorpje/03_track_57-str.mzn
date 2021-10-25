var string: H;
var string: K;
var string: F;
var string: G;
var string: J;
var string: L;
var string: M;
constraint
  ((F) ++ ("aa") ++ (F) ++ (F) ++ ("a") ++ (F) ++ ("aa") ++ (F) ++ (F) ++ ("b") ++ (H) ++ (H) ++ ("b") ++ (J) ++ (J) ++ ("b") ++ (L))
    =
    (("a") ++ (F) ++ (F) ++ (G) ++ (H) ++ ("a") ++ (J) ++ (H) ++ ("a") ++ (J) ++ ("b") ++ (K) ++ (L) ++ (K) ++ (L) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
