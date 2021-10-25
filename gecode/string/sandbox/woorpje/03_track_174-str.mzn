var string: S;
var string: J;
var string: X;
var string: H;
var string: K;
var string: R;
var string: U;
var string: G;
var string: V;
var string: L;
var string: M;
var string: Z;
constraint
  ((J) ++ ("a") ++ (H) ++ (K) ++ (K) ++ (K) ++ (J) ++ ("a") ++ (J) ++ ("a") ++ (H) ++ (K) ++ (K) ++ (K) ++ (J) ++ ("baa") ++ (L) ++ (L) ++ (K) ++ ("b") ++ (S) ++ ("a") ++ (S) ++ (S) ++ ("b") ++ (S) ++ (S) ++ ("b") ++ (X))
    =
    (("aa") ++ (H) ++ (H) ++ (H) ++ ("a") ++ (H) ++ (H) ++ ("a") ++ (H) ++ (G) ++ (H) ++ (H) ++ (H) ++ (H) ++ (H) ++ (M) ++ (L) ++ ("a") ++ (M) ++ (L) ++ (M) ++ (M) ++ (M) ++ (M) ++ (L) ++ ("a") ++ (M) ++ (L) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (R) ++ (R) ++ (R) ++ (R) ++ ("b") ++ (U) ++ (V) ++ (U) ++ (V) ++ ("b") ++ (Z) ++ (Z) ++ ("baa"));
solve satisfy;
