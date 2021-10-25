var string: L;
var string: D;
var string: E;
var string: H;
constraint
  ((D) ++ (E) ++ ("a") ++ (D) ++ (E) ++ ("b") ++ (L)) =
    (("a") ++ (H) ++ (H) ++ (H) ++ ("baa"));
solve satisfy;
