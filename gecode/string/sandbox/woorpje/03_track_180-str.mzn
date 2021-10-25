var string: K;
var string: N;
var string: F;
var string: E;
var string: J;
var string: L;
constraint
  ((F) ++ (F) ++ (E) ++ ("a") ++ (F) ++ (F) ++ (E) ++ ("b") ++ (K) ++ (K) ++ ("b") ++ (N))
    =
    (("aa") ++ (F) ++ (E) ++ (J) ++ (J) ++ (J) ++ (J) ++ ("b") ++ (L) ++ (L) ++ ("baa"));
solve satisfy;
