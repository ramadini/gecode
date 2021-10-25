var string: I;
var string: H;
var string: G;
var string: F;
var string: J;
constraint
  ((G) ++ (F) ++ ("aa") ++ (G) ++ (F) ++ ("ab") ++ (G) ++ (I) ++ ("b") ++ (I))
    =
    (("a") ++ (F) ++ ("a") ++ (G) ++ (G) ++ (H) ++ (G) ++ (H) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
