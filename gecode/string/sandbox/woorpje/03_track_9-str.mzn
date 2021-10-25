var string: G;
var string: F;
constraint
  (("a") ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ ("ab") ++ (F) ++ (F) ++ ("ba"))
    =
    (("aaa") ++ (F) ++ ("a") ++ (G) ++ (G) ++ ("b") ++ (G) ++ (G) ++ ("baa"));
solve satisfy;
