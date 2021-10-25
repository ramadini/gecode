var string: S;
var string: J;
var string: I;
var string: R;
var string: H;
var string: K;
var string: G;
var string: V;
var string: N;
var string: M;
var string: P;
constraint
  ((G) ++ ("aa") ++ (I) ++ (I) ++ (I) ++ (I) ++ (I) ++ ("aa") ++ (G) ++ ("aa") ++ (I) ++ (I) ++ (I) ++ (I) ++ (I) ++ ("ab") ++ (J) ++ (M) ++ ("b") ++ (P) ++ (P) ++ ("b") ++ (P) ++ (R) ++ ("b") ++ (S))
    =
    (("a") ++ (H) ++ (H) ++ (H) ++ ("a") ++ (M) ++ (K) ++ ("aa") ++ (M) ++ (K) ++ ("aaba") ++ (N) ++ ("a") ++ (N) ++ ("a") ++ (N) ++ ("a") ++ (N) ++ ("b") ++ (R) ++ (R) ++ (R) ++ (R) ++ ("b") ++ (V) ++ (V) ++ ("baa"));
solve satisfy;
