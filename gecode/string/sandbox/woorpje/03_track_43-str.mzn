var string: F;
var string: E;
constraint
  ((E) ++ (E) ++ ("a") ++ (E) ++ (E) ++ ("ba")) =
    (("aa") ++ (F) ++ (F) ++ (F) ++ ("baa"));
solve satisfy;
