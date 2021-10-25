var string: I;
var string: H;
var string: G;
var string: F;
var string: J;
constraint
  ((F) ++ (G) ++ ("aa") ++ (F) ++ (G) ++ ("ab") ++ (G) ++ (H) ++ (G) ++ ("ab") ++ (I) ++ ("b") ++ (I))
    =
    (("a") ++ (G) ++ (G) ++ ("aaaa") ++ (H) ++ (H) ++ ("a") ++ (H) ++ (H) ++ (H) ++ ("a") ++ (H) ++ ("b") ++ (H) ++ ("a") ++ (H) ++ ("ab") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
