var string: N;
var string: J;
var string: I;
var string: K;
var string: H;
var string: G;
var string: L;
var string: O;
var string: M;
var string: P;
var string: Q;
constraint
  ((I) ++ ("aaaa") ++ (H) ++ ("aa") ++ (I) ++ ("aaaa") ++ (H) ++ ("ab") ++ (K) ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("b") ++ (L) ++ ("aab") ++ (O) ++ (N) ++ ("b") ++ (P))
    =
    (("a") ++ (H) ++ (H) ++ (G) ++ ("a") ++ (J) ++ (K) ++ (I) ++ (J) ++ (K) ++ (I) ++ ("baa") ++ (L) ++ ("aa") ++ (L) ++ ("ba") ++ (M) ++ ("a") ++ (M) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
