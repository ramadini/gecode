var string: K;
var string: G;
var string: E;
var string: N;
var string: M;
constraint
  ((E) ++ (G) ++ (G) ++ (G) ++ ("a") ++ (E) ++ (G) ++ (G) ++ (G) ++ ("b") ++ (K) ++ (K) ++ ("b") ++ (N))
    =
    (("a") ++ (E) ++ (E) ++ (E) ++ ("aa") ++ (M) ++ ("a") ++ (M) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
