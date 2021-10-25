var string: J;
var string: I;
var string: K;
var string: H;
var string: V;
var string: F;
var string: R;
var string: L;
var string: N;
var string: O;
constraint
  ((I) ++ (I) ++ (I) ++ ("a") ++ (F) ++ ("a") ++ (I) ++ (I) ++ (I) ++ ("a") ++ (F) ++ ("b") ++ (L) ++ (J) ++ (N) ++ (K) ++ ("b") ++ (N) ++ (O) ++ ("b") ++ (V))
    =
    (("a") ++ (H) ++ (F) ++ (F) ++ (H) ++ ("aa") ++ (N) ++ (N) ++ (K) ++ (N) ++ (N) ++ (N) ++ (K) ++ (N) ++ ("b") ++ (R) ++ (N) ++ (R) ++ (N) ++ ("b") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
