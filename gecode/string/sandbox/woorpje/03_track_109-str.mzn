var string: H;
var string: N;
var string: E;
var string: J;
var string: L;
var string: M;
constraint
  ((E) ++ ("a") ++ (E) ++ ("b") ++ (L) ++ ("b") ++ (N)) =
    (("a") ++ (H) ++ (H) ++ (H) ++ ("a") ++ (J) ++ (J) ++ (J) ++ (J) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
