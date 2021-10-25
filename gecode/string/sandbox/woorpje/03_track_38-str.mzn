var string: H;
var string: I;
var string: G;
var string: F;
var string: J;
constraint
  ((G) ++ ("a") ++ (F) ++ ("a") ++ (G) ++ ("a") ++ (F) ++ ("b") ++ (H) ++ (H) ++ (G) ++ ("abaaba"))
    =
    (("aaaaaaaaa") ++ (H) ++ (H) ++ (G) ++ (G) ++ (H) ++ (H) ++ (G) ++ (G) ++ ("b") ++ (I) ++ (I) ++ (I) ++ (I) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
