var string: N;
var string: I;
var string: K;
var string: H;
var string: R;
var string: F;
var string: L;
var string: O;
var string: M;
constraint
  ((F) ++ ("aa") ++ (F) ++ ("aba") ++ (I) ++ (K) ++ ("b") ++ (L) ++ ("b") ++ (O))
    =
    (("a") ++ (I) ++ (H) ++ ("aa") ++ (L) ++ (L) ++ ("b") ++ (M) ++ (N) ++ (M) ++ (N) ++ ("b") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
