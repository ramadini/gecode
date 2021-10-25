var string: S;
var string: T;
var string: X;
var string: I;
var string: K;
var string: H;
var string: R;
var string: G;
var string: L;
var string: O;
var string: M;
var string: P;
constraint
  ((I) ++ (K) ++ (G) ++ ("a") ++ (I) ++ (K) ++ (G) ++ ("b") ++ (L) ++ (L) ++ ("b") ++ (P) ++ (P) ++ (P) ++ (O) ++ ("b") ++ (S) ++ (S) ++ ("b"))
    =
    (("a") ++ (H) ++ (G) ++ ("aa") ++ (G) ++ (G) ++ ("aa") ++ (M) ++ ("a") ++ (M) ++ ("ab") ++ (O) ++ (R) ++ (R) ++ ("a") ++ (O) ++ (R) ++ (R) ++ ("aba") ++ (T) ++ ("a") ++ (T) ++ ("b") ++ (X) ++ (X) ++ ("baa"));
solve satisfy;
