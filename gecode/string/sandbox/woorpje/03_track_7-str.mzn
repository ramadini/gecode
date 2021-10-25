var string: D;
var string: E;
constraint
  (("a") ++ (D) ++ ("aa") ++ (D) ++ ("ba")) =
    (("a") ++ (D) ++ (E) ++ (E) ++ (E) ++ ("baa"));
solve satisfy;
