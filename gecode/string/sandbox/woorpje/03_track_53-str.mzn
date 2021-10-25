var string: H;
var string: F;
var string: E;
var string: L;
var string: M;
var string: Q;
constraint
  ((E) ++ ("a") ++ (E) ++ ("b") ++ (L) ++ (L) ++ ("b") ++ (M)) =
    (("a") ++ (F) ++ (H) ++ (M) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
