var string: H;
var string: K;
var string: E;
var string: J;
constraint
  ((E) ++ (E) ++ ("a") ++ (E) ++ (E) ++ ("b") ++ (K)) =
    (("a") ++ (H) ++ (H) ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
