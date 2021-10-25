var string: H;
var string: K;
var string: I;
var string: G;
var string: E;
var string: M;
constraint
  ((H) ++ (E) ++ (G) ++ ("a") ++ (H) ++ (E) ++ (G) ++ ("b") ++ (I) ++ (K) ++ ("b") ++ (K))
    =
    (("aa") ++ (E) ++ ("a") ++ (G) ++ (H) ++ (H) ++ (H) ++ (H) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
