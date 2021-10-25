var string: H;
var string: K;
var string: I;
var string: J;
constraint
  (("a") ++ (H) ++ ("a") ++ (H) ++ ("aaaaa") ++ (H) ++ ("a") ++ (H) ++ ("a") ++ (H) ++ ("aaaa") ++ (H) ++ ("a") ++ (H) ++ ("aaaaa") ++ (H) ++ ("a") ++ (H) ++ ("a") ++ (H) ++ ("aab") ++ (I) ++ ("baa") ++ (I) ++ ("b") ++ (J) ++ (K) ++ ("b") ++ (K))
    =
    (("a") ++ (H) ++ ("a") ++ (H) ++ ("aa") ++ (H) ++ ("aa") ++ (H) ++ (H) ++ ("a") ++ (H) ++ ("aa") ++ (H) ++ (H) ++ (H) ++ (H) ++ (H) ++ ("a") ++ (H) ++ ("aaa") ++ (H) ++ (H) ++ (H) ++ ("a") ++ (H) ++ ("aaab") ++ (J) ++ ("a") ++ (J) ++ ("a") ++ (J) ++ ("a") ++ (J) ++ ("aba") ++ (J) ++ ("a") ++ (J) ++ ("baabaa"));
solve satisfy;
