var string: H;
var string: K;
var string: I;
constraint
  (("aaaaaaa") ++ (H) ++ ("aaaaaaaaaa") ++ (H) ++ ("aab") ++ (I) ++ (I) ++ (I) ++ ("aab") ++ (I) ++ (I) ++ (I) ++ ("abaaba"))
    =
    (("a") ++ (H) ++ (H) ++ ("aa") ++ (I) ++ ("aaaa") ++ (I) ++ (I) ++ ("aaaa") ++ (I) ++ ("b") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("aa") ++ (I) ++ ("b") ++ (K) ++ (K) ++ (K) ++ (K) ++ ("b") ++ (K) ++ (K) ++ ("baa"));
solve satisfy;
