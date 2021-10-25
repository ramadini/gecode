var string: S;
var string: N;
var string: J;
var string: T;
var string: X;
var string: H;
var string: K;
var string: R;
var string: U;
var string: G;
var string: L;
var string: Y;
var string: P;
constraint
  ((J) ++ (K) ++ (H) ++ ("a") ++ (J) ++ (K) ++ (H) ++ ("b") ++ (L) ++ (K) ++ (K) ++ ("b") ++ (P) ++ ("b") ++ (S) ++ (U) ++ ("b") ++ (Y))
    =
    (("a") ++ (G) ++ ("a") ++ (G) ++ (G) ++ (H) ++ (H) ++ (G) ++ ("a") ++ (G) ++ ("a") ++ (N) ++ ("a") ++ (K) ++ (K) ++ (N) ++ ("a") ++ (K) ++ (K) ++ ("b") ++ (R) ++ (R) ++ (R) ++ (R) ++ ("b") ++ (U) ++ (T) ++ (U) ++ (T) ++ ("b") ++ (X) ++ (X) ++ ("baa"));
solve satisfy;
