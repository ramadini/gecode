var string: D;
var string: E;
var string: F;
constraint
  ((D) ++ (D) ++ ("a") ++ (D) ++ (D) ++ ("ba")) =
    (("a") ++ (D) ++ (E) ++ (F) ++ (F) ++ ("baa"));
solve satisfy;
