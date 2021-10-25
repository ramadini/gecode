var string: H;
var string: K;
var string: F;
var string: G;
var string: E;
var string: L;
constraint
  ((H) ++ (F) ++ (F) ++ ("a") ++ (H) ++ (F) ++ (F) ++ ("b") ++ (K) ++ (K) ++ ("b") ++ (K))
    =
    (("a") ++ (G) ++ (E) ++ (H) ++ (H) ++ (H) ++ (H) ++ ("b") ++ (L) ++ (L) ++ ("baa"));
solve satisfy;
