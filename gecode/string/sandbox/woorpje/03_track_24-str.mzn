var string: G;
var string: F;
var string: H;
constraint
  ((F) ++ (F) ++ (F) ++ (F) ++ ("a") ++ (F) ++ (F) ++ (F) ++ (F) ++ ("b") ++ (G) ++ (F) ++ ("b") ++ (G))
    =
    (("aaaa") ++ (F) ++ (F) ++ ("a") ++ (F) ++ ("ab") ++ (H) ++ (H) ++ ("baa"));
solve satisfy;
