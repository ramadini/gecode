var string: S;
var string: J;
var string: I;
var string: K;
var string: H;
var string: R;
var string: G;
var string: U;
var string: N;
var string: M;
var string: P;
var string: Q;
constraint
  ((G) ++ (G) ++ (G) ++ (I) ++ ("a") ++ (G) ++ (G) ++ (G) ++ (I) ++ ("b") ++ (K) ++ ("a") ++ (J) ++ ("a") ++ (K) ++ (K) ++ (J) ++ ("ab") ++ (M) ++ (M) ++ ("b") ++ (S) ++ (P) ++ ("b") ++ (S))
    =
    (("a") ++ (H) ++ ("aaaa") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (M) ++ (M) ++ (K) ++ ("a") ++ (M) ++ (M) ++ (K) ++ ("ab") ++ (M) ++ (N) ++ (M) ++ (N) ++ ("b") ++ (R) ++ (Q) ++ (R) ++ (Q) ++ ("b") ++ (U) ++ (U) ++ ("baa"));
solve satisfy;
