var string: I;
var string: K;
var string: H;
var string: G;
var string: N;
var string: O;
var string: M;
var string: P;
constraint
  ((G) ++ (G) ++ (I) ++ (I) ++ ("a") ++ (G) ++ (G) ++ (I) ++ (I) ++ ("b") ++ (K) ++ ("aa") ++ (K) ++ ("aab") ++ (M) ++ (K) ++ (K) ++ ("ab") ++ (O) ++ (O) ++ ("b") ++ (O))
    =
    (("a") ++ (H) ++ ("a") ++ (I) ++ (I) ++ (I) ++ (I) ++ (K) ++ (K) ++ (K) ++ (K) ++ (K) ++ (K) ++ (K) ++ (K) ++ ("b") ++ (K) ++ (K) ++ (M) ++ (M) ++ (K) ++ (K) ++ (M) ++ (M) ++ ("b") ++ (O) ++ (N) ++ (O) ++ (N) ++ ("b") ++ (P) ++ (P) ++ ("baa"));
solve satisfy;
