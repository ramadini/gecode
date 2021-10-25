var string: H;
var string: K;
var string: F;
var string: G;
var string: J;
var string: L;
var string: M;
constraint
  ((F) ++ ("aaa") ++ (F) ++ ("aaba") ++ (H) ++ (H) ++ (H) ++ ("ba") ++ (K) ++ ("b") ++ (M))
    =
    (("a") ++ (G) ++ ("aa") ++ (H) ++ ("a") ++ (J) ++ (H) ++ ("a") ++ (J) ++ ("ba") ++ (L) ++ ("a") ++ (L) ++ ("baabaa"));
solve satisfy;
