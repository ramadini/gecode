var string: J;
var string: I;
var string: K;
var string: H;
var string: G;
var string: L;
var string: N;
var string: O;
var string: M;
var string: P;
var string: Q;
constraint
  ((G) ++ ("aaaaaa") ++ (G) ++ ("aa") ++ (G) ++ ("aaa") ++ (G) ++ ("aaaaaa") ++ (G) ++ ("aa") ++ (G) ++ ("aaba") ++ (K) ++ (I) ++ (K) ++ (K) ++ (I) ++ (K) ++ (K) ++ ("b") ++ (K) ++ (K) ++ ("b") ++ (M) ++ (O) ++ ("b") ++ (Q))
    =
    (("a") ++ (H) ++ ("aa") ++ (G) ++ (G) ++ ("aa") ++ (J) ++ (J) ++ ("a") ++ (I) ++ (K) ++ (I) ++ (J) ++ (J) ++ ("a") ++ (I) ++ (K) ++ (I) ++ ("b") ++ (M) ++ (L) ++ (M) ++ (L) ++ ("b") ++ (N) ++ (M) ++ (N) ++ (M) ++ ("b") ++ (P) ++ (P) ++ ("baa"));
solve satisfy;
