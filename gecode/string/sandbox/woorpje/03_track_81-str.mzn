var string: H;
var string: K;
var string: J;
var string: L;
var string: N;
var string: M;
constraint
  (("aaaaaaaaaaaaaaaaabaa") ++ (H) ++ ("b") ++ (K) ++ ("b") ++ (N)) =
    (("a") ++ (H) ++ (J) ++ ("a") ++ (J) ++ (J) ++ (J) ++ ("a") ++ (J) ++ (J) ++ ("b") ++ (L) ++ (J) ++ (L) ++ (J) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
