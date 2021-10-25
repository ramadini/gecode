var string: R;
var string: K;
var string: H;
var string: G;
var string: F;
var string: L;
var string: M;
var string: Q;
constraint
  ((F) ++ (G) ++ (G) ++ (G) ++ ("a") ++ (F) ++ (G) ++ (G) ++ (G) ++ ("b") ++ (K) ++ ("aab") ++ (L) ++ ("b") ++ (R))
    =
    (("a") ++ (H) ++ ("aa") ++ (G) ++ (L) ++ (L) ++ ("b") ++ (M) ++ (M) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
