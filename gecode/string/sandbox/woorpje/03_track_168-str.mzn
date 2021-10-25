var string: J;
var string: I;
var string: K;
var string: R;
var string: V;
var string: F;
var string: H;
var string: N;
var string: M;
constraint
  ((I) ++ (F) ++ ("aaa") ++ (I) ++ (F) ++ ("aab") ++ (K) ++ (J) ++ ("b") ++ (R) ++ (N) ++ ("b") ++ (V))
    =
    (("a") ++ (H) ++ (H) ++ ("aa") ++ (M) ++ (N) ++ (J) ++ (M) ++ (N) ++ (J) ++ ("b") ++ (R) ++ (R) ++ (R) ++ (R) ++ ("b") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
