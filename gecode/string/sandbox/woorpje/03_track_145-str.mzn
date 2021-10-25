var string: J;
var string: H;
var string: K;
var string: G;
var string: L;
var string: O;
var string: M;
var string: Q;
constraint
  ((G) ++ ("a") ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ ("a") ++ (G) ++ (G) ++ ("a") ++ (G) ++ ("a") ++ (G) ++ ("a") ++ (G) ++ ("a") ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ ("a") ++ (G) ++ (G) ++ ("a") ++ (G) ++ ("a") ++ (G) ++ ("b") ++ (K) ++ ("a") ++ (K) ++ ("a") ++ (K) ++ ("a") ++ (K) ++ (K) ++ ("b") ++ (M) ++ (L) ++ ("b") ++ (M) ++ (O) ++ ("b") ++ (O))
    =
    (("a") ++ (G) ++ ("aaa") ++ (H) ++ (G) ++ ("a") ++ (G) ++ (G) ++ (G) ++ ("aaa") ++ (J) ++ (K) ++ (K) ++ ("aa") ++ (J) ++ (K) ++ (K) ++ ("aab") ++ (L) ++ ("aa") ++ (L) ++ ("aab") ++ (M) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
