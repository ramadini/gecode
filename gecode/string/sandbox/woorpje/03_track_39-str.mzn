var string: I;
var string: K;
var string: H;
var string: G;
var string: J;
constraint
  (("aaaaaaa") ++ (G) ++ ("aaaaaaaaaa") ++ (G) ++ ("aab") ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("a") ++ (I) ++ (I) ++ (I) ++ ("ba") ++ (I) ++ (I) ++ (I) ++ ("b") ++ (J) ++ (K) ++ ("ba"))
    =
    (("a") ++ (G) ++ (G) ++ ("aa") ++ (H) ++ (I) ++ ("a") ++ (H) ++ (I) ++ ("ab") ++ (J) ++ (J) ++ (J) ++ ("a") ++ (J) ++ (J) ++ (J) ++ ("ab") ++ (K) ++ (K) ++ (K) ++ (K) ++ ("b") ++ (K) ++ (K) ++ ("baa"));
solve satisfy;
