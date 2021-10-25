var string: S;
var string: J;
var string: X;
var string: I;
var string: K;
var string: H;
var string: U;
var string: R;
var string: V;
var string: L;
var string: M;
var string: P;
constraint
  ((J) ++ (I) ++ (I) ++ ("a") ++ (J) ++ (I) ++ (I) ++ ("b") ++ (L) ++ (K) ++ ("aab") ++ (P) ++ (S) ++ ("b") ++ (U) ++ (S) ++ ("b"))
    =
    (("a") ++ (J) ++ (H) ++ ("a") ++ (K) ++ ("a") ++ (M) ++ (K) ++ ("a") ++ (M) ++ ("b") ++ (R) ++ (R) ++ (R) ++ ("a") ++ (R) ++ (R) ++ (R) ++ ("ab") ++ (V) ++ (U) ++ (V) ++ (U) ++ ("b") ++ (X) ++ (X) ++ ("baa"));
solve satisfy;
