var string: I;
var string: K;
var string: G;
var string: E;
var string: J;
constraint
  ((E) ++ ("aaa") ++ (E) ++ ("aab") ++ (G) ++ (G) ++ ("b") ++ (K)) =
    (("a") ++ (G) ++ ("a") ++ (G) ++ (I) ++ (G) ++ (I) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
