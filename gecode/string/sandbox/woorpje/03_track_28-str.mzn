var string: J;
var string: H;
var string: I;
var string: K;
var string: G;
var string: L;
var string: N;
var string: M;
var string: P;
constraint
  ((H) ++ ("a") ++ (H) ++ ("a") ++ (H) ++ (H) ++ (G) ++ ("aa") ++ (H) ++ (H) ++ ("a") ++ (H) ++ ("a") ++ (H) ++ ("a") ++ (H) ++ (H) ++ (G) ++ ("aa") ++ (H) ++ (H) ++ ("b") ++ (I) ++ (I) ++ ("b") ++ (L) ++ ("aaba") ++ (N) ++ ("ba"))
    =
    (("aa") ++ (H) ++ (H) ++ (H) ++ (H) ++ ("aa") ++ (H) ++ (H) ++ (H) ++ (H) ++ (H) ++ (H) ++ ("aaa") ++ (J) ++ (J) ++ (K) ++ (J) ++ (J) ++ (J) ++ (J) ++ (K) ++ (J) ++ (J) ++ ("b") ++ (M) ++ ("a") ++ (M) ++ ("ab") ++ (N) ++ (N) ++ (N) ++ (N) ++ ("b") ++ (P) ++ (P) ++ ("baa"));
solve satisfy;
