var string: J;
var string: I;
var string: K;
var string: H;
var string: G;
var string: N;
var string: M;
var string: P;
var string: Q;
constraint
  ((G) ++ (G) ++ (G) ++ (G) ++ (G) ++ ("aa") ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ ("ab") ++ (K) ++ ("a") ++ (K) ++ (K) ++ (K) ++ ("a") ++ (K) ++ (I) ++ ("b") ++ (K) ++ (K) ++ ("ba") ++ (N) ++ ("b") ++ (Q))
    =
    (("aa") ++ (H) ++ (H) ++ (H) ++ ("a") ++ (H) ++ ("aaa") ++ (G) ++ (H) ++ ("aa") ++ (H) ++ (J) ++ (J) ++ (I) ++ (K) ++ (J) ++ (J) ++ (I) ++ (K) ++ ("b") ++ (M) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (M) ++ (M) ++ ("b") ++ (P) ++ (P) ++ ("baa"));
solve satisfy;
