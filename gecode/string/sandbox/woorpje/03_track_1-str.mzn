var string: H;
var string: K;
var string: I;
var string: J;
var string: L;
constraint
  (("aaaaaaa") ++ (H) ++ ("aaaaaaaaaa") ++ (H) ++ ("aab") ++ (I) ++ (H) ++ (I) ++ (I) ++ ("b") ++ (J) ++ ("ab") ++ (K) ++ ("aba"))
    =
    (("a") ++ (H) ++ (H) ++ ("aa") ++ (I) ++ (H) ++ (I) ++ (H) ++ (I) ++ (H) ++ (I) ++ (H) ++ ("b") ++ (I) ++ ("a") ++ (I) ++ ("abaaaab") ++ (L) ++ (L) ++ ("baa"));
solve satisfy;
