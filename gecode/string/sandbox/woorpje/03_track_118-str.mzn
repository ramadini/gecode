var string: I;
var string: G;
var string: E;
var string: J;
constraint
  ((E) ++ (E) ++ (E) ++ (G) ++ ("a") ++ (E) ++ (E) ++ (E) ++ (G) ++ ("b") ++ (I) ++ (I) ++ ("b") ++ (I))
    =
    (("a") ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
