var string: I;
var string: H;
var string: K;
var string: G;
var string: J;
var string: L;
constraint
  ((G) ++ (G) ++ (G) ++ (G) ++ (G) ++ ("aa") ++ (G) ++ (G) ++ (G) ++ (G) ++ (G) ++ ("ab") ++ (I) ++ ("aaab") ++ (I) ++ ("b") ++ (J) ++ (J) ++ ("b") ++ (L))
    =
    (("a") ++ (G) ++ ("aaaaaaaaaaaaa") ++ (H) ++ (H) ++ ("a") ++ (H) ++ ("aaa") ++ (H) ++ (H) ++ (H) ++ ("a") ++ (H) ++ ("aaa") ++ (H) ++ ("ba") ++ (J) ++ ("aaa") ++ (J) ++ ("aab") ++ (J) ++ (K) ++ (J) ++ (K) ++ ("b") ++ (K) ++ (K) ++ ("baa"));
solve satisfy;
