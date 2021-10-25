var string: J;
var string: H;
var string: K;
var string: I;
var string: G;
var string: L;
var string: O;
var string: M;
var string: Q;
constraint
  ((H) ++ (H) ++ ("aaaaa") ++ (H) ++ (H) ++ ("aaaab") ++ (K) ++ ("aa") ++ (K) ++ ("a") ++ (K) ++ (K) ++ ("ab") ++ (K) ++ (K) ++ ("b") ++ (M) ++ ("ab") ++ (Q))
    =
    (("a") ++ (I) ++ (G) ++ (J) ++ (J) ++ ("a") ++ (K) ++ (J) ++ (J) ++ ("a") ++ (K) ++ ("b") ++ (L) ++ (L) ++ (L) ++ (L) ++ ("b") ++ (M) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (O) ++ (O) ++ ("baa"));
solve satisfy;
