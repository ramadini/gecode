var string: I;
var string: H;
var string: G;
var string: F;
var string: J;
constraint
  ((F) ++ (G) ++ (G) ++ ("aaa") ++ (G) ++ ("a") ++ (F) ++ (G) ++ (G) ++ ("aaa") ++ (G) ++ ("b") ++ (H) ++ (H) ++ (G) ++ ("ba") ++ (I) ++ ("b") ++ (I))
    =
    (("aaa") ++ (G) ++ (G) ++ (F) ++ ("a") ++ (G) ++ (H) ++ (H) ++ ("aa") ++ (H) ++ (H) ++ ("aab") ++ (I) ++ (I) ++ (I) ++ (I) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
