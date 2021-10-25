var string: I;
var string: K;
var string: G;
var string: E;
var string: F;
var string: M;
var string: Q;
constraint
  ((E) ++ (G) ++ (F) ++ ("a") ++ (E) ++ (G) ++ (F) ++ ("b") ++ (K) ++ (I) ++ ("b") ++ (M))
    =
    (("a") ++ (E) ++ (F) ++ (I) ++ (I) ++ (M) ++ (I) ++ (M) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
