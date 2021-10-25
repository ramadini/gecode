var string: H;
var string: I;
var string: K;
var string: J;
var string: L;
constraint
  ((H) ++ ("aaa") ++ (H) ++ ("aaaaaaaa") ++ (H) ++ ("aaa") ++ (H) ++ ("aaaaaaabaa") ++ (I) ++ (I) ++ (I) ++ ("b") ++ (I) ++ ("ab") ++ (J) ++ (J) ++ ("b") ++ (L))
    =
    (("aaaaaaa") ++ (H) ++ ("aaaaaaa") ++ (I) ++ (I) ++ ("aaaa") ++ (I) ++ (I) ++ ("aaaab") ++ (J) ++ (J) ++ (J) ++ (J) ++ ("b") ++ (J) ++ (K) ++ (J) ++ (K) ++ ("baabaa"));
solve satisfy;
