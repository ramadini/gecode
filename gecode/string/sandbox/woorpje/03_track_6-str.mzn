var string: H;
var string: I;
var string: K;
var string: G;
var string: L;
var string: M;
constraint
  (("aa") ++ (H) ++ (H) ++ ("aaaaa") ++ (H) ++ (H) ++ ("aab") ++ (H) ++ ("a") ++ (H) ++ (I) ++ ("ba") ++ (L) ++ ("ba"))
    =
    (("aaa") ++ (G) ++ (G) ++ (G) ++ (I) ++ (H) ++ ("a") ++ (H) ++ (I) ++ (H) ++ ("a") ++ (H) ++ ("b") ++ (K) ++ (K) ++ (K) ++ (K) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
