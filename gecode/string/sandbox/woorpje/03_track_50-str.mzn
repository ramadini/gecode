var string: I;
var string: K;
var string: H;
var string: G;
var string: J;
var string: L;
constraint
  (("a") ++ (G) ++ ("aaaa") ++ (G) ++ ("aa") ++ (G) ++ ("aaaaaaaa") ++ (G) ++ ("aaaa") ++ (G) ++ ("aa") ++ (G) ++ ("aaaaaab") ++ (I) ++ (H) ++ ("aa") ++ (H) ++ ("b") ++ (J) ++ ("aab") ++ (K) ++ ("ab") ++ (K))
    =
    (("a") ++ (H) ++ (G) ++ ("aaa") ++ (G) ++ ("a") ++ (G) ++ ("aa") ++ (H) ++ ("a") ++ (H) ++ ("aaa") ++ (H) ++ ("a") ++ (H) ++ ("a") ++ (H) ++ ("aaa") ++ (H) ++ ("abaa") ++ (J) ++ ("aa") ++ (J) ++ ("baaaab") ++ (L) ++ (L) ++ ("baa"));
solve satisfy;
