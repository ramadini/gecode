var string: H;
var string: K;
var string: I;
var string: G;
var string: L;
var string: N;
var string: O;
var string: M;
var string: P;
constraint
  ((H) ++ (I) ++ ("aa") ++ (H) ++ (I) ++ ("ab") ++ (K) ++ (K) ++ ("aaaab") ++ (L) ++ (K) ++ ("b") ++ (N) ++ ("b") ++ (P))
    =
    (("a") ++ (G) ++ (G) ++ (I) ++ ("aa") ++ (I) ++ (I) ++ (K) ++ (I) ++ (I) ++ (K) ++ ("b") ++ (K) ++ (K) ++ ("a") ++ (K) ++ (K) ++ (K) ++ ("a") ++ (K) ++ ("b") ++ (M) ++ (O) ++ (M) ++ (O) ++ ("b") ++ (O) ++ (O) ++ ("baa"));
solve satisfy;
