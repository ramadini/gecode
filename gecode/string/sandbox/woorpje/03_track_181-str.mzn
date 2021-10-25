var string: J;
var string: H;
var string: I;
var string: F;
var string: G;
var string: L;
var string: M;
var string: P;
var string: Q;
constraint
  ((F) ++ (H) ++ ("a") ++ (F) ++ (H) ++ ("b") ++ (I) ++ (M) ++ ("b") ++ (Q))
    =
    (("aaa") ++ (G) ++ (J) ++ (L) ++ (J) ++ (L) ++ ("b") ++ (P) ++ (P) ++ ("baa"));
solve satisfy;
