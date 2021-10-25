var string: I;
var string: K;
var string: H;
var string: G;
var string: E;
constraint
  (("aa") ++ (E) ++ ("aaa") ++ (E) ++ ("b") ++ (G) ++ (G) ++ ("b") ++ (K)) =
    (("a") ++ (G) ++ ("a") ++ (I) ++ (H) ++ (I) ++ (H) ++ ("b") ++ (I) ++ (I) ++ ("baa"));
solve satisfy;
