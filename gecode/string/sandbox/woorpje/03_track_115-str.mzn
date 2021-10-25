var string: E;
var string: J;
var string: H;
var string: K;
var string: I;
var string: F;
var string: N;
var string: Q;
constraint
  ((E) ++ (E) ++ ("a") ++ (E) ++ (E) ++ ("b") ++ (K) ++ (K) ++ ("b") ++ (N))
    =
    (("a") ++ (F) ++ (H) ++ (I) ++ (J) ++ (I) ++ (J) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
