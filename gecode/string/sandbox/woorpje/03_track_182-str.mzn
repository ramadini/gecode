var string: N;
var string: H;
var string: K;
var string: I;
var string: G;
var string: L;
var string: M;
var string: P;
var string: Q;
constraint
  ((H) ++ (G) ++ ("aaaaa") ++ (H) ++ (G) ++ ("aaaab") ++ (I) ++ (I) ++ (I) ++ (I) ++ (I) ++ (I) ++ (I) ++ (I) ++ ("b") ++ (L) ++ (L) ++ (K) ++ (K) ++ ("b") ++ (N) ++ (N) ++ ("b") ++ (Q))
    =
    (("a") ++ (G) ++ (G) ++ (I) ++ ("aaaa") ++ (K) ++ (K) ++ (K) ++ (K) ++ ("b") ++ (L) ++ (L) ++ (L) ++ ("a") ++ (L) ++ (L) ++ (L) ++ ("ab") ++ (M) ++ (N) ++ (M) ++ (N) ++ ("b") ++ (P) ++ (P) ++ ("baa"));
solve satisfy;
