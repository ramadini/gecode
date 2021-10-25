var string: H;
var string: G;
var string: F;
constraint
  ((F) ++ (F) ++ ("a") ++ (F) ++ (F) ++ ("baab") ++ (H)) =
    (("aaaaaa") ++ (G) ++ ("a") ++ (G) ++ ("baabaa"));
solve satisfy;
