var string: I;
var string: G;
var string: F;
var string: H;
constraint
  (("aaaa") ++ (F) ++ ("aaaaaaa") ++ (F) ++ ("aab") ++ (G) ++ (H) ++ ("ba") ++ (H) ++ ("b") ++ (I))
    =
    (("a") ++ (G) ++ (F) ++ ("a") ++ (G) ++ (G) ++ (G) ++ (G) ++ ("b") ++ (H) ++ ("a") ++ (H) ++ ("abaabaa"));
solve satisfy;
