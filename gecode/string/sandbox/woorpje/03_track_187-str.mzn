var string: N;
var string: T;
var string: I;
var string: H;
var string: R;
var string: K;
var string: U;
var string: G;
var string: L;
var string: M;
var string: P;
var string: Q;
constraint
  ((I) ++ (H) ++ (H) ++ (I) ++ (H) ++ (H) ++ (I) ++ ("a") ++ (H) ++ (I) ++ (H) ++ (H) ++ (I) ++ (H) ++ (I) ++ (H) ++ ("a") ++ (I) ++ (H) ++ (H) ++ (I) ++ (H) ++ (H) ++ (I) ++ ("a") ++ (H) ++ (I) ++ (H) ++ (H) ++ (I) ++ (H) ++ (I) ++ (H) ++ ("b") ++ (L) ++ ("a") ++ (M) ++ (M) ++ (L) ++ (L) ++ ("b") ++ (P) ++ (P) ++ ("b") ++ (P) ++ (Q) ++ ("b") ++ (T))
    =
    (("a") ++ (H) ++ (I) ++ ("a") ++ (I) ++ (I) ++ (I) ++ (I) ++ (I) ++ (I) ++ (G) ++ (I) ++ (H) ++ (H) ++ (I) ++ (K) ++ (L) ++ ("a") ++ (K) ++ (L) ++ ("aba") ++ (N) ++ (M) ++ ("aa") ++ (N) ++ (M) ++ ("ab") ++ (R) ++ (R) ++ ("b") ++ (U) ++ (U) ++ ("baa"));
solve satisfy;
