var string: H;
var string: K;
var string: I;
var string: G;
var string: J;
constraint
  ((H) ++ (H) ++ ("aaaaaa") ++ (G) ++ ("aa") ++ (H) ++ (H) ++ ("aaaaaa") ++ (G) ++ ("ab") ++ (I) ++ (H) ++ (H) ++ ("a") ++ (H) ++ (H) ++ ("a") ++ (H) ++ ("b") ++ (I) ++ ("a") ++ (I) ++ ("aba") ++ (K) ++ ("ba"))
    =
    (("aaaaaaaaaa") ++ (G) ++ ("aaaa") ++ (I) ++ (H) ++ (H) ++ (I) ++ ("a") ++ (I) ++ (H) ++ (I) ++ (I) ++ (H) ++ (H) ++ (I) ++ ("a") ++ (I) ++ (H) ++ (I) ++ ("b") ++ (J) ++ ("aa") ++ (J) ++ ("aab") ++ (J) ++ (K) ++ (J) ++ (K) ++ ("b") ++ (K) ++ (K) ++ ("baa"));
solve satisfy;
