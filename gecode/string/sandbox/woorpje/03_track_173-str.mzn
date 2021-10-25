var string: S;
var string: J;
var string: H;
var string: K;
var string: R;
var string: N;
var string: M;
var string: P;
constraint
  ((J) ++ ("aa") ++ (H) ++ ("a") ++ (J) ++ ("aa") ++ (H) ++ ("b") ++ (K) ++ (J) ++ ("b") ++ (P) ++ (N) ++ ("b") ++ (S))
    =
    (("a") ++ (J) ++ (J) ++ (M) ++ (M) ++ (M) ++ (M) ++ (M) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (R) ++ (R) ++ (R) ++ (R) ++ ("b") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
