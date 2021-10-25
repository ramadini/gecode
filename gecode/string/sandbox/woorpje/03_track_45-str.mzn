var string: I;
var string: G;
var string: H;
constraint
  (("aaaaaaaaaaaaaaaaab") ++ (G) ++ (G) ++ (G) ++ (G) ++ ("b") ++ (I) ++ (I) ++ ("b") ++ (I))
    =
    (("a") ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ (H) ++ (G) ++ (G) ++ (G) ++ (H) ++ ("b") ++ (I) ++ ("a") ++ (I) ++ ("abaabaa"));
solve satisfy;
