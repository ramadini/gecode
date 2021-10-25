var string: J;
var string: H;
var string: K;
var string: G;
var string: N;
var string: O;
var string: M;
var string: Q;
constraint
  (("aaaaaa") ++ (G) ++ ("aaa") ++ (G) ++ ("aaaaaaaa") ++ (G) ++ ("aaa") ++ (G) ++ ("ab") ++ (K) ++ (K) ++ ("aa") ++ (K) ++ ("baaa") ++ (M) ++ ("b") ++ (M) ++ (N) ++ ("b") ++ (O))
    =
    (("a") ++ (H) ++ ("aaaa") ++ (H) ++ (H) ++ ("aaa") ++ (J) ++ (K) ++ ("aaa") ++ (J) ++ (K) ++ ("baaaaaaaab") ++ (M) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
