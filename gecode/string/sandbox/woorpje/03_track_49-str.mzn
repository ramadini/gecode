var string: H;
var string: I;
var string: G;
var string: F;
var string: J;
constraint
  (("a") ++ (F) ++ (F) ++ (F) ++ ("aaaaaa") ++ (F) ++ (F) ++ (F) ++ ("aaaab") ++ (H) ++ (G) ++ ("a") ++ (G) ++ ("b") ++ (H) ++ ("ab") ++ (I))
    =
    (("a") ++ (G) ++ (F) ++ ("a") ++ (F) ++ (G) ++ (G) ++ (H) ++ (H) ++ (G) ++ (G) ++ (H) ++ (H) ++ ("b") ++ (I) ++ (I) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
