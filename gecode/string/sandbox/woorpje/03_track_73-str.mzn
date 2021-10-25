var string: S;
var string: J;
var string: I;
var string: K;
var string: R;
var string: V;
var string: G;
var string: U;
var string: M;
var string: P;
constraint
  ((I) ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (G) ++ (G) ++ (G) ++ ("aa") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (G) ++ (G) ++ (G) ++ ("ab") ++ (K) ++ (J) ++ ("ab") ++ (P) ++ ("ab") ++ (S) ++ (S) ++ ("b") ++ (V))
    =
    (("a") ++ (J) ++ (G) ++ (I) ++ (I) ++ ("a") ++ (I) ++ (J) ++ (J) ++ (M) ++ ("a") ++ (M) ++ ("a") ++ (J) ++ (J) ++ (M) ++ ("a") ++ (M) ++ ("abaa") ++ (M) ++ ("aa") ++ (M) ++ ("b") ++ (R) ++ (R) ++ (R) ++ (R) ++ ("b") ++ (U) ++ (U) ++ ("baa"));
solve satisfy;
