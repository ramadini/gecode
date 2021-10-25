var string: D;
var string: E;
constraint
  (("a") ++ (D) ++ ("aa") ++ (D) ++ ("b") ++ (E)) =
    (("a") ++ (D) ++ (E) ++ ("aabaa"));
solve satisfy;
