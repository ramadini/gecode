var string: I;
var string: G;
var string: F;
constraint
  ((F) ++ ("a") ++ (G) ++ ("aaa") ++ (F) ++ ("a") ++ (G) ++ ("aab") ++ (G) ++ (G) ++ (G) ++ ("ab") ++ (I) ++ (I) ++ ("ba"))
    =
    (("aaaaa") ++ (F) ++ ("aa") ++ (G) ++ (G) ++ ("aa") ++ (G) ++ (G) ++ ("aab") ++ (I) ++ ("a") ++ (I) ++ ("ab") ++ (I) ++ (I) ++ ("baa"));
solve satisfy;
