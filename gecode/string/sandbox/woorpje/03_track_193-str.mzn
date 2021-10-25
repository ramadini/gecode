var string: I;
var string: R;
var string: K;
var string: F;
var string: G;
var string: L;
var string: O;
var string: M;
var string: Q;
constraint
  ((F) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ ("aaba") ++ (L) ++ (I) ++ ("b") ++ (O) ++ (O) ++ ("b") ++ (Q))
    =
    (("aaa") ++ (G) ++ (G) ++ (G) ++ (K) ++ (L) ++ ("a") ++ (K) ++ (L) ++ ("ab") ++ (L) ++ (M) ++ (L) ++ (M) ++ ("b") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
