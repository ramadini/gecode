var string: H;
var string: K;
var string: I;
var string: G;
var string: J;
var string: N;
var string: M;
constraint
  ((H) ++ (H) ++ (H) ++ ("a") ++ (G) ++ (H) ++ (H) ++ (H) ++ ("a") ++ (H) ++ (H) ++ (H) ++ ("a") ++ (G) ++ (H) ++ (H) ++ (H) ++ ("baa") ++ (I) ++ ("ab") ++ (K) ++ (J) ++ ("b") ++ (N))
    =
    (("a") ++ (G) ++ (H) ++ (G) ++ (G) ++ (H) ++ ("a") ++ (G) ++ ("a") ++ (H) ++ ("a") ++ (I) ++ (H) ++ ("a") ++ (I) ++ ("b") ++ (J) ++ (K) ++ (J) ++ (K) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
