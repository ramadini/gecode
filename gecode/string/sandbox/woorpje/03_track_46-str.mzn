var string: I;
var string: K;
var string: H;
var string: G;
var string: J;
var string: L;
constraint
  (("aaaaaa") ++ (G) ++ ("aa") ++ (G) ++ ("aa") ++ (G) ++ ("aaaaaaa") ++ (G) ++ ("aa") ++ (G) ++ ("aa") ++ (G) ++ ("b") ++ (I) ++ ("aba") ++ (I) ++ (J) ++ ("b") ++ (K) ++ (J) ++ ("b") ++ (K))
    =
    (("a") ++ (G) ++ ("aa") ++ (G) ++ (G) ++ (G) ++ ("aaaaaa") ++ (H) ++ (H) ++ (H) ++ ("aa") ++ (H) ++ (H) ++ (H) ++ ("aab") ++ (I) ++ ("a") ++ (J) ++ (I) ++ ("a") ++ (J) ++ ("b") ++ (K) ++ (K) ++ (K) ++ (K) ++ ("b") ++ (L) ++ (L) ++ ("baa"));
solve satisfy;
