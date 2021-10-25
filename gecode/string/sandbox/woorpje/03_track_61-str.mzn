var string: I;
var string: K;
var string: H;
var string: G;
var string: J;
constraint
  ((G) ++ ("aaaaaaa") ++ (G) ++ ("aaaaaab") ++ (I) ++ (I) ++ ("aa") ++ (I) ++ (H) ++ ("ab") ++ (I) ++ ("aab") ++ (J) ++ (J) ++ ("b") ++ (K))
    =
    (("aaaaaaaaaaaaaaaaaa") ++ (I) ++ (H) ++ (H) ++ (H) ++ ("a") ++ (I) ++ (H) ++ (H) ++ (H) ++ ("b") ++ (J) ++ ("a") ++ (J) ++ (J) ++ (J) ++ ("a") ++ (J) ++ (J) ++ ("ba") ++ (K) ++ ("a") ++ (K) ++ ("baabaa"));
solve satisfy;
