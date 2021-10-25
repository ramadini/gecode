var string: I;
var string: H;
var string: F;
var string: G;
var string: J;
constraint
  ((F) ++ ("aa") ++ (F) ++ ("abaaaaba") ++ (I) ++ ("ba")) =
    (("a") ++ (G) ++ (G) ++ ("aa") ++ (H) ++ (H) ++ (H) ++ (H) ++ ("b") ++ (H) ++ (H) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
