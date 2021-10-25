var string: N;
var string: J;
var string: I;
var string: K;
var string: F;
var string: G;
var string: L;
var string: M;
var string: P;
var string: Q;
constraint
  ((F) ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (F) ++ (I) ++ ("aa") ++ (I) ++ ("b") ++ (K) ++ (K) ++ (J) ++ (K) ++ ("b") ++ (L) ++ ("b") ++ (P))
    =
    (("a") ++ (G) ++ (I) ++ (I) ++ (G) ++ (G) ++ (L) ++ (J) ++ (L) ++ (J) ++ ("b") ++ (N) ++ (M) ++ (N) ++ (M) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
