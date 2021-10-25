var string: H;
var string: K;
var string: E;
var string: L;
constraint
  ((E) ++ ("a") ++ (E) ++ ("b") ++ (K)) =
    (("a") ++ (H) ++ (H) ++ (L) ++ (L) ++ ("baa"));
solve satisfy;
