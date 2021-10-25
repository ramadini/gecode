var string: J;
var string: H;
var string: K;
var string: G;
var string: L;
var string: N;
var string: O;
var string: M;
var string: Q;
constraint
  ((G) ++ (H) ++ ("a") ++ (G) ++ (H) ++ ("b") ++ (J) ++ (K) ++ ("aab") ++ (L) ++ (L) ++ ("b") ++ (N) ++ (N) ++ ("b") ++ (O))
    =
    (("a") ++ (H) ++ (G) ++ (J) ++ (J) ++ ("aa") ++ (J) ++ (J) ++ ("aab") ++ (M) ++ ("aa") ++ (M) ++ ("aab") ++ (M) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
