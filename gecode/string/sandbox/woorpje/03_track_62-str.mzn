var string: H;
var string: I;
var string: G;
var string: F;
var string: J;
constraint
  ((G) ++ ("a") ++ (F) ++ ("aaa") ++ (G) ++ ("a") ++ (F) ++ ("aab") ++ (G) ++ (H) ++ (H) ++ ("b") ++ (H) ++ (I) ++ ("b") ++ (I))
    =
    (("aaa") ++ (G) ++ (G) ++ (G) ++ ("aa") ++ (G) ++ ("aa") ++ (G) ++ ("b") ++ (H) ++ ("a") ++ (H) ++ ("ab") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
