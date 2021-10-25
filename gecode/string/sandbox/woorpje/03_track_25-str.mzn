var string: I;
var string: K;
var string: H;
var string: G;
var string: J;
var string: L;
constraint
  ((G) ++ (G) ++ ("a") ++ (G) ++ (G) ++ ("b") ++ (I) ++ ("aaaaab") ++ (I) ++ (I) ++ ("ba") ++ (K) ++ ("ba"))
    =
    (("a") ++ (H) ++ ("aaaa") ++ (I) ++ ("aaaaa") ++ (I) ++ ("aab") ++ (J) ++ ("a") ++ (J) ++ ("ab") ++ (K) ++ (K) ++ (K) ++ (K) ++ ("b") ++ (L) ++ (L) ++ ("baa"));
solve satisfy;
