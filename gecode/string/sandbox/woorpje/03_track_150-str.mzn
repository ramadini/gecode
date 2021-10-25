var string: I;
var string: K;
var string: H;
var string: V;
var string: G;
var string: R;
var string: F;
var string: L;
var string: M;
var string: P;
constraint
  ((G) ++ (G) ++ (G) ++ (G) ++ ("a") ++ (G) ++ (I) ++ ("a") ++ (G) ++ (G) ++ (G) ++ (G) ++ ("a") ++ (G) ++ (I) ++ ("b") ++ (K) ++ ("a") ++ (L) ++ (L) ++ ("ba") ++ (P) ++ ("b") ++ (V))
    =
    (("a") ++ (H) ++ (F) ++ (G) ++ (M) ++ ("a") ++ (M) ++ ("ab") ++ (R) ++ (R) ++ ("b") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
