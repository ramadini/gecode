var string: N;
var string: I;
var string: K;
var string: H;
var string: R;
var string: G;
var string: L;
var string: O;
constraint
  ((I) ++ (I) ++ (H) ++ (I) ++ ("a") ++ (I) ++ (I) ++ (H) ++ (I) ++ ("b") ++ (L) ++ (K) ++ (I) ++ ("ba") ++ (L) ++ ("b") ++ (O))
    =
    (("a") ++ (G) ++ (H) ++ (G) ++ (H) ++ (L) ++ ("a") ++ (K) ++ (K) ++ (L) ++ ("a") ++ (K) ++ (K) ++ ("b") ++ (N) ++ (O) ++ (N) ++ (O) ++ ("b") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
