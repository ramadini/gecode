var string: S;
var string: H;
var string: K;
var string: R;
var string: U;
var string: G;
var string: M;
var string: P;
constraint
  (("aaaa") ++ (G) ++ (G) ++ ("aaaaa") ++ (G) ++ (G) ++ ("aa") ++ (G) ++ ("aaaaa") ++ (G) ++ (G) ++ ("aaaaa") ++ (G) ++ (G) ++ ("aa") ++ (G) ++ ("b") ++ (K) ++ ("a") ++ (K) ++ (K) ++ ("aa") ++ (K) ++ ("ab") ++ (P) ++ (P) ++ ("b") ++ (S) ++ (S) ++ ("ba"))
    =
    (("a") ++ (H) ++ (G) ++ (G) ++ (G) ++ ("a") ++ (G) ++ (M) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (P) ++ (M) ++ (P) ++ (M) ++ ("b") ++ (R) ++ (R) ++ ("b") ++ (U) ++ (U) ++ ("baa"));
solve satisfy;
