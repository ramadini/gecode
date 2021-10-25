var string: J;
var string: I;
var string: K;
var string: R;
var string: G;
var string: L;
var string: N;
var string: M;
var string: P;
constraint
  ((J) ++ ("aa") ++ (J) ++ ("ab") ++ (N) ++ (L) ++ ("a") ++ (K) ++ ("b") ++ (P) ++ (P) ++ ("ba"))
    =
    (("a") ++ (G) ++ (I) ++ ("aa") ++ (M) ++ (M) ++ (M) ++ (M) ++ ("baaaab") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
