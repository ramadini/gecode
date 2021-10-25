var string: H;
var string: I;
var string: G;
var string: F;
var string: L;
var string: N;
var string: O;
var string: Q;
constraint
  ((G) ++ (G) ++ (H) ++ (G) ++ ("a") ++ (G) ++ ("a") ++ (G) ++ (G) ++ (H) ++ (G) ++ ("a") ++ (G) ++ ("b") ++ (I) ++ ("ab") ++ (N) ++ ("b") ++ (Q))
    =
    (("a") ++ (I) ++ (F) ++ (G) ++ ("a") ++ (F) ++ (L) ++ ("aa") ++ (L) ++ ("aab") ++ (L) ++ (L) ++ (L) ++ (L) ++ ("b") ++ (O) ++ (O) ++ ("baa"));
solve satisfy;
