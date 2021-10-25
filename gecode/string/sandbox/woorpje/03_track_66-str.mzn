var string: H;
var string: K;
var string: I;
var string: G;
var string: J;
var string: L;
constraint
  ((G) ++ (H) ++ ("aaaa") ++ (G) ++ (H) ++ ("aaab") ++ (I) ++ (H) ++ ("ab") ++ (I) ++ (J) ++ (I) ++ (I) ++ ("b") ++ (J) ++ ("ab") ++ (K))
    =
    (("aaaaaaa") ++ (G) ++ ("aaaaaaa") ++ (I) ++ ("aaaaa") ++ (I) ++ ("aab") ++ (J) ++ (J) ++ (I) ++ (J) ++ (J) ++ (J) ++ (I) ++ (J) ++ ("b") ++ (K) ++ (K) ++ (K) ++ (K) ++ ("b") ++ (L) ++ (L) ++ ("baa"));
solve satisfy;
