var string: I;
var string: K;
var string: G;
var string: E;
var string: J;
constraint
  ((E) ++ (E) ++ ("a") ++ (E) ++ ("a") ++ (E) ++ (E) ++ ("a") ++ (E) ++ ("b") ++ (G) ++ (G) ++ ("b") ++ (K))
    =
    (("aa") ++ (E) ++ ("a") ++ (E) ++ (G) ++ (I) ++ (G) ++ (I) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
