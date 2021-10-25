var string: L;
var string: F;
var string: E;
var string: H;
constraint
  ((E) ++ ("a") ++ (E) ++ ("b") ++ (L)) =
    (("a") ++ (F) ++ (H) ++ (H) ++ ("baa"));
solve satisfy;
