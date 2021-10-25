var string: H;
var string: G;
var string: E;
var string: F;
var string: J;
var string: N;
var string: L;
constraint
  ((F) ++ ("a") ++ (G) ++ ("a") ++ (F) ++ ("a") ++ (G) ++ ("ba") ++ (J) ++ ("b") ++ (L))
    =
    (("a") ++ (E) ++ (G) ++ (G) ++ (H) ++ (J) ++ (H) ++ (J) ++ ("b") ++ (N) ++ (N) ++ ("baa"));
solve satisfy;
