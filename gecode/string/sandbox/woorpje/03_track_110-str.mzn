var string: D;
var string: E;
var string: F;
constraint
  ((E) ++ (D) ++ ("a") ++ (E) ++ (D) ++ ("b") ++ (F)) =
    (("a") ++ (E) ++ ("a") ++ (E) ++ (E) ++ ("baa"));
solve satisfy;
