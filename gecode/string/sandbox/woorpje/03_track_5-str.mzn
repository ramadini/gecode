var string: H;
var string: K;
var string: I;
var string: G;
var string: J;
var string: L;
var string: M;
constraint
  (("aaaa") ++ (H) ++ ("aaaaa") ++ (H) ++ ("b") ++ (H) ++ (H) ++ (H) ++ (H) ++ ("b") ++ (K) ++ ("ba"))
    =
    (("a") ++ (G) ++ ("aaa") ++ (I) ++ ("a") ++ (J) ++ (H) ++ (I) ++ ("a") ++ (J) ++ (H) ++ ("b") ++ (L) ++ (J) ++ (L) ++ (J) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
