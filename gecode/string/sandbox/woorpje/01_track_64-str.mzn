var string: K;
var string: G;
var string: E;
var string: O;
var string: C;
constraint
  (("b") ++ (C) ++ ("bb") ++ (K) ++ (E) ++ (O) ++ (O) ++ (C) ++ (C) ++ ("baca") ++ (C) ++ (O))
    = (("b") ++ (C) ++ ("b") ++ (G) ++ ("ba") ++ (E) ++ ("abc"));
solve satisfy;
