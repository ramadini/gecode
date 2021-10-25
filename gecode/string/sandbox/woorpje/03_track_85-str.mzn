var string: H;
var string: K;
var string: I;
var string: G;
var string: J;
var string: N;
var string: M;
constraint
  (("aaa") ++ (H) ++ ("aaaaaa") ++ (H) ++ ("aab") ++ (H) ++ (J) ++ ("b") ++ (J) ++ (K) ++ ("b") ++ (N))
    =
    (("a") ++ (H) ++ ("a") ++ (G) ++ (H) ++ (I) ++ (H) ++ (I) ++ ("ba") ++ (J) ++ ("a") ++ (J) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
