var string: F;
var string: E;
var string: D;
constraint
  (("a") ++ (E) ++ ("aa") ++ (E) ++ ("b") ++ (F)) =
    (("a") ++ (D) ++ (D) ++ (E) ++ (E) ++ ("baa"));
solve satisfy;
