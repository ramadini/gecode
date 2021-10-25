var string: I;
var string: H;
var string: F;
var string: G;
var string: J;
constraint
  ((F) ++ (F) ++ ("a") ++ (F) ++ (F) ++ ("aaaa") ++ (F) ++ (F) ++ ("a") ++ (F) ++ (F) ++ ("aaabaaaab") ++ (I) ++ (I) ++ ("b") ++ (I))
    =
    (("a") ++ (G) ++ (F) ++ (F) ++ ("aa") ++ (F) ++ (F) ++ (H) ++ ("a") ++ (H) ++ ("ab") ++ (H) ++ (I) ++ (H) ++ (I) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
