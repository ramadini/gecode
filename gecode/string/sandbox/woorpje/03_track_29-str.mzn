var string: H;
var string: K;
var string: G;
var string: L;
var string: M;
constraint
  ((H) ++ (H) ++ ("aaaaa") ++ (H) ++ (H) ++ ("aaaaba") ++ (H) ++ ("aab") ++ (K) ++ (L) ++ ("ba"))
    =
    (("a") ++ (G) ++ (G) ++ (H) ++ (H) ++ (H) ++ ("a") ++ (H) ++ (H) ++ (H) ++ ("a") ++ (H) ++ ("b") ++ (L) ++ (L) ++ (L) ++ (L) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
