var string: J;
var string: H;
var string: K;
var string: I;
var string: G;
var string: L;
var string: N;
var string: O;
var string: M;
var string: P;
constraint
  ((G) ++ (H) ++ ("a") ++ (I) ++ ("aa") ++ (G) ++ (H) ++ ("a") ++ (I) ++ ("ab") ++ (K) ++ (K) ++ (J) ++ ("abaa") ++ (L) ++ ("b") ++ (N) ++ ("ba"))
    =
    (("aaaa") ++ (H) ++ ("aaa") ++ (H) ++ (I) ++ ("a") ++ (J) ++ ("a") ++ (J) ++ (K) ++ ("aa") ++ (J) ++ ("a") ++ (J) ++ (K) ++ ("ab") ++ (L) ++ ("a") ++ (M) ++ (L) ++ ("a") ++ (M) ++ ("b") ++ (O) ++ (O) ++ ("b") ++ (P) ++ (P) ++ ("baa"));
solve satisfy;
