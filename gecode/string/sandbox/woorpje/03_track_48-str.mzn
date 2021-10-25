var string: H;
var string: I;
var string: F;
var string: G;
var string: J;
constraint
  (("aaa") ++ (F) ++ ("aaaaaa") ++ (F) ++ ("aab") ++ (H) ++ ("aaab") ++ (H) ++ ("ab") ++ (I))
    =
    (("a") ++ (G) ++ ("aa") ++ (H) ++ (H) ++ (H) ++ (H) ++ (H) ++ (H) ++ (H) ++ (H) ++ ("b") ++ (H) ++ (I) ++ (H) ++ (I) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
