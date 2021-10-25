var string: I;
var string: H;
var string: G;
var string: F;
var string: J;
constraint
  ((F) ++ (F) ++ (G) ++ ("a") ++ (F) ++ (F) ++ (G) ++ ("b") ++ (G) ++ ("aba") ++ (I) ++ ("b") ++ (I))
    =
    (("a") ++ (G) ++ ("aaaa") ++ (H) ++ ("aa") ++ (H) ++ ("aab") ++ (I) ++ (I) ++ (I) ++ (I) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
