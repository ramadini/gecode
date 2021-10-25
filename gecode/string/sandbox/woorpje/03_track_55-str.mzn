var string: I;
var string: K;
var string: G;
var string: E;
constraint
  ((E) ++ (E) ++ ("aaa") ++ (E) ++ (E) ++ ("aab") ++ (I) ++ (G) ++ ("b") ++ (I))
    =
    (("a") ++ (G) ++ (G) ++ (I) ++ (G) ++ (I) ++ ("b") ++ (K) ++ (K) ++ ("baa"));
solve satisfy;
