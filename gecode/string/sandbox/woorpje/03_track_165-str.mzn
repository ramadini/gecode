var string: I;
var string: K;
var string: H;
var string: R;
var string: L;
var string: O;
var string: M;
var string: P;
constraint
  ((I) ++ ("aaaaaa") ++ (I) ++ ("aaaaab") ++ (K) ++ ("a") ++ (I) ++ (K) ++ ("b") ++ (L) ++ (O) ++ ("b") ++ (P))
    =
    (("a") ++ (H) ++ ("aa") ++ (L) ++ (I) ++ (L) ++ (I) ++ ("b") ++ (M) ++ (M) ++ ("b") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
