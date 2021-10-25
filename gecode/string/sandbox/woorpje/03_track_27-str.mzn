var string: S;
var string: I;
var string: K;
var string: H;
var string: R;
var string: G;
var string: V;
var string: N;
var string: M;
var string: P;
constraint
  ((G) ++ (I) ++ (I) ++ (I) ++ ("a") ++ (I) ++ (G) ++ (I) ++ ("a") ++ (G) ++ (I) ++ (I) ++ (I) ++ ("a") ++ (I) ++ (G) ++ (I) ++ ("b") ++ (K) ++ (K) ++ ("aa") ++ (K) ++ (K) ++ ("a") ++ (K) ++ ("b") ++ (M) ++ (M) ++ (N) ++ ("b") ++ (P) ++ (S) ++ ("ba"))
    =
    (("a") ++ (H) ++ (I) ++ (I) ++ ("aaa") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (M) ++ ("a") ++ (M) ++ (M) ++ (M) ++ ("a") ++ (K) ++ (K) ++ (M) ++ ("a") ++ (M) ++ (M) ++ (M) ++ ("a") ++ (K) ++ (K) ++ ("b") ++ (N) ++ (M) ++ ("a") ++ (N) ++ (M) ++ ("ab") ++ (R) ++ (R) ++ (R) ++ (R) ++ ("b") ++ (V) ++ (V) ++ ("baa"));
solve satisfy;
