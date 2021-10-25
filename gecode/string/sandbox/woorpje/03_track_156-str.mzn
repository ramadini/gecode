var string: S;
var string: J;
var string: I;
var string: K;
var string: H;
var string: V;
var string: G;
var string: R;
var string: U;
var string: O;
var string: M;
var string: P;
constraint
  ((G) ++ (I) ++ (I) ++ ("aaaa") ++ (I) ++ ("a") ++ (G) ++ (I) ++ (I) ++ ("aaaa") ++ (I) ++ ("b") ++ (K) ++ (K) ++ (K) ++ (J) ++ ("b") ++ (O) ++ (P) ++ ("b") ++ (P) ++ (S) ++ ("b") ++ (V))
    =
    (("a") ++ (H) ++ (I) ++ ("aaa") ++ (I) ++ ("a") ++ (M) ++ (M) ++ (M) ++ (J) ++ (M) ++ (M) ++ (M) ++ (J) ++ ("b") ++ (M) ++ (M) ++ (M) ++ ("a") ++ (M) ++ (M) ++ (M) ++ ("ab") ++ (R) ++ ("a") ++ (R) ++ ("ab") ++ (U) ++ (U) ++ ("baa"));
solve satisfy;
