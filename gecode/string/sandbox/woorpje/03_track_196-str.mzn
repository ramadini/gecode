var string: S;
var string: J;
var string: T;
var string: R;
var string: K;
var string: V;
var string: G;
var string: L;
var string: Y;
var string: M;
var string: Z;
var string: P;
var string: W;
constraint
  ((G) ++ (J) ++ (K) ++ (K) ++ (K) ++ ("aa") ++ (K) ++ ("a") ++ (G) ++ (J) ++ (K) ++ (K) ++ (K) ++ ("aa") ++ (K) ++ ("b") ++ (K) ++ ("aab") ++ (R) ++ (R) ++ ("b") ++ (T) ++ (W) ++ ("b") ++ (Y))
    =
    (("a") ++ (K) ++ ("aa") ++ (K) ++ ("aa") ++ (K) ++ ("aa") ++ (K) ++ ("aa") ++ (M) ++ ("a") ++ (M) ++ (L) ++ (M) ++ ("a") ++ (M) ++ ("a") ++ (M) ++ (L) ++ (M) ++ ("ab") ++ (S) ++ (P) ++ (S) ++ (P) ++ ("b") ++ (T) ++ (V) ++ (T) ++ (V) ++ ("b") ++ (Z) ++ (Z) ++ ("baa"));
solve satisfy;
