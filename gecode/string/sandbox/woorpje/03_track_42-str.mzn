var string: H;
var string: G;
var string: F;
var string: I;
constraint
  ((F) ++ ("aaa") ++ (F) ++ ("aab") ++ (H) ++ ("a") ++ (H) ++ (H) ++ ("b") ++ (H) ++ (I) ++ ("ba"))
    =
    (("a") ++ (G) ++ (G) ++ ("aaa") ++ (H) ++ (H) ++ (H) ++ ("a") ++ (H) ++ (H) ++ (H) ++ ("b") ++ (I) ++ ("a") ++ (I) ++ ("ab") ++ (I) ++ (I) ++ ("baa"));
solve satisfy;
