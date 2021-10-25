var string: H;
var string: I;
var string: G;
var string: F;
var string: L;
var string: N;
var string: O;
var string: P;
constraint
  ((H) ++ (F) ++ ("a") ++ (G) ++ (G) ++ (G) ++ (G) ++ ("a") ++ (H) ++ (F) ++ ("a") ++ (G) ++ (G) ++ (G) ++ (G) ++ ("b") ++ (L) ++ ("b") ++ (N) ++ ("b") ++ (P))
    =
    (("a") ++ (F) ++ (G) ++ (H) ++ (H) ++ (G) ++ (F) ++ (I) ++ ("aa") ++ (I) ++ ("aab") ++ (L) ++ (L) ++ (L) ++ (L) ++ ("b") ++ (O) ++ (O) ++ ("baa"));
solve satisfy;
