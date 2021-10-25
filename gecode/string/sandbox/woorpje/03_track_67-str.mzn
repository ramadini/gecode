var string: I;
var string: K;
var string: H;
var string: G;
var string: L;
var string: N;
var string: O;
constraint
  ((G) ++ (I) ++ ("a") ++ (H) ++ (I) ++ ("aaa") ++ (G) ++ (I) ++ ("a") ++ (H) ++ (I) ++ ("aaba") ++ (K) ++ (K) ++ (K) ++ (K) ++ (I) ++ (K) ++ ("b") ++ (L) ++ ("aaba") ++ (O) ++ ("b") ++ (O))
    =
    (("a") ++ (H) ++ (H) ++ ("a") ++ (H) ++ (H) ++ (H) ++ (H) ++ ("a") ++ (H) ++ (H) ++ (H) ++ (H) ++ (H) ++ ("a") ++ (H) ++ (H) ++ (K) ++ (K) ++ (K) ++ (K) ++ ("a") ++ (K) ++ (K) ++ ("a") ++ (K) ++ (K) ++ (K) ++ (K) ++ ("a") ++ (K) ++ (K) ++ ("abaa") ++ (K) ++ ("aa") ++ (K) ++ ("ba") ++ (N) ++ ("a") ++ (N) ++ ("baabaa"));
solve satisfy;
