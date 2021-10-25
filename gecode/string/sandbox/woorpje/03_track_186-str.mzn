var string: S;
var string: N;
var string: J;
var string: R;
var string: K;
var string: I;
var string: U;
var string: G;
var string: F;
var string: L;
constraint
  ((G) ++ ("a") ++ (G) ++ ("b") ++ (K) ++ (L) ++ ("b") ++ (R) ++ (R) ++ ("b") ++ (S))
    =
    (("aaa") ++ (F) ++ (J) ++ (I) ++ (L) ++ (L) ++ ("a") ++ (L) ++ (L) ++ (L) ++ ("a") ++ (L) ++ ("b") ++ (R) ++ (N) ++ (R) ++ (N) ++ ("b") ++ (U) ++ (U) ++ ("baa"));
solve satisfy;
