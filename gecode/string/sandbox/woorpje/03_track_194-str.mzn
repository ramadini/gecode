var string: H;
var string: D;
var string: E;
var string: F;
constraint
  ((E) ++ (D) ++ ("a") ++ (E) ++ (D) ++ ("b") ++ (H)) =
    (("a") ++ (F) ++ (F) ++ (F) ++ (F) ++ ("baa"));
solve satisfy;
