var string: N;
var string: J;
var string: I;
var string: K;
var string: H;
var string: G;
var string: L;
var string: O;
var string: M;
var string: Q;
constraint
  ((G) ++ (I) ++ ("aaaaaaaa") ++ (G) ++ (I) ++ ("aaaaaaab") ++ (K) ++ ("aa") ++ (K) ++ (J) ++ (J) ++ ("b") ++ (M) ++ (L) ++ (L) ++ ("ab") ++ (O) ++ (O) ++ ("b") ++ (Q))
    =
    (("a") ++ (H) ++ ("aaaaaaaaa") ++ (J) ++ (K) ++ ("aa") ++ (K) ++ ("aa") ++ (J) ++ (K) ++ ("aa") ++ (K) ++ ("b") ++ (M) ++ (M) ++ (L) ++ ("a") ++ (M) ++ (M) ++ (L) ++ ("ab") ++ (N) ++ (N) ++ ("b") ++ (O) ++ (O) ++ ("baa"));
solve satisfy;
