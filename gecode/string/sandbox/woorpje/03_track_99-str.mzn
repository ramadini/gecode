var string: H;
var string: D;
var string: E;
constraint
  ((D) ++ (D) ++ ("a") ++ (D) ++ (D) ++ ("b") ++ (H)) =
    (("a") ++ (E) ++ (E) ++ ("aabaa"));
solve satisfy;
