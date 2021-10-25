var string: H;
var string: K;
var string: I;
var string: F;
var string: E;
var string: G;
var string: M;
constraint
  ((H) ++ ("a") ++ (F) ++ (H) ++ ("a") ++ (H) ++ ("a") ++ (F) ++ (H) ++ ("b") ++ (I) ++ (K) ++ ("b") ++ (K))
    =
    (("a") ++ (G) ++ (E) ++ (H) ++ (K) ++ (H) ++ (K) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
